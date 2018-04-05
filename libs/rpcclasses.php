<?php
/** @file rpcclasses.php
 * @brief Helper classes
 * 
 * @author Xaver Bauer
 * @date 21.01.2018
 * @version 2.0.1
 * Added PhpDoc Tags
 * @package rpcclasses
 * @brief Helper classes
 * @copydetails rpcclasses.php
 */

const
	URL_SCHEME 	= 1,
	URL_USER   	= 2,
	URL_PASS   	= 4,
	URL_HOST	= 8,
	URL_PORT	= 16,
	URL_PATH	= 32,
	URL_QUERY  	= 64,
	URL_FRAGMENT= 128, 
	
	URL_URLHOST = URL_SCHEME+URL_USER+URL_PASS+URL_HOST+URL_PORT;

	
/** @brief Url utillities 
 * 
 */
class URL {
/// @cond VARIABLES
	/// @cond VPROTECTED
	protected $_URL = ['scheme'=>'','user'=>'','pass'=>'','host'=>'','port'=>0,'path'=>'','query'=>'','fragment'=>''];
	/// @endcond
/// @endcond
	/**
	 * Create a new URL Object
	 * @param	string	$Host	Url_to_parse
	 */
	public function __construct($Host){
		$this->Set($Host);
	}
	/**
	 * @return string	Return object as UrlString
	 */
	public function __toString(){
		return $this->Get();
	}
	/**
	 * same as parse_url function but return all keys empty or not
	 *  
	 * @param string $Url Url to Parse
	 * @return null|string[] extracted Url parts ['scheme','user','pass','host'....]
	 
	 */
	public static function parse($Url, array $Excludes = null ){
		if(empty($Url))return null;
		$firstDel=($Url[0]=='/' || $Url[0]=='\\')?$Url[0]:'';
		if($firstDel)while(strlen($Url)>0 && $Url[0]==$firstDel)$Url=substr($Url, 1);
		$r=array_merge(['scheme'=>'','user'=>'','pass'=>'','host'=>'','port'=>0,'path'=>'','query'=>'','fragment'=>''],parse_url($Url));
		if($r['scheme'] && empty($r['host'] && (strpos($Url,':')||strpos($Url,'@'))))return static::parse('http://'.$Url);
		if(empty($firstDel) && empty($r['host']) && !empty($r['path'])){
			$r['host']=$r['path'];
			$r['path']='';
		}elseif(!empty($firstDel) && !empty($r['host'])){
			if(!$FillDefaults && empty($r['path']))$r['path']='';
			$r['path']=$r['host'].$r['path'];
			$r['host']=$firstDel;
		}elseif(!empty($firstDel))$r['host']= empty($r['host'])?$firstDel:$firstDel.$r['host'];
// 		$r['port']=intval($r['port']);
		if(!empty($Excludes))foreach($Excludes as $key)unset($r[$key]);
		return $r;		
	}
	/**
	 * @param string $Host set the hostpart of Url
	 * @return NULL|boolean
	 */
	public final function Set( $Host){
		if(empty($Host))return ($this->Clear()); 
		if(preg_match('/\<.*\>/',$Host)){
			$search=array_keys($this->_URL);
			$replace=array_values($this->_URL);
			foreach($search as &$v)$v="<$v>";
			$Host=str_ireplace($search, $replace, $Host);
		}
		$this->Clear();
		$url=static::parse($Host);
		if(empty($url['host']))return false;
		$this->_URL=$url;
		return true;
	}
	public final function Scheme($value=null){
		if(!is_null($value))$this->_URL['scheme']=$value;		
		return $this->_URL['scheme'];
	}
	public final function User($value=null){
		if(!is_null($value))$this->_URL['user']=$value;		
		return $this->_URL['user'];
	}
	public final function Pass($value=null){
		if(!is_null($value))$this->_URL['pass']=$value;		
		return $this->_URL['pass'];
	}
	public final function Host($value=null){
		if(!is_null($value))$this->_URL['host']=$value;		
		return $this->_URL['host'];
	}
	public final function Port($value=null){
		if(!is_null($value))$this->_URL['port']=$value;		
		if(empty($this->_URL))return false;
		return $this->_URL['port'];
	}
	public final function Path($value=null){
		if(!is_null($value))$this->_URL['path']=$value;		
		return $this->_URL['path'];
	}
	public final function Query($value=null){
		if(!is_null($value))$this->_URL['query']=$value;		
		return $this->_URL['query'];
	}
	public final function Fragment($value=null){
		if(!is_null($value))$this->_URL['fragment']=$value;		
		return $this->_URL['fragment'];
	}
	public final function Clear(){
		$this->_URL=['scheme'=>'','user'=>'','pass'=>'','host'=>'','port'=>0,'path'=>'','query'=>'','fragment'=>''];
	}
	public final function Get($WhatUrlOptions=0){
		if($WhatUrlOptions != 0){
			$keys=array_keys($this->_URL); $return=[];
			for($j=0; $j<9; $j++)if((1 << $j)&$WhatUrlOptions)$return[$keys[$j]] =$this->_URL[$keys[$j]];
		}else $return=$this->_URL;
		if(!empty($return['scheme']))$return['scheme']=$return['scheme'].'://';
		if(!empty($return['user'])||!empty($return['pas']))$return['user']=$return['user'].':';
		if(!empty($return['port']))$return['port']=':'.$return['port'];
		if(!empty($return['path']) && $return['path'][0]!='/')$return['path']='/'.$return['path'];
		if(!empty($return['query']))$return['query']='?'.$return['query'];
		if(!empty($return['fragment']))$return['fragment']='#'.$return['fragment'];
		return implode('',$return);
	}
}
/** @brief Ip utillities
 * 
 */
 class IP {
/// @cond VARIABLES
	const reg_ex = '/(([012]\d{1,2}|\d{1,2}).([012]\d{1,2}|\d{1,2}).([012]\d{1,2}|\d{1,2}).([012]\d[0-5]|\d{1,2}))/';	
	protected $_octeds = [0,0,0,0];
/// @endcond	
	function __construct($IPString=''){
		$this->Set($IPString);
	}
	function __toString(){
		return implode('.',$this->_octeds);
	}
	public static final function parse($String){
	    preg_match(self::reg_ex, $String,$m);
		return (!preg_match(self::reg_ex, $String,$m) || !static::valid($m[1]))?false:array_splice($m,2);
	}
	public static final function valid($IPString){
		return ip2long($IPString)!==false;
	}
	public static final function Range($FromIP, $ToIP){
		$FromIP="$FromIP";$ToIp="$ToIP"; // Converts Objects to String //  --- must return (__toString()) a valid ip 
		$start=ip2long($FromIP); $end=ip2long($ToIP);
		if($start===false||$end===false) return false;
		for($j=$start;$j<=$end;$j++){
			$ip=long2ip($j);
			preg_match(self::reg_ex,$ip,$octeds);
			if($octeds[4]==255||$octeds[4]==0)continue;
			$range[]=$ip;
		}	
		return $range;
	}
	public static final function RangeFromTo($BaseIP,$From=0,$To=254, $OctedNum=4){
		if($OctedNum<1||$OctedNum>4)return false;
		if($From<0||$From>255||$To<0||$To>255)return false;
		if($OctedNum==4 && $To>254)$To=254;
		if(!$o=static::parse($BaseIP))return false;
		$o[--$OctedNum]=$From;
		$From=implode('.', $o);
		$o[$OctedNum]=$To;
		return static::Range($From, implode('.',$o));				
	}
	
	public final function Get(){
		return implode('.',$this->_octeds);
	}
	public final function Set($IP){
		if(empty($IP)){$this->_octeds=[0,0,0,0];return true;}
		if(is_object($IP)){
			if ($IP instanceof IP ){
				$this->_octeds=$IP->_octeds;
				return true; 
			}else return false;
		}elseif(is_array($IP)){
			if($count($IP)!=3)return false;
			if(!static::valid(implode('.',$IP)))return false;
			$this->_octeds=$IP;
			return true;
		}//else if(!$this->Valid($IP)) return false;// && !$IP=net::ping($IP))return false;
		return (bool)$this->Octeds($IP);
	}
	public final function Octed($OctedNumber, $OctedValue =null, $AutoCorrect=false){
		if($OctedNumber<1)if(!$AutoCorrect)return false;else $OctedNumber=1;
		if($OctedNumber>4)if(!$AutoCorrect)return false;else $OctedNumber=4;
		if(is_null($OctedValue))return $this->_octeds[--$OctedNumber];
		$OctedValue=intval($OctedValue);
		if($OctedValue<0)if(!$AutoCorrect)return false;else $OctedValue=0;
		if($OctedValue>255)if(!$AutoCorrect)return false;else $OctedValue=255;
		$this->_octeds[--$OctedNumber]=$OctedValue;
		return true;
	}
	public final function OctedOne($OctedValue=null){
		return $this->Octed(1,$OctedValue);
	}
	public final function OctedTwo($OctedValue=null){
		return $this->Octed(2,$OctedValue);
	}
	public final function OctedThree($OctedValue=null){
		return $this->Octed(3,$OctedValue);
	}
	public final function OctedFour($OctedValue=null){
		return $this->Octed(4,$OctedValue);
	}
	public final function Octeds($IPString=null){
		if(empty($IPString))return $this->_octeds;
		if($ok=ip2long($IPString) && preg_match(self::reg_ex,$IPString,$octeds)){
			$octeds=array_splice($octeds, 2);
			for($j=0;$j<4;$j++)if(!$ok=$octeds[$j]>=0&&$octeds[$j]<256)break;
		}
		return $ok?$this->_octeds=$octeds:false;
	}
}
/** @brief Net utillities
 * 
 */
