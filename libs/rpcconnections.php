<?php
abstract class RPCConnection implements iRpcLogger{
//	use ErrorHandler;
	protected $_creditials = [];
	protected $_logger = null;
	private $_connectionType = 0; 
	function __construct(array $creditials,$Logger , $ConnectionType){
		$this->_creditials=$creditials;	
		$this->_connectionType=$ConnectionType;
		if($Logger)$this->AttachLogger($Logger);
	}
	function __destruct(){
		$this->DetachLogger($this->_logger);
	}
	
	abstract public function Execute($url, $serviceID, $functionname,array $arguments, array $filter=null);
	public function AttachLogger(RpcLogger $Logger=null){
		if($Logger)$this->_logger=$Logger->Attach($this);
	}
	public function DetachLogger(RpcLogger $Logger=null){
		if($Logger && $Logger != $this->_logger )return;
		$this->_logger=$Logger?$Logger->Detach($this):$Logger;
	}
	public function ConnectionType(){return $this->_connectionType;}
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
	public static function CreatePacket( $Method, $Url='/', array $Arguments=null, $Content=null){
		$out=["$Method $Url HTTP/1.1"];
		if($Arguments)foreach($Arguments as $vN=>$v)$out[]="$vN: $v";
		if(!is_null($Content)){
			$out[]="CONTENT-LENGTH: ".strlen($Content);
			if($Content)$out[]=$Content;
		}
		return implode("\n",$out)."\n\n";
	}
	protected function error($Message, $ErrorCode=null, $Params=null /* ... */){
		if(!$this->_logger)return null;
		if(is_numeric($Message)){
			$Params=array_slice(func_get_args(),1);
		}
		elseif($Params && !is_array($Params))$Params=array_slice(func_get_args(),2);
		
		return $this->_logger->Error($ErrorCode, $Message, $Params);
	}
	protected function debug($DebugOption, $Message, $Params=null /* ... */){
		if(!$this->_logger)return;
		if($Params && !is_array($Params))$Params=array_slice(func_get_args(),2);
		$this->_logger->Debug($DebugOption, $Message, $Params);
	}
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
}
class RPCSoapConnection extends RPCConnection{
	function __construct(array $creditials,$Logger){
		parent::__construct($creditials,$Logger,CONNECTION_TYPE_SOAP);
	}
	public function Execute( $url,$serviceID, $functionname,array $arguments, array $filter=null){
		$params=array(
				'location' 	 => $url,
				'uri'		 => $serviceID,
				'noroot'     => true,
				'exceptions'=> false,
				'trace'		=> true
		);
		if($this->_creditials[0])	$params['login']=$this->_creditials[0];
		if($this->_creditials[1])	$params['password']=$this->_creditials[1];
		$client = new SoapClient( null,	$params);
		$params=array();
		foreach($arguments as $key=>$value)$params[]=new SoapParam($value, $key);
		$response = $client->__soapCall($functionname,$params);
		if(is_soap_fault($response))return $this->error($response->faultstring,$response->faultcode);
		return $response;
	}
	
}
class RPCUrlConnection extends RPCConnection{
	function __construct(array $creditials,$Logger){
		parent::__construct($creditials,$Logger,CONNECTION_TYPE_URL);
	}
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
}
abstract class RPCCurlConnection extends RPCConnection{
	private $_curl=null;
	abstract protected function encodeRequest($FunctionName, $Arguments);
	abstract protected function decodeRequest($Result);
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
// exit(var_dump($url));		
		curl_setopt($this->_curl, CURLOPT_POSTFIELDS, $postData);
		if(!$result = curl_exec($this->_curl))return $this->error(ERR_EmptyResponse);
// exit(var_dump($result));		
		return $this->decodeRequest($result);	
	}
}
class RPCXMLConnection extends RPCCurlConnection{
	function __construct(array $creditials,$Logger){
		parent::__construct($creditials,$Logger ,CONNECTION_TYPE_XML);
	}
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
	function __construct(array $creditials,$Logger){
		parent::__construct($creditials,$Logger,CONNECTION_TYPE_JSON);
	}
	protected $_requestID = null;
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

}

?>