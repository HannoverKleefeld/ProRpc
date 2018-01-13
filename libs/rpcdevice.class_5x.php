<?php
#TODO 27.12.17 Create functions for Event Handling 
#TODO Transfer all Messages to messages.inc
#TODO add  function GetIcon(int Index, bool RawData)
#TODO rewrite Function CreateHelp

if(!defined('LIB_INCLUDE_DIR'))require_once 'loader.php';
if(!defined('RPC_DEBUG'))define ('RPC_DEBUG','');
require_once LIB_INCLUDE_DIR . '\rpc.defines.inc';
require_once LIB_INCLUDE_DIR . '\rpc.constants'.RPC_DEBUG.'.inc'; 	
require_once LIB_INCLUDE_DIR . '\rpc.messages.inc';
 	
if(!function_exists('Sys_Ping')){
	function Sys_Ping($host, $timeout=1){
		return net::ping($host,$timeout);
	}
}
class RPCEvalHandler {
 	public static function CatchError($ErrLevel, $ErrMessage) {
echo "ErrorLevel: $ErrLevel\n";		
 		if($ErrLevel != 0){
			throw new ErrorHandlerExeption("$ErrMessage  in  'rpc.predefines.inc' entry(s) FUNCTION_SOURCE",$ErrLevel+60000);
		}
		return false;
	}
}
// const 
// 	RPCXmlErrorChecks = [ 'regex_message'=>'//i','regex_code'=>'//i'];
// const 
// 	RPJonsonErrorChecks = [];
class RPCPacket {
	function __construct($Method, $Url='/', $Content=''){
		$this->_method=$Method;
		$this->_url=$Url;
		$this->_content=$Content;
	}
	function Add($Param, $Value){
		$this->$Param=$Value;
		return $this;
	}	
	function __toString(){
		$out=["{$this->_method} {$this->_url} HTTP/1.1"];
		foreach(get_object_vars ($this) as $vN=>$v){
			if($vN[0]=='_')continue;
			$out[]="$vN: $v";
		}
		if(!is_null($this->_content)){
			$len=strlen($this->_content);
			$out[]="CONTENT-LENGTH: $len";
			if($len)$out[]=$this->_content;
				
		}	
		return implode("\n",$out)."\n\n";
	}
	function toString(){
		return self::__toString();
	}	
}

