<?php
/** @file rpclogger.php
 * @brief Manage RpcApi Debug and Error Messages
 * 
 * PHP Classes and Interfaces to manage RpcApi Debug and Error Messages
 * @author Xaver Bauer
 * @date 03.04.2018
 * @version 2.0.2
 * - Fix RpcLogger::Debug with more DEBUG flags now works correct   
 * @date 21.01.2018
 * @version 2.0.1
 * Added PhpDoc Tags
 * @package rpclogger
 * @brief Manage RpcApi Debug and Error Messages
 * @copydetails rpclogger.php
 */
require_once 'rpcmessage.php';
/** @brief Manage RpcApi Debug and Error Messages
 * 
 * PHP Class to manage RpcApi Debug and Error Messages\n
 * allows the RpcApi to output errors and debug messages to the screen and the logfile\n
 * The protected method doOutput can be overridden to render all output from the screen itself\n
 * The public methods \ref Debug and \ref Error can be used for user-specific messages
 * @author Xaver Bauer
 * @date 21.01.2018
 * @version 2.0.1
 * Added PhpDoc Tags
 */
class RpcLogger {
/// @cond VARIABLES	
	/// @cond VPROTECTED
	/**
 	* @param null|array Holds the API Errors  
    */
	protected $errors = null;
	/**
 	* @param null|object The Message translation Object  
    */
	protected $oMessage = null;
	/**
 	* @param array Holds the current attached stack classnames  
    */
	protected $classnames=[];
	/**
 	* @param int Current logoptions   
    */
	protected $logoptions;
	/**
 	* @param resource If logfilename set then this holds the open Filehandle
    */
	protected $logFileHandle=0;
	/**
 	* @param string The current Logfilename or empty string
    */
	protected $logFileName = '';
	protected $_runtimeDebugName='';
	/// @endcond
/// @endcond	
	/** 
	 * @brief Create a new RpcLogger object
	 * @param int $LogOptions @ref options_logger.
	 * @param string $LogFileName The filename to write debug and error messages.
	 * @param RpcMessage $MessageObject Message translation Object
	 * @see RpcMessage RpcLogger::SetLogOptions
	 *
	 */
	function __construct($LogOptions=DEBUG_ALL, $LogFileName='',RpcMessage $MessageObject=null){
		$this->logoptions=$LogOptions;
		$this->SetMessage($MessageObject);
		$this->SetLogFile($LogFileName);
	}
/// @cond PRIVATE	
	/** @brief Destroy this object
	 * 
	 * if open close Logfilehandle then remove this object from memory  
	 */
	function __destruct(){
		if($this->logFileHandle)fclose($this->logFileHandle);
	}
	/** @brief Restore from serialization
	 * 
	 * If filename not empty reopen Logfile 
	 */
	function __wakeup(){
		$this->SetLogFile($this->logFileName);
	}
 	/** @brief Run before serilization
 	 * @return string[] a list of all property names to serialize
 	 */
 	function __sleep(){
 		return ['errors','oMessage','classnames','logoptions','logFileName','_runtimeDebugName'];
 	}
/// @endcond
 	/** @brief Set logfile
	 * 
	 * Set new logfile and (if not empty) open it for writing
	 * @param string $LogFileName
	 * @return bool True if logfilename not empty and logfile open succsesfully
	 */
	public function SetLogFile($LogFileName){
		if($this->logFileHandle)fclose($this->logFileHandle);
		$this->logFileName=$LogFileName;
		if(!$LogFileName)return ($this->logFileHandle=0);
		if(!$this->logFileHandle=fopen($LogFileName, "w"))$this->logFileName='';
		return true;
	}
	/** @brief Set options
	 * 	 
	 * Set Current Log and Error Options 
	 * @param int $LogOptions @ref options_logger
	 */
	public function SetLogOptions($LogOptions){ $this->logoptions=$LogOptions;}
	/** Set a new RpcMessage object
	 * @param object $MessageObject New RpcMessage object
	 */

