<?php
/**
 * @mainpage
 * PHP Class to manage RPC/UPNP Devices
 * - create devices
 * - call devices
 * - predefine your own functions for devices
 * - Handles Player, TV Samsung, Sony AVR, Dreambox, Plex, IP-Symcon, Fritz!box
 * - Support for sending remote KEY codes  
 * @author Xaver Bauer
 * @date 23.01.2018
 * @version 2.0.9
 */

require_once 'rpcclasses.php';
require_once 'rpcconstants.inc';
require_once 'rpcmessages.inc';
require_once 'rpcmessage.php';
require_once 'rpclogger.php';
require_once 'rpcconnections.php';
if(!function_exists('boolstr')){
	/**
	 * @param bool $bool
	 * @return string
	 */
	function boolstr(bool $bool){
		return $bool?'true':'false';
	}
}
if(!function_exists('mixed2value')){
	/**
	 * @param mixed $mixed Value to be convert
	 * @return boolean|number|string|mixed Converted value
	 */
	function mixed2value($mixed){
		if(is_numeric($mixed))$mixed=is_float($mixed)?floatval($mixed):intval($mixed);
		elseif(strcasecmp($mixed,'true')==0) $mixed=TRUE;
		elseif(strcasecmp($mixed,'false')==0) $mixed=FALSE;
		return $mixed;
	}
}
class RPCErrorHandler extends Exception {
 	public static function CatchError($ErrLevel, $ErrMessage) {
echo "ErrorLevel: $ErrLevel\n";		
 		if($ErrLevel != 0){
			throw new RPCErrorHandler("$ErrMessage  in  source code",$ErrLevel);
		}
		return false;
	}
}