abstract class RPCConnection {
	use ErrorHandler;
	protected $_creditials = [];
	private $_connectionType = 0; 
	function __construct(array $creditials, $ConnectionType){
		$this->_creditials=$creditials;	
		$this->_connectionType=$ConnectionType;
	}
	abstract public function Execute($url,$serviceID,$functionname,array $arguments, array $filter=null);
	public function ConnectionType(){return $this->_connectionType;}
	public function SendPacket($url, $content ){
		$p=parse_url($url);
		$port=empty($p['port']) ? 80 : $p['port'];
		$host=empty($p['path']) ? $p['host'] : $p['path'];
		$fp = fsockopen($host, $port, $errno, $errstr, 2);
		if(!$fp)return $this->error("Error opening socket to $host at $port: ".$errstr,$errno);
		$this->debug(DEBUG_CALL,'send packet =>'.debug::export($content,'|'),509);
		$size=fputs ($fp,$content);
		$this->debug(DEBUG_CALL,'send packet size =>'.$size,509);
		stream_set_timeout ($fp,1);
		$response = ""; $retries=2;
		while (!feof($fp)){
			$response.= fgetss($fp,128); // filters xml answer
			if(--$retries == 0 && !$response)break;
		}
		fclose($fp);
		$this->debug(DEBUG_CALL,'send packet return =>'.($response?'true':'false'),509);
		return $this->decodePacket($response);
	}
	public static function CreatePacket($Method, $Url='/', array $Arguments=null, $Content=null){
		$out=["$Method $Url HTTP/1.1"];
		if($Arguments)foreach($Arguments as $vN=>$v)$out[]="$vN: $v";
		if(!is_null($Content)){
			$out[]="CONTENT-LENGTH: ".strlen($Content);
			if($Content)$out[]=$Content;
		}
		return implode("\n",$out)."\n\n";
	}
	private function decodePacket($Result){
		if(is_null($Result)) return $this->error(ERR_CantGetConnection);
		if(empty($Result)) return $this->error(ERR_EmptyResponse);
		$data=null; 
		$response = preg_split("/\n/", $Result);
		if(preg_match('/HTTP\/(\d.\d) (\d+) ([ \w]+)/', $response[0],$m)){
			$code=intval($m[2]);
			$msg=empty($m[3])?'Unknown':(is_numeric($m[3])?"Unknown Message ({$m[3]})":$m[3]);
			if($code!=200&&$code!=202)return $this->error(ERR_InvalidResposeCode,$code,$msg);
			array_shift($response);
			$data=['HTTP_VERSION'=>$m[1], 'HTTP_CODE'=>$code, 'HTTP_MSG'=>$msg];
		}
		$count=count($response);
		for($j=0;$j<$count;$j++){
			$line=$response[$j];
			if(($pos=strpos($line,':'))===false || $pos > 20){ // is Content
				$data['CONTENT']=implode("\n",array_slice($response, $j));
				break;
			}else {
				$m=explode(':',trim($line));
				if(isSet($m[1])){
					$b=trim(array_shift($m));
					$data[$b]=trim(implode(':',$m));
				}
			}	
		}
		if(is_null($data))return $this->error(ERR_InvalidResponceFormat,'HTTP-HEADER');
		return $data;			
	}	
}
class RPCSoapConnection extends RPCConnection{
	public function Execute($url,$serviceID,$functionname,array $arguments, array $filter=null){
		$this->clearError();
		$params=array(
				'location' 	 => $url,
				'uri'		 => $serviceID,
				'noroot'     => true,
				'exceptions'=> true,
				'trace'		=> true
		);
		if($this->_creditials[CREDIT_USER])	$params['login']=$this->_creditials[CREDIT_USER];
		if($this->_creditials[CREDIT_PASS])	$params['password']=$this->_creditials[CREDIT_PASS];
		$client = new SoapClient( null,	$params);
		$params=array();
		foreach($arguments as $key=>$value)$params[]=new SoapParam($value, $key);
		$response = $client->__soapCall($functionname,$params);
		if(is_soap_fault($response))return $this->error($response->faultstring,$response->faultcode);
		// 				var_dump($r->faultcode, $r->faultstring, $ex->faultactor, $ex->detail, $ex->_name, $ex->headerfault);
		return $response;
	}
	
}
class RPCUrlConnection extends RPCConnection{
	public function Execute($url,$serviceID,$functionname,array $arguments, array $filter=null){
		$this->clearError();
		$chr=strpos($url,'?')===false ? '?' : '&';
		if(preg_match("/%function%/i", $url))$url=str_ireplace('%function%', $functionname, $url);
		else if($chr=='&') 	$url.=$functionname;
		else $url.='/'.$functionname;
		$args=http_build_query($arguments);
		if($args)$url.=$chr.$args;
		$this->debug(DEBUG_CALL, "Call $url",100);
		if(!$result=file_get_contents($url))return $this->error(ERR_EmptyResponse);
		if(substr($result,0,5)=='<?xml'){
			if($r=$this->Filter($result,$filter))$result=$r;
		}elseif($result[0]=='['||$result[0]=='{'){
			if($r=json_decode($json,true))$result=$r;
			$strdecode = function(&$item, $key) {if ( is_string($item) )$item = utf8_decode($item);	else if ( is_array($item) )	array_walk_recursive($item, $strdecode);};
			if($r)array_walk_recursive($result, $strdecode);
		}	
// 		if(is_array($result)){
			
// 		}
		return $result;
	}
	protected function Filter($subject, $pattern){
		if(!is_array($pattern))$pattern=explode(',',$pattern);
		$multi=(count($pattern)>1);
		$StringToType=function ($var){
			if(is_string($var)){
				if(is_numeric($var))$var=is_float($var)?floatval($var):intval($var);
				else if(strcasecmp($var, 'true')==0)$var=true;
				else if(strcasecmp($var, 'false')==0)$var=false;
			}
			return $var;	
		};
		
		foreach($pattern as $pat){
			if(!$pat)continue;
			preg_match('/\<'.$pat.'\>(.+)\<\/'.$pat.'\>/i',$subject,$matches);
			if($multi){
				$cleanPat=str_replace(['.','<','>','?','^','\\','|','(',')','!'],'',$pat);
				if(isSet($matches[1])) {
					$n[$cleanPat]=$StringToType($matches[1]);
				}else
					$n[$cleanPat]=false;
			} else {
				if(!isSet($matches[1]))return false;
				return $StringToType($matches[1]);
			}
		}
		return $n;
	}
	
}
abstract class RPCCurlConnection extends RPCConnection{
	private $_curl=null;
	abstract protected function encodeRequest($FunctionName, $Arguments);
	abstract protected function decodeRequest($Result);
	public function Execute($url,$serviceID,$functionname,array $arguments, array $filter=null){
		$this->clearError();
		if(is_null($this->_curl)){
			$this->_curl=curl_init();
			curl_setopt($this->_curl, CURLOPT_URL, $url);
			
			if($this->_creditials[CREDIT_USER] || $this->_creditials[CREDIT_PASS]){
				curl_setopt($this->_curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
				curl_setopt($this->_curl, CURLOPT_USERPWD, $this->_creditials[CREDIT_USER]. ":" . $this->_creditials[CREDIT_PASS]);
			}
			if(empty($this->_creditials[CREDIT_CAFILE])){
				curl_setopt($this->_curl, CURLOPT_SSL_VERIFYPEER, 0);
				curl_setopt($this->_curl, CURLOPT_SSL_VERIFYHOST, 0);
			}else {
				curl_setopt($this->_curl, CURLOPT_SSL_VERIFYPEER, true);
				curl_setopt($this->_curl, CURLOPT_CAINFO,$this->_creditials[CREDIT_CAFILE]);
			}
			curl_setopt($this->_curl, CURLOPT_HEADER, 0);
			curl_setopt($this->_curl, CURLOPT_RETURNTRANSFER, 1);
			curl_setopt($this->_curl, CURLOPT_HTTPHEADER, array("CONTENT-TYPE: application/json; charset='utf-8'"));
			curl_setopt($this->_curl, CURLOPT_POST, 1);
		}
		if(!$postData=$this->encodeRequest($functionname, $arguments))return null;
		curl_setopt($this->_curl, CURLOPT_POSTFIELDS, $postData);
		if(!$result = curl_exec($this->_curl))return $this->error(ERR_EmptyResponse);
		return $this->decodeRequest($result);	
	}
}
class RPCXMLConnection extends RPCCurlConnection{
	protected function encodeRequest($FunctionName, $Arguments){
		$request=xmlrpc_encode_request($FunctionName,$Arguments,['encoding'=>'utf-8']);
		return $request;
	}
	protected function decodeRequest($Result){
		if(empty($Result)) return $this->error(ERR_EmptyResponse);
		$Result = xmlrpc_decode($Result, "utf-8");
		if (is_null($Result)) return $this->error(ERR_InvalidResponceFormat,'xml');
		if(!empty($Result['faultCode'])||!empty($Result['faultString']))
			return $this->error(@$Result['faultString']?$Result['faultString']:'unknown error',$Result['faultCode']?$Result['faultCode']:110);
		return $Result;
		
	}	
}	
class RPCJsonConnection extends RPCCurlConnection{
	protected $_requestID = null;
	protected function encodeRequest($FunctionName, $Arguments){
		if (!is_scalar($FunctionName)) return $this->error(ERR_MethodNoScala);
		if (!is_array($Arguments)) return $this->error(ERR_FormatArray);
		$params = array_values($Arguments);
		utf8_encode_array($params);
		return json_encode(["jsonrpc" => "2.0","method" => $FunctionName,"params" => $params,"id" => $this->_requestID = round(fmod(microtime(true)*1000, 10000))]);
	}
	protected function decodeRequest($Result){
		if($Result=== false)return $this->error(ERR_RequestEmptyResponse);
		$Response= json_decode($Result, true);
		if (is_null($Response)) return $this->error(ERR_InvalidResponceFormat,'json');
		utf8_decode_array($Response);
		if (isset($Response['error'])) return $this->error($Response['error']['message']);
		if (!isset($Response['id'])) return $this->error(ERR_NoResponseID);
		if ($Response['id'] != $this->_requestID)return $this->error(ERR_InvalidResponseID,$this->_requestID,$Response['id']);
		return $Response['result'];
	}

}
class RPCDevice {
	use ErrorHandler;
	protected $_device = null;
	protected $_connection = null;
	protected $_deviceFileName = '';
	private $_isOnline = null;
	private $_cachedEvents=null;
	protected $_cachedFunctionsServiceName=null;
	protected $_lastServiceName='';
	protected $_lastFunction = null;
	
