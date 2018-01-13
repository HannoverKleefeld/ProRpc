<?php 
#TODO 27.12.17 WebHook for Event send functions
#TODO Delete Variables when Apply and is checked Delete Unwanted
require_once LIB_INCLUDE_DIR . '\rpc.defines.inc';
abstract class RPCModule	extends IPSBaseModule {
	static $VERSION = 1.0;
	function Create(){
		parent::Create();
// 		$this->RegisterMessage($this->InstanceID,10505); // Status hat sich ge�ndert
		$this->RegisterMessage($this->InstanceID,10503); // Instanzinterface verf�gbar
		$this->registerPropertyInteger('RPC_INSTANCE',0);
 		$this->registerPropertyString('ACTIONS','[]');
 		$this->registerPropertyBoolean('DEL_UNWANTED',false);
	}
	
	function ApplyChanges(){
		parent::ApplyChanges();
		$this->ConnectParent("{165092DF-XA00-4980-SWIT-20171212XLIB}");
		
// 		if($this->ReadPropertyBoolean('DEL_UNWANTED')){
// 			$propnames=GetPropNames($this->getProps());
// 			foreach($propnames as $ident){
// 				if($id=@$this->GetIDForIdent($ident)){
					
// 				}
// 			}

// 		}
			
// 		echo var_export($actions);
	}
	function ReceiveData($JSONString){
		$this->SendDebug(__FUNCTION__,utf8_decode($JSONString),0);
		$data = json_decode($JSONString,true);
		if(!empty($data['Buffer']['Values'])){
			utf8::decode_array($data['Buffer']['Values']);
			$this->SendDebug(__FUNCTION__,'SetValues '.	str_replace("\n",'',var_export($data['Buffer']['Values'],true)),0);
			if(!isset($data['Buffer']['Values'][NAME_INSTANCE_ID]))return $this->error('No instanceID received');
			$instanceID=$data['Buffer']['Values'][NAME_INSTANCE_ID];
			unset($data['Buffer']['Values'][NAME_INSTANCE_ID]);
			$this->debug('InstanceID %s received: ',$instanceID);
			foreach($data['Buffer']['Values'] as $ident=>$value){
				$this->setValue($ident, $value);
			}
		}elseif(!empty($data['Buffer']['Command'])){
			switch ($data['Buffer']['Command']){
				case 'API_PROPS':  $this->updateByApiProps($data['Buffer']['Value']);break;
			}
		}else return false;
		return true;
	}
	function GetConfigurationForm(){
		$form["elements"][]=["type"=> "Select", "name"=>"RPC_INSTANCE", "caption"=> "Device InstanceID","options"=> [
				["label"=>'0',   "value"=> 0],
				["label"=>'1',  "value"=> 1],
				["label"=>'2', "value"=> 2],
				["label"=>'3',   "value"=> 3]
				
		]];
 		$form["elements"][]=["type"=> "CheckBox", "name"=>"DEL_UNWANTED", "caption"=> "check if you want to delete not used variables."];	
		
 		$actions=json_decode($this->ReadPropertyString('ACTIONS'),true);
// 		utf8::decode_array($actions);
		foreach($actions as &$v)$v=$v['NAME'];
		$actions=implode(',',$actions);
		$form["actions"][]=["type"=> "Label", "label"=>"Actions : ".$actions];
 		$form["actions"][]=["type"=> "Button", "label"=>"About","onClick"=>"IPS_RequestAction(\$id,'ABOUT_ME','');"];
		
				
		$form["status"]=[
// 				["code"=>101, "icon"=>"inactive",  "caption"=> "Interface is disconnected"],
				["code"=>102, "icon"=>"active",  "caption"=> "Instance is connected and ready"],
				["code"=>200, "icon"=>"error",   "caption"=> "Instance is disconnected"],
				["code"=>201, "icon"=>"error",   "caption"=> "Connected instance does not support > ".str_replace('RPC','',get_class($this))." <"]
		];
		
		return json_encode($form);
	}	
	function RequestAction($ident,$value){
		if($ident=='ABOUT_ME'){
			if($about=$this->aboutModule()){
				if ($value===TRUE) return $about;
				echo $about."\n";
				return true;
			}
		}
	}
	function MessageSink ( $Zeitstempel, $SenderID, $NachrichtID, $Daten ){
		if($NachrichtID==11101){ // Instanz wurde verbunden
			$NachrichtID='Switch %s connected';
			$this->onInterfaceChanged(true);
		}elseif($NachrichtID==11102){ // Instanz wurde getrennt
			$NachrichtID='Switch disconnected';
			$this->onInterfaceChanged(false);
		}//	elseif($NachrichtID==10505){ $NachrichtID='status changed';	}
		elseif($NachrichtID==10503){
			$NachrichtID='Switch pressent';
			$this->RegisterMessage($this->InstanceID,11101); // Instanz wurde verbunden
			$this->RegisterMessage($this->InstanceID,11102); // Instanz wurde getrennt
			$this->UnRegisterMessage($this->InstanceID,10503);
			$this->onInterfaceChanged(true);
		}else $NachrichtID='Unknown MessageID '.$NachrichtID. ' Data: %s';
		$this->debug($NachrichtID,implode($Daten));
	}
	