class NET {
/// @cond VARIABLES
	static $lastError = '';
/// @endcond	
	#TODO fix local_adress function
	public static function local_address(){
		static $myaddr=null;
		if(!$myaddr && $r=NET::arpExec(false,[NET::local_ip()]))$myaddr=[$key=key($r),$r[$key]];
		return $myaddr;
	}
	public static function local_ip(){ 
		return self::pingExec(gethostname());
	}

	public static final function ping($host, $timeout = 100) {
	    /* ICMP ping packet with a pre-calculated checksum */
	    $package = "\x08\x00\x7d\x4b\x00\x00\x00\x00PingHost";
	    $socket  = socket_create(AF_INET, SOCK_RAW, 1);
	    
	    socket_set_option($socket, SOL_SOCKET, SO_RCVTIMEO, array('sec' =>0, 'usec' =>  $timeout*1000));
	    if(!socket_connect($socket, $host, null))return false;
	    $ts = microtime(true);
	    if(!socket_send($socket, $package, strLen($package), 0))
	       $result=false;
	    elseif(@socket_read($socket, 255))
	        $result = (microtime(true) - $ts)*1000;
	    else $result = false;
	    
	    socket_close($socket);
	    return $result;
	}	

	public static final function pingExec($Host, $TimeoutSec=1, $Repeats=1){
		if(!$Host)return false;
		ob_start();
		exec(sprintf('ping -4 -a -n %d -w %d %s',$Repeats,$TimeoutSec,escapeshellarg($Host)), $res, $rval);
		ob_clean();
		if($rval != 0 || !$res) return false;
		$res=implode(' ',$res);
		if(preg_match('/ ([\w\d\.\-_]+) \[(.+)\]/',$res,$m))$Host=$m[2];
		elseif(preg_match('/((\d+)\.(\d+)\.(\d+)\.(\d+))/',$res,$m))$Host=$m[1];
		return $Host;
	}
	public static final function arpExec($ExtendedInfo=false, array $FilterIps=null){
		$ExtendedInfo=$ExtendedInfo===true?' -v':'';
		ob_start();
		exec("arp -a$ExtendedInfo",$res, $rval);
		ob_clean();
		if( $rval != 0 || !$res )return false;
		if(!is_null($FilterIps)){
			$res=implode('|',$res);
			$found=null;
			foreach($FilterIps as $ip){
				$cip=str_replace('.', '\.', $ip);
				if(preg_match("/$cip.+(([a-f\d]{2}-){5}([a-f\d]{2}))/i",$res,$m)){
					$found[$ip]=$m[1];
				}
			}
			return $found;
		}
		$interface=$interfaces=null;
 		$fclean=function($index)use($res){$r=&$res[$index];while(strpos($r,'  ')!==false)$r=str_replace('  ', ' ', $r);	return trim($r);};
		$addInterface=function($interface)use($ExtendedInfo, &$interfaces){
			if(!$interface || count($interface['clients'])==0)return;
			$interfaces[$interface['myip']]=$interface['clients'];
		};	
		for($index=0;$index < count($res);$index++){
 			if(empty($line=$fclean($index))){
				$addInterface($interface);
				$interface=null;
				$index++;
				$line=$fclean($index);
				if(preg_match('/:.((\d+)\.(\d+)\.(\d+)\.(\d+)).---.(0x(\w+))/',$line,$m))$interface=['myip'=>$m[1],'id'=>$m[6],'clients'=>[]];
				continue;
			}
			if(empty($interface))continue;
			if(preg_match('/((\d+)\.(\d+)\.(\d+)\.(\d+)) (([a-f\d]{2})-([a-f\d]{2})-([a-f\d]{2})-([a-f\d]{2})-([a-f\d]{2})-([a-f\d]{2})) (.*)/i',$line,$m)){
				$interface['clients'][]=['ip'=>$m[1],'mac'=>$m[6],'state'=>$m[13]];
			}elseif($ExtendedInfo && preg_match('/((\d+)\.(\d+)\.(\d+)\.(\d+)) .* (.*)/',$line,$m))$interface['clients'][]=['ip'=>$m[1],'mac'=>'','state'=>$m[6]];
		}
		$addInterface($interface);
 		if($interfaces && !$ExtendedInfo){
// 			$key=key($interfaces);
  			$interfaces=array_pop($interfaces);
  		}
		return $interfaces;
	}
	public static final function DiscoverNetworkIps($FromIP=null, $ToIP=null){
		if($FromIp && $ToIp){
			if(!$range=IP::Range($FromIP, $ToIP))return;
		}else{
			if(!$FromIP && !$FromIP=NET::local_ip())return;
			if(!$range=IP::RangeFromTo($FromIP))return;
		}
		$result=null;
		foreach($range as $ip)
			if($t=NET::ping($ip,10))$result[$ip]=[$t,gethostbyaddr($ip)];	
		return $result;
	}
	public static final function DiscoverSsdpDevices($timeout = null, $IncludeInfo = true, $IncludeSsdpUrls = true) {
		if (is_null ( $timeout )) $timeout = 5;
		$request = "M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1900\r\nMAN: \"ssdp:discover\"\r\nMX: 10\r\nST: ssdp:all\r\nUSER-AGENT: MacOSX/10.8.2 UPnP/1.1 PHP-UPnP/0.0.1a\r\n\r\n";
		$socket = socket_create ( AF_INET, SOCK_DGRAM, 0 );
		socket_set_option ( $socket, SOL_SOCKET, SO_BROADCAST, true );
		socket_sendto ( $socket, $request, strlen ( $request ), 0, '239.255.255.250', 1900 );
		socket_set_option ( $socket, SOL_SOCKET, SO_RCVTIMEO, array ('sec' => 1 , 'usec' => '0' ) );
		$read = [ $socket ];
		$write = $except = $find = $check = [ ];
		$name = $port = $info=null;
		$response = '';
		while ( socket_select ( $read, $write, $except, 1 ) && $read ) {
			socket_recvfrom ( $socket, $response, 2048, null, $name, $port );
			if (is_null ( $response )) continue;
			foreach ( explode ( "\r\n", $response ) as $row ) {
				if (stripos ( $row, 'loca' ) === 0) {
					$location = str_ireplace ( 'location: ', '', $row );
					$url = array ('scheme' => '','host' => '','port' => '','path' => '','query' => '');
					$url = array_merge ( $url, parse_url ( $location ) );
					if (in_array ( $location, $check ) === false) {
						$check [] = $location;
						if($IncludeInfo || $IncludeSsdpUrls){
							$xml=simplexml_load_file($location);
							$info=$IncludeInfo?XML::LoadServiceInfo ( $xml ):'';
						}	$urls=$IncludeSsdpUrls?XML::GetScpdUrls($xml->device[0]):'';
						
						
						$find [] = ['URL' =>$location,'INFO'=>$info,'SCPD'=>$urls ];
					}
				}
			}
		}
		return $find;
	}
}
/** @brief Xml utillities
 * 
 */