class RPC implements iRpcLogger{
	const my_version="2.37.1";
	private $_device=null;
	private $_fileName='';
	private $_logger = null;
	private $_error    = false;
	private $_isOnline = null;
	private $_connection=null;
	private $_importDone=false;
	private $_static = []; // Used for Functions with source code  
	/**
	 * @param string $Source Json config filename or full Url to device description xml
	 * @param RpcLogger $Logger
	 */
	function __construct($Source=null,RpcLogger $Logger=null){
		if($Logger)$this->AttachLogger($Logger);
		if($Source)$this->Load($Source);
	}
	/**
	 * Remove Logger Object and Delete this object from Memory  
	 */
	function __destruct(){
		$this->DetachLogger($this->_logger);
	}
	/**
	 * @param string $FunctionName Name of Remote function to call 
	 * @param array $Arguments Params for Remote call (value2,value2,..) or ('paramname'=>value [,...] )
	 * @return NULL|boolean|mixed
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
	/** Returns true if device online or false if not
	 * @return boolean
	 */
	public function IsOnline(){
		if(!is_null($this->_isOnline))return $this->_isOnline;
		if(empty($this->_device->{DEVICE_CONFIG}->{CONFIG_HOST}))return false;
		return $this->_isOnline=(bool)NET::ping($this->_device->{DEVICE_CONFIG}->{CONFIG_HOST})!==false;
	}
	/**
	 * {@inheritDoc}
	 * @see iRpcLogger::AttachLogger()
	 */
	public function AttachLogger(RpcLogger $Logger=null){
		if($Logger)$this->_logger=$Logger->Attach($this);
	}
	/**
	 * {@inheritDoc}
	 * @see iRpcLogger::DetachLogger()
	 */
	public function DetachLogger(RpcLogger $Logger=null){
		if($Logger && $Logger != $this->_logger )return;
		$this->_logger=$Logger?$Logger->Detach($this):$Logger;
	}
	/**
	 * Load or Import device. If Source extention is .json then load config when not import config
	 * @param string $Source Json config filename or full Url to device description xml
	 * @return boolean|number|NULL|boolean|NULL
	 */
	public function Load($Source){
		if(!preg_match('/\.json/i',$Source))
			return $this->_import($Source);
		else  
			return $this->_load($Source);
	}
	/**
	 * Save the current device to loaded filename. If device imported then the filename created automatic 
	 */
	public function Save(){
		$this->_save($this->_fileName);
	}
	/**
	 * @return string The current filename if loaded or imported when not returns empty string
	 */
	public function GetFilename(){
		return $this->_fileName;
	}
	/**
	 * Returns true if the device is imported
	 * @return boolean
	 */
	public function DeviceImported(){
		return $this->_importDone;
	}
	/**
	 * Returns true if api has errors
	 * @param int $ErrorNo Empty or number of error to check
	 * @return boolean
	 */
	public function HasError($ErrorNo=0){
		return ($this->_logger)?$this->_error=$this->_logger->HasError($ErrorNo):$this->_error;
	}
	/**
	 * Set the current device Options
	 * - OPTIONS_NEED_HOST		The Device need a Hostvalue
	 * - OPTIONS_NEED_PORT		The Device need a Portvalue
	 * - OPTIONS_NEED_USER_PASS	The Device need User and/or Passoword
	 * @param int $Options Combined flags of OPTIONS_xxx
	 * @param string $mode Valid modes are "set" (default), "add" or "del"
	 */
	public function SetOptions($Options, $mode='set'){
		$o=&$this->_device->{DEVICE_CONFIG}->{CONFIG_OPTIONS};
		switch($mode){
			case 'set' : $o=$Options;break;
			case 'add' : $o=$o | $Options;break;
			case 'del' : $o-=($o&$Options);
		}
	}
	/**
	 * Returns true if FunctionName exists
	 * @param string $FunctionName Functionname to search for
	 * @return boolean
	 * @see GetFunction($FunctionName, $ServiceName) GetServiceNames()
	 */
	public function FunctionExists($FunctionName){
		return (bool)$this->GetFunction($FunctionName);
	}
	/**
	 * Returns current asiggned Logger object. 
	 * This function does not remove references to this class
	 * @return RpcLogger
	 */
	public function GetLogger(){
		return $this->_logger;
	}
	/**
	 * Returns the current device definition class.
	 * Changes of the returned Object has no effect in this device config.
	 * @return NULL|stdClass
	 */
	public function GetModelDef(){
		return empty($this->_device->{DEVICE_DEF})?null: clone $this->_device->{DEVICE_DEF};
	}
	/**
	 * Returns the current device config class.
	 * ATTENTION: Changes of the returned Object has full effect in this device config.
	 * @return NULL|stdClass
	 */
	public function GetConfig(){
		return empty($this->_device->{DEVICE_CONFIG})?null:$this->_device->{DEVICE_CONFIG};
	}
	/**
	 * Returns a array with all loaded service names or null if no device loaded
	 * @return NULL|array
	 */
	public function GetServiceNames(){
		return empty($this->_device)?null:array_keys(get_object_vars($this->_device->{DEVICE_SERVICES}));
	}
	/**
	 * @param string $ServiceName
	 * @return NULL|stdClass
	 */
	public function GetService($ServiceName){
		return (empty($this->_device->{DEVICE_SERVICES}->$ServiceName))?$this->error(ERR_ServiceNotFound,$ServiceName):$this->_cloneService($this->_device->{DEVICE_SERVICES}->$ServiceName);
	}
	public function GetIcon(int $Index=null){
		if(empty($this->_device->{DEVICE_ICONS}))return null;
		if(is_null($Index)){
			$icons=$this->_device->{DEVICE_ICONS};
			foreach($icons as &$icon){
				$icon = clone $icon;	
				$icon->{ICON_URL}=$this->_deviceUrl().$icon->{ICON_URL};
			}
		}else if(!empty($this->_device->{DEVICE_ICONS}[$Index])){
			$icons=clone $this->_device->{DEVICE_ICONS}[$Index];
			$icons->{ICON_URL}=$this->_deviceUrl().$icons->{ICON_URL};
		}else $icons=null;
		return $icons;	
	}
	public function GetIconImageTag(int $Index=null){
		if(!$icons=$this->GetIcon($Index))return null;
		if(!is_null($Index))$icons=[$icons];
		foreach($icons as &$f)$f='<img src="'.$f->{ICON_URL}.'" width="'.$f->{ICON_WIDTH}.'" height="'.$f->{ICON_HEIGHT}.'">';
		return count($icons)==1?$icons[0]:$icons;
	}
	
