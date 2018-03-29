<?php
	
/**
 * @author Xaver Bauer
 *
 */
abstract class IPSControlModule extends IPSBaseModule {
	protected $api=null;
	protected $logger=null;
	/**
	 * {@inheritDoc}
	 * @see IPSModule::Create()
	 */
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
	/**
	 * {@inheritDoc}
	 * @see IPSModule::MessageSink()
	 */
	function MessageSink ( $TimeStamp, $SenderID, $MessageID, $Data ){
		if($MessageID==11101){ // Instanz wurde verbunden
			$MessageID='Gateway %s connected';
			$this->onInterfaceChanged(true, intval(implode($Data)));
		}elseif($MessageID==11102){ // Instanz wurde getrennt
			$MessageID='Gateway disconnected';
			$this->onInterfaceChanged(false,0);
		}//	elseif($NachrichtID==10505){ $NachrichtID='status changed';	}
		elseif($MessageID==10503){
			$MessageID='Gateway pressent';
			$this->RegisterMessage($this->InstanceID,11101); // Instanz wurde verbunden
			$this->RegisterMessage($this->InstanceID,11102); // Instanz wurde getrennt
			$this->UnRegisterMessage($this->InstanceID,10503);
			$this->onInterfaceChanged(true,intval(implode($Data)));
		}else $MessageID='Unknown MessageID '.$MessageID. ' Data: %s';
		$this->SendDebug(__FUNCTION__, sprintf($MessageID ,implode($Data)),0);
	}
	/**
	 * {@inheritDoc}
	 * @see IPSModule::GetConfigurationForm()
	 */
	function GetConfigurationForm(){
		$form["elements"][]=["type"=> "Select", "name"=>"InstanceID", "caption"=> "Device InstanceID","options"=> [["label"=>'0',"value"=> 0],["label"=>'1',"value"=> 1],["label"=>'2',"value"=> 2],["label"=>'3',"value"=> 3]]];
		if($this->getStatus()>109){
			$form["status"]=[
					["code"=>200, "icon"=>"error",   "caption"=> "Instance is disconnected"],
					["code"=>201, "icon"=>"error",   "caption"=> "Connected instance does not support > ".str_ireplace('RPC','',get_class($this))." <"]
			];
		}
		return json_encode($form);
	}	
	function RequestAction($Ident,$Value){
		if(($ok=parent::RequestAction($Ident, $Value))!==false)return $ok;
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
	/**
	 * {@inheritDoc}
	 * @see IPSModule::ReceiveData()
	 */
	function ReceiveData($JSONString){
		$this->SendDebug(__CLASS__,"Receive: $JSONString",0);
		$rd=json_decode($JSONString);
		if($rd->ObjectID==$this->InstanceID)return null; // ignore self sended Data
		if(empty($rd->Buffer))return null; // ignore no Buffer 
		if(empty($rd->Buffer->Command))return null; // ignore no Command
		if($rd->Buffer->Command !=API_PROPS_IDENT){
			if (!isset($rd->Buffer->Data->InstanceID)){
				IPS_LogMessage(__CLASS__,"ERROR! ReceiveData Command {$rd->Buffer->Command} with empty InstanceID");
				return null;
			}
			if($rd->Buffer->Data->InstanceID != ($test=$this->ReadPropertyInteger('InstanceID'))){
				$this->SendDebug(__FUNCTION__,"Command {$rd->Buffer->Command} InstanceID >{$rd->Buffer->Data->InstanceID}< not eqal self InstanceID >$test<",0);
				return null;			
			}
			if($rd->Buffer->Command=='DataChanged' || $rd->Buffer->Command=='PropsChanged'){
				$rd->Buffer->Data=json_decode(json_encode($rd->Buffer->Data),true);
				if(isset($rd->Buffer->Data[0]))$rd->Buffer->Data=$rd->Buffer->Data[0];
				else unset($rd->Buffer->Data['InstanceID']);
			}
// 			IPS_LogMessage(__CLASS__,var_export($rd->Buffer->Data,true));
		}
		switch($rd->Buffer->Command){
			case API_PROPS_IDENT : 
				$this->updateVariablesByProps($rd->Buffer->Data);
				break;
			case 'DataChanged' :
// 				$this->SendDebug(__FUNCTION__, "Data(s) changed => ".implode(',',$rd->Buffer->Data), 0);
				$this->dataChanged($rd->Buffer->Data);
				break;
			case 'PropsChanged':
// 				$this->SendDebug(__FUNCTION__, "Prop(s) changed => ".$rd->Buffer->Data, 0);
				$this->_propsChanged($rd->Buffer->Data);				
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
	
	/**
	 * @method UpdateStatus
	 * @param bool $Force
	 * @return bool
	 */
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
			}else SetValue($id,$sec); 
		}else SetValue($id,$sec);
		return true;
	}
	