class XML {
	public static function extractFromItem($XmlItem, array $Allowed){
		$result=null;
		$toValue=function($value){return !is_numeric($value)?$value:(is_float($value)?floatval($value):intval($value));};
		foreach($Allowed as $key=>$value){
			if(is_numeric($key)){unset($Allowed[$key]);$Allowed[$value]=$key=$value;}
			if(isset($XmlItem->{$key}))$result[$value]=$toValue((string)$XmlItem->{$key});
		}
		if(is_null($result))foreach($XmlItem as $key=>$value){
			$v=trim((string)$value);
			if($v=='' &&!empty($value[0])){
				$v=null;
				foreach($value as $vkey=>$val){
					if(!empty((string)$val))
						$v[]=(string)$val;	
				}
			}
			if($v && $keyID=@$Allowed[(string)$key])
				$result[$keyID]=is_array($v)?$v:$toValue($v) ;
		}
		if($result==null){
			exit(var_dump($XmlItem,$Allowed));
		}
		return $result;
	}
	public static function GetScpdUrls(SimpleXMLElement $XmlDevice){
		$url=[];
		if(!empty($XmlDevice->serviceList)){
			foreach($XmlDevice->serviceList->service as $service)
				if(!empty($SCPDURL=(string)$service->SCPDURL))$url[]=$SCPDURL;
		}
		if(!empty($XmlDevice->deviceList)){
			foreach($XmlDevice->deviceList->device as $device){
				if(!empty($device->serviceList)) $url+=static::GetScpdUrls($device);
				if(!empty($device->deviceList))
					foreach($device->deviceList->device as $d)$url+=static::GetScpdUrls($d);
			}
		}
		return $url;
	}
	public static function LoadServiceInfo($xmlOrFilename){
		static $convertInfo=['deviceType','friendlyName','manufacturer','manufacturerURL','modelDescription','modelName','modelNumber', 'modelURL','serialNumber','UDN'];
		if(!is_object($xmlOrFilename) )	$xmlOrFilename=simplexml_load_file($xmlOrFilename);
		if(empty($xmlOrFilename))return null;
		$info=static::extractFromItem(empty($xmlOrFilename->device[0])?$xmlOrFilename->device:$xmlOrFilename->device[0],$convertInfo);
		if(!empty($info['deviceType'])){
			$t=explode(':', $info['deviceType']);
			while (($v=array_pop($t)) && is_numeric($v));
			$info['deviceType']=$v?$v:$s;
		}
		return $info;
	}	

}
/** @brief Utf8 utillities
 * 
 */
