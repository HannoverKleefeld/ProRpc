<?php
if(!DEFINED('ERROR_HANDLER_FUNCTION'))DEFINE('ERROR_HANDLER_FUNCTION','Error_Handler') ;
if(!DEFINED('DEBUG_HANDLER_FUNCTION'))DEFINE('DEBUG_HANDLER_FUNCTION','Debug_Handler') ;

class ErrorHandlerExeption extends Exception {


}


trait ErrorHandler {
	private $_lastError = null;
	private $_raiseOnError = false;
	private $_exitOnError = false;
	private $_multipleErrors = false;
	private $_DebugRefObjectPtr = null;
	private $_DebugRefFuncName  = '';
	
	static $_debugLevel = DEBUG_NONE;
	
	public function HasError($ErrorID=0){ 
		if(!$ErrorID) return !is_null($this->_lastError);
		if(!$this->_multipleErrors)return $this->_lastError[1]==$ErrorID;
		foreach($this->_lastError as $e)if($e[1]==$Err)return true;
		return false;
	}
	public function SetDebugLevel(int $Level){ static::$_debugLevel=$Level;}
	public function SetMultipleErrors(bool $MultipleErrors){$this->_multipleErrors=$MultipleErrors;}
	public function SetRaiseOnError(bool $RaiseOnError){$this->_raiseOnError=$RaiseOnError;}
	public function SetExitOnError(bool $ExitOnError){$this->_exitOnError=$ExitOnError;}
	public function SetDebugHandler(object $object=null, string $DebugRefFuncName = ''){
		if(is_null($object))return is_null($this->_DebugRefFuncName=$this->_DebugRefObjectPtr=null);
		if(empty($DebugRefFuncName))$DebugRefFuncName='debug';
//		elseif(method_exists($object,'RequestAction'))$DebugRefFuncName='RequestAction';
		if(method_exists($object,$DebugRefFuncName))$this->_DebugRefFuncName=$DebugRefFuncName;
		else return (bool)$this->_DebugRefFuncName='';
		$this->_DebugRefObjectPtr=&$object;
	}
	public function LastErrorCode(){ 
		if(!$e=$this->_lastError)return 0;
		if($this->_multipleErrors) $e=array_pop($e);
		return $e[1];
	}	
	public function LastError(){
		if(!$e=$this->_lastError)return '';
		if($this->_multipleErrors) $e=array_pop($e);
		return $e[0];
	}
	public function GetError(bool $clearError=null, $plain=false){
		if(is_null($this->_lastError))return $plain?[]:'';
		if($plain)return $this->_multipleErrors?$this->_lastError:[$this->_lastError];
		if($this->_multipleErrors)foreach($this->_lastError as $e)$error[]="({$e[1]}) ".$e[0];
		else $error=["({$this->_lastError[1]}) ".$this->_lastError[0]];
		if(is_null($clearError)||$clearError!==false)$this->_lastError=null;
		return implode("\n",$error);
	}
	public function MergeErrors($object){
		if(!is_object($object)||!method_exists($object,'hasError')||!$object->hasError())return true;
		$e=$object->getError(true,true);
		if($this->_multipleErrors){
			$this->_lastError=empty($this->_lastError)?$e:array_merge($this->_lastError, $e);
		}else $this->_lastError=array_pop($e);
	}
	public static function StaticDebug(int $Level, string $Message , $ErrorCode=-1, $CalledClass='', $ErrorRefObjectPtr=null, $ErrorRefFuncName=''){
		if(!static::$_debugLevel || !(static::$_debugLevel & $Level))return false;
		// ip-symcon fix	
		if(is_object($ErrorRefObjectPtr) && $ErrorRefFuncName=='RequestAction'){
			return $ErrorRefObjectPtr->{$ErrorRefFuncName}('debug',[$Message,$ErrorCode,$CalledClass]);
		}
		elseif(is_object($ErrorRefObjectPtr)&&!empty($ErrorRefFuncName))return $ErrorRefObjectPtr->{$ErrorRefFuncName}($Message,$ErrorCode,$CalledClass);
		
		
		if(function_exists(DEBUG_HANDLER_FUNCTION) && call_user_func_array(DEBUG_HANDLER_FUNCTION,[$Message,$ErrorCode,$CalledClass]))return null;
		if($CalledClass)$CalledClass="[$CalledClass] ";
		echo "DEBUG: $CalledClass($ErrorCode) $Message\n";
		return false;
	}
	public static function GetMessage(int $MessageNumber , $Params=null /* ... */){
		require_once LIB_INCLUDE_DIR . '/config/messages.inc';
		if(empty(Messages[$MessageNumber])) return "message ($MessageNumber) not found!";
		$cs=preg_match_all('/(%)/', Messages[$MessageNumber]);
		if($cs==0)return Messages[$MessageNumber];
		$arguments=[Messages[$MessageNumber]];
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
	protected function clearError(){
		$this->_lastError=null;
	}
	
	protected function error($Message, $ErrorCode=null){
		if(is_numeric($Message)){$ErrorCode=func_get_args();	array_shift($ErrorCode);}
		if(is_numeric($Message) && $m=static::GetMessage($Message, $ErrorCode)){  // Search for Message
			$ErrorCode=$Message; $Message=$m;
		}else if(is_array($ErrorCode))$ErrorCode='';
		
		
		if(function_exists(ERROR_HANDLER_FUNCTION) && call_user_func_array(ERROR_HANDLER_FUNCTION, [$Message,$ErrorCode,get_class($this)])===true)return null;
		if($this->_raiseOnError)throw new ErrorHandlerExeption($Message,$ErrorCode);
		if($this->_exitOnError)exit("ERROR: ".get_class($this)."($ErrorCode) $Message");
		if($this->_multipleErrors)$this->_lastError[]=[$Message,$ErrorCode];else $this->_lastError=[$Message,$ErrorCode];
		echo "ERROR: ".get_class($this)."($ErrorCode) $Message\n";
		return null;
	}
	protected function debug(int $Level, string $Message, $ErrorCode=-100){
		if(!static::$_debugLevel || !(static::$_debugLevel & $Level))return;
		if(empty($Message)&&$ErrorCode>500)$Message=$this->GetMesage($ErrorCode);
		return static::StaticDebug($Level, $Message,$ErrorCode, get_class($this),$this->_DebugRefObjectPtr,$this->_DebugRefFuncName);
	}
	
}
