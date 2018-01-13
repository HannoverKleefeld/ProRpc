<?php
const 
	NAME_INSTANCE_ID = 'InstanceID',
	EVENT_INSTANCE_ID = 99;

class IPSBaseModule extends IPSModule {
	
	protected function debug($Message, $ErrorCode=null, $ErrorClass=null /*,..... */){
		if(is_numeric($Message) || strpos($Message,'%s')!==false){
			$ErrorParams=func_get_args();
			array_shift($ErrorParams);
			$Message=$this->GetMessage($Message,$ErrorParams);
			$this->SendDebug(get_class($this),$Message,0);	
		}else{
			if(is_null($ErrorClass))$ErrorClass=get_class($this);
			$this->SendDebug($ErrorClass,$ErrorCode?"($ErrorCode) $Message":$Message,0);
		}	
		return true;
	}
	protected function error($Message, $ErrorCode=null){
		if(empty($Message))return null;
 		if(is_numeric($Message) || strpos($Message,'%s')!==false){
			$ErrorParams=func_get_args();
			array_shift($ErrorParams);
			if(is_numeric($Message))$ErrorCode=$Message;  
			$Message=$this->GetMessage($Message,$ErrorParams);
			
 		}
		if(!is_numeric($ErrorCode))$ErrorCode=null;
 		IPS_LogMessage(get_class($this), 'ERROR:'.($ErrorCode?" ($ErrorCode) ":' ').$Message);
		return null;
	}	
	protected function GetMessage($MessageID , $Params=null /* ... */){
 		if(is_numeric($MessageID)){
 			require_once LIB_INCLUDE_DIR . '/config/messages.inc';
			if(empty(Messages[$MessageID]))return '';
			$Message=$this->Translate(Messages[$MessageID]);
 		}else $Message=$this->Translate($MessageID);
		$cs=preg_match_all('/(%)/', $Message);
		if($cs==0)return $Message;
		$arguments=[$Message];
		if(!is_null($Params)){
			if(!is_array($Params)){
				$args=func_get_args();
				array_shift($Params);
			}else $args=&$Params;
			$numargs=count($args);
			for ($i = 0; $i < $numargs; $i++)$arguments[]=$args[$i];
		}
		while(count($arguments)<$cs)$arguments[]=NULL;
		return str_replace(['%s '], '', call_user_func_array('sprintf', $arguments));
	}
	protected function setValue(string $ident, $value, $force=false){
		if( ($id=@$this->GetIDForIdent($ident)) && ($force || GetValue($id)!=$value))return SetValue($id,$value);
		return ($id>0);
	}
	protected function getValue(string $ident){
		return $id=@$this->GetIDForIdent($ident)?GetValue($id):null;
	}
	protected function getStatus(){
		return IPS_GetInstance($this->InstanceID)['InstanceStatus'];
	}
	
	
	protected function setProperty($name, $value, $store=false){
		if(is_array($value))$value=json_encode($value);
		if(IPS_GetProperty($this->InstanceID,$name)==$value) return false;
		IPS_SetProperty($this->InstanceID,$name,$value);
		return $store?IPS_ApplyChanges($this->InstanceID):true;
	}
	

	
}
?>