class UTF8 {
	public static function encode_array(array &$Array){
		$strencode = function(&$item, $key) {
			if (is_object($item))static::encode_object($item);
			elseif(is_array($item))array_walk_recursive($item, $strencode);
			elseif(is_string($item) )$item=utf8_encode($item); 
		};
		array_walk_recursive($Array, $strencode);
	}
	public static function decode_array(array &$Array){
		$strdecode = function(&$item, $key) {
			if(is_object($item))static::decode_object($item);
			elseif(is_array($item))array_walk_recursive($item, $strdecode);
			elseif(is_string($item))$item=utf8_decode($item);	
		};
		array_walk_recursive($Array, $strdecode);
	}
	public static function encode_object(stdClass &$Object){
		foreach(get_object_vars($Object) as $key=>$item){
			if(is_object($item))static::encode_object($Object->$key);
			elseif(is_array($item))static::encode_array($Object->$key);
			elseif(is_string($item))$Object->$key=utf8_encode($Object->$key);
		}
	}
	public static function decode_object(stdClass &$Object){
		foreach(get_object_vars($Object) as $key=>$item){
			if(is_array($item))static::decode_array($Object->$key);
			elseif(is_string($item))$Object->$key=utf8_decode($item);
			elseif(is_object($item))static::decode_object($Object->$key);
		}
	}
	public static function encode(&$Item){
		if(is_object($Item))static::encode_object($Item);
		elseif(is_array($Item))static::encode_array($Item);
		elseif(is_string($Item))$Item=utf8_encode($Item);
	}
	public static function decode(&$Item){
		if(is_object($Item))static::decode_object($Item);
		elseif(is_array($Item))static::decode_array($Item);
		elseif(is_string($Item))$Item=utf8_decode($Item);
	}
}
/** @brief Debug utillities
 * 
 */
