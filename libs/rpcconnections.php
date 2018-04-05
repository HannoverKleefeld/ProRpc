<?php
/** @file rpcconnections.php
 * @brief Classes to make RpcApi calls
 * 
 * PHP Classes to make RpcApi Calls
 * @author Xaver Bauer
 * @date 31.03.2018
 * @version 2.0.2
 * - Fix error in connection when no logger is given
 * @date 21.01.2018
 * @version 2.0.1
 * Added PhpDoc Tags
 * @package rpcconnection
 * @brief Classes to make RpcApi calls
 * @copydetails rpcconnections.php
 */

/** @brief Base connection class
 * 
 * PHP Base Connection Class
 * @author Xaver Bauer
 * @date 21.01.2018
 * @version 2.0.1
 * Added PhpDoc Tags
 * @subpackage RpcConnection
 */
abstract class RpcConnection implements iRpcLogger{
/// @cond VARIABLES
	/// @cond VPROTECTED
	/**
	 * @param array Login Creaditials
	 */
	protected $_creditials = []; 
	/**
	 * @param RpcLogger Error and Debug handler object
	 */
	protected $_logger = null;
	/// @endcond
	/// @cond VPRIVATE
	/**
	 * @param int $_connectionType Current @ref types_connection
	 */
	private $_connectionType = 0;
	/// @endcond
/// @endcond	
	/** @brief Create a new RpcConnection object
	 * @param array $creditials Current login creditials
	 * @param object $Logger Error and Debug RpcLogger object
	 * @param int $ConnectionType Current @ref types_connection
	 * @see RpcApi::SetCreditials
	 */
	function __construct(array $creditials, RpcLogger $Logger = null, int $ConnectionType){
		$this->_creditials=$creditials;	
		$this->_connectionType=$ConnectionType;
		if($Logger)$this->AttachLogger($Logger);
	}
/// @cond PRIVATE
	/**
	 * detach attached Logger then remove this object from memory 
	 */
	function __destruct(){
		$this->DetachLogger($this->_logger);
	}
/// @endcond
	/** @brief Execute a call
	 * @param string $url Url to service
	 * @param string $serviceID Service identification
	 * @param string $functionname Functionname to call
	 * @param array $arguments @ref how_to_args
	 * @param null|array $filter Array of Strings to include in result array   
	 * @return mixed Result from function call
	 */
	abstract public function Execute($url, $serviceID, $functionname,array $arguments, array $filter=null);
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
	/** @brief Get connection type
	 * @return int the current @ref types_connection
	 */
	public function ConnectionType(){return $this->_connectionType;}
	/** @brief Send packet create with @ref CreatePacket
	 * 
	 * Make a plain call to userspezified url. 
	 * The content must be created by @ref CreatePacket 
	 * @param string $url Url to send to
	 * @param string $content Content to send , created by CreatePacked
	 * @return NULL|string the result of call or null if error 
	 */
	public function SendPacket( $url,  $content ){
		$p=parse_url($url);
		$port=empty($p['port']) ? 80 : $p['port'];
		$host=empty($p['path']) ? $p['host'] : $p['path'];
		$fp = @fsockopen($host, $port, $errno, $errstr, 1);
		if(!$fp)return $this->error(ERR_OpenSoketTo,$host,$port,$errstr,$errno);
		$this->debug(DEBUG_CALL+DEBUG_DETAIL,'send packet =>'.debug::export($content,'|'),509);
		$size=fputs ($fp,$content);
		$this->debug(DEBUG_CALL,'send packet size =>'.$size,509);
		stream_set_timeout ($fp,1);
		$response = ""; $retries=2;
		while (!feof($fp)){
			$response.= fgetss($fp,128); // filters xml answer
			if(--$retries == 0 && !$response)break;
		}
		fclose($fp);
		$this->debug(DEBUG_CALL+DEBUG_DETAIL,'send packet return =>'.($response?'true':'false'),509);
		return $this->decodePacket($response);
	}
	/** @brief Create packet to send with @ref SendPacket
	 * @param string $Method methods to implement in Header 'GET','NOTIFY', ....
	 * @param string $Url Url to implement in Header. Deafult='\'
	 * @param array $Arguments Arguments to implement in Header. Default=null 
	 * @param string $Content Content to send. Default=null
	 * @return string A formated Header Packet to send with SendPacket
	 */ 
	public static function CreatePacket( $Method, $Url='/', array $Arguments=null, $Content=null){
		$out=["$Method $Url HTTP/1.1"];
		if($Arguments)foreach($Arguments as $vN=>$v)$out[]="$vN: $v";
		if(!is_null($Content)){
			$out[]="CONTENT-LENGTH: ".strlen($Content);
			if($Content)$out[]=$Content;
		}
		return implode("\n",$out)."\n\n";
	}
/// @cond POTECTED
	/** @brief Logs an error
	 * @param int|string $Message Message-number or text to output
	 * @param int|mixed $ErrorCode ErrorCode or mixed Param to output
	 * @param mixed $Params Params to replace in Message 
	 * @return NULL
	 * 	
	 * <b>Demo:</b><br>
	 \code
	 $this->error('This is a simple error'); // logs 'This is a simple error'
	 $this->error('name %s has error ','The Name'); // logs 'name The Name has error' 
	 $this->error(ERR_FunctionNotFound,'Function Name'); // logs translated id ERR_FunctionNotFound 
	 $this->error('%s and %s are %s its %s',1,2,3,'cool'); // logs '1 and 2 are 3 its cool'
	 \endcode
	 *  @see RpcLogger::SetLogOptions
	 */
	protected function error($Message, $ErrorCode=null, $Params=null /* ... */){
		if(!$this->_logger)return null;
		if(is_numeric($Message)){
			$Params=array_slice(func_get_args(),1);
		}
		elseif($Params && !is_array($Params))$Params=array_slice(func_get_args(),2);
		
		return $this->_logger->Error($ErrorCode, $Message, $Params);
	}
	/** @brief Logs an debug message
	 * 
	 * logs an debug message if given $DebugOption in own options  
	 * @param int $DebugOption @ref options_logger 
	 * @param int|string $Message Message-number or Text to debug 
	 * @param mixed $Params params to replace in message
	 * @return null
	 * 
	 * <b>Demo:</b><br>
	 * <code>
	 * $this->debug(DEBUG_INFO,'This is a error');<br>
	 * $this->debug(DEBUG_ERRORS ,'name %s has error ','The Name');<br> 
	 * $this->debug(DEBUG_CALL,ERR_FunctionNotFound,'Function Name');<br>
	 * $this->debug(DEBUG_INFO + DEBUG_DETAIL,'%s and %s are %s its %s',1,2,3,'cool');<br>
	 * </code>
	 * @see RpcLogger::SetLogOptions
	 */
	protected function debug($DebugOption, $Message, $Params=null /* ... */){
		if(!$this->_logger)return;
		if($Params && !is_array($Params))$Params=array_slice(func_get_args(),2);
		$this->_logger->Debug($DebugOption, $Message, $Params);
	}
/// @endcond
/// @cond PRIVATE
	/** @brief Method to decode calls
	 * 
	 * This is the standard method to decode calls send with SendPacket. Extract Headervalues und content from Plain/Text returns 
	 * @param string $Result Result from SendPacket
	 * @return NULL|string[] Decoded packet values as array of [Key=>Value]
	 */
	private function decodePacket( $Result){
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
					$b=strtoupper(trim(array_shift($m)));
					if($b=='SUBSCRIPTION-ID')$b='SID';
					$data[$b]=trim(implode(':',$m));
				}
			}	
		}
		if(is_null($data))return $this->error(ERR_InvalidResponceFormat,'HTTP-HEADER');
		return $data;			
	}	
