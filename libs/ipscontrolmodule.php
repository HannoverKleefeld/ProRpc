<?php
	
abstract class IPSControlModule extends IPSBaseModule {
	function Create(){
		parent::Create();
		$this->SetBuffer('ApiProps',0);
		$this->registerPropertyInteger('InstanceID',0);
		$this->RegisterMessage($this->InstanceID,10503); // Instanzinterface verf�gbar
		$this->ConnectParent('{19650302-XABA-MAJA-GATE-20180101XLIB}');
	}
	function ApplyChanges(){
		parent::ApplyChanges();
 		$id=$this->RegisterVariableInteger('LastUpdate','LastUpdate','~UnixTimestamp',0);
 		IPS_SetHidden($id,true);
	}
	function MessageSink ( $TimeStamp, $SenderID, $MessageID, $Data ){
		if($MessageID==11101){ // Instanz wurde verbunden
			$MessageID='Gateway %s connected';
			$this->onInterfaceChanged(true, intval($Data));
		}elseif($MessageID==11102){ // Instanz wurde getrennt
			$MessageID='Gateway disconnected';
			$this->onInterfaceChanged(false,0);
		}//	elseif($NachrichtID==10505){ $NachrichtID='status changed';	}
		elseif($MessageID==10503){
			$MessageID='Gateway pressent';
			$this->RegisterMessage($this->InstanceID,11101); // Instanz wurde verbunden
			$this->RegisterMessage($this->InstanceID,11102); // Instanz wurde getrennt
			$this->UnRegisterMessage($this->InstanceID,10503);
			$this->onInterfaceChanged(true,intval($Data));
		}else $MessageID='Unknown MessageID '.$MessageID. ' Data: %s';
		$this->SendDebug(__FUNCTION__, sprintf($MessageID ,implode($Data)),0);
	}
	function GetConfigurationForm(){
		$form["elements"][]=["type"=> "Select", "name"=>"InstanceID", "caption"=> "Device InstanceID","options"=> [["label"=>'0',"value"=> 0],["label"=>'1',"value"=> 1],["label"=>'2',"value"=> 2],["label"=>'3',"value"=> 3]]];
//  		$form["elements"][]=["type"=> "CheckBox", "name"=>"DEL_UNWANTED", "caption"=> "check if you want to delete not used variables."];	
		
//  		$actions=json_decode($this->ReadPropertyString('ACTIONS'),true);
// 		utf8::decode_array($actions);
// 		foreach($actions as &$v)$v=$v['NAME'];
// 		$actions=implode(',',$actions);
// 		$form["actions"][]=["type"=> "Label", "label"=>"Actions : ".$actions];
//  		$form["actions"][]=["type"=> "Button", "label"=>"About","onClick"=>"IPS_RequestAction(\$id,'ABOUT_ME','');"];
		
				
		$form["status"]=[
				["code"=>102, "icon"=>"active",  "caption"=> "Instance is connected and ready"],
				["code"=>200, "icon"=>"error",   "caption"=> "Instance is disconnected"],
				["code"=>201, "icon"=>"error",   "caption"=> "Connected instance does not support > ".str_ireplace('RPC','',get_class($this))." <"]
		];
		
		return json_encode($form);
	}	
	function RequestAction($Ident,$Value){
		switch ($Ident){
			case 'ABOUT_MODULE': break;
			case 'DEBUG_API'   :  $debug=json_decode($Value); $this->SendDebug($debug[0],$debug[1],0); break;
			default: return false;
		}
// 		if($ident=='ABOUT_ME'){
// 			if($about=$this->aboutModule()){
// 				if ($value===TRUE) return $about;
// 				echo $about."\n";
// 				return true;
// 			}
// 		}
		return true;
	}
	function ReceiveData($JSONString){
		$this->SendDebug(__CLASS__,"Receive: $JSONString",0);
		$rd=json_decode($JSONString);
		if(empty($rd->Buffer))return null;
		if(empty($rd->Buffer->Command))return null;
		switch($rd->Buffer->Command){
			case API_PROPS_IDENT : 
				$this->updateVariablesByProps($rd->Buffer->Data);
				break;
			default: 
				if (method_exists($this, $rd->Buffer->Command)){
					if(empty($rd->Buffer->Data)){
						$this->{$rd->Buffer->Command}();
					}else{
					if(!is_object($rd->Buffer->Data))$rd->Buffer->Data=[$rd->Buffer->Data];
						call_user_func_array([$this,$rd->Buffer->Command], (array)$rd->Buffer->Data);	
					}
				}else IPS_LogMessage(__CLASS__,"ERROR => Invalid command ->{$rd->Buffer->Command}<- received");
		}
	}
	