	function __construct($deviceJsonConfigFileName=null, $user=null, $pass=null, $caFile=null){
		if(!empty($deviceJsonConfigFileName))$this->load($deviceJsonConfigFileName);
		if(!is_null($user)||!is_null($pass)||!is_null($caFile))$this->setCreditials($user, $pass,$caFile);
	}
	function __destruct(){
		$this->debug(DEBUG_INFO,"Connection closed",205);
	}
	function __call($functionname, $arguments){
		//		if(!is_null($this->_lastError))return null;

		if(count($arguments)<2 && !empty($arguments[0]) && is_array($arguments[0]))$arguments=$arguments[0];
		$this->debug(DEBUG_CALL,sprintf('Call %s(%s)',$functionname,debug::as_array($arguments)),200);
		if(empty($this->_device))return $this->error(ERR_UPNPDeviceNotLoaded);
		if(!$function=$this->GetFunction($functionname)) return $this->error(ERR_FunctionNotFound,$functionname);
		if(!$this->IsOnline())return $this->error(ERR_DeviceNotOnline);
		$this->clearError();
//print_r($function);exit;
		$filter=null; $args=[];
// if($function[FUNCTION_NAME]=="RemoteControl")var_dump($function);
		
		if(!empty($function[FUNCTION_PARAMS][PARAM_IN])){
// if($function[FUNCTION_NAME]=="RemoteControl")var_dump($function[FUNCTION_PARAMS][PARAM_IN]);
			$values=$this->createFunctionValues($function[FUNCTION_PARAMS][PARAM_IN],$arguments);
//if($functionname=="RemoteControl")exit(var_dump($values));
			if($this->hasOption(OPT_CHECK_PARAMS)){
				foreach ($function[FUNCTION_PARAMS][PARAM_IN] as $param){
					if(!$this->checkValue($param, $values[$param[VALUE_NAME]])){
// 						Echo "ERROR \n";
// exit(var_dump($param[VALUE_NAME],$values[$param[VALUE_NAME]]));		
						return null;
					}
					$args[$param[VALUE_NAME]]=$values[$param[VALUE_NAME]];
				}
			}else $args=$values;
		}
		$arguments=$args;
//if($functionname=="RemoteControl") exit(var_dump($arguments,false));
		// print_r($arguments);exit;
 		if(!empty($function[FUNCTION_PARAMS][PARAM_OUT])&& $this->hasOption(OPT_RESULT_FILTER)){
			$filterDef=empty($this->_device[DEVICE_SERVICES][$this->_lastServiceName][SERVICE_FILTER_DEF])?'%s':$this->_device[DEVICE_SERVICES][$this->_lastServiceName][SERVICE_FILTER_DEF];
			foreach ($function[FUNCTION_PARAMS][PARAM_OUT] as $param){
				$filter[]=sprintf($filterDef,$param[VALUE_NAME]);
			}
		}
		if(empty($function[FUNCTION_SOURCE])){
 			$url=$this->getUrl(	$this->_device[DEVICE_SERVICES][$this->_lastServiceName][SERVICE_PORT],	$this->_device[DEVICE_SERVICES][$this->_lastServiceName][SERVICE_CTRL_URL]);
			$result=$this->callConnection($url,$function[FUNCTION_NAME], $arguments, $filter);
		} else 	$result=$this->_callSource($function[FUNCTION_NAME],$function[FUNCTION_SOURCE], $arguments, $filter);
		if(is_null($result)&&empty($this->_lastError))$result=true;
		$this->debug(DEBUG_CALL,sprintf('Call %s returns %s',$functionname,debug::export($result)),200);
		return $result;
	}
// 	private function _StrSetPos($string, $max, $offset=0){
// 		$len = strlen($string);
// 		if($len<$max)return $string;
// 		$start = $offset?$offset + (int)($max/2):0;
// 		if($start>$len)	$start=(int)$len/2;
// 		return substr($string,$start,$max);
// 	}
	public function GetVersion(){
		return empty($this->_device[INFO_VERSION])?0.0:$this->_device[INFO_VERSION];
	}
	public function IsOnline(){
		if(is_null($this->_isOnline))$this->_isOnline=Sys_Ping($this->_device[DEVICE_CONFIG][CONFIG_HOST],1);
		return $this->_isOnline;
	}
	public function Load($filename){
// 		if(stripos($filename,'.json')===false)$filename.='.json';
		if(!file_exists($filename)) return $this->error(ERR_FileNotFound,$filename);
		if($this->_device=json_decode(file_get_contents($filename),true))$this->loaded();
		if(empty($this->_device[DEVICE_CONFIG][CONFIG_HOST])){
			$this->_device=null;
			return $this->error(ERR_InvalidConfigFile,$filename);
		}
		$this->_deviceFileName=$filename;
		return !is_null($this->_device);
	}
		