/// @endcond
}
/** @brief Soap connection class
 * 
 * PHP Soap Connection Class
 * @author Xaver Bauer
 * @date 21.01.2018
 * @version 2.0.1
 * Added PhpDoc Tags
 * @subpackage RpcSoapConnection
 */

class RpcSoapConnection extends RpcConnection{
	/**
	 * {@inheritDoc}
	 */
	function __construct(array $creditials,$Logger){
		parent::__construct($creditials,$Logger,CONNECTION_TYPE_SOAP);
	}
	/**
	 * {@inheritDoc}
	 * @see RpcConnection::Execute()
	 */
	public function Execute( $url,$serviceID, $functionname,array $arguments, array $filter=null){
		$params=array(
//  				'soap_version'   => SOAP_1_1,
				'location' 	 => $url,
				'uri'		 => $serviceID,
 				'noroot'     => true,
				'exceptions'=> false,
 				'trace'		=> false,
 				'connection_timeout'=>10
		);
		if($this->_creditials[0])	$params['login']=$this->_creditials[0];
		if($this->_creditials[1])	$params['password']=$this->_creditials[1];
		
		$client = new SoapClient( null,	$params);
		$params=array();
		foreach($arguments as $key=>$value)$params[]=new SoapParam($value, $key);
		try {
			$response = $client->__soapCall($functionname,$params);
			if(is_soap_fault($response))return $this->error($response->faultstring,$response->faultcode);
		}catch(RpcErrorHandler $e){
			$msg=trim(str_ireplace(['SoapClient::__doRequest()','in ',' source code'], '', $e->getMessage()));
			return $this->error($msg,$e->getCode());	
		}
		return $response;
	}
	
}