	abstract protected function getPropDef($prop);
	abstract protected function getProps();
	abstract protected function aboutModule();
	protected function onApiChanged(){}
	protected function onInterfaceChanged(bool $connected){
		if($connected){
			$apiProps=$this->forwardRequest('RequestApiProps',[array_sum($this->getProps())]);
			$this->updateByApiProps($apiProps);
		}else{
			$this->SetStatus(200);
		}
	}
	protected function updateByApiProps(array $Api_Props){
		$wanted_actions=[];
		$myPropsArray=$this->getProps();
		$myProps = array_sum($myPropsArray);
		$apiProps=$Api_Props['PROPS'];

		$propnames=GetPropNames($myProps);
		if(!($myProps & $apiProps)){
			$this->SetStatus(201);
		}else{
			$this->onApiChanged();
			$this->SetStatus(102);
			foreach($myPropsArray as $prop){
				
				if(!$def=$this->getPropDef($prop)){
// 					if(!empty($propnames[$prop]))$wanted_actions[]=['NAME'=>$propnames[$prop]];
					continue;
				}
				list ($ident,$name,$profile,$type,$pos) = $def;
				if(!empty($propnames[$prop]))$ident=$propnames[$prop];			
													
				if($apiProps & $prop){
					$this->createVariable($ident, $type, $name, $profile,$pos);
					$wanted_actions[$prop]=['NAME'=>$ident];
				}
			}	
		}
// 		utf8::encode_array($wanted_actions);
		$newactions=json_encode($wanted_actions);
		if(($myactions=$this->ReadPropertyString('ACTIONS'))!=$newactions){
			$myactions=json_decode($myactions,true);
			foreach($myactions as $action){
				if(in_array($action, $wanted_actions)===false){
					$this->DisableAction($action['NAME']);
					if(($id=@$this->GetIDForIdent($action['NAME'])))
						if($this->ReadPropertyBoolean('DEL_UNWANTED'))
							@$this->UnRegisterVariable($action['NAME']); 
						else IPS_SetHidden($id,true);
				} 
			}
			$this->setProperty('ACTIONS',$newactions,true);
		}
		// Enable wanted Actions and Filter
		$this->_updateReceiveFilter();
	}

	
	
	public function UpdateRequest(){
		if($this->getStatus()!=102)return null;
		if($data=$this->forwardRequest('RequestDataUpdate', [array_sum($this->getProps())])){
			foreach($data as $ident=>$value)$this->setValue($ident, $value);
		}
		return true;
	}
	
	
	protected function forwardRequest(string $function, array $arguments){
		$data['Buffer']=['Function'=>$function,'Arguments'=>$arguments];
		$data['DataID']="{165092DF-SWIT-4980-XB00-20171212XLIB}";
		$data['ObjectID']=$this->InstanceID;
		utf8::encode_array($data);
		$data=json_encode($data);
		$this->SendDebug(__FUNCTION__,"Send: $data",0);
		$result=$this->SendDataToParent($data);
		$this->SendDebug(__FUNCTION__,"Return: $result",0);
		if($result=json_decode($result,true))utf8::decode_array($result);
		if(empty($result)||!isset($result['Result'])){
			if(isset($result['Error']))IPS_LogMessage(get_class($this),"ERROR in ".__FUNCTION__." Call: $function(".implode(',',$arguments).") => ".$result['Error']['msg']);
			return null; // Error
		}
		return $result['Result'];
	}
	
	protected function getRpcInstance(){
		return $this->ReadPropertyInteger('RPC_INSTANCE');	
	}
	
	protected function createVariable(string $ident,int $type, $name='',$profile='',$pos=0){
				
		if(!($id=@$this->GetIDForIdent($ident))){
			if($type==0)$id=$this->RegisterVariableBoolean($ident,$name,$profile,$pos);
			elseif($type==1)$id=$this->RegisterVariableInteger($ident,$name,$profile,$pos);
			elseif($type==2)$id=$this->RegisterVariableFloat($ident,$name,$profile,$pos);
			elseif($type==3)$id=$this->RegisterVariableString($ident,$name,$profile,$pos);
		}
		return $id;
	}
	private function _updateReceiveFilter(){
		$filter=['.*API_PROPS.*'];
 		$apiInstanceID=$this->ReadPropertyInteger('RPC_INSTANCE');
 		$apiInstanceIDName=NAME_INSTANCE_ID;
 		$filter[]=".*\"$apiInstanceIDName\":$apiInstanceID.*";
		$actions=json_decode($this->ReadPropertyString('ACTIONS'),true);
		foreach($actions as $action){
			$this->EnableAction($ident=$action['NAME']);
			if($id=@$this->GetIDForIdent($ident))IPS_SetHidden($id,false);
			$filter[]=".*$ident.*";
		}
		$this->SetReceiveDataFilter (implode('|',$filter));
		$this->debug("Set receive filter: %s",implode('|',$filter));
	}	
}



?>