	public function Help($FunctionName='', $HelpMode= HELP_FULL, $ReturnResult=false){
		$help=[];$functions=$this->_functionList();
		if(empty($FunctionName)){
			foreach($functions as $FunctionName=>$service){
				$help = array_merge($help,$this->createHelp($this->_device[DEVICE_SERVICES][$service][SERVICE_FUNCTIONS][$FunctionName],$FunctionName,$HelpMode));	
			}
// 			foreach($this->_device[DEVICE_SERVICES] as $sn=>$service)
// 				foreach($service[SERVICE_FUNCTIONS] as $fn=>$function)
// 					$help = array_merge($help,$this->createHelp($function,$fn,$HelpMode));
		}else if($function=$this->GetFunction($FunctionName))
			$help = $this->createHelp($function,$FunctionName,$HelpMode);
		if($ReturnResult) return implode("\n",$help);
		echo implode("\n",$help)."\n";
	}
	public function SetCreditials($user=null, $pass=null, $caFile=null){
		if(!empty($this->_device)){
			if(!is_null($user))$this->_device[DEVICE_CONFIG][CONFIG_CREDITIALS][CREDIT_USER]=$user;
			if(!is_null($pass))$this->_device[DEVICE_CONFIG][CONFIG_CREDITIALS][CREDIT_PASS]=$pass;
			if(!is_null($caFile))$this->_device[DEVICE_CONFIG][CONFIG_CREDITIALS][CREDIT_CAFILE]=$caFile;
		}
	}	
	public function SetOptions($options, $set=null){
		if(empty($this->_device))return;
		$c=&$this->_device[DEVICE_CONFIG][CONFIG_OPTIONS];
		$c= (is_null($set)||$set==true) ? $c | $options : $c - ($c & $options);
	}
	public function GetConfigParam($ConfigParam){
		return @$this->_device[DEVICE_CONFIG][$ConfigParam];
	}
	public function GetInfoParam($InfoParam){
		return @$this->_device[DEVICE_INFO][$InfoParam];
	}
/*
 * RPC Service Handling 
 */
	public function GetService($ServiceName){
		return empty($this->_device[DEVICE_SERVICES][$ServiceName])?null : $this->_device[DEVICE_SERVICES][$ServiceName];
	}
	public function ServiceList($ResultReturn=false){
		$r=array_keys($this->_device[DEVICE_SERVICES]);
		sort($r);
		if($ResultReturn)return $r;
		else var_export($r);
	}
	
/*
 * RPC Function Handling
 */	
	public function FunctionList($ResultReturn=false, $ServiceName=null){
		if(is_null($ServiceName))
			$r=array_keys($this->_functionList());
		else $r=@array_keys($this->_device[DEVICE_SERVICES][$ServiceName][SERVICE_FUNCTIONS]);
		if($ResultReturn)return $r;else if(!empty($r))echo implode("\n",$r);
	}
	public function GetFunctionParam($functionName, $paramName, $ServiceName=null){
		if(!$func=$this->GetFunction($functionName,$ServiceName))return null;
		if($paramName=='')return empty($func[FUNCTION_PARAMS][PARAM_OUT][0])?null:$func[FUNCTION_PARAMS][PARAM_OUT][0];
		if(!empty($func[FUNCTION_PARAMS][PARAM_OUT]))foreach($func[FUNCTION_PARAMS][PARAM_OUT] as $p)if(strcasecmp($p[VALUE_NAME], $paramName)==0)return $p;
		if(!empty($func[FUNCTION_PARAMS][PARAM_IN]))foreach($func[FUNCTION_PARAMS][PARAM_IN] as $p)if(strcasecmp($p[VALUE_NAME], $paramName)==0)return $p;
		return null;
	}
	public function FunctionExist($FunctionName, $ServiceName=null){
		return empty($ServiceName)?!empty($this->_functionList()[$FunctionName]):!empty($this->_device[DEVICE_SERVICES][$ServiceName][SERVICE_FUNCTIONS][$FunctionName]);
	}
	public function GetFunction($functionname,$ServiceName=null){
		if(empty($ServiceName)){
			$list=$this->_functionList();
			if(empty($list[$functionname]))return null;
			//$service=$this->_lastServiceName;
// 			if($service && $list[$functionname]!=$service)return null;
			$this->_lastServiceName=$ServiceName=$list[$functionname];
		}	
		return @$this->_device[DEVICE_SERVICES][$ServiceName][SERVICE_FUNCTIONS][$functionname];
	}
	
 	public function GetKeyCodes(){
 		if(empty($this->_device[DEVICE_KEYCODES]))
 			return null;
 		$r=[];
 		for($key=KEY_0; $key <= KEY_SRCDOWN;$key++)
 			if(isset($this->_device[DEVICE_KEYCODES][$key]))$r[]=$key;
  		return $r;
 	}
 	public function GetHost(){
		return $this->_device[DEVICE_CONFIG][CONFIG_HOST];
 	}
 	public function GetPort($ReturnCurentServicePort=false){
 		if($ReturnCurentServicePort && $this->_lastServiceName)return $this->_device[DEVICE_SERVICES][$this->_lastServiceName][SERVICE_PORT];
 		return $this->_device[DEVICE_CONFIG][CONFIG_PORT];
 	}
 	public static function OptionNames($Options){
 		$opts=['CHECK_PARAMS','RESULT_FILTER','DEFAULTS_TO_END','CACHE_ICONS','INCLUDE_DESC'];
 		foreach([OPT_CHECK_PARAMS,OPT_RESULT_FILTER,OPT_DEFAULTS_TO_END,OPT_CACHE_ICONS,OPT_INCLUDE_DESC] as $id=>$opt)
 			if(!($Options & $opt))unset($opts[$id]);
 			return implode(', ',$opts);
 			
 	}
 	public static function ConnectionTypeName($ConnectionType){
 		static $convert=[CONNECTION_TYPE_SOAP=>'SOAP',CONNECTION_TYPE_JSON=>'JSON',CONNECTION_TYPE_URL=>'URL',CONNECTION_TYPE_XML=>'XML'];
 		return empty($convert[$ConnectionType])?'Unknown': $convert[$ConnectionType];
 	}
 	
