<?php
// if (!defined ('RPC_ERROR_HANDLER_FUNCTION'))define('RPC_ERROR_HANDLER_FUNCTION',null);	
// if (!defined ('RPC_DEBUG_HANDLER_FUNCTION'))define('RPC_DEBUG_HANDLER_FUNCTION',null);	

require_once 'rpcmessage.php';

class RpcLogger {
	protected $errors = null;
	protected $oMessage = null;
	protected $classnames=[];
	protected $logoptions;
	protected $logFileHandle=0;
	protected $_runtimeDebugName='';
	
	function __construct($LogOptions=DEBUG_ALL, $LogFileName='',RpcMessage $MessageObject=null){
		$this->logoptions=$LogOptions;
		$this->SetMessage($MessageObject);
		$this->SetLogFile($LogFileName);
	}
	function __destruct(){
		if($this->logFileHandle)fclose($this->logFileHandle);
	}
	public function SetLogFile($LogFileName){
		if($this->logFileHandle)fclose($this->logFileHandle);
		if(!$LogFileName)return $this->logFileHandle=0;
		$this->logFileHandle=fopen($LogFileName, "w");
		return true;
	}
	public function SetMessage(RpcMessage $MessageObject=null){
		$this->oMessage= $MessageObject;
	}
	public function Error($ErrorCode, $Message, $Params=null /* ... */){
		if($Params && !is_array($Params))$Params=array_slice(func_get_args(),2);
		$this->log($Message, $Params,true,$ErrorCode);
		if($this->logoptions & LOG_OPT_THROW_ON_ERROR)throw new RPCErrorHandler($Message,$ErrorCode);
		if($this->logoptions & LOG_OPT_EXIT_ON_ERROR)exit('Error Exit');
		return null;
	}
	public function Debug($LogOption, $Message, $Params=null /* ... */){
		if(!$this->logoptions & $LogOption)return;
		$this->_runtimeDebugName='';
		foreach(ALL_DEBUG as $opt)if($LogOption & $opt){$this->_runtimeDebugName=NAMES_DEBUG[$opt];break;}
		if($Params && !is_array($Params))$Params=array_slice(func_get_args(),2);
		return $this->log($Message, $Params);
	}
	
	public function SetLogOptions($LogOptions){ $this->logoptions=$LogOptions;}
	public function Attach($Object){
		if(!in_array($n=get_class($Object), $this->classnames))$this->classnames[]=$n;
		return $this;
	}
	public function Detach($Object){
		if(($pos=array_search(get_class($Object), $this->classnames))!==false)unset($this->classnames[$pos]);
	}
	public function HasError($ErrorID=0){ 
		if(!$ErrorID) return !is_null($this->errors);
		foreach($this->errors as $e)if($e[1]==$ErrorID)return true;
		return false;
	}
	public function LastErrorCode(){ 
		if(!$e=$this->errors)return 0;
		$e=array_pop($e);
		return $e[1];
	}	
	public function LastErrorMessage(){
		if(!$e=$this->errors)return '';
		$e=array_pop($e);
		return $e[0];
	}
	public function GetError(bool $clearError=null, $plain=false){
		if(is_null($this->errors))return $plain?[]:'';
		if($plain)return $this->errors;
		foreach($this->errors as $e)$error[]="({$e[1]}) ".$e[0];
		if(is_null($clearError)||$clearError!==false)$this->errors=null;
		return implode("\n",$error);
	}
	public function Merge(RpcLogger $Logger){
		if(!$errors=$Logger->GetError(true,true))return;
		$this->errors=empty($this->errors)?$errors:array_merge($this->errors,$errors);
	}
	protected function doOutput($Message,$Class,$AsError){
		echo "$Message\n";
	}
	private function log($Message, array $Params=null,$AsError=false, $Code=null, $Class=''){
		if(is_numeric($Message)){
			if(empty($this->oMessage))$this->oMessage=new RpcMessage();
			$m=$this->oMessage->Get($Message,$Params);
			$Code=$Message;
			$Message=$m;
		}elseif($cs=preg_match_all('/(%[0-9sdx\-]+)/', $Message) && !is_null($Params)){
			while(!$Params || count($Params)<$cs)$Params[]=NULL;
			array_unshift($Params, $Message);
			$Message=preg_replace('/(%[0-9sdx\-]+)/', '', call_user_func_array('sprintf', $Params));	
		}elseif($cs)$Message=preg_replace('/(%[0-9sdx\-]+)/', '??', $Message);
			
		if(!$Class && !$Class=end($this->classnames))$Class=get_called_class();
		$Class=str_ireplace('connection','',$Class);
		if($AsError){
 			$this->errors[]=[$Message,$Code,$Class];
 			$Message=sprintf('[%4s] %s',$Code,$Message);
			$Prefix='ERROR';			
		}else $Prefix='DEBUG';
		if($this->_runtimeDebugName && !$AsError){
			if($this->logoptions&LOG_OPT_SHORT_MESSAGES)
				$Message=sprintf("%s:%s",$Class,$Message);
			else	
				$Message=sprintf("%s: %-7s %-6s %s",$Prefix,$Class,$this->_runtimeDebugName, $Message);
		}else{
			if($this->logoptions&LOG_OPT_SHORT_MESSAGES)
				$Message=sprintf("%s:%s",$Class,$Message);
			else
				$Message=sprintf("%s: %-7s %s",$Prefix,$Class,$Message);
		}	
		if($this->logFileHandle)fwrite($this->logFileHandle,date('d.m.y - H:i:s ').$Message."\n");
		
		if(!$AsError || $this->logoptions & DEBUG_ERRORS) $this->doOutput($Message,$Class,$AsError);	
	}
}
interface iRpcLogger {
	function AttachLogger(RpcLogger $Logger=null);
	function DetachLogger(RpcLogger $Logger=null);
}


?>