class DEBUG {
	public static function export($var, $LineBreak='', $MaxLength=0){
		$r=str_replace(["\t",'  ',"\n",'( ',') ',' => ',', ',',)',"\r"],[' ',' ',$LineBreak,'(',')','=>',',',')',$LineBreak], var_export($var,true));		
		return $MaxLength&&strlen($r)>$MaxLength?substr($r,0,$MaxLength-3).'...':$r;
	}
	public static function as_array($array){
		if(!is_array($array)) $array=is_object($array)?(array)$array:[$array];
		$out=[];
		foreach($array as $key=>$item)
			if (is_array($item))$out[]='['.static::as_array($item).']';	else $out[]=is_bool($item)?($item?'true':'false'):$item;
		return implode(',',$out);
	}
	public static function as_string(string $s, $Rows = 2){
		$out=[];$oc=0;$count=strlen($s);
		$row = $Rows; $spacer=($row>1)?'    ':'';
		for($j=0; $j<$count;$j++){
			$c=$s[$j];	$ord=ord($c);$hex=dechex($c);
			if($c=="\n"||$c=="\r"||$c=="\f")$c='CR';
			$spacer=($Rows>1&&$row<>$Rows)?'  | ':'';
			$out[$oc]=sprintf("$spacer S[%3d] = %-2s : %03s : %02x",$j,$c,$ord,$hex);
			if($row-- < 1){ 
				if($Rows>1)$out[$oc++].=" Text: ".substr($s,$j-$Rows, $Rows+1);
				$row = $Rows;
			}
		}		
		if($row<1)$out[$oc].=" Text: ".substr($s,$j-$Rows, $Rows+1);
		return implode("\n",$out);
	}
}