	/**
	 * @param int $Props The props to validate
	 * @return bool True if device has $Props
	 */
	public function HasProps(int $Props){
		if(empty($this->_device->{DEVICE_DEF}->{DEF_PROPS}))return false;
		return ($this->_device->{DEVICE_DEF}->{DEF_PROPS} & $Props);
	}
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
	public function GetFunction($FunctionName, $ServiceName=null){
		return $this->_findFunctionService($FunctionName,$ServiceName);
	}
	public function GetEventVars($ServiceName='', $IncludeName=false){
		if($ServiceName){
			if(!$service=$this->GetService($ServiceName)) return null;
			if(empty($service->{SERVICE_EVENTS}))return $this->error(ERR_ServiceHasNoEvents,$ServiceName);
			$r=array_keys(get_object_vars($service->{SERVICE_EVENTS}));
			return $IncludeName ? [$ServiceName=>$r]: $r; 
		}
		$return=[];
		foreach($this->_device->{DEVICE_SERVICES} as $sn=>$service)if(!empty($service->{SERVICE_EVENTS})){
				$r=array_keys(get_object_vars($service->{SERVICE_EVENTS}));
				if($IncludeName)$return[$sn]=$r; else $return=array_merge($return,$r);  
		}
		return $return;
	}
  	public function RegisterEvent($ServiceName, $CallbackUrl, $RunTimeSec=0){
  		
  		return ($r=$this->RegisterEvents($ServiceName, $CallbackUrl,$RunTimeSec))?$r[0]:false;
  	}
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
	public function RefreshEvent($SID, $Service, $RunTimeSec=0){
		$r=[EVENT_SID=>$SID,EVENT_TIMEOUT=>$RunTimeSec,EVENT_SERVICE=>$Service];
		if($this->_sendEvents($r,null,$RunTimeSec)){
			return $r[EVENT_TIMEOUT];			
		}else return false;
	}
 	public function RefreshEvents(array &$EventArray){
 		$this->_error=false;
		return $this->_sendEvents($EventArray,null,$RunTimeSec);
 	}
	public function UnregisterEvent($SID, $Service){
		$r=[EVENT_SID=>$SID,EVENT_TIMEOUT=>0,EVENT_SERVICE=>$Service];
		return $this->_sendEvents($r,true);
	}
 	public function UnRegisterEvents(&$EventArray) {
 		$this->_error=false;
		return $this->_sendEvents($EventArray,true);
 	}

