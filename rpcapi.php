<?php
/** @file rpcapi.php
 * @brief PHP Class to manage RPC/UPNP Devices
 * 
 * PHP Class to manage RPC/UPNP Devices
 * - create devices
 * - call devices
 * - predefine your own functions for devices
 * - Handles Player, TV Samsung, Sony AVR, Dreambox, Plex, IP-Symcon, Fritz!box
 * - Support for sending remote KEY codes

@author Xaver Bauer
@date 02.04.2018
@version 2.1.4
@package rpcapi
@brief Class to manage RPC/UPNP Devices
@copydetails rpcapi.php
*/
/** @page getting_started  Getting started 
*/
/** @page version_history API Version History
@date 02.04.2018
@version 2.1.4
- Fix Error loading file without dirname. Now looks in RPC_CONFIG_DIR to found file
- Fix Returnresult of GetFilename returns only Filename when no filename given at load and missing config is found in RPC_CONFIG_DIR
@date 25.03.2018
@version 2.1.3
- to save space in config exclude descriptions in seperate file
- add RpcApi::SetServiceDesc
- add RpcApi::SetFunctionDesc
- add RpcApi::SetParamDesc
@date 19.03.2018 
@version 2.1.2
- add RpcApi::GetInfo @copybrief RpcApi::GetInfo
- add param \b $HelpWidht to RpcApi::Help
- add Help for internal functions 
@date 16.02.2018
@version 2.1.1
- add DEF_PATH to device Config .. holds the device description path
- add RPC_CACHE_PATH
- change ImportRpcDevice Parameter UseCache added
@date 12.02.2018
@version 2.1.0
- add RpcApi::GetIcon @copybrief RpcApi::GetIcon
- add RpcApi::GetIconImageTag @copybrief RpcApi::GetIconImageTag
- add RpcApi::HasProps @copybrief RpcApi::HasProps
@date 23.01.2018
@version 2.0.9
- add doc comments

 */

require_once 'rpcinit.inc';
require_once 'rpcclasses.php';
require_once 'rpcconstants.inc';
require_once 'rpcmessages.inc';
require_once 'rpcmessage.php';
require_once 'rpclogger.php';
require_once 'rpcconnections.php';



/** @brief Exception Error Handler 
 * 
 * Error Handler Exception
 * @subpackage RpcErrorHandler
 */
class RpcErrorHandler extends Exception {
 	public static function CatchError($ErrLevel, $ErrMessage) {
  		if($ErrLevel != 0){
			throw new RpcErrorHandler("$ErrMessage  in  source code",$ErrLevel);
		}
		return false;
	}

}
/** @brief Class to control RPC/UPNP Devices
 * 
 * Main PHP Class to manage RPC/UPNP Devices
 * 
 * @subpackage RpcApi
 */