	// must return emty array() or array of Props : array( PROP => array( VariableType, Profilename, Position [, icon ] ), PROP => .... )
	/**
	 * @method getProps
	 * @return array
	 */
	abstract protected function getProps();//:array;
	/**
	 * @method getPropsAsInt
	 * @return int
	 */
	protected function getPropsAsInt(){
		return array_sum(array_keys($this->getProps()));
	}
	/**
	 * @method getValueByProp
	 * @param int $Prop
	 * @param boolean $Force
	 * @return NULL|mixed
	 */
	protected function getValueByProp(int $Prop, $Force=false){
		if(!array_key_exists($Prop, $this->getProps()) ) return null; 
		return $Force? null:$this->getValueByIdent(NAMES_PROPS[$Prop]);
	}
	/**
	 * @method setValueByProp
	 * @param int $Prop
	 * @param mixed $Value
	 * @return NULL|boolean
	 */
	protected function setValueByProp(int $Prop, $Value){
		if(!array_key_exists($Prop, $this->getProps()) ) return null; 
		return $this->setValueByIdent(NAMES_PROPS[$Prop], $Value);
	}
	/**
	 * @method getValueByIdent
	 * @param string $Ident
	 * @return NULL|mixed
	 */
	protected function getValueByIdent(string $Ident){
		if(!$id=@$this->GetIDForIdent($Ident))return null;
		return GetValue($id,$Value);
	}	
	/**
	 * @method setValueByIdent
	 * @param string $Ident
	 * @param unknown $Value
	 * @return NULL|boolean
	 */
	protected function setValueByIdent(string $Ident, $Value){
		if(!$id=@$this->GetIDForIdent($Ident))return null;
		if(GetValue($id)==$Value)return false;
		return SetValue($id,$Value);
	}	
	/**
	 * @method apiHasProp
	 * @param int $Prop
	 * @return number
	 */
	protected function apiHasProp(int $Prop){
		return intval($this->GetBuffer('ApiProps') & $Prop);
	}
	/**
	 * @method onInterfaceChanged
	 * @param bool $connected
	 * @param int $InterfaceID
	 */
	protected function onInterfaceChanged(bool $connected, int $InterfaceID){
		if($connected){
			$this->SetBuffer('ApiObjectID',$InterfaceID);
			$apiProps=$this->forwardRequest('RequestProps',[array_sum($this->getProps())]);
			$this->updateVariablesByProps($apiProps);
		}else{
			$this->SetBuffer('ApiObjectID',0);
			$this->updateVariablesByProps(0);
			$this->SetStatus(200);
		}
		IPS_LogMessage('TEST','InterfaceID:'.$InterfaceID);
	}
	/**
	 * @method onDataChanged
	 * @param int $Prop
	 * @param mixed $Value
	 */
	protected function onDataChanged(int $Prop, $Value){}
	/**
	 * @method updateVariablesByProps
	 * @param int $NewProps
	 * @return boolean
	 */
	protected function updateVariablesByProps(int $NewProps){
		$this->SetBuffer('ApiProps',$NewProps);
		$myProps = $this->getProps();
		$filter=[];//['.*InstanceID.*'];
		foreach($myProps as $prop=>$def){
			if(is_null($def))continue;
			$id = @$this->GetIDForIdent(NAMES_PROPS[$prop]);
			if($NewProps & $prop){
				if($id)
					IPS_SetHidden($id,false);
				else {
					$id=$this->createVariable(NAMES_PROPS[$prop],$this->Translate($def[0]),$def[1],$def[2]);
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
		if(count($myProps)==0 || $NewProps & array_sum(array_keys($myProps))) {
			$this->SetStatus(102);
			$this->UpdateStatus(true);
			return true;
		}
		$this->SetStatus(201);
		return false;
	}
	/**
	 * @method forwardRequest
	 * @param string $function
	 * @param array $arguments
	 * @return NULL|mixed
	 */
	protected function forwardRequest(string $function, array $arguments){
		if(!isset($arguments['InstanceID']))
			$arguments['InstanceID']=$this->ReadPropertyInteger('InstanceID');
		$data['Buffer']=['Function'=>$function,'Arguments'=>$arguments];
		$data['DataID']="{19650302-GATE-MAJA-PRPC-20180101XLIB}";
		$data['ObjectID']=$this->InstanceID;
// 		utf8::encode_array($data);
		$data=json_encode($data);
		$this->SendDebug(__FUNCTION__,"Send: $data",0);
		$result=$this->SendDataToParent($data);
		if(is_object($result)){
			$this->SendDebug(__FUNCTION__,"Return: Object",0);
			return $result;
		}
		$this->SendDebug(__FUNCTION__,"Return: $result",0);
		$result=json_decode($result,true); //utf8::decode_array($result);
		if(empty($result)||!isset($result['Result'])){
			if(isset($result['Error']))IPS_LogMessage(get_class($this),"ERROR in ".__FUNCTION__." Call: $function(".implode(',',$arguments).") => ".$result['Error']['message']);
			return null; // Error
		}
		return $result['Result'];
	}
	/**
	 * @method createVariable
	 * @param string $ident
	 * @param int $type
	 * @param string $profile
	 * @param int $pos
	 * @return int
	 */
	protected function createVariable(string $ident,int $type, $profile='',$pos=0){
		if($type==0)$id=$this->RegisterVariableBoolean($ident,$ident,$profile,$pos);
		elseif($type==1)$id=$this->RegisterVariableInteger($ident,$ident,$profile,$pos);
		elseif($type==2)$id=$this->RegisterVariableFloat($ident,$ident,$profile,$pos);
		elseif($type==3)$id=$this->RegisterVariableString($ident,$ident,$profile,$pos);
		return $id;
	}
	/**
	 * @method updateReceiveFilter
	 * @param array $Filter
	 */
	protected function updateReceiveFilter(array $Filter){
		array_unshift($Filter, '.*DataChanged.*');
		array_unshift($Filter, '.*PropsChanged.*');
		array_unshift($Filter, '.*'.API_PROPS_IDENT.'.*');
		$Filter=implode('|',$Filter);
 		$this->SetReceiveDataFilter ($Filter);
		$this->SendDebug(__FUNCTION__,"Set receive filter: $Filter",0);
	}
	

	/**
	 * @method getApi
	 * @return RPC
	 */
	protected function getApi(){
		if($this->api)return $this->api;
		if(!$InterfaceID=intval($this->GetBuffer('ApiObjectID')))return null;
		if($this->api=RGATE_GetApi($InterfaceID)){
			if($this->logger=$this->api->GetLogger())
				$this->logger->SetParent($this);
		}
		return $this->api;
	}
	
	// Only used From ReceiveData [Command]
	/**
	 * @method dataChanged
	 * @param array $PropValues
	 * @return boolean
	 */
	protected function dataChanged(array &$PropValues){
		$myProps=$this->getProps();
		foreach($PropValues as $Key=>$Value){
			$Prop=$Key;
			if(!is_numeric($Prop)){
				$uname=strtoupper($Prop);
				foreach($myProps as $Prop=>$tmp)if($found=$uname==NAMES_PROPS[$Prop])break;
				if(!$found)continue;
			}elseif(!array_key_exists($Prop, $myProps))continue;
			if(is_null($Value))
				$this->getValueByProp($Prop,true);
			else 
				if($this->setValueByIdent(NAMES_PROPS[$Prop],$Value))$this->onDataChanged($Prop, $Value);
			unset($PropValues[$Key]);
		}
		return count($PropValues)==0;
	}
	/**
	 * @method _propsChanged
	 * @param unknown $Props
	 * @return NULL|boolean
	 */
	private function _propsChanged($Props){
		if(!is_array($Props)){
			if(empty($Props))return null;
			foreach($this->getProps() as $p=>$v)
				if($Prop & $p)$data[$p]=null;
		}else $data=&$Props;
		return count($data)>0?$this->dataChanged($data):false;
	}
}
?>