	public function Help($FunctionName='', $HelpMode= HELP_FULL, $ReturnResult=false){
		require_once 'rpchelp.inc';
// 		CreateHelpFromRPCClass($this);
		$help=[];
		if(empty($FunctionName))foreach($this->_device->{DEVICE_SERVICES} as $serviceName=>$service){
			if($HelpMode!=HELP_FULL)$help[]="-- Service Name : $serviceName ----------------";
			$fhelp=[];
			foreach($service->{SERVICE_FUNCTIONS} as $FunctionName=>$function){
				$values=empty($function->{FUNCTION_PARAMS}->{PARAMS_IN})?null:$this->_createFunctionValues($function->{FUNCTION_PARAMS}->{PARAMS_IN});
				$fhelp[$FunctionName] = CreateHelp($function,$FunctionName, $values, $HelpMode,$serviceName);	
			}
			ksort($fhelp);
			foreach($fhelp as $h)$help=array_merge($help,$h);
		}elseif($function=$this->_findFunctionService($FunctionName)){
			$values=empty($function->{FUNCTION_PARAMS}->{PARAMS_IN})?null:$this->_createFunctionValues($function->{FUNCTION_PARAMS}->{PARAMS_IN});
			$help = CreateHelp($function->{SERVICE_FUNCTIONS},$FunctionName,$values,$HelpMode, $function->{SERVICE_NAME});
		}
		if($ReturnResult) return implode("\n",$help);
		echo implode("\n",$help)."\n";
	}
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
//	Log Error/Debug 	
	protected function error($Message, $ErrorCode=null, $Params=null /* ... */){
		if(!$this->_logger){ $this->_error=true; return null;}
		if(is_numeric($Message))$Params=array_slice(func_get_args(),1);
		elseif($Params && !is_array($Params))$Params=array_slice(func_get_args(),2);
		$this->_error=true;
		return $this->_logger->Error($ErrorCode, $Message, $Params);
	}
	protected function debug($DebugOption, $Message, $Params=null /* ... */){
		if(!$this->_logger)return;
		if($Params && !is_array($Params))$Params=array_slice(func_get_args(),2);
		$this->_logger->Debug($DebugOption, $Message, $Params);
	}
 // Connection Calls   
	protected function & GetConnection($ConnectionType=CONNECTION_TYPE_SOAP){
		if($this->_connection && $this->_connection->ConnectionType()==$ConnectionType){
			$this->_connection->AttachLogger($this->_logger);
			return $this->_connection;
		}
		$this->_connection=null;
		$creditials=[$this->_device->{DEVICE_CONFIG}->{CONFIG_LOGIN_U},$this->_device->{DEVICE_CONFIG}->{CONFIG_LOGIN_P},''];
		$this->debug(DEBUG_CALL, 'Open new '.NAMES_CONNECTION_TYPE[$ConnectionType].' connection to '.$this->_deviceUrl(),201);
		switch($ConnectionType){
			case CONNECTION_TYPE_SOAP: $this->_connection=new RPCSoapConnection($creditials,$this->_logger);break;
			case CONNECTION_TYPE_JSON: $this->_connection=new RPCJSonConnection($creditials,$this->_logger);break;
			case CONNECTION_TYPE_URL : $this->_connection=new RPCUrlConnection ($creditials,$this->_logger);break;
			case CONNECTION_TYPE_XML : $this->_connection=new RPCXMLConnection($creditials,$this->_logger);break;
		}
		return $this->_connection;
	}
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
   		$old_handler = set_error_handler('RPCErrorHandler::CatchError');
		try {	eval($__source);}
		catch(Exception $e){ $_R=$this->error($e->GetMessage(),$e->getCode());}
  		set_error_handler($old_handler);
		return $_R;
	}
	private function _callConnection(stdClass $Service, $Arguments, $Filter=null){
		if(!$connection=$this->GetConnection($Service->{SERVICE_CONNTYPE}))return $this->error(ERR_CantGetConnection);
		$url=$this->_deviceUrl($Service->{SERVICE_PORT}). $Service->{SERVICE_CTRLURL};
		$result=$connection->execute($url,$Service->{SERVICE_ID}, $Service->{SERVICE_FUNCTIONS}->{FUNCTION_NAME}, $Arguments, $Filter);
		if($this->_logger)$connection->DetachLogger($this->_logger);
		return $result;
	}
//  Internal Helper Methods
	private function _cloneService(stdClass $Service, $FunctionName=null){
		if($FunctionName && empty($Service->{SERVICE_FUNCTIONS}->$FunctionName))return null;
		$r=clone $Service;
		if($FunctionName)$r->{SERVICE_FUNCTIONS}=$Service->{SERVICE_FUNCTIONS}->$FunctionName;
		else unset($r->{SERVICE_FUNCTIONS});
		if(empty($r->{SERVICE_CONNTYPE}))$r->{SERVICE_CONNTYPE}=$this->_device->{DEVICE_CONFIG}->{CONFIG_CONNTYPE};
		if(empty($r->{SERVICE_PORT}))$r->{SERVICE_PORT}=$this->_device->{DEVICE_CONFIG}->{CONFIG_PORT};
		if(empty($r->{SERVICE_HOST}))$r->{SERVICE_HOST}=$this->_device->{DEVICE_CONFIG}->{CONFIG_HOST};
		return $r;
	}
	private function _findFunctionService($functionname,$ServiceName=null){
		$result=null;
		if(empty($ServiceName)){
			foreach($this->_device->{DEVICE_SERVICES} as $sn=>$service)if($result=$this->_cloneService($service,$functionname))	break;
		}elseif(!empty($this->_device->{DEVICE_SERVICES}->$ServiceName->{SERVICE_FUNCTIONS}->$functionname))
			$result= $this->_cloneService($this->_device->{DEVICE_SERVICES}->$ServiceName,$functionname);
		return $result;
	}
	private function _deviceUrl($Port=null){
		if(is_null($Port))$Port=$this->_device->{DEVICE_CONFIG}->{CONFIG_PORT};
		$Port=($Port)?":$Port":'';
		return $this->_device->{DEVICE_CONFIG}->{CONFIG_SCHEME}."://".$this->_device->{DEVICE_CONFIG}->{CONFIG_HOST}.$Port;
	}
	private function _hasOption($Option){
		return (bool)$this->_device->{DEVICE_DEF}->{DEF_OPTIONS} & $Option;
	}