 /*
  * RPC Event Handling
  * 
  */ 
 	public function HasEvents(){
 		return (bool)($this->_device[DEVICE_CONFIG][CONFIG_PROPS_OPTIONS] & PROP_EVENTS);
 	}
 	public function GetEventVars($Service='', $Function=''){
 		if(empty($items=$this->GetEvents())) return null;
 		$return=[];
 		if(empty($Service))foreach($items as $s){
			if(!empty($Function)){
				if(!empty($s[$Function])){ $return=array_merge($return,$s[$Function]); break; }
			}else foreach($s as $f)$return=array_merge($return,$f);
 		}elseif((($s=@$items[$Service])) && (!$Function || !@$s[$Function]) ){
 			if($Function)$return=$s[$Function];
 			else foreach($s as $f)$return=array_merge($return,$f);
 		}
 		return $return; 		
 	}
 	public function & GetEvents(){
 		if(!is_null($this->_cachedEvents))return $this->_cachedEvents;
 		if(!$this->HasEvents())return $this->error(ERR_DeviceHasNoEvents,get_class($this));
 		foreach($this->_device[DEVICE_SERVICES] as $sname=>$service)foreach($service[SERVICE_FUNCTIONS] as $fname=>$function){
 			if(empty($function[FUNCTION_EVENTS]))continue;
 			$this->_cachedEvents[$sname][$fname]=$function[FUNCTION_EVENTS];
 		}
 		return $this->_cachedEvents;
 	}
 	public function GetEventFunctionNames(){
 		if(empty($items=$this->GetEvents())) return null; else $return=[];
 		foreach($items as $sname=>$service) $return=array_merge($return,array_keys($service));
 		return count($return)>0?$return:null;
 	}
 	public function GetEventServiceNames(){
 		if(empty($items=$this->GetEvents())) return null; else $return=[];
 		return array_keys($items);
 	}
  	public function RegisterEvent($Service, $CallbackUrl, $RunTimeSec=300){
  		if(empty($Service))
 			if($services=$this->GetEventServiceNames()){
 			foreach($services as $Service)if($ok=$this->RegisterEvent($Service, $CallbackUrl,$RunTimeSec))break;
 		}
 		if(!$_service=$this->GetService($Service)){
 			if($this->GetFunction($Service))
 				$Service=$this->_lastServiceName;
 			else return $this->error(Err_ServiceNotFound, $Service);
 		}
 		if(empty($eventUrl=$_service[SERVICE_EVENT_URL])) return $this->error(ERR_ServiceHasNoEvents,$Service);
 		$port=isset($_service[SERVICE_PORT])?$_service[SERVICE_PORT]:$this->_device[DEVICE_CONFIG][CONFIG_PORT];
 		$conType=isset($_service[SERVICE_CONNECTION_TYPE])?$_service[SERVICE_CONNECTION_TYPE]:$this->_device[DEVICE_CONFIG][CONFIG_CONNECTION_TYPE];
 		if(!$con=$this->GetConnection($conType)) return $this->error(ERR_CantGetConnection);
 		$host = $this->_device[DEVICE_CONFIG][CONFIG_HOST].':'.$port;
  		if(empty($RunTimeSec))$RunTimeSec="Infinite";else $RunTimeSec="Second-$RunTimeSec";
  		$request=$con->CreatePacket('SUBSCRIBE', $eventUrl ,[
  				'HOST'=>$host,	'CALLBACK'=>"<$CallbackUrl>", 'NT'=>'upnp:event', 'TIMEOUT'=>$RunTimeSec
 		]);
  		$result=$con->SendPacket($host,$request);
  		$this->MergeErrors($con);
		if(empty($result))return null;
		if(empty($sid=$result['SID']) && empty($sid=$result['SUBSCRIPTION-ID']))return $this->error(ERR_InvalidResposeSID);
		if($result['TIMEOUT'])$result['TIMEOUT']=intval(str_ireplace('Second-', '', $result['TIMEOUT']));
		return ['SID'=>$sid,'TIMEOUT'=>$result['TIMEOUT']];
	}
 	public function RefreshEvent($EventSID, $Service, $RunTimeSec=300){
 		if(empty($Service))								  return $this->error(ERR_ServiceNameEmpty);
 		if(!$_service=$this->GetService($Service))		  return $this->error(Err_ServiceNotFound, $Service);
 		if(empty($eventUrl=$_service[SERVICE_EVENT_URL])) return $this->error(ERR_ServiceHasNoEvents,$Service);
 		$port=isset($_service[SERVICE_PORT])?$_service[SERVICE_PORT]:$this->_device[DEVICE_CONFIG][CONFIG_PORT];
 		$conType=isset($_service[SERVICE_CONNECTION_TYPE])?$_service[SERVICE_CONNECTION_TYPE]:$this->_device[DEVICE_CONFIG][CONFIG_CONNECTION_TYPE];
 		if(!$con=$this->GetConnection($conType)) return $this->error(ERR_CantGetConnection);
 		$host = $this->_device[DEVICE_CONFIG][CONFIG_HOST].':'.$port;
  		if(empty($RunTimeSec))$RunTimeSec="Infinite";else $RunTimeSec="Second-$RunTimeSec";
 		$request=$con->CreatePacket('SUBSCRIBE', $eventUrl ,[
 				'HOST'=>$host,	'SID'=>$EventSID, 'Subscription-ID'=>$EventSID,	'TIMEOUT'=>$RunTimeSec
 		],null);
 		$result=$con->SendPacket($host,$request);
		if(empty($result))return null;
		if($result['TIMEOUT'])$result['TIMEOUT']=intval(str_ireplace('Second-', '', $result['TIMEOUT']));
		if(empty($sid=$result['SID']) && empty($sid=$result['SUBSCRIPTION-ID']))return $this->error(ERR_InvalidResposeSID);
		return ['SID'=>$sid,'TIMEOUT'=>$result['TIMEOUT']];
 	}
 	public function UnRegisterEvent($EventSID, $Service) {
 		if(empty($Service))								  return $this->error(ERR_ServiceNameEmpty);
 		if(!$_service=$this->GetService($Service))		  return $this->error(Err_ServiceNotFound,$Service);
 		if(empty($eventUrl=$_service[SERVICE_EVENT_URL])) return $this->error(ERR_ServiceHasNoEvents,$Service);
 		$port=isset($_service[SERVICE_PORT])?$_service[SERVICE_PORT]:$this->_device[DEVICE_CONFIG][CONFIG_PORT];
 		$conType=isset($_service[SERVICE_CONNECTION_TYPE])?$_service[SERVICE_CONNECTION_TYPE]:$this->_device[DEVICE_CONFIG][CONFIG_CONNECTION_TYPE];
 		if(!$con=$this->GetConnection($conType)) return $this->error(ERR_CantGetConnection);
 		$host = $this->_device[DEVICE_CONFIG][CONFIG_HOST].':'.$port;
 		$request=$con->CreatePacket('UNSUBSCRIBE', $eventUrl ,[
 				'HOST'=>$host, 
// 				'Subscription-ID'=>$EventSID,
 				'SID'=>$EventSID
 		],null);
 		$result=$con->SendPacket($host,$request);
// exit(__FUNCTION__.": $host  => SID: $EventSID => ".debug::export($result));
		if(empty($result))return null;
//  		if(empty($result['SID']))return $this->error(ERR_InvalidResposeSID);
		return true;
 	}
	public function & GetConnection($conType=null){
		if(is_null($conType))$conType=@$this->_device[DEVICE_SERVICES][$this->_lastServiceName][SERVICE_CONNECTION_TYPE];
		if(is_null($conType))$this->debug(DEBUG_INFO,__FUNCTION__ . "ConType: ".(is_null($conType)?'null':$conType),1);
		if(is_null($conType))$conType=@$this->_device[DEVICE_CONFIG][CONFIG_CONNECTION_TYPE];
		if(is_null($conType))$this->debug(DEBUG_INFO,__FUNCTION__ . "ConType: ".(is_null($conType)?'null':$conType),2);
		if(is_null($conType))return $this->_connection;
		if(!is_null($this->_connection) && $this->_connection->ConnectionType()==$conType)return $this->_connection;
		$creditials=empty($this->_device[DEVICE_CONFIG][CONFIG_CREDITIALS])?['','','']:$this->_device[DEVICE_CONFIG][CONFIG_CREDITIALS];
		$conTypeName=static::ConnectionTypeName($conType);
		$this->debug(DEBUG_CALL, "Open $conTypeName connection to ".$this->_device[DEVICE_CONFIG][CONFIG_HOST],201);
		switch($conType){
			case CONNECTION_TYPE_SOAP: $this->_connection=new RPCSoapConnection($creditials,$conType);break;
			case CONNECTION_TYPE_JSON: $this->_connection=new RPCJSonConnection($creditials,$conType);break;
			case CONNECTION_TYPE_URL : $this->_connection=new RPCUrlConnection ($creditials,$conType);break;
			case CONNECTION_TYPE_XML : $this->_connection=new RPCXMLConnection($creditials,$conType);break;
		}
		if($this->_connection){
			$this->_connection->SetDebugLevel(self::$_debugLevel);
			$this->_connection->SetDebugHandler($this->_DebugRefObjectPtr,$this->_DebugRefFuncName);
		}
		return $this->_connection;
	}
	protected function clear($FullClear=false){
		$this->_device = null;
		$this->_connection = null;
		$this->_deviceFileName = '';
		$this->_isOnline = null;
		$this->_cachedEvents=null;
		$this->_cachedFunctionsServiceName=null;
		$this->_lastServiceName='';
		$this->_lastFunction = null;
	}
	