/** @brief Cache for remote import 
 * 
 */
class Cache {
/// @cond VARIABLES
	/// @cond VPROTECTED
	protected $_cachePath = __DIR__ . '/';
	/// @endcond
	public $Hits = ['cache'=>0,'remote'=>0];
/// @endcond
	function __construct($CachePath){
		$this->SetCachePath($CachePath);
	}
	public function Load($Name, $AsXML=false, $force=false){
		$fn = $this->_buildCacheFilename($Name);
		if($force || !file_exists($fn)){
			$content=file_get_contents($Name);
			$this->Hits['remote']++;
			file_put_contents($fn, $content);
			return $AsXML?simplexml_load_string($content):$content;
		}else { $Name=$fn; $this->Hits['cache']++;}
		return $AsXML?simplexml_load_file($Name):file_get_contents($Name);
	}
	public function SetCachePath($CachePath){
		if(!empty($CachePath) && $CachePath[strlen($CachePath)-1]!='/')$CachePath.='/';
		if($this->_cachePath=$CachePath)@mkdir($CachePath,755);
// 		return $this;
	}
	
	private function _buildCacheFilename($filename, $defExt='xml'){
		$ext=@pathinfo(parse_url($filename)['path'])['extension'];
		if($ext)$filename=str_replace('.'.$ext, '', $filename);else $ext=$defExt;
		return $this->_cachePath . str_replace([':','//','/','.'],['_','','_','-'], $filename).'.'.$ext;
	}
	
}



?>