// 	Value Handling	
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
// 	Clear, Load, Save, Import 	
	private function _clear($full=true){
		$this->_device=null;
		$this->_fileName='';
		$this->_connection=null;
		$this->_static = [];
		$this->_error = false;
		$this->_isOnline = null;
		if($this->_logger)$this->_logger->GetError(true,true);
	}
	private function _load($JsonConfigFileName){
 		if($config=json_decode(file_get_contents($JsonConfigFileName))){
 			UTF8::decode($config);
 			if(empty($config->{DEVICE_DEF}))return $this->error(ERR_NoRpcConfigFile,$JsonConfigFileName);
 			if(empty($config->{DEVICE_DEF}->{DEF_VERSION})||$config->{DEVICE_DEF}->{DEF_VERSION}!=self::my_version)return $this->error(ERR_ConfigFileVersionMismatch,@$config->{DEVICE_DEF}->{DEF_VERSION},self::my_version); 
 			$this->_device=$config;
			$this->_fileName=$JsonConfigFileName;
			return true;
 		}else return $this->error(ERR_FileNotFound,$JsonConfigFileName);
 	}
  	private function _save($JsonConfigFileName=''){
		if(empty($this->_device))return $this->error(ERR_NoDataToSave);
		$file=pathinfo($JsonConfigFileName);
		if(empty($file['basename'])){
			if(!empty($this->_device->{DEVICE_DEF}->{DEF_MANU}))$fn=ucfirst($this->_device->{DEVICE_DEF}->{DEF_MANU});
			elseif(!empty($this->_device->{DEVICE_INFO}->manufacturer))$fn=ucfirst($this->_device->{DEVICE_INFO}->manufacturer);
			elseif(!empty($this->_device->{DEVICE_INFO}->deviceType))$fn=ucfirst($this->_device->{DEVICE_INFO}->deviceType);
			else $fn='Unknown';
			if(!empty($this->_device->{DEVICE_DEF}->{DEF_MODEL}))$fn.=' ['.$this->_device->{DEVICE_DEF}->{DEF_MODEL}.']';
			elseif(!empty($this->_device->{DEVICE_INFO}->friendlyName))$fn.='-'.$this->_device->{DEVICE_INFO}->friendlyName;
			$file['basename']=trim(str_replace([':',',','  '],['_','-',' ',' ',' '], $fn));
		}
		$file['dirname']=!empty($file['dirname'])?$file['dirname'].'/':(defined('RPC_CONFIG_DIR')?RPC_CONFIG_DIR.'/':'config/');
		if($file['dirname'])@mkdir($file['dirname'],755,true);
		$this->_fileName=$file['dirname'].$file['basename'].'.json';
 		$this->debug(empty($JsonConfigFileName)?DEBUG_BUILD:DEBUG_INFO, "Save file to $this->_fileName");
 		$config=$this->_device;
		UTF8::encode($config);
		return file_put_contents($this->_fileName, json_encode($config));
  	}
	private function _import($UrlToDescXml){
  		require_once 'rpcimport.inc';
  		$this->_device=ImportRpcDevice($UrlToDescXml,self::my_version,$this->_logger);
  		return $this->_importDone=($this->_device && !$this->HasError())?$this->_save():false;
	}
// 	Events
	private function _sendEvents(array &$EventArray, $CallbackUrl,  $RunTimeSec = 0){
		foreach($EventArray as &$event){
 			if(empty($service=@$event[EVENT_SERVICE])) return $this->error(ERR_ServiceNameEmpty);
 			if(empty($service=$this->_cloneService($this->_device->{DEVICE_SERVICES}->$service))) return $this->error(Err_ServiceNotFound,$event->SERVICE);
 			if(empty($eventUrl=$service->{SERVICE_EVENTURL})) return $this->error(ERR_ServiceHasNoEvents,$event->SERVICE);
 	  		if(!$con=$this->GetConnection($service->{SERVICE_CONNTYPE})) return $this->error(ERR_CantGetConnection);
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
}



?>