 	protected function dataTypeToPHPtype($DataType){
		static $convert=[DATATYPE_BOOL=>'bool',DATATYPE_INT=>'int',DATATYPE_UINT=>'int',DATATYPE_BYTE=>'int', DATATYPE_FLOAT=>'float',DATATYPE_STRING=>'string',DATATYPE_ARRAY=>'array',DATATYPE_OBJECT=>'object',DATATYPE_MIXED=>'',DATATYPE_UNKNOWN=>'unknown'];
 		return empty($convert[$DataType])?'':$convert[$DataType];
 	}
 	protected function loaded(){
		$this->debug(DEBUG_INFO,sprintf('Device %s Loaded',$this->_device[DEVICE_INFO][INFO_NAME]),501);
	}
	protected function hasOption($option){ 
		return $this->_device[DEVICE_CONFIG][CONFIG_OPTIONS] & $option; 
	}
	protected function createFunctionValues(array $funcParams_IN, array $arguments=[]){
		$boNumericKeys = count($arguments)==0 || is_numeric(key($arguments));
		$in_first=$in_defaults=$values=[];
		$boUseFirst = $this->hasOption(OPT_DEFAULTS_TO_END);
		foreach ($funcParams_IN as $param)if(isset($param[VALUE_DEFAULT]) ||!$boUseFirst)	$in_defaults[$param[VALUE_NAME]]=isset($param[VALUE_DEFAULT])?$param[VALUE_DEFAULT]:null; else $in_first[]=$param[VALUE_NAME];
		foreach($in_first as $pn){$values[$pn]=$boNumericKeys?array_shift($arguments):@$arguments[$pn];}
		foreach($in_defaults as $pn=>$value){if(is_null($values[$pn]=$boNumericKeys?array_shift($arguments):@$arguments[$pn]))$values[$pn]=$value;}
		return $values;
	}
	protected function createHelp(array $function, $functionDisplayName, $HelpMode, $HelpWidht = 80){
		$paramdesc=null;
		if(!empty($function[FUNCTION_PARAMS][PARAM_IN])) {
			$v=$this->createFunctionValues($function[FUNCTION_PARAMS][PARAM_IN]);
			$maxWidth=1;foreach ($function[FUNCTION_PARAMS][PARAM_IN] as $param)$maxWidth=max($maxWidth,strlen($param[VALUE_NAME])+2);
			
			foreach ($function[FUNCTION_PARAMS][PARAM_IN] as $param){
				$typ=$this->dataTypeToPHPtype($param[VALUE_TYPE]);
				$desc=isset($param[VALUE_DESC_ID])?$this->getDescription($param[VALUE_DESC_ID],50):'';
				if(isset($param[VALUE_DEFAULT]))$desc=sprintf('[%s] ',$param[VALUE_DEFAULT]).$desc;
				$paramdesc['in'][$param[VALUE_NAME]]=sprintf("   %{$maxWidth}s %-6s %s",'$'.$param[VALUE_NAME],$typ,$desc);
				$value=&$v[$param[VALUE_NAME]];
				$value=is_null($value)?"$typ \${$param[VALUE_NAME]}":"$typ \${$param[VALUE_NAME]}=".($typ=='string'?"\"$value\"":$value);
				if(isset($param[VALUE_MIN]))$value.=" [{$param[VALUE_MIN]}-{$param[VALUE_MAX]}]";
				elseif(isset($param[VALUE_LIST]))$value.=" [".implode('|', $param[VALUE_LIST])."]";
			}
			$in = implode(', ',$v);if($in)$in=" $in ";
		}else $in='';
		if(!empty($function[FUNCTION_PARAMS][PARAM_OUT])) {
			$out=[];
			$maxWidth=1;foreach ($function[FUNCTION_PARAMS][PARAM_OUT] as $param)$maxWidth=max($maxWidth,strlen($param[VALUE_NAME])+2);
			foreach( $function[FUNCTION_PARAMS][PARAM_OUT] as $param){
				$typ=isset($param[VALUE_TYPE])?$this->dataTypeToPHPtype($param[VALUE_TYPE]):'';
				$desc=isset($param[VALUE_DESC_ID])?$this->getDescription($param[VALUE_DESC_ID],50):'';
				$paramdesc['out'][$param[VALUE_NAME]]=sprintf("   %{$maxWidth}s %-6s %s",'$'.$param[VALUE_NAME],$typ,$desc);
				$out[]="$typ ".$param[VALUE_NAME];
			}
			$out=implode(', ',$out);
			if($out)if(count($function[FUNCTION_PARAMS][PARAM_OUT]) > 1)$out=" => array[ $out ]";else $out=" => $out";
		}else $out='';
		
		$desc=isset($function[FUNCTION_DESC_ID])?$this->getDescription($function[FUNCTION_DESC_ID]):false;
		if($HelpMode > HELP_SHORT && ( $desc!==false||$paramdesc)){
			$fmt='  | %-'.($HelpWidht-7).'s |';
			$formatDesc=function($str, $maxwidht=0) use($fmt,$HelpWidht){
				static $from=['ä','Ä','ö','Ö','ü','Ü','ß','·','°','„','“'];
				$maxwidht=$maxwidht?$maxwidht:$HelpWidht - 7;
				$to=[chr(200),chr(201),chr(202),chr(203),chr(204),chr(205),chr(206),chr(207),chr(208),chr(209),chr(210)];
				$string=str_replace($from, $to, str_replace(["\R","\t",'–'], ['','','-'], $str));
				if(strlen($string)<=$maxwidht){
					$return=[$string];
				}else foreach(explode("\n",$string) as $string){
					$string=rtrim($string);
					if(strlen($string)<=$maxwidht && strpos($string,"\n")===false){
						$return[]=$string;continue;
					}
					foreach(explode("\n",wordwrap($string,$maxwidht)) as $substring){
						$return[]=$substring;
					}
				}
				foreach($return as &$v)	$v=str_replace($to,$from,sprintf($fmt,$v));
				return $return;
			};
		
			$help[]='\*+'.str_repeat('-', $HelpWidht-5).'+';
			if($HelpMode > HELP_NORMAL && $paramdesc){
				$helpStart=count($help);
				$help[]='';
				$lineParams=[];
				
				$help[]=sprintf($fmt,' '.$this->GetMessage(MSG_PARAMS).':');
				if(isset($paramdesc['in'])){
					$help[]=sprintf($fmt,sprintf("  %5s:",$this->GetMessage(MSG_IN)));
					foreach($paramdesc['in'] as $k=>$v)	{
						$help=array_merge($help,$formatDesc($v));
						$lineParams[]="\$$k";
					}
				}	
				if(isset($paramdesc['out'])){
					$help[]=sprintf($fmt,sprintf("  %5s:",$this->GetMessage(MSG_OUT)));
					foreach($paramdesc['out'] as $k=>$v)$help=array_merge($help,$formatDesc($v));
				}
				$maxWidth=60 - strlen($functionDisplayName);
				while(strlen($params=implode(',',$lineParams))>$maxWidth){
					if(end($lineParams)=='...')array_pop($lineParams);
					$count=count($lineParams);
					if($count<1){
						$lineParams[$count]=substr($lineParams[$count],0,$maxWidth-3).'...';
						break;
					}
					$lineParams[$count-1]='...';
				}
				if(strlen($params)>$maxWidth)$params=substr($params,0,$maxWidth-3).'...';
				
				$help[$helpStart]=sprintf($fmt,sprintf('%s: %s(%s)',$this->GetMessage(MSG_FUNCTION),$functionDisplayName,$params));
			}
			if($desc){
				$help[]=sprintf($fmt,$this->GetMessage(MSG_DESCRIPTION));
				$help=array_merge($help,$formatDesc($desc));
			}
			$help[]='  +'.str_repeat('-', $HelpWidht-5).'+*/';
			//$help[]=" $desc";
		
		}else $help[]="$functionDisplayName ($in)$out";
		return $help;
	}
	protected function getDescription($DescriptionID, $WordWarp=0){
		if(empty($this->_device[DEVICE_DESCRIPTIONS]))	return $this->error("this Device have no Descriptions",524);	
		if(!is_array($this->_device[DEVICE_DESCRIPTIONS])){
			if(!file_exist(RPC_CONFIG_DIR."/".$this->_device[DEVICE_DESCRIPTIONS]))return $this->error(sprintf('Desriptionfile %s not found',RPC_CONFIG_DIR."/".$this->_device[DEVICE_DESCRIPTIONS]),524);
			if(!$desc=json_decode(file_get_contents(RPC_CONFIG_DIR."/".$this->_device[DEVICE_DESCRIPTIONS]))) return $this->error(sprintf('Invalid desriptionfile %s',RPC_CONFIG_DIR."/".$this->_device[DEVICE_DESCRIPTIONS]),524);
			utf8::decode_array($desc);
			$this->_device[DEVICE_DESCRIPTIONS]=$desc;
			$this->debug(DEBUG_INFO,"Descriptions load",522);
		}
		return empty($this->_device[DEVICE_DESCRIPTIONS][$DescriptionID])?"Description for ID $DescriptionID not found":($WordWarp?wordwrap($this->_device[DEVICE_DESCRIPTIONS][$DescriptionID],$WordWarp):$this->_device[DEVICE_DESCRIPTIONS][$DescriptionID]);
	}
	