/** @brief Url connection class
 * 
 * PHP Url Connection Class
 * @author Xaver Bauer
 * @date 21.01.2018
 * @version 2.0.1
 * Added PhpDoc Tags
 * @subpackage RpcUrlConnection
 */

class RpcUrlConnection extends RpcConnection{
	/**
	 * {@inheritDoc}
	 * @see RpcConnection::__construct($creditials,$Logger)
	 */
	function __construct(array $creditials, $Logger){
		parent::__construct($creditials, $Logger,CONNECTION_TYPE_URL);
	}
	/**
	 * {@inheritDoc}
	 * @see RpcConnection::Execute()
	 */
	public function Execute( $url, $serviceID, $functionname,array $arguments, array $filter=null){
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
			if($r)UTF8::decode($result);
		}
		if(isset($result['error'])){
			return $this->error($result['error']['message']);
		}elseif(isset($result['resulttext'])){
			if(!$result['result'])return $this->error($result['resulttext']);
			unset($result['result'],$result['resulttext']);
		}
		return is_array($result)&&count($result)==0?true:$result;
	}
/// @cond PROTECTED
	/** Filter a xml callresult by filter
	 * @param string $Subject Call result as xml string
	 * @param array $Filter Array of Strings to include in result array
	 * @return array|boolean KeyValuePair as array or if $Filter set false if no Filtervalues passed 
	 */
	protected function Filter($Subject, $Filter){
		if(!empty($Filter[FILTER_PATTERN_REMOVE])){
			$patternRemove=$Filter[FILTER_PATTERN_REMOVE];	unset($Filter[FILTER_PATTERN_REMOVE]);
		}else $patternRemove='';//else $PatternRemove='.+:';
		if($patternRemove)$Subject=preg_replace("/\<$patternRemove/i", '<', preg_replace("/\<\/$patternRemove/i", '</',$Subject));
		if(!$c=count($Filter))return $Subject;
		$StringToType=function ($var){
			if(is_string($var)){
				if(is_numeric($var))$var=is_float($var)?floatval($var):intval($var);
				else if($var=='true'||$var=='True'||$var=='TRUE')$var=true;
				else if($var=='false'||$var=='False'||$var=='FALSE')$var=false;
			}
			return $var;	
		};
		if($c==1 && $Filter[0]=='*'){
			$n=json_decode(json_encode(simplexml_load_string($Subject)),true);
			if($n && count($n)==1)$n=array_shift($n);
			foreach ($n as $k=>&$var)if(!is_array($var))$var=$StringToType($var);
			return $n;
		}
		$multi=(count($Filter)>1);
		foreach($Filter as $pat){
			if(!$pat)continue;
			preg_match('/\<'.$pat.'\>(.+)\<\/'.$pat.'\>/i',$Subject,$matches);
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
/// @endcond
}

/** @brief Abstract curl connection class
 * 
 * PHP Base Curl Connection Class
 * @author Xaver Bauer
 * @date 21.01.2018
 * @version 2.0.1
 * Added PhpDoc Tags
 * @subpackage RpcCurlConnection
 */

abstract class RpcCurlConnection extends RpcConnection{
/// @cond VARIABLES
	/// @cond VPRIVATE
	/**
	 * @param resource CurlID holder for curl calls 
	 */
	private $_curl=null;
	/// @endcond
/// @endcond	
/// @cond PROTECTED
	/** Prepare/create a call content
	 * @param string $FunctionName Functionname to call
	 * @param array $Arguments Arguments for call
	 * @return string Content for send to device
	 */
	abstract protected function encodeRequest($FunctionName, $Arguments);
	/**
	 * @param string $Result Function call result
	 * @return null|array KeyValuePairs array of call return or null if error 
	 */
	abstract protected function decodeRequest($Result);
/// @endcond
	
	/**
	 * {@inheritDoc}
	 * @see RpcConnection::Execute()
	 */

	public function Execute( $url, $serviceID, $functionname,array $arguments, array $filter=null){
		if(is_null($this->_curl)){
			$this->_curl=curl_init();
			curl_setopt($this->_curl, CURLOPT_URL, $url);
			if($this->_creditials[0] || $this->_creditials[1]){
				curl_setopt($this->_curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
				curl_setopt($this->_curl, CURLOPT_USERPWD, $this->_creditials[0]. ":" . $this->_creditials[1]);
			}
			if(empty($this->_creditials[2])){
				curl_setopt($this->_curl, CURLOPT_SSL_VERIFYPEER, 0);
				curl_setopt($this->_curl, CURLOPT_SSL_VERIFYHOST, 0);
			}else {
				curl_setopt($this->_curl, CURLOPT_SSL_VERIFYPEER, true);
				curl_setopt($this->_curl, CURLOPT_CAINFO,$this->_creditials[2]);
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

/** @brief Xml connection class
 * 
 * PHP Xmx Connection Class
 * @author Xaver Bauer
 * @date 21.01.2018
 * @version 2.0.1
 * Added PhpDoc Tags
 * @subpackage RpcXMLConnection
 */

class RpcXMLConnection extends RpcCurlConnection{
	/**
	 * {@inheritDoc}
	 * @see RpcConnection::__construct($creditials,$Logger)
	 */
	function __construct(array $creditials,$Logger){
		parent::__construct($creditials,$Logger ,CONNECTION_TYPE_XML);
	}
/// @cond PROTECTED
	
	/**
	 * {@inheritDoc}
	 * @see RpcCurlConnection::encodeRequest()
	 */
	protected function encodeRequest($FunctionName, $Arguments){
		$request=xmlrpc_encode_request($FunctionName,$Arguments,['encoding'=>'utf-8']);
		return $request;
	}
	/**
	 * {@inheritDoc}
	 * @see RpcCurlConnection::decodeRequest()
	 */
	protected function decodeRequest($Result){
		if(empty($Result)) return $this->error(ERR_EmptyResponse);
		$Result = xmlrpc_decode($Result, "utf-8");
		if (is_null($Result)) return $this->error(ERR_InvalidResponceFormat,'xml');
		if(!empty($Result['faultCode'])||!empty($Result['faultString']))
			return $this->error(@$Result['faultString']?$Result['faultString']:'unknown error',$Result['faultCode']?$Result['faultCode']:110);
		return $Result;
		
	}	
/// @endcond
}	

/** @brief Json connection class
 * 
 * PHP Json Connection Class
 * @author Xaver Bauer
 * @date 21.01.2018
 * @version 2.0.1
 * Added PhpDoc Tags
 * @subpackage RpcJsonConnection
 */

class RpcJsonConnection extends RpcCurlConnection{
/// @cond VARIABLES
	/// @cond VPROTECTED
	/**
	 * @param null|int Last send ID to compare with receive ID. if not equal then error
	 */
	protected $_requestID = null;
	/// @endcond
/// @endcond
	/**
	 * {@inheritDoc}
	 * @see RpcConnection::__construct($creditials,$Logger)
	 */
	function __construct(array $creditials,$Logger){
		parent::__construct($creditials,$Logger,CONNECTION_TYPE_JSON);
	}
/// @cond PROTECTED
	protected function encodeRequest($FunctionName, $Arguments){
		if (!is_scalar($FunctionName)) return $this->error(ERR_MethodNoScala);
		if (!is_array($Arguments)) return $this->error(ERR_FormatArray);
		$params = array_values($Arguments);
		utf8::encode_array($params);
		return json_encode(["jsonrpc" => "2.0","method" => $FunctionName,"params" => $params,"id" => $this->_requestID = round(fmod(microtime(true)*1000, 10000))]);
	}
	protected function decodeRequest($Result){
		if($Result=== false)return $this->error(ERR_RequestEmptyResponse);
		$Response= json_decode($Result, true);
		if (is_null($Response)) return $this->error(ERR_InvalidResponceFormat,'json');
		utf8::decode_array($Response);
		if (isset($Response['error'])) return $this->error($Response['error']['message']);
		if (!isset($Response['id'])) return $this->error(ERR_NoResponseID);
		if ($Response['id'] != $this->_requestID)return $this->error(ERR_InvalidResponseID,$this->_requestID,$Response['id']);
		return $Response['result'];
	}
/// @endcond
}

?>