	public function SetMessage(RpcMessage $MessageObject=null){
		$this->oMessage= $MessageObject;
	}
	/** @brief Write a error message to log
	 * 
	 * Do the following
	 * - throws error if LOG_OPT_THROW_ON_ERROR is set
	 * - exit error if LOG_OPT_EXIT_ON_ERROR is set
	 * 
	 * @param int $ErrorCode
	 * @param int|string $Message Message- Number to translate with RpcMessage or String to output
	 * @param null|array $Params Once or more Params to replace in Message. Default=null
	 * @throws RpcErrorHandler Throws if $Message is a number an not found by RpcMessage
	 * @return null Returns always null
	 * 
	 * <b>Demo:</b><br>
	 * <code>
	 * $logger->Error(100,'This is a error');<br>
	 * $logger->Error(100,'name %s has error ','The Name');<br> 
	 * $logger->Error(0,ERR_FunctionNotFound,'Function Name');<br>
	 * $logger->Error(0,'%s and %s are %s its %s',1,2,3,'cool');<br>
	 * </code>
	 * @see @ref options_logger
	 * @see RpcLogger::SetLogOptions
	 */
	public function Error($ErrorCode, $Message, $Params=null /* ... */){
		if($Params && !is_array($Params))$Params=array_slice(func_get_args(),2);
		$this->_log($Message, $Params,true,$ErrorCode);
		if($this->logoptions & LOG_OPT_THROW_ON_ERROR)throw new RpcErrorHandler($Message,$ErrorCode);
		if($this->logoptions & LOG_OPT_EXIT_ON_ERROR)exit('Error Exit');
		return null;
	}
	/** @brief Write a debug message to log
	 * @param int $LogOption @ref debug_options to validate before output
	 * @param int|string $Message Message- Number to translate with RpcMessage or String to output
	 * @param null|array $Params Once or more Params to replace in Message. Default=null
	 * @return null Returns always null
	 * 
	 * <b>Demo:</b><br>
	 * <code>
	 * $logger->Debug(DEBUG_INFO,'This is a error');<br>
	 * $logger->Debug(DEBUG_ERRORS ,'name %s has error ','The Name');<br> 
	 * $logger->Debug(DEBUG_CALL,ERR_FunctionNotFound,'Function Name');<br>
	 * $logger->Debug(DEBUG_INFO + DEBUG_DETAIL,'%s and %s are %s its %s',1,2,3,'cool');<br>
	 * </code>
	 * @see @ref options_logger
	 * @see RpcLogger::SetLogOptions
	 */
	public function Debug($LogOption, $Message, $Params=null /* ... */){
		if(($this->logoptions & $LogOption) != $LogOption)return;
		$this->_runtimeDebugName='';
		foreach(ALL_DEBUG as $opt)if($LogOption & $opt){$this->_runtimeDebugName=NAMES_DEBUG[$opt];break;}
		if($Params && !is_array($Params))$Params=array_slice(func_get_args(),2);
		return $this->_log($Message, $Params);
	}
	/** @brief Attach Object to current classnames holder
	 * @param object $Object The Object add to $classnames 
	 * @return object Pointer of self
	 */
	public function Attach($Object){
		if(!in_array($n=get_class($Object), $this->classnames))$this->classnames[]=$n;
		return $this;
	}
	/** @brief Remove Object from current classnames holder
	 * @param object $Object The Object to remove from $classnames
	 */
	public function Detach($Object){
		if(($pos=array_search(get_class($Object), $this->classnames))!==false)unset($this->classnames[$pos]);
	}
	/** @brief Returns current error state
	 * @param int $ErrorID Error number to check. Default=0
	 * @return boolean True if logger in ErrorState
	 */
	public function HasError($ErrorID=0){ 
		if(!$ErrorID) return !is_null($this->errors);
		foreach($this->errors as $e)if($e[1]==$ErrorID)return true;
		return false;
	}
	/** @brief Get the last error code
	 * @return int the last Errorcode or empty if no error set
	 */
	public function LastErrorCode(){ 
		if(!$e=$this->errors)return 0;
		$e=array_pop($e);
		return $e[1];
	}	
	/** @brief Get the last error message
	 * @return string the last Errormessage or empty if no error set
	 */
	public function LastErrorMessage(){
		if(!$e=$this->errors)return '';
		$e=array_pop($e);
		return $e[0];
	}
	/** @brief Get Info of stored errors
	 * @param bool $clearError if True the errors cleared after this call
	 * @param bool $plain if true then return the errors as array
	 * @return array|string|null  Wanted value or null if no error set
	 */
	public function GetError(bool $clearError=null, $plain=false){
		if(is_null($this->errors))return $plain?[]:'';
		if($plain)return $this->errors;
		foreach($this->errors as $e)$error[]="({$e[1]}) ".$e[0];
		if(is_null($clearError)||$clearError!==false)$this->errors=null;
		return implode("\n",$error);
	}
	/** @brief Merge errors from given Logger  
	 * @param object $Logger Class of RpcLogger
	 */
	public function Merge(RpcLogger $Logger){
		if(!$errors=$Logger->GetError(true,true))return;
		$this->errors=empty($this->errors)?$errors:array_merge($this->errors,$errors);
	}
/// @cond PROTECTED
	/** @brief Output message to screen
	 * 
	 * This function outputs the error to screen. You can ovverride this Method to make your own outputs
	 * @param string $Message the Message of error
	 * @param string $Class the class of error
	 * @param bool $AsError true if Message is a Error message
	 */
	protected function doOutput($Message,$Class,$AsError){
		echo "$Message\n";
	}
/// @endcond
/// @cond PRIVATE	
	/** @brief Logs an message
 	 * @param int|string $Message MessageNumber or Message to output
	 * @param null|array $Params Params to print out
	 * @param bool $AsError default=false
	 * @param null|int $Code the error code. default=null
	 * @param string $Class the ErrorClass default=''
	 */
	private function _log($Message, array $Params=null,$AsError=false, $Code=null, $Class=''){
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
/// @endcond
}
/** @brief Inferface to Attach and Detach RpcLogger
 * 
 * This interface ensures that a consistent implementation of the RpcLogger is possible
 * @author Xaver Bauer
 * @date 21.01.2018
 * @version 2.0.1
 * Added PhpDoc Tags
 */
interface iRpcLogger {
	/** @brief Link a Logger
	 * @param RpcLogger $Logger The logger to attach
	 * @return RpcLogger The same as $Logger
	 */
	function AttachLogger(RpcLogger $Logger=null);
	/** @brief Unlink a Logger
	 * @param RpcLogger $Logger The logger to detach
	 */
	function DetachLogger(RpcLogger $Logger=null);
}


?>