	protected function toUrl(array $params){
		if(isset($params[CONFIG_HOST])){
			list($scheme,$host,$port,$path)=[$params[CONFIG_SCHEME],$params[CONFIG_HOST],$params[CONFIG_PORT],''];
		}elseif(isset($params['host'])){
			list($scheme,$host,$port,$path)=[$params['scheme'],$params['host'],$params['port'],$params['path']];
		}
		$url=$scheme?$scheme."://".$host:$host;
		if($port)$url.=":$port";
		if(empty($path)) return $url;
		return $url.=$port>0?($path[0]!='/'?'/'.$path:$path):($path[0]=='/'?substr($path,1):$path) ;
	}
	protected function callConnection($url,$function, $arguments, $filter=null, $service=null){
		if(!$con=$this->GetConnection())return $this->error(ERR_CantGetConnection);
		$url=$this->getUrl(	$this->_device[DEVICE_SERVICES][$this->_lastServiceName][SERVICE_PORT],	$this->_device[DEVICE_SERVICES][$this->_lastServiceName][SERVICE_CTRL_URL]);
		if(empty($service))$service=$this->_device[DEVICE_SERVICES][$this->_lastServiceName][SERVICE_ID];
		if(!empty($service) && $con->ConnectionType()==CONNECTION_TYPE_URL  && $service[0]=='?'){
			$cmds=explode('&', substr($service,1));
			foreach($cmds as &$command){
				$params=explode('=',$command);
				if(count($params)>1){
					if(empty($params[1]) || is_numeric($params[1]))continue;
					if (!$this->FunctionExist($params[1]))continue;
					$result=$this->{$params[1]}();
					if($this->HasError()) return $this->error("$service !! Invalid parameter found");
					$params[1]=$result;
				}
				$command=implode('=', $params);
			}
			$url.="?".implode('&', $cmds);
			$service='';
		}
		$result=$con->execute($url,$service, $function, $arguments, $filter);
		$this->MergeErrors($con); //if(is_null($result) && !$this->HasError())$result=false;
		return $result;
	}
	