	public function UpdateStatus(bool $Force){
		$id=$this->GetIDForIdent('LastUpdate');
		list($usec, $sec) = explode(" ", microtime());
		if(!$Force){
			$t=GetValue($id);
	// 		IPS_LogMessage('info',"saved: $t, sec: $sec, usec: $usec");
			if($sec-$t   < 1 ){
				if($usec < 0.5) {
	// 				IPS_LogMessage('return',"saved: $t, sec: $sec, usec: $usec");
					return false;
				}
			// 			echo "saved: $t, sec: $sec, usec: $usec";
			}
		}
		SetValue($id,$sec);
		return true;
	}
	
	// must return array of Props ( VariableType, Profilename, Position [, icon ] )
	abstract protected function getProps();//:array;
	
	protected function getValue(int $Prop, $Force=false){
		if($Force)return null;
		if(!$id=@$this->GetIDForIdent(NAMES_PROPS[$Prop]))return null;
		return GetValue($id);
	}
	protected function setValue(int $Prop, $Value, $Force=false){
		if(!$id=@$this->GetIDForIdent(NAMES_PROPS[$Prop]))return false;
		if(GetValue($id)==$Value)return true;
		return ($Force)? null: SetValue($id,$Value);
	}
	protected function setValues(array $PropValues, $Force=false){
		$myProps=$this->getProps();
		foreach($PropValues as $Prop=>$Value){
			if(!is_numeric($Prop)){
				$uname=strtoupper($Prop);
				foreach($myProps as $Prop)if($found=$uname==NAMES_PROPS[$Prop])break;
				if(!$found)continue;
			}
			if(array_key_exists($Prop, $myProps))
				$this->setValue($Prop,$Value,$Force);
		}
	}
	
	protected function apiHasProp(int $Prop){
		return intval($this->GetBuffer('ApiProps') & $Prop);
	}
	protected function onInterfaceChanged(bool $connected, int $InterfaceID){
		if($connected){
			$apiProps=$this->forwardRequest('RequestProps',[array_sum($this->getProps())]);
			$this->updateVariablesByProps($apiProps);
		}else{
			$this->updateVariablesByProps(0);
			$this->SetStatus(200);
		}
	}
	protected function updateVariablesByProps($NewProps){
		$this->SetBuffer('ApiProps',$NewProps);
		$myProps = $this->getProps();
		if($NewProps & array_sum(array_keys($myProps))) {
			$this->SetStatus(102);
		}else $this->SetStatus(201);
		$filter=['.*InstanceID.*'];
		foreach($myProps as $prop=>$def){
			if(is_null($def))continue;
			$id = @$this->GetIDForIdent(NAMES_PROPS[$prop]);
			if($NewProps & $prop){
				if($id)
					IPS_SetHidden($id,false);
				else {
					$id=$this->createVariable(NAMES_PROPS[$prop],$def[0],$def[1],$def[2]);
					if(!empty($def[3]))IPS_SetIcon($id,$def[3]);
				}
				$this->EnableAction(NAMES_PROPS[$prop]);
				$filter[]='.*'.NAMES_PROPS[$prop].'.*';
			}elseif($id){
				IPS_SetHidden($id,true);
				$this->DisableAction(NAMES_PROPS[$prop]);
			}
		}
		$this->updateReceiveFilter($filter);
		$this->UpdateStatus(true);
	}
	protected function forwardRequest(string $function, array $arguments){
		$arguments['InstanceID']=$this->ReadPropertyInteger('InstanceID');
		$data['Buffer']=['Function'=>$function,'Arguments'=>$arguments];
		$data['DataID']="{19650302-GATE-MAJA-PRPC-20180101XLIB}";
		$data['ObjectID']=$this->InstanceID;
// 		utf8::encode_array($data);
		$data=json_encode($data);
		$this->SendDebug(__FUNCTION__,"Send: $data",0);
		$result=$this->SendDataToParent($data);
		$this->SendDebug(__FUNCTION__,"Return: $result",0);
		$result=json_decode($result,true); //utf8::decode_array($result);
		if(empty($result)||!isset($result['Result'])){
			if(isset($result['Error']))IPS_LogMessage(get_class($this),"ERROR in ".__FUNCTION__." Call: $function(".implode(',',$arguments).") => ".$result['Error']['message']);
			return null; // Error
		}
		return $result['Result'];
	}
	protected function createVariable(string $ident,int $type, $profile='',$pos=0){
		if($type==0)$id=$this->RegisterVariableBoolean($ident,$ident,$profile,$pos);
		elseif($type==1)$id=$this->RegisterVariableInteger($ident,$ident,$profile,$pos);
		elseif($type==2)$id=$this->RegisterVariableFloat($ident,$ident,$profile,$pos);
		elseif($type==3)$id=$this->RegisterVariableString($ident,$ident,$profile,$pos);
		return $id;
	}
	protected function updateReceiveFilter(array $Filter=null){
		if(empty($Filter))$Filter=[];
		
		array_unshift($Filter, '.*'.API_PROPS_IDENT.'.*');
		$Filter=implode('|',$Filter);
 		$this->SetReceiveDataFilter ($Filter);
		$this->SendDebug(__FUNCTION__,"Set receive filter: $Filter",0);
	}

}
?>