class RpcApi implements iRpcLogger, JsonSerializable {
/// @cond VARIABLES
	/// @cond VPRIVATE
	/**
 	* @param null|array Runtime configuration of service functions 
    */
	private $_device=null;
   /**
 	* @param string Loaded or imported configuration filename 
    */
	private $_fileName='';
   /**
 	* @param RpcLogger Error and Debug handler object
    */
	private $_logger = null;
   /**
 	* @param bool true if api has errors
    */
	private $_error    = false;
   /**
 	* @param bool True if device online  
    */
	private $_isOnline = null;
   /**
 	* @param RPCConnection Runtime connection object  
    */
	private $_connection=null;
   /**
 	* @param bool True if a new device imported  
    */
	private $_importDone=false;
   /**
 	* @param array Static variable holder used in user functions from inject with source code 
    */
	private $_static = [];
	/// @endcond
/// @endcond
	/** @brief Create new Object
	 * 
	 * Create a new RpcApi Object from given Source.<br>
	 * @param string $Source Json config filename or full Url to device description xml
	 * @param RpcLogger $Logger RPC Logger Object
	 * @see @ref load_api
 	 */
	function __construct($Source='',RpcLogger $Logger=null){		
		if($Logger)$this->AttachLogger($Logger);
		if($Source)$this->Load($Source);
	}
/// @cond FPRIVATE	
	/** @brief Destroy API
	 * 
	 * do the following
	 * - save the descriptions file if changed
	 * - remove RpcLogger object but dont destroy it
	 * - delete this API from memory  
	 */
	function __destruct(){
		$this->DetachLogger($this->_logger);
		if(RPC_EXTERNAL_DESCRIPTIONS && function_exists('RpcDescriptionsSave'))RpcDescriptionsSave();
	}
	/** @brief Standard service function call
	 * 
	 * @param string $FunctionName Name of Remote function to call 
	 * @param array $Arguments If needed the @ref how_to_args
	 * @retval null|mixed The return value of the called function or null if an Error detected
	 * @see @ref how_to_call or @ref how_to_args 	 
	 * @see RpcApi::HasError
	 * @see @link Api_Call_Example.php
	 */
	function __call($FunctionName, $Arguments){
 		if(count($Arguments)==1 && !empty($Arguments[0]) && is_array($Arguments[0]))$Arguments=$Arguments[0]; // for Calls [Functionname](array params)
 		$this->_error=false;
		$this->debug(DEBUG_CALL,sprintf('Method %s(%s)',$FunctionName, DEBUG::as_array($Arguments)));
 		if(empty($this->_device))return $this->error(ERR_DeviceNotLoaded);
 		if(empty($this->_device->{DEVICE_CONFIG}->{CONFIG_HOST}))return $this->error(ERR_HostNameNotSet);
		if(!$service=$this->_findFunctionService($FunctionName)) return $this->error(ERR_FunctionNotFound,$FunctionName);
		if(!$this->IsOnline())return $this->error(ERR_DeviceNotOnline);
		$filter=null; $args=[];
		$function=&$service->{SERVICE_FUNCTIONS};					
		if(!empty($function->{FUNCTION_PARAMS}->{PARAMS_IN})){
			$values=$this->_createFunctionValues($function->{FUNCTION_PARAMS}->{PARAMS_IN},$Arguments );
			if($this->_hasOption(OPTION_CHECK_VALUES))foreach ($function->{FUNCTION_PARAMS}->{PARAMS_IN} as $param){
				$vname=$param->{VALUE_NAME};
				if(is_string($values[$vname]) && $values[$vname]=='<CLEAR>'){unset($values[$vname]);	continue;}
				if(!$this->_validateValue($param, $values[$vname]))return null;
				$args[$vname]=$values[$vname];
			}else{
				foreach($values as $key=>$value)if(is_string($value) && $value=='<CLEAR>')unset($values[$key]);
				$args=$values;
			}
		}
		$Arguments=$args;
		if(!empty($service->{SERVICE_FILTER}))$filter[FILTER_PATTERN_REMOVE]=$service->{SERVICE_FILTER};
		if(!empty($function->{FUNCTION_PARAMS}->{PARAMS_OUT})&& $this->_hasOption(OPTION_RESULT_FILTER)){
			foreach ($function->{FUNCTION_PARAMS}->{PARAMS_OUT} as $param)$filter[]=$param->{VALUE_NAME};
		}else $filter[]='*';
		$result=empty($function->{FUNCTION_SOURCE})?$this->_callConnection($service, $Arguments, $filter):$this->_callSource($service, $Arguments, $filter);
		if(is_null($result)&&!$this->HasError())$result=true;
		if(!$this->HasError())$this->debug(DEBUG_CALL,sprintf('Method %s returns %s',$FunctionName,DEBUG::export($result)));
		return $result;
	}
	function __wakeup(){
// 		echo __CLASS__ . " => WAKEUP\n"; 	
	}
	function __sleep(){
// 		echo __CLASS__ . " => SLEEP\n";
		return ['_device','_fileName','_logger'];
	}
	function jsonSerialize (){
		return [$this->_device];
	}
/// @endcond
	/** @brief Checks if device online
	 * 
	 * @return bool true if device online or false if not
	 */
	public function IsOnline(){
		if(!is_null($this->_isOnline))return $this->_isOnline;
		if(empty($this->_device->{DEVICE_CONFIG}->{CONFIG_HOST}))return false;
		return $this->_isOnline=(bool)NET::ping($this->_device->{DEVICE_CONFIG}->{CONFIG_HOST})!==false;
	}
	/** @brief Assign given RpcLogger
	 * 
	 * {@inheritDoc}
	 * @see iRpcLogger::AttachLogger
	 */
	public function AttachLogger(RpcLogger $Logger=null){
		if($Logger)$this->_logger=$Logger->Attach($this);
	}
	/** @brief Remove given RpcLogger
	 *  
	 * {@inheritDoc}
	 * @see iRpcLogger::DetachLogger
	 */
	public function DetachLogger(RpcLogger $Logger=null){
		if($Logger && $Logger != $this->_logger )return;
		$this->_logger=$Logger?$Logger->Detach($this):$Logger;
	}
	/** @brief Load device data
	 * 
	 * Load or Import device. If Source extention is .json then load config when not import config
	 * @param string $Source Json config filename or full Url to device description xml
	 * @return bool True if load successfully otherwise false
	 * @see @ref load_api and @link Api_Call_Example.php
	 */
	public function Load($Source){
		if(!preg_match('/\.json/i',$Source))
			return $this->_import($Source);
		else  
			return $this->_load($Source);
	}
	/** @brief Save the curent device
	 * 
	 * Save the current device to loaded filename. If device imported then the filename created automatic
	 * @return bool true if save ok    
	 */
	public function Save(){
		return $this->_save($this->_fileName);
	}
	/** @brief Returns the current filename
	 * 
	 * @return string The current filename if loaded or imported when not returns empty string
	 */
	public function GetFilename(){
		return $this->_fileName;
	}
	/** @brief Checks if device imported
	 * 
	 * Returns true if the device is imported
	 * @return boolean
	 */
	public function IsImported(){
		return $this->_importDone;
	}
	/** @brief Checks if device has errors
	 * 
	 * This function returns the current state of RpcApi. If true then Api has Errors. You can get the ApiErrorState in two ways.
	 * -# Call this function
	 * -# Call assigned RpcLogger
	 * <br><br>To get the current Error(s) you have to call the linked RpcLogger 
	 * @param int $ErrorNo Empty or number of error to check
	 * @retval bool Returns true if api has errors
	 * @see RpcLogger::LastErrorMessage
	 */
	public function HasError($ErrorNo=0){
		return $this->_error || ($this->_logger && $this->_logger->HasError($ErrorNo));
	}
	/** @brief Set device options
	 * 
	 * Set the device Options 
	 * @param int $Options Combined flags of @ref options_device
	 * @param string $mode Valid modes are "set" (default), "add" or "del"
	 */
	public function SetOptions($Options, $mode='set'){
		$o=&$this->_device->{DEVICE_CONFIG}->{CONFIG_OPTIONS};
		if($Options<0){$mode='del';$Options=abs($Options);}
		switch($mode){
			case 'set' : $o=$Options;break;
			case 'add' : $o=$o | $Options;break;
			case 'del' : $o-=($o&$Options);
		}
	}
	/** @brief Set api options
	 * 
	 * Set the Api options
	 * @param int $Options Combined flags of @ref options_api
	 * @param string $mode Valid modes are "set" (default), "add" or "del"
	 */
	public function SetApiOptions($Options, $mode='set'){
		$o=&$this->_device->{DEVICE_DEF}->{DEF_OPTIONS};
		if($Options<0){$mode='del';$Options=abs($Options);}
		switch($mode){
			case 'set' : $o=$Options;break;
			case 'add' : $o=$o | $Options;break;
			case 'del' : $o-=($o&$Options);
		}
	}
	/** @brief Set login information
	 * 
	 * Set current login informations for device calls
	 * @param string $Username Username for remote connection
	 * @param string $Password Password for remote connection
	 */
	public function SetCreditials($Username, $Password){
		if(empty($this->_device))return $this->error(ERR_DeviceNotLoaded);
		$this->_device->{DEVICE_CONFIG}->{CONFIG_LOGIN_U}=$Username;
		$this->_device->{DEVICE_CONFIG}->{CONFIG_LOGIN_P}=$Password;
	}
	/** @brief Checks if a function exists
	 * 
	 * @param string $FunctionName Functionname to search for
	 * @return boolean Returns true if FunctionName exists
	 * @see RpcApi::GetFunction and RpcApi::GetServiceNames
	 */
	public function FunctionExists($FunctionName){
		return (bool)$this->GetFunction($FunctionName);
	}
	/** @brief Get assigned RpcLogger
	 * 
	 * This function does not DetachLogger from this class
	 * @return object asiggned RpcLogger
	 */
	public function GetLogger(){
		return $this->_logger;
	}
	/** @brief Get model info
	 * 
	 * @return null|object The current @ref class_def or null if device not loaded
	 * @note Changes of the returned @ref class_def has no effect in this device config
	 */
	public function GetModelDef(){
		return empty($this->_device->{DEVICE_DEF})?null: clone $this->_device->{DEVICE_DEF};
	}
	/** @brief Get device config
	 * 
	 * @return null|object @ref class_config or null if not loaded
	 * @attention Changes of the returned @ref class_config has full effect in this device
	 */
	public function GetConfig(){
		return empty($this->_device->{DEVICE_CONFIG})?null:$this->_device->{DEVICE_CONFIG};
	}
	/** @brief Get icon from loaded device
	 * 
	 * This function returns @ref class_icon for all or the one specified in index
	 * @param int $Index The index of Icon. Default all icons 
	 * @return null|array A List of @ref class_icon or null if not found
	 */
	public function GetIcon(int $Index=null){
		if(empty($this->_device->{DEVICE_ICONS}))return null;
		if(is_null($Index)){
			$icons=$this->_device->{DEVICE_ICONS};
			foreach($icons as &$icon){
				$icon = clone $icon;	
				$icon->{ICON_URL}=$this->_deviceUrl().$icon->{ICON_URL};
			}
		}else if(!empty($this->_device->{DEVICE_ICONS}[$Index])){
			$icon=clone $this->_device->{DEVICE_ICONS}[$Index];
			$icon->{ICON_URL}=$this->_deviceUrl().$icons->{ICON_URL};
			$icons=[$icon];
		}else $icons=null;
		return $icons;	
	}
	/** @brief Get icon as html img tag
	 *    
	 * This function returns an HTML image tag for all or the one specified in index<br>&lt;img src=&quot;ICON_URL&quot; width=&quot;ICON_WIDTH&quot; height=&quot;ICON_HEIGHT&quot;&gt;
	 * @param int $Index The index of Icon . Default=null
	 * @return null|string|string[] The html img(s) Tag 
	 */
	public function GetIconImageTag(int $Index=null){
		if(!$icons=$this->GetIcon($Index))return null;
		if(!is_null($Index))$icons=[$icons];
		foreach($icons as &$f)$f='<img src="'.$f->{ICON_URL}.'" width="'.$f->{ICON_WIDTH}.'" height="'.$f->{ICON_HEIGHT}.'">';
		return count($icons)==1?$icons[0]:$icons;
	}
	/** @brief Detailed device informations
	 * 
	 * @return null|object detailed @ref class_info 
	 */
	public function GetInfo(){
		if(empty($this->_device->{DEVICE_INFO}))return null;
		return clone $this->_device->{DEVICE_INFO};
	}
	/** @brief Check of props included
	 * 
	 * This function checks if device has property of given @ref options_props
	 * @param int $Props The @ref options_props to validate
	 * @return bool True if device has one of $Props
	 */
	public function HasProps(int $Props){
		if(empty($this->_device->{DEVICE_DEF}->{DEF_PROPS}))return false;
		return ($this->_device->{DEVICE_DEF}->{DEF_PROPS} & $Props);
	}
	/** @brief Get all service names
	 * 
	 * @retval null|string[] List with all service names
	 * if the return is not null then the array contains a string list of all registered service names  
	 */
	public function GetServiceNames(){
		return empty($this->_device)?null:array_keys(get_object_vars($this->_device->{DEVICE_SERVICES}));
	}
	/** @brief Get registerd service class
	 * 
	 * @param string $ServiceName Service name to found
	 * @return null|object The @ref class_service or null if not found
	 * 
	 */
	public function GetService($ServiceName){
		return (empty($this->_device->{DEVICE_SERVICES}->$ServiceName))?$this->error(ERR_ServiceNotFound,$ServiceName):$this->_cloneService($this->_device->{DEVICE_SERVICES}->$ServiceName);
	}
	/** @brief Get list of registered functions
	 * 
	 * if $IncludeServiceNames is true then returns a KeyValuePair array [ServiceName]=FunctionNames when not get a simple string array with FunctionNames ['name1','name2'...]<br>
	 * if no Param given then returns a list of all registered device functions.
	 * @param string $ServiceName if set only functions from this service returned. Default=''
	 * @param bool $IncludeServiceName if set you get . Default=false
	 * @return null|array null if no service or function found
	 */
	public function GetFunctionNames($ServiceName='', $IncludeServiceName=false){
		if($ServiceName){
			if(empty($this->_device->{DEVICE_SERVICES}->$ServiceName))return $this->error(ERR_ServiceNotFound,$ServiceName);
			$r=array_keys(get_object_vars($this->_device->{DEVICE_SERVICES}->$ServiceName->{SERVICE_FUNCTIONS}));
			return $IncludeServiceName? [$ServiceName=>$r]:$r;
		}
		$return=[];
		foreach($this->_device->{DEVICE_SERVICES} as $sn=>$service){
			$r=array_keys(get_object_vars($service->{SERVICE_FUNCTIONS}));
			if($IncludeServiceName)$return[$sn]=$r;else $return=array_merge($return,$r);
		}
		return $return;
	}
	/** @brief Get registered function
	 *    
	 * @param string $FunctionName Name of function to found
	 * @param string $ServiceName if set $FunctionName must exists in service $ServiceName 
	 * @return null|object The @ref class_function or null if not found
	 * 
	 */
	public function GetFunction($FunctionName, $ServiceName=null){
		return $this->_findFunctionService($FunctionName,$ServiceName);
	}
	/** @brief Get registered event variables
	 * 
	 * @param string $ServiceName if set only EventVars from given service returned
	 * @param bool $IncludeNames true or false
	 * @return null|array List of EventVar names
	 * if \b $ServiceName empty
	 * @snippet snippets.inc function_GetFunction_without_service
	 * if \b $IncludeNames = \c true
	 * @snippet snippets.inc function_GetFunction_with_names
	 */
	public function GetEventVars($ServiceName='', $IncludeNames=false){
		if($ServiceName){
			if(!$service=$this->GetService($ServiceName)) return null;
			if(empty($service->{SERVICE_EVENTS}))return $this->error(ERR_ServiceHasNoEvents,$ServiceName);
			$r=array_keys(get_object_vars($service->{SERVICE_EVENTS}));
			return $IncludeNames ? [$ServiceName=>$r]: $r; 
		}
		$return=[];
		foreach($this->_device->{DEVICE_SERVICES} as $sn=>$service)if(!empty($service->{SERVICE_EVENTS})){
				$r=array_keys(get_object_vars($service->{SERVICE_EVENTS}));
				if($IncludeNames)$return[$sn]=$r; else $return=array_merge($return,$r);  
		}
		return $return;
	}
  	/** @brief Register event
  	 * 
  	 * Register one or all device Events. If no events present then return false;
  	 * @param string $ServiceName if set register only events from $ServiceName
  	 * @param string $CallbackUrl Url for processing event messages 
  	 * @param int $RunTimeSec Lifetime of registred events  default=0 (auto)
  	 * @return false|array if true returns @ref struct_event
  	 * @see RpcApi::UnregisterEvent , RpcApi::RefreshEvent
  	 */
  	public function RegisterEvent($ServiceName, $CallbackUrl, $RunTimeSec=0){
  		
  		return ($r=$this->RegisterEvents($ServiceName, $CallbackUrl,$RunTimeSec))?$r[0]:false;
  	}
  	/** @brief Register events
  	 * 
  	 * The only diferent to RegisterEvent is that this mehtod return an array of results from RegisterEvent
  	 * @param string $ServiceName if set register only events from $ServiceName
  	 * @param string $CallbackUrl Url for processing event messages 
  	 * @param int $RunTimeSec Lifetime of registred events  default=0 (auto)
  	 * @return bool|array if true returns list of @ref struct_event
  	 * @see RpcApi::UnregisterEvents , RpcApi::RefreshEvents
  	 */
  	public function RegisterEvents($ServiceName, $CallbackUrl, $RunTimeSec=0){
 		if(!preg_match('/^http/i',$CallbackUrl))return $this->error(ERR_InvalidCallbackUrl,$CallbackUrl);
  		$this->_error=false;
   		$events=$services=null;
  		if(empty($ServiceName)){
 			foreach($this->_device->{DEVICE_SERVICES} as $service)
 				if(!empty($service->{SERVICE_EVENTS}))$services[]=$service;
  		}elseif(!empty($this->_device->{DEVICE_SERVICES}->$ServiceName)){
 			if(empty($this->_device->{DEVICE_SERVICES}->$ServiceName->{SERVICE_EVENTS}))
 				return $this->error(ERR_ServiceHasNoEvents,$ServiceName);
 			$services[]=$this->_device->{DEVICE_SERVICES}->$ServiceName;
 		}else return $this->error(ERR_ServiceNotFound, $ServiceName); 
 		if(empty($services)) return $this->error(ERR_NoEventServiceFound);	
  		foreach($services as $service){
			$events[]=[EVENT_SID=>'',EVENT_TIMEOUT=>$RunTimeSec,EVENT_SERVICE=>$service->{SERVICE_NAME}];		
		}
		return $this->_sendEvents($events,$CallbackUrl,$RunTimeSec)?$events:null;
	}
	/**
	 * @param string $SID The SID to refresh
	 * @param string $Service The associated servicename
	 * @param int $RunTimeSec Lifetime of sid. Default=0 (auto)
	 * @return bool|int The new Livetime in seconds or false on error
 	 * @see RpcApi::RegisterEvent , RpcApi::UnregisterEvent
 	 */
	public function RefreshEvent($SID, $Service, $RunTimeSec=0){
		$r=[EVENT_SID=>$SID,EVENT_TIMEOUT=>$RunTimeSec,EVENT_SERVICE=>$Service];
		if($this->_sendEvents($r,null,$RunTimeSec)){
			return $r[EVENT_TIMEOUT];			
		}else return false;
	}
 	/**
 	 * Same as RefreshEvent but needs an array of events as parameter.  
 	 * @param array $EventArray List of events @ref struct_event to be refreshed
 	 * @retval bool True if no error
 	 * @see RpcApi::RegisterEvents , RpcApi::UnregisterEvents
 	 */
 	public function RefreshEvents(array &$EventArray){
 		$this->_error=false;
		return $this->_sendEvents($EventArray,null,$RunTimeSec);
 	}
	/** @brief Deletes an event
	 * @param string $SID The SID to delete
	 * @param string $Service The associated servicename
	 * @return bool True if event deleted successfully
   	 * @see RpcApi::RegisterEvent ,RpcApi::RefreshEvent
	 */
	public function UnregisterEvent($SID, $Service){
		$r=[EVENT_SID=>$SID,EVENT_TIMEOUT=>0,EVENT_SERVICE=>$Service];
		return $this->_sendEvents($r,true);
	}
 	/** @brief Unregister service events 
 	 * 
 	 * Delete all event in given events @ref struct_event. 
 	 * @param array $EventArray List of events @struct_event to be deleted
 	 * @return bool True if all events deleted successfully
 	 * @see RpcApi::RegisterEvents , RpcApi::RefreshEvents
 	 */
 	public function UnregisterEvents(&$EventArray) {
 		$this->_error=false;
		return $this->_sendEvents($EventArray,true);
 	}
	/** @brief Get help for functions
	 * 
	 * This method provides help information about all registered functions.
	 * - If a name is specified and this function exists then the result returns only to this information.
	 * - If no name is specified then all functions are listed
	 * - If # as name given then returns only API functions
	 * @param string $FunctionName function name you need help with or empty for everyone
	 * @param int $HelpMode once of HELP_SHORT (default), HELP_NORMAL or HELP_FULL 
	 * @param boolean $ReturnResult if true method returns the result as string when not echo the result
	 * @param int $HelpWidht the max widht of created Help (only $HelpMode != HELP_SHORT)
	 * @return string|null if $returnResult then return null
	 */
	public function Help($FunctionName='', $HelpMode= HELP_SHORT, $ReturnResult=false, $HelpWidht=80){
		require_once 'rpchelp.inc';
		$help=[];
		if(!empty($FunctionName) && ($FunctionName=='#' || method_exists($this, $FunctionName))){
			if($FunctionName=='#')$FunctionName='';
			$help=CreateInternalHelp($FunctionName,'RpcApi',$HelpMode, $HelpWidht);
		}else if(empty($FunctionName)){
			$help=CreateInternalHelp('','RpcApi',$HelpMode, $HelpWidht);
			if($HelpMode!=HELP_FULL)array_unshift($help,"--- RpcApi functions ---");
			foreach($this->_device->{DEVICE_SERVICES} as $serviceName=>$service){
		
				if($HelpMode!=HELP_FULL)$help[]="--- Service $serviceName functions ---";
				$fhelp=[];
				foreach($service->{SERVICE_FUNCTIONS} as $FunctionName=>$function){
					$values=empty($function->{FUNCTION_PARAMS}->{PARAMS_IN})?null:$this->_createFunctionValues($function->{FUNCTION_PARAMS}->{PARAMS_IN});
					$fhelp[$FunctionName] = CreateHelp($function,$FunctionName, $values, $HelpMode,$serviceName,$HelpWidht, @$service->{SERVICE_DESC});	
				}
				ksort($fhelp);
				foreach($fhelp as $h)$help=array_merge($help,$h);
			}
		}elseif($function=$this->_findFunctionService($FunctionName)){
			$values=empty($function->{FUNCTION_PARAMS}->{PARAMS_IN})?null:$this->_createFunctionValues($function->{FUNCTION_PARAMS}->{PARAMS_IN});
			$help = CreateHelp($function->{SERVICE_FUNCTIONS},$FunctionName,$values,$HelpMode, $function->{SERVICE_NAME},$HelpWidht, @$function->{SERVICE_DESC});
		}
		if($ReturnResult) return implode("\n",$help);
		echo implode("\n",$help)."\n";
	}
	/** @brief Information about playing Track
	 *  
	 * This method provides information about the currently playing movie, TV station or music track
	 * @param int $InstanceID Device instanceid. Default=0
	 * @return null|array if PositionInfo supported then returns @ref struct_track
	 */
	public function GetCurrentInfo($InstanceID=0){
		if(!$info = $this->GetPositionInfo($InstanceID))return null;
		if(preg_match('/<item .*>(.*)<\/item>/',$info['TrackMetaData'],$m))$info['TrackMetaData']=$m[0];
		$info['TrackMetaData']=str_replace(['r:','dc:','upnp:'],'' , $info['TrackMetaData']);
		if(!$xml=@simplexml_load_string('<?xml version="1.0"?>'.$info['TrackMetaData']))return null;
		$track=(array)$xml;
		$track = $track['@attributes'] + $track;
		if(isset($track['streamContent'])){
			$track['streamContent']=(array)$track['streamContent'];
		}
		unset($track['@attributes'],$track['class'],$track['streamContent']);
		$track['duration']=$info['TrackDuration'];
		$track['relTime']=$info['RelTime'];
		$track['track']=$info['Track'];
		foreach ($track as $key=>&$v) if(is_numeric($v))if(is_float($v))$v=floatval($v);else $v=intval($v);elseif($v=='true')$v=true;elseif($v=='false')$v=false;
		return $track;
	} 	
#TODO Add Desc for functions
	public function SetServiceDesc($ServiceName, $Description){
		require_once 'rpcdescriptions.inc';
		return RpcDescriptionDeviceSet($this->_device, $Description,$ServiceName);
	}
	public function SetFunctionDesc($FunctionName, $Description, $ServiceName=''){
		require_once 'rpcdescriptions.inc';
		return RpcDescriptionDeviceSet($this->_device, $Description, $ServiceName,$FunctionName);
	}
	public function SetParamDesc($ParamName, $Description,$ParamsType=0, $FunctionName='', $ServiceName=''){
		require_once 'rpcdescriptions.inc';
		return RpcDescriptionDeviceSet($this->_device, $Description, $ServiceName,$FunctionName,$ParamName);
	}
/// @cond PROTECTED
	/** @brief Logs an error
	 *  
	 * @param int|string $Message Message- Number to translate with RpcMessage or String to output
	 * @param mixed $ErrorCode ErrorCode or Param to output. Default=null
	 * @param mixed $Params Once or more Params to replace in Message. Default=null
	 * @return NULL
	 * 	
	 * <b>Demo:</b><br>
	\code
$this->error('This is a simple error'); // logs 'This is a simple error'
$this->error('name %s has error ','The Name'); // logs 'name The Name has error' 
$this->error(ERR_FunctionNotFound,'Function Name'); // logs translated id ERR_FunctionNotFound 
$this->error('%s and %s are %s its %s',1,2,3,'cool'); // logs '1 and 2 are 3 its cool'
	\endcode
	 */
	protected function error($Message, $ErrorCode=null, $Params=null /* ... */){
		if(!$this->_logger){ $this->_error=true; return null;}
		if(is_numeric($Message))$Params=array_slice(func_get_args(),1);
		elseif($Params && !is_array($Params))$Params=array_slice(func_get_args(),2);
		$this->_error=true;
		return $this->_logger->Error($ErrorCode, $Message, $Params);
	}
	/** Logs an debug message
	 * @param int $DebugOption Once ore more of @ref options_logger to compare
	 * @param int|string $Message Message- Number to translate with RpcMessage or String to output
	 * @param mixed $Params Once or more Params to replace in Message. Default=null
	 */
	protected function debug($DebugOption, $Message, $Params=null /* ... */){
		if(!$this->_logger)return;
		if($Params && !is_array($Params))$Params=array_slice(func_get_args(),2);
		$this->_logger->Debug($DebugOption, $Message, $Params);
	}
 	/** @brief Create a connection object
 	 * 
	 * @param int $ConnectionType The @ref types_connection type to create
	 * @return null|object an object of the base class RpcConnection depending on the passed parameter $ConnectionType
	 */
	protected function getConnection($ConnectionType=CONNECTION_TYPE_SOAP){
		if($this->_connection && $this->_connection->ConnectionType()==$ConnectionType){
			$this->_connection->AttachLogger($this->_logger);
			return $this->_connection;
		}
		$this->_connection=null;
		$creditials=[$this->_device->{DEVICE_CONFIG}->{CONFIG_LOGIN_U},$this->_device->{DEVICE_CONFIG}->{CONFIG_LOGIN_P},''];
		$this->debug(DEBUG_CALL, 'Open new '.NAMES_CONNECTION_TYPE[$ConnectionType].' connection to '.$this->_deviceUrl(),201);
		switch($ConnectionType){
			case CONNECTION_TYPE_SOAP: $this->_connection=new RpcSoapConnection($creditials,$this->_logger);break;
			case CONNECTION_TYPE_JSON: $this->_connection=new RpcJSonConnection($creditials,$this->_logger);break;
			case CONNECTION_TYPE_URL : $this->_connection=new RpcUrlConnection ($creditials,$this->_logger);break;
			case CONNECTION_TYPE_XML : $this->_connection=new RpcXMLConnection($creditials,$this->_logger);break;
		}
		return $this->_connection;
	}
/// @endcond
/// @cond PRIVATE	
	/** @brief Eval function source
	 * 
	 * call source of an user service function
	 * @param object $Service Service @ref class_service
	 * @param array $Arguments	The @ref how_to_args
	 * @param array $Filter The result filter
	 * @return NULL|mixed The result of function call or null if an error occurs
	 */
	private function _callSource(stdClass $Service, $Arguments, $Filter){
		foreach($Arguments as $EXPORT=>$arg)$$EXPORT=&$Arguments[$EXPORT]; // Export Arguments
		if(isset($Service->{SERVICE_FUNCTIONS}->{FUNCTION_EXPORT}) && isset($Service->{SERVICE_EXPORT}))foreach($Service->{SERVICE_FUNCTIONS}->{FUNCTION_EXPORT} as $EXPORT){
			if(empty($Service->{SERVICE_EXPORT}->$EXPORT))return $this->error(ERR_SourceExportFailed,$f->{FUNCTION_NAME},$EXPORT);
			else $$EXPORT = &$Service->{SERVICE_EXPORT}->$EXPORT;
		}
		$__source=str_ireplace(['return '],'return $_R=', $Service->{SERVICE_FUNCTIONS}->{FUNCTION_SOURCE});
		unset($EXPORT,$Service,$Arguments,$Filter,$arg);
		$STATIC= &$this->_static;
		$_R_=null;
		$this->debug(DEBUG_CALL+DEBUG_DETAIL, 'Source: '.debug::export($__source),200);
   		$old_handler = set_error_handler('RpcErrorHandler::CatchError');
		try {	eval($__source);}
		catch(Exception $e){ $_R=$this->error($e->GetMessage(),$e->getCode());}
  		set_error_handler($old_handler);
		return $_R;
	}
	/** @brief Call service with connection
	 * 
	 * call connection with an service function
	 * @param object $Service Service @ref class_service
	 * @param array $Arguments	If needed the @ref how_to_args
	 * @param array $Filter The result filter. Default=null
	 * @return NULL|mixed The result of function call or null if an error occurs
	 */
	private function _callConnection(stdClass $Service, $Arguments, $Filter=null){
		if(!$connection=$this->getConnection($Service->{SERVICE_CONNTYPE}))return $this->error(ERR_CantGetConnection);
		$url=$this->_deviceUrl($Service->{SERVICE_PORT}). $Service->{SERVICE_CTRLURL};
		$result=$connection->execute($url,$Service->{SERVICE_ID}, $Service->{SERVICE_FUNCTIONS}->{FUNCTION_NAME}, $Arguments, $Filter);
		if($this->_logger)$connection->DetachLogger($this->_logger);
		return $result;
	}
	/** @brief Clone a service object
	 * 
	 * this method provides a copy of the selected service.<br>
	 * If the function name is specified then the copy contains only this function<br>
	 * otherwise all functions of the service
	 * @param object $Service @ref service_class 
	 * @param string $FunctionName If set only this function include in service return result
	 * @param bool $Clone if true a clone of service returned
	 * @return NULL|object The copy of @ref service_class or null if service not exists
	 */
	private function _cloneService(stdClass $Service, $FunctionName='', $Clone=true){
		if($FunctionName && empty($Service->{SERVICE_FUNCTIONS}->$FunctionName))return null;
		$r=$Clone?clone $Service:$Service;
		if($FunctionName)$r->{SERVICE_FUNCTIONS}=$Service->{SERVICE_FUNCTIONS}->$FunctionName;
		else unset($r->{SERVICE_FUNCTIONS});
		if(empty($r->{SERVICE_CONNTYPE}))$r->{SERVICE_CONNTYPE}=$this->_device->{DEVICE_CONFIG}->{CONFIG_CONNTYPE};
		if(empty($r->{SERVICE_PORT}))$r->{SERVICE_PORT}=$this->_device->{DEVICE_CONFIG}->{CONFIG_PORT};
		if(empty($r->{SERVICE_HOST}))$r->{SERVICE_HOST}=$this->_device->{DEVICE_CONFIG}->{CONFIG_HOST};
		return $r;
	}
	/** @brief Find a function in services
	 * @param string $functionname Function to find
	 * @param string|null $ServiceName if set search only in this service
	 * @param bool $Clone if true a clone of service returned
	 * @return object|null @ref service_class or null if function not found 
	 */
	private function _findFunctionService($functionname,$ServiceName=null, $Clone=true){
		$result=null;
		if(empty($this->_device))return $this->error(ERR_DeviceNotLoaded);
		if(empty($ServiceName)){
			
			foreach($this->_device->{DEVICE_SERVICES} as $sn=>$service){
				if($result=$this->_cloneService($service,$functionname,$Clone))	
				break;
			}
		}elseif(!empty($this->_device->{DEVICE_SERVICES}->$ServiceName->{SERVICE_FUNCTIONS}->$functionname))
			$result= $this->_cloneService($this->_device->{DEVICE_SERVICES}->$ServiceName,$functionname,$Clone);
		return $result;
	}
	/**
	 * @param int|null $Port If set then use this port in return result
	 * @return string The device baseurl
	 */
	private function _deviceUrl($Port=null){
		if(is_null($Port))$Port=$this->_device->{DEVICE_CONFIG}->{CONFIG_PORT};
		$Port=($Port)?":$Port":'';
		return $this->_device->{DEVICE_CONFIG}->{CONFIG_SCHEME}."://".$this->_device->{DEVICE_CONFIG}->{CONFIG_HOST}.$Port;
	}
	/**
	 * @param int $Option The @ref api_options to find
	 * @return bool True is api has $Option otherwise false 
	 */
	private function _hasOption($Option){
		return (bool)$this->_device->{DEVICE_DEF}->{DEF_OPTIONS} & $Option;
	}
	/**
	 * @param object $Param The @ref function_param_class with which the value is tested
	 * @param mixed $Value The value to check 
	 * @return bool|null False or null if $Value not passed the check
	 */
	private function _validateValue(stdClass $Param, $Value){
		if(is_null($Param))return false;
		if(is_null($Value))return $this->error(ERR_ParamIsEmpty, $Param->{VALUE_NAME});
 		$min=$max=null;
		switch($Param->{VALUE_TYPE}){
			case DATATYPE_BOOL : if(!is_bool($value)) return $this->error(ERR_InvalidParamTypeBool,$Param->{VALUE_NAME}); break;
			case DATATYPE_BYTE : if(!is_int($value))  return $this->error(ERR_InvalidParamTypeUint,$Param->{VALUE_NAME}); $min=0;$max=255;break;
			case DATATYPE_INT  : if(!is_int($value))  return $this->error(ERR_InvalidParamTypeNum,$Param->{VALUE_NAME});  $min=-65535;$max=65535;break;
			case DATATYPE_UINT : if(!is_int($value))  return $this->error(ERR_InvalidParamTypeUint,$Param->{VALUE_NAME}); $min=0;$max=4294836225;break;
			case DATATYPE_FLOAT: if(!is_float($value))return $this->error(ERR_InvalidParamTypeNum,$Param->{VALUE_NAME});  break;
		}
		if(!is_null($min)){
 			if(!empty($Param->{VALUE_MIN} && $Param->{VALUE_MIN}>$min))$min=$Param->{VALUE_MIN}; 				
 		    if(!empty($Param->{VALUE_MAX} && $Param->{VALUE_MAX}<$max))$max=$Param->{VALUE_MIN}; 				
			if($value< $min) return $this->error(ERR_ValueToSmal,$value,$Param->{VALUE_NAME},$min,$max);
			if($value> $max) return $this->error(ERR_ValueToBig,$value,$Param->{VALUE_NAME}, $min,$max);
		}
		if(isset($Param->{VALUE_LIST})){
			foreach($Param->{VALUE_LIST} as $pv)if($ok=$value==$pv)break;
			if(!$ok)return $this->error(ERR_ValueNotAllowed, $value ,$Param->{VALUE_NAME}, implode(', ',$Param->{VALUE_LIST}));
		}
		return true;
	}
	/**
	 * Create @ref function_arguments from given Arguments
	 * @param array $ParamsIn Array of @ref function_param_class
	 * @param array $Arguments Array of @ref function_arguments
	 * @return array Array of @ref function_arguments
	 */
	private function _createFunctionValues(array $ParamsIn, array $Arguments=[]){
		if(is_null($ParamsIn))return [];
		$boNumericKeys = count($Arguments)==0 || is_numeric(key($Arguments));
		$in_first=$in_defaults=$values=[];
		$boUseFirst = $this->_hasOption(OPTION_DEFAULT_TO_END);
		foreach ($ParamsIn as $param)if(isset($param->{VALUE_DEFAULT}) ||!$boUseFirst)$in_defaults[$param->{VALUE_NAME}]=isset($param->{VALUE_DEFAULT})?$param->{VALUE_DEFAULT}:null; else $in_first[]=$param->{VALUE_NAME};
		foreach($in_first as $pn){$values[$pn]=$boNumericKeys?array_shift($Arguments):@$Arguments[$pn];}
		foreach($in_defaults as $pn=>$value){if(is_null($values[$pn]=$boNumericKeys?array_shift($Arguments):@$Arguments[$pn]))$values[$pn]=$value;}
		return $values;
	}
	/**
	 * Clear the completed Api before new load or import 
	 * @param boolean $full Unused 
	 */
	private function _clear($full=true){
		$this->_device=null;
		$this->_fileName='';
		$this->_connection=null;
		$this->_static = [];
		$this->_error = false;
		$this->_isOnline = null;
		if($this->_logger)$this->_logger->GetError(true,true);
	}
	/**
	 * @param string $JsonConfigFileName The config file to load 
	 * @return bool True if device loaded successfully otherwise false
	 */
	private function _load($JsonConfigFileName){
		$p=pathinfo($JsonConfigFileName);
		$emptyDir=empty($p['dirname'])||$p['dirname'][0]=='.';
		if(!file_exists($JsonConfigFileName) ){
			if($emptyDir && file_exists(RPC_CONFIG_DIR.'/'. $JsonConfigFileName))
				$JsonConfigFileName=RPC_CONFIG_DIR.'/'.$JsonConfigFileName;					
			else return $this->error(ERR_FileNotFound,$JsonConfigFileName);
		}
 		if($config=json_decode(file_get_contents($JsonConfigFileName))){
 			UTF8::decode($config);
 			if(empty($config->{DEVICE_DEF}))return (bool)$this->error(ERR_NoRpcConfigFile,$JsonConfigFileName);
 			if(empty($config->{DEVICE_DEF}->{DEF_VERSION})||$config->{DEVICE_DEF}->{DEF_VERSION}!=RPC_API_VERSION)return (bool)$this->error(ERR_ConfigFileVersionMismatch,@$config->{DEVICE_DEF}->{DEF_VERSION},RPC_API_VERSION); 
 			$this->_device=$config;
			$this->_fileName=$emptyDir?pathinfo($JsonConfigFileName)['basename']:$JsonConfigFileName;
			return true;
 		}else return (bool)$this->error(ERR_FileNotFound,$JsonConfigFileName);
 	}
  	/**
  	 * @param string $JsonConfigFileName
  	 * @return bool True if saved successfully 
  	 */
  	private function _save($JsonConfigFileName=''){
		if(empty($this->_device))return $this->error(ERR_NoDataToSave);
		require_once  'rpcimport.inc';
		if($fn=RpcDeviceSave($this->_device,$JsonConfigFileName,$this->_logger))
			$this->_fileName=$fn;
		return (bool)$fn;
  	}
	/**
	 * @param string $UrlToDescXml Url to import / build from
	 * @return bool True if import successfully otherwise false
	 */
	private function _import($UrlToDescXml){
  		require_once 'rpcimport.inc';
  		$this->_device=RpcImportDevice($UrlToDescXml,$this->_logger);
  		return $this->_importDone=($this->_device && !$this->HasError())?$this->_save():false;
	}
	/**
	 * @param array $EventArray Array of @ref struct_event
	 * @param string $CallbackUrl url to call for events 
	 * @param int $RunTimeSec default is automatic
	 * @return null|bool 
	 */
	private function _sendEvents(array &$EventArray, $CallbackUrl,  $RunTimeSec = 0){
		foreach($EventArray as &$event){
 			if(empty($service=@$event[EVENT_SERVICE])) return $this->error(ERR_ServiceNameEmpty);
 			if(empty($service=$this->_cloneService($this->_device->{DEVICE_SERVICES}->$service))) return $this->error(Err_ServiceNotFound,$event->SERVICE);
 			if(empty($eventUrl=$service->{SERVICE_EVENTURL})) return $this->error(ERR_ServiceHasNoEvents,$event->SERVICE);
 	  		if(!$con=$this->getConnection($service->{SERVICE_CONNTYPE})) return $this->error(ERR_CantGetConnection);
  	 		$host = $service->{SERVICE_HOST}.':'.$service->{SERVICE_PORT};
	  		if(empty($RunTimeSec))$RunTimeSec="Infinite";else $RunTimeSec="Second-$RunTimeSec";
	  		if(is_string($CallbackUrl)){ // Register
		  		$mode=1;
	  			$request=$con->CreatePacket('SUBSCRIBE', $eventUrl ,['HOST'=>$host,	'CALLBACK'=>"<$CallbackUrl>", 'NT'=>'upnp:event', 'TIMEOUT'=>$RunTimeSec]);
	  		}elseif($event[EVENT_SID]){
			  	if(is_null($CallbackUrl)){ // Update
	  				$mode=2;
			  		$request=$con->CreatePacket('SUBSCRIBE', $eventUrl ,['HOST'=>$host,	'SID'=>$event[EVENT_SID], 'Subscription-ID'=>$event[EVENT_SID],	'TIMEOUT'=>$RunTimeSec],null);
	  			}elseif(is_bool($CallbackUrl)&& $CallbackUrl===true){ // UnRegister
	  				$mode=3;
	 				$request=$con->CreatePacket('UNSUBSCRIBE', $eventUrl ,['HOST'=>$host, 'SID'=>$event[EVENT_SID], 'Subscription-ID'=>$event[EVENT_SID]],null);
	  			}else continue; //return $this->error();
	  		}else continue;//return $this->error();
	 		$error=false;
	  		$result=$con->SendPacket($host,$request);
	  		
  			$con->DetachLogger($this->_logger);
	  		if($this->HasError())break;
 			if($error=empty($result)){ $this->error(ERR_EmptyResponse);break; }
			if($mode==3){ 
				$event[EVENT_SID]='';$event[EVENT_TIMEOUT]=0;
			}elseif(empty($result['SID'])){
				$this->error(ERR_NoResponseSID);
				break;
			}elseif(!($timeout=$result['TIMEOUT']) || !$timeout=intval(str_ireplace('Second-', '', $timeout))){
				if(is_null($timeout)){
					$this->error(ERR_InvalidTimeouResponse);
					break;
				}
				$timeout=$RunTimeSec>0 ? $RunTimeSec : 300;
			}
			if($mode==1){
				$event[EVENT_SID]=$result['SID'];
				$event[EVENT_TIMEOUT]=$timeout;
				$event[EVENT_SERVICE]=$service->{SERVICE_NAME};
			}elseif($mode==2){
				if($result['SID'] != $event[EVENT_SID]) return $this->error(ERR_NotMySIDReceived,$result['SID'],$event[EVENT_SID]);	
				$event[EVENT_TIMEOUT]=$timeout;
			}
 		}

 		return !$this->_error;
	}
/// @endcond	
	
}



?>