	private function _callSource($FunctionName, $Source, array $Arguments, $filter=null){
		foreach($Arguments as $name=>&$arg)	$arg="\$$name=".var_export($arg,true).';';
		$Arguments=implode($Arguments);
		$Source=str_ireplace(['return '],'$F_RESULT=', $Source);
		$code=$Arguments.$Source;
		$this->debug(DEBUG_CALL, 'Code: '.debug::export($code),200);
//   		$old_handler = set_error_handler('RPCEvalHandler::CatchError');
		try {
			$F_RESULT=null;
			eval("\$that=\$this;".$code);
		}catch(ErrorHandlerExeption $e){
			$message=$e->GetMessage();
			$code=$e->getCode();
			if($code == 60008) { // Varable not found
				$source=debug::export($Source);
				if(preg_match('/: (\w+)/',$message,$m) || preg_match('/undefined constant (\w+)/',$message,$m)){
					$m=strpos($m[0],'constant')!==false?trim($m[1]):'$'.trim($m[1]);
					if($pos=strpos($source,$m)){
						$this->error($message,$code);
						$message=str_replace($m,"-->$m<--", $source);
						if($pos > 80)
							$message=substr($message,$pos-40,80).'....';
// 							$message='ERROR at '.$message;
					}
				}
			}
			$F_RESULT=$this->error($message,$code);
		}catch(Exception $e){
			$message=$e->GetMessage();
			$code=$e->getCode();
			$F_RESULT=$this->error($message,$code);
		}
//  		set_error_handler($old_handler);
		return $F_RESULT;
	}
	private function getUrl($port=null, $path=null){
		$url=$this->_device[DEVICE_CONFIG][CONFIG_SCHEME]."://".$this->_device[DEVICE_CONFIG][CONFIG_HOST];
		if(!$port)$port=$this->_device[DEVICE_CONFIG][CONFIG_PORT];
		if($port)$url.=":$port";
		if(empty($path)) return $url;
		return $url.=$port>0?($path[0]!='/'?'/'.$path:$path):($path[0]=='/'?substr($path,1):$path) ;
	}
	private function checkValue(array $param, $value){
		if(is_null($value))return $this->error(ERR_ParamIsEmpty, $param[VALUE_NAME]);
// echo "CheckValue {$param[VALUE_NAME]} $value\n";
		$updateMinMax=function($min,$max)use(&$param){
			if(empty($param[VALUE_MIN])||$param[VALUE_MIN]<$min)$param[VALUE_MIN]=$min;
			if(empty($param[VALUE_MAX])||$param[VALUE_MAX]>$max)$param[VALUE_MAX]=$max;
			if(empty($param[VALUE_STEP]))$param[VALUE_STEP]=1;
		};
		switch($param[VALUE_TYPE]){
			case DATATYPE_BOOL : if(!is_bool($value)) return $this->error(ERR_InvalidParamTypeBool,$param[VALUE_NAME]); break;
			case DATATYPE_BYTE : if(!is_int($value))  return $this->error(ERR_InvalidParamTypeUint,$param[VALUE_NAME]); $updateMinMax(0,255);break;
			case DATATYPE_INT  : if(!is_int($value))  return $this->error(ERR_InvalidParamTypeNum,$param[VALUE_NAME]);  $updateMinMax(-65535, 65535);	break;
			case DATATYPE_UINT : if(!is_int($value))  return $this->error(ERR_InvalidParamTypeUint,$param[VALUE_NAME]); $updateMinMax(0,4294836225);	break;
			case DATATYPE_FLOAT: if(!is_float($value))return $this->error(ERR_InvalidParamTypeNum,$param[VALUE_NAME]);  break;
		}
		if(isset($param[VALUE_MIN])){
			if($value < $param[VALUE_MIN])	   return $this->error(ERR_ValueToSmal,$value,$param[VALUE_NAME],$param[VALUE_MIN],$param[VALUE_MAX]);
			elseif($value > $param[VALUE_MAX]) return $this->error(ERR_ValueToBig,$value,$param[VALUE_NAME],$param[VALUE_MIN],$param[VALUE_MAX]);
		}
		if(isset($param[VALUE_LIST])){
			foreach($param[VALUE_LIST] as $pv)if($ok=$value==$pv)break;
			if(!$ok)return $this->error(ERR_ValueNotAllowed, $value ,$param[VALUE_NAME], implode(', ',$param[VALUE_LIST]));
		}
//if($this->_lastFunction[FUNCTION_NAME]=="RemoteControl") exit(var_dump($param,$value));
		return true;
	}

	private function getKeyCode($KEY_code){
		if(empty($this->_device[DEVICE_KEYCODES][$KEY_code]))return $this->error(ERR_DeviceHasNoKeyCodes, get_called_class(),$KEY_codes);
		return $this->_device[DEVICE_KEYCODES][$KEY_code];
	}
	private function & _functionList(){
		if($this->_cachedFunctionsServiceName)return $this->_cachedFunctionsServiceName;
		$this->_cachedFunctionsServiceName=[];
		foreach($this->_device[DEVICE_SERVICES] as $sn=>$service)
			foreach( $service[SERVICE_FUNCTIONS] as $fn=>$function)	$this->_cachedFunctionsServiceName[$fn]=&$this->_device[DEVICE_SERVICES][$sn][SERVICE_NAME];
 		ksort($this->_cachedFunctionsServiceName);
		return $this->_cachedFunctionsServiceName;
	}

}
?>