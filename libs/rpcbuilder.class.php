<?php
#TODO Transfer all debug Messages to messages.inc


/**
 * @author Xavier
 *
 */
class RPCBuilder extends RPCDevice{
	const  discover_user_agent = 'MacOSX/10.8.2 UPnP/1.1 PHP-UPnP/0.0.1a';
	static $VERSION = 1.87;
	protected $_cachePath = '';
	protected $_useFileCache = true;
	protected $_defaultValues = [];
	protected $_creditials = [CREDIT_USER=>'',CREDIT_PASS=>'',CREDIT_CAFILE=>''];
	protected $_options = OPT_CHECK_PARAMS + OPT_RESULT_FILTER + OPT_INCLUDE_DESC;
	protected $_connectionType = CONNECTION_TYPE_SOAP;
	private $_infoComplete= false;
	private $_importConfig= null;
	private $_serviceDefaults=[];
	
	public function clear($FullClear=false){
		parent::clear($FullClear);
		$this->_infoComplete= false;
		$this->_importConfig= null;
		$this->_serviceDefaults=[];
		$this->_connectionType = CONNECTION_TYPE_SOAP;
		if($FullClear){
			$this->_cachePath = '';
			$this->_useFileCache = true;
			$this->_defaultValues = [];
			$this->_creditials = [CREDIT_USER=>'',CREDIT_PASS=>'',CREDIT_CAFILE=>''];
			$this->_options = OPT_CHECK_PARAMS + OPT_RESULT_FILTER + OPT_INCLUDE_DESC;
		}
		$this->debug(DEBUG_BUILD, "API reset successfully");
	}
	
	
	function __construct(string $deviceJsonConfigFileName=null, string $cachePath=null){
		require_once LIB_INCLUDE_DIR . '/config/rpc.predefined.inc';
		parent::__construct($deviceJsonConfigFileName);
		if(!is_null($cachePath))$this->setCachePath($cachePath);
	}
	public function SetCreditials(string $user=null, string $pass=null, string $caFile=null){
		$this->_creditials=[CREDIT_USER=>$user,CREDIT_PASS=>$pass,CREDIT_CAFILE=>$caFile];
		parent::SetCreditials($user, $pass,$caFile);
	}
	public function SetOptions(int $options, bool $set=null){
		$c=&$this->_options;
		$c= (is_null($set)||$set===true) ? $c | $options : $c - ($c & $options);
		parent::SetOptions($options,$set);
	}
	
	public function SetCachePath(string $path){
		if ($this->_useFileCache=!empty($path) && $path[strlen($path)-1]!='/')$path.='/';
		$this->_cachePath=$path;
		if($this->_useFileCache)@mkdir($path,755);
	}
	public function AddParamDefaultValue(string $paramName, $value){
		if(is_array($value)){
			if(is_string(key($value)))foreach($value as $pn=>$value) $this->addParamDefaultValue($pn, $value);
		}else {
			$this->_defaultValues[$paramName]=[VALUE_DEFAULT=>$value];
		}
	}
	public function AddToCache(string $source, string $host, $port=0){
		if(!$this->_useFileCache)return $this->error(ERR_NotInCacheMode);
		if(is_file($source)){
			$source_parts=pathinfo($source);
			if($source_parts['dirname']=='.'||$source_parts['dirname']=='/')$source_parts['dirname']='';else $source_parts['dirname'].='/';
		}else{
			return $this->error(ERR_FileNotFound, $filename);
		}
		$this->debug(DEBUG_BUILD, "Cache ".$source_parts['basename'],100);
		$source_content=file_get_contents($source);
		$xml=simplexml_load_string($source_content);
		$urls=static::GetSCPDUrls($xml->device);
		if(!$port && !empty($xml->userDef->defaultPort))$port=(int)$xml->userDef->defaultPort;
		$ok=true;
		foreach($urls as &$url){
			$filename=pathinfo($url)['basename'];
			if($ok=file_exists($filename=$source_parts['dirname'].$filename)){
				$this->debug(DEBUG_BUILD, "found $filename",101);
				if($ok=!empty(($sub_content = file_get_contents($filename)))){
					$subUrl='http://'. $host. ':'.$port.$url;
					$this->debug(DEBUG_INFO, "Put $subUrl",200);
					$ok=file_put_contents($this->_buildCacheFilename($subUrl), $sub_content);
				}
				$this->debug(DEBUG_BUILD, "$filename cached: ".($ok?'true':'false'),100);
			}else {
				$this->error(ERR_SubFileNotFound,$filename,$source_parts['basename']);
				break;
			}
		}
		if($ok){
			$baseUrl='http://'. $host. ':'.$port.'/'.$source_parts['basename'];
			$ok=file_put_contents($this->_buildCacheFilename($baseUrl), $source_content);
			$this->debug(DEBUG_INFO, "Put $baseUrl",200);
		}
		$this->debug(DEBUG_INFO, $source_parts['basename'] ." cached: ".($ok?'true':'false'),100);
		return $ok?$baseUrl:false;
	}
	public function Create(string $url_or_filename,$ReturnResult=false, $ClearBefore=false){
		$this->debug(DEBUG_INFO,  "Start creating $url_or_filename",500);
		$this->debug(DEBUG_BUILD, "With Options: ".static::OptionNames($this->_options),500);
		if($ClearBefore!==false)$this->_device=null;
		$this->_importDevice($url_or_filename);
//		$this->debug(DEBUG_BUILD, "Options: ".static::OptionNames($this->_device[DEVICE_CONFIG][CONFIG_PROPS_OPTIONS]),500);
		return $ReturnResult?$this->_device:'';
	}
	public function Save($filename=''){
		if(empty($this->_device))return $this->error("Devive data are empty. Save aborted!");
		$file=pathinfo($filename);
		if(empty($file['basename'])){
			if(empty($this->_deviceFileName)){
				if(!empty($this->_device[DEVICE_INFO][INFO_MANU_ID])){
					$filename=ucfirst($this->_device[DEVICE_INFO][INFO_MANU_ID]);
				}elseif(!empty($this->_device[DEVICE_INFO][INFO_MANU_FACT])){
					$filename=ucfirst($this->_device[DEVICE_INFO][INFO_MANU_FACT]);
				}elseif(!empty($this->_device[DEVICE_INFO][INFO_TYPE])){
					$filename=ucfirst($this->_device[DEVICE_INFO][INFO_TYPE]);
				}else $filename='Unknown';
				if(!empty($this->_device[DEVICE_INFO][INFO_MODEL_ID]))
					$filename.=' ['.$this->_device[DEVICE_INFO][INFO_MODEL_ID].']';
				elseif(!empty($this->_device[DEVICE_INFO][INFO_NAME]))
					$filename.='-'.$this->_device[DEVICE_INFO][INFO_NAME];
			}else $filename=$this->_deviceFileName;
			$file['basename']=trim(str_replace([':',',','  '],['_','-',' ',' ',' '], $filename));
		}
		$file['dirname']=empty($file['dirname'])?RPC_CONFIG_DIR.'/':$file['dirname'].'/';
		@mkdir($file['dirname'],755,true);
		$this->_deviceFileName=$file['dirname'].$file['basename'].'.json';
		if(!empty($this->_device[DEVICE_DESCRIPTIONS])){
			utf8::encode_array($this->_device[DEVICE_DESCRIPTIONS]);
			$dn=$file['basename'].'.desc';
			file_put_contents($file['dirname'].$dn, json_encode($this->_device[DEVICE_DESCRIPTIONS]));
			$this->_device[DEVICE_DESCRIPTIONS]=$dn;
			$this->debug(DEBUG_BUILD,"File ".$file['dirname']."$dn saved!",505);
		}else $this->_device[DEVICE_DESCRIPTIONS]=null;
		$this->debug(DEBUG_BUILD,"File {$this->_deviceFileName} saved!",505);
		return file_put_contents($this->_deviceFileName, json_encode($this->_device));
	}
	public static function LoadServiceInfo($xmlOrFilename){
		static $convertInfo=['deviceType'=>INFO_TYPE,'friendlyName'=>INFO_NAME,'manufacturer'=> INFO_MANU_FACT,'manufacturerURL'=>INFO_MANU_URL,'modelDescription'=>INFO_DESC, 'modelName'=> INFO_MODEL_NAME,'modelNumber'=>INFO_MODEL_NR, 'modelURL'=>INFO_MODEL_URL,'serialNumber'=>INFO_SERIAL,'UDN'=>INFO_UDN];
		if(!is_object($xmlOrFilename) )	$xmlOrFilename=simplexml_load_file($xmlOrFilename);
		if(empty($xmlOrFilename))return null;
		$_info=static::ExtractFromXMLItem($xmlOrFilename->device[0], $convertInfo);
		if(!empty($_info[INFO_TYPE]))$_info[INFO_TYPE]=static::LastNameFromString($_info[INFO_TYPE]);
		if($_info){
			foreach([INFO_NAME,INFO_MANU_FACT,INFO_MODEL_NAME,INFO_MODEL_NR] as $id){
				$mi=static::DetectManufaturerInfo($_info[$id]);
				if(empty($_info[INFO_MANU_ID]))$_info[INFO_MANU_ID]=$mi['MANU'];
				if(empty($_info[INFO_MODEL_ID]))$_info[INFO_MODEL_ID]=$mi['MODEL'];
				if(!empty($_info[INFO_MANU_ID])&&!empty($_info[INFO_MODEL_ID]))break;
			}
		}
		return $_info;
	}
	public static function Discover(int $timeout=null,$searchIp=null, $searchPort=null){
		if(is_null($timeout))$timeout=5;
		static::StaticDebug(DEBUG_INFO, __FUNCTION__ . "Start => Timeout $timeout , Search: $searchIp $searchPort",100);
		$request = 'M-SEARCH * HTTP/1.1'."\r\n";
		$request .= 'HOST: 239.255.255.250:1900'."\r\n";
		$request .= 'MAN: "ssdp:discover"'."\r\n";
		$request .= 'MX: 10'."\r\n";
		$request .= 'ST: ssdp:all'."\r\n";
//		$request .= "ST:ssdp:rootdevice\r\n";
		$request .= 'USER-AGENT: '.self::discover_user_agent."\r\n";
		$request .= "\r\n";
		$socket = socket_create(AF_INET, SOCK_DGRAM, 0);
		socket_set_option($socket, SOL_SOCKET, SO_BROADCAST, true);
		socket_sendto($socket, $request, strlen($request), 0, '239.255.255.250', 1900);
		socket_set_option($socket, SOL_SOCKET, SO_RCVTIMEO, array('sec'=>$timeout, 'usec'=>'0'));
		$read = [$socket];
		$write = $except = $find = $check = [];
		$name = $port = null;
		if($searchIp && !is_array($searchIp))$searchIp=array($searchIp);
		if($searchPort && !is_array($searchPort))$searchPort=array($searchPort);
		$response = '';
		while (socket_select($read, $write, $except, 1) && $read) {
			socket_recvfrom($socket, $response, 2048, null, $name, $port);
			if(is_null($response))continue;
			foreach( explode("\r\n", $response) as $row ) {
				if( stripos( $row, 'loca') === 0 ){
					$location = str_ireplace( 'location: ', '', $row );
					$url=array('scheme'=>'','host'=>'','port'=>'','path'=>'','query'=>'');
					$url=array_merge($url,parse_url($location));
					
					if($searchIp && !$url['host']==$searchIp)break;
					if($searchPort && !$url['port']==$searchPort)break;
					if(in_array($location, $check)===false){
						$check[]=$location;
						$find[]=['HOST'=>$url['host'].':'.$url['port'],'URL'=>$location, 'INFO'=>static::LoadServiceInfo($location)];
					}
				}
			}
		}
		static::StaticDebug(DEBUG_INFO, __FUNCTION__ . "End => found ".count($find)." devices online",100);
		return $find;
	}
	public static function GetSCPDUrls(SimpleXMLElement $xmlDevice){
		$url=[];
		if(!empty($xmlDevice->serviceList)){
			foreach($xmlDevice->serviceList->service as $service)
				if(!empty($SCPDURL=(string)$service->SCPDURL))$url[]=$SCPDURL;
		}
		if(!empty($xmlDevice->deviceList)){
			foreach($xmlDevice->deviceList->device as $device){
				if(!empty($device->serviceList)) $url+=static::GetSCPDUrls($device);
				if(!empty($device->deviceList))
					foreach($device->deviceList->device as $d)$url+=static::GetSCPDUrls($d);
			}
		}
		return $url;
	}
	public static function ExtractFromXMLItem($xml, array $allowed){
		$result=null;
		foreach($xml as $key=>$value){
			if(!empty(trim((string)$value))&& $keyID=@$allowed[(string)$key])
				$result[$keyID]=(string)$value;
		}	
		return $result;
	}
	public static function LastNameFromString(string $s){
		$t=explode(':', $s);
		while (($v=array_pop($t)) && is_numeric($v));
		return $v?$v:$s;
	}
	public static function DetectManufaturerInfo($data){
		if(empty($data))return false;
		$findModelNameByModelID=function($model_id){
			foreach (PRE_Defines as $manu_id=> $define){
				if(empty($define[PREDEF_ITEMS]))continue;
				foreach ($define[PREDEF_ITEMS] as $item){
					if(in_array($model_id,$item[PREDEF_MODELS])!==false)return $manu_id;
				}
			}
			return false;
		};
		$isValidModelID=function($manu_id, $model_id){
			if(empty(PRE_Defines[$manu_id][PREDEF_ITEMS]))return true;
			foreach(PRE_Defines[$manu_id][PREDEF_ITEMS] as $item){
				if(in_array($model_id,$item[PREDEF_MODELS])!==false)return true;
			}
			return false;
		};
		$model_id=$manu_id=false;
		foreach (PRE_Defines as $name=> $define){
			if($reg=@$define[PREDEF_DETECTION][DETECTION_MANU]){
				if(preg_match($reg, $data,$m)){
					array_shift($m);
					self::StaticDebug(DEBUG_INFO, "Manufacturer ".implode(',', $m)." found with $reg in $data",200,get_called_class());
					$manu_id = $name;
				}
			}
			if($reg=@$define[PREDEF_DETECTION][DETECTION_MODEL]){
				// echo "Check ( $data ) : $reg \n";
				if(preg_match($reg, $data,$m) && $model_id=strtoupper(trim($m[0]))){
					array_shift($m);
					self::StaticDebug(DEBUG_INFO, "ModelNumber(s) ".implode(',', $m)." found with $reg in $data",200,get_called_class());
					if($manu_id){
						// echo "Check $model_id in Manu $manu_id\n";
						if(!$isValidModelID($manu_id,$model_id))
							$model_id=false;
					}else{
						$manu_id= $findModelNameByModelID($model_id);
					}
					// if($model_id)echo "found ModelID: $model_id in Manu '$manu_id'\n";
				}
			}
			if($manu_id||$model_id)break;
		}
		
		return ['MANU'=>$manu_id,'MODEL'=>$model_id];
	}
	public function Test(){
		$r=$this->__call('GetVolume', [false]);
		
exit(debug::export($r));
		
		
		
//var_export($args);
		
		
	}
	
	private function _buildCacheFilename(string $filename, $defExt='xml'){
		$ext=@pathinfo(parse_url($filename)['path'])['extension'];
		if($ext)$filename=str_replace('.'.$ext, '', $filename);else $ext=$defExt;
		return $this->_cachePath. str_replace([':','//','/','.'],['_','','_','-'], $filename).'.'.$ext;
	}
	private function _loadContent(string $filename, $force=false){
		if($this->_useFileCache){
			$fn = $this->_buildCacheFilename($filename);
			if($force || !file_exists($fn)){
				$content=file_get_contents($filename);
				file_put_contents($fn, $content);
				return $content;
			}else{
				$this->debug(DEBUG_BUILD, "Load from cache $filename",502);
				$filename=$fn;
			}
		}
		return file_get_contents($filename);
	}
	private function _importDevice(string $filename){
		$xml=simplexml_load_string($this->_loadContent($filename));
		$this->_importConfig=$this->_parseUrl($filename);
		
		if(!$this->_importConfig[CONFIG_PORT] && !empty($xml->userDef->defaultPort)){
			$this->_importConfig[CONFIG_PORT]=(int)$xml->userDef->defaultPort;
		}
		if(empty($this->_device)){
			$this->_device=[INFO_VERSION=>self::$VERSION, DEVICE_CONFIG=>$this->_importConfig];
			$_saved_=null;
		}else { // Save Device Config for Restore
			$_saved_=[$this->_device[DEVICE_INFO][INFO_MODEL_ID],$this->_device[DEVICE_INFO][INFO_MANU_ID],
					$this->_device[DEVICE_CONFIG][CONFIG_PROPS_OPTIONS],
					$this->_device[DEVICE_CONFIG][CONFIG_PROPS_GROUPS]
			];
			// set manu_id and model_id empty to detect new ... restore at end of import
			$this->_device[DEVICE_INFO][INFO_MODEL_ID]=null;
			$this->_device[DEVICE_INFO][INFO_MANU_ID]=null;
			$this->_serviceDefaults=[];
			$this->_infoComplete=false;
		}
		$this->_cachedFunctionsServiceName=null;
		$descriptionCount = empty($this->_device[DEVICE_DESCRIPTIONS])?0: count($this->_device[DEVICE_DESCRIPTIONS]);
		$serviceCount = empty($this->_device[DEVICE_SERVICES])?0:count(array_keys($this->_device[DEVICE_SERVICES]));
		$functionCount = $serviceCount?count($this->FunctionList(true)):0;		
		$this->_importDeviceInfo($xml->device[0]);
		if(($manu_id=$this->_device[DEVICE_INFO][INFO_MANU_ID]) && !empty(PRE_Defines[$manu_id])) {
			$model_id=$this->_device[DEVICE_INFO][INFO_MODEL_ID];
			if($item=$this->_getPREDefined($manu_id, $model_id)){
				$this->debug(DEBUG_BUILD, sprintf('Predefines for manufacturer %s [%s] found',$manu_id,$model_id),550);
// exit(var_dump($item[PREDEF_KEYFUNC],$item[PREDEF_KEYCODES]));				
				if(!empty($item[PREDEF_KEYCODES])){
					$this->_device[DEVICE_KEYCODES]=$item[PREDEF_KEYCODES];
					$this->_device[DEVICE_CONFIG][CONFIG_PROPS_OPTIONS]=$this->_device[DEVICE_CONFIG][CONFIG_PROPS_OPTIONS] | PROP_REMOTE;
					$this->debug(DEBUG_BUILD, sprintf('Predefines for KEYCODES loaded',$manu_id,$model_id),551);
				}
				if(!empty($item[DEVICE_CONFIG])){
					$this->debug(DEBUG_BUILD,'Predefines for DEVICE loaded',552);
					$this->_device[DEVICE_CONFIG]=array_merge($this->_device[DEVICE_CONFIG],$item[DEVICE_CONFIG]);
				}
				if(!empty($item[DEVICE_SERVICES])){
					$this->debug(DEBUG_BUILD,'Predefines for CONFIG loaded',553);
					$this->_serviceDefaults=$item[DEVICE_SERVICES];
				}
			}
		}
		
		if(!empty($xml->device->iconList))$this->_importDeviceIcons($xml->device->iconList,$this->_importConfig);
		if(!empty($xml->device->serviceList))$this->_importDeviceServices($xml->device->serviceList,$this->_importConfig[CONFIG_PORT]);
		if(!empty($xml->device->deviceList))$this->_importDeviceList($xml->device->deviceList,$this->_importConfig[CONFIG_PORT]);
		if($_saved_){
			list($this->_device[DEVICE_INFO][INFO_MODEL_ID],$this->_device[DEVICE_INFO][INFO_MANU_ID],$props,$groups)=$_saved_;
			if(isset($item[DEVICE_CONFIG][CONFIG_PROPS_OPTIONS]))if(is_array($item[DEVICE_CONFIG][CONFIG_PROPS_OPTIONS]))$item[DEVICE_CONFIG][CONFIG_PROPS_OPTIONS]=array_sum($item[DEVICE_CONFIG][CONFIG_PROPS_OPTIONS]);
			if(isset($item[DEVICE_CONFIG][CONFIG_PROPS_GROUPS]))if(is_array($item[DEVICE_CONFIG][CONFIG_PROPS_GROUPS]))	$item[DEVICE_CONFIG][CONFIG_PROPS_GROUPS]=array_sum($item[DEVICE_CONFIG][CONFIG_PROPS_GROUPS]);
			$this->_device[DEVICE_CONFIG][CONFIG_PROPS_OPTIONS]=$props | $this->_device[DEVICE_CONFIG][CONFIG_PROPS_OPTIONS];
			$this->_device[DEVICE_CONFIG][CONFIG_PROPS_GROUPS]=$groups |  $this->_device[DEVICE_CONFIG][CONFIG_PROPS_GROUPS];
		}
		if(empty($this->_device[DEVICE_INFO][INFO_MANU_ID]))$this->_device[DEVICE_INFO][INFO_MANU_ID]='Generic';
		if(empty($this->_device[DEVICE_INFO][INFO_MODEL_ID]))$this->_device[DEVICE_INFO][INFO_MODEL_ID]='Generic';
		// Check and add Descriptions for servicecs
		if($descriptions=empty($xml->descriptions)?null:$this->_importDescriptions($xml->descriptions)){
			foreach($this->_device[DEVICE_SERVICES] as &$s)if(isset($s[SERVICE_DESC_ID]))if(!$this->_addDescription(@$descriptions[$s[SERVICE_DESC_ID]],$s[SERVICE_DESC_ID]))unset($s[SERVICE_DESC_ID]);
			$descriptions=count($descriptions);
		}else $descriptions=0; 	
		
		$props=$this->_device[DEVICE_CONFIG][CONFIG_PROPS_OPTIONS];
		if($props & PROP_BALANCE_CONTROL){
			if (defined('PRE_DefinesFunctions')){
				foreach(PRE_DefinesFunctions[DEVICE_SERVICES] as $services){
					
					if((!$set=@$services[SERVICE_FUNCTIONS]['SetBalance'])||!$get=@$services[SERVICE_FUNCTIONS]['GetBalance'])continue;
					$sn=&$services[SERVICE_NAME];
					if(empty($this->_device[DEVICE_SERVICES][$sn])){
						$this->_device[DEVICE_SERVICES][$sn]=[
								SERVICE_NAME=>$sn,
								SERVICE_PORT=>$this->_device[DEVICE_CONFIG][CONFIG_PORT],
								SERVICE_CTRL_URL=>'',
								SERVICE_EVENT_URL=>''
						];					
					}
					// Add and Convert String Descriptions 
					// for set
					if(isset($set[FUNCTION_DESC_ID]))$set[FUNCTION_DESC_ID]=$this->_addDescription($set[FUNCTION_DESC_ID]);
					if(!empty($set[FUNCTION_PARAMS][PARAM_IN]))foreach($set[FUNCTION_PARAMS][PARAM_IN] as &$param){
						if(isset($param[VALUE_DESC_ID]))$param[VALUE_DESC_ID]=$this->_addDescription($param[VALUE_DESC_ID]);
						$param[VALUE_TYPE]=$this->_dataTpeXMLtoRCP($param[VALUE_TYPE]);	
					}	
					if(!empty($set[FUNCTION_PARAMS][PARAM_OUT]))foreach($set[FUNCTION_PARAMS][PARAM_OUT] as &$param)
						if(isset($param[VALUE_DESC_ID]))$param[VALUE_DESC_ID]=$this->_addDescription($param[VALUE_DESC_ID]);
					// for get
					if(isset($get[FUNCTION_DESC_ID]))$get[FUNCTION_DESC_ID]=$this->_addDescription($get[FUNCTION_DESC_ID]);
					if(!empty($get[FUNCTION_PARAMS][PARAM_IN]))foreach($get[FUNCTION_PARAMS][PARAM_IN] as &$param){
						if(isset($param[VALUE_DESC_ID]))$param[VALUE_DESC_ID]=$this->_addDescription($param[VALUE_DESC_ID]);
						$param[VALUE_TYPE]=$this->_dataTpeXMLtoRCP($param[VALUE_TYPE]);	
					}	
					if(!empty($get[FUNCTION_PARAMS][PARAM_OUT]))foreach($get[FUNCTION_PARAMS][PARAM_OUT] as &$param)
						if(isset($param[VALUE_DESC_ID]))$param[VALUE_DESC_ID]=$this->_addDescription($param[VALUE_DESC_ID]);
						
						
					$this->_device[DEVICE_SERVICES][$sn][SERVICE_FUNCTIONS]['SetBalance']=$set;
					$this->debug(DEBUG_BUILD,'Predefined function SetBalance for service '.$sn.' imported',552);
	
					$this->_device[DEVICE_SERVICES][$services[SERVICE_NAME]][SERVICE_FUNCTIONS]['GetBalance']=$get;
					$this->debug(DEBUG_BUILD,'Predefined function GetBalance imported',552);
					break;
				}
//				$this->_device[DEVICE_CONFIG]=array_merge($this->_device[DEVICE_CONFIG],$item[DEVICE_CONFIG]);
			}
		}
		$this->_cachedFunctionsServiceName=null;
		$descriptionCount =  (empty($this->_device[DEVICE_DESCRIPTIONS])?$descriptionCount: count($this->_device[DEVICE_DESCRIPTIONS]))-$descriptionCount;
		$serviceCount = count(array_keys($this->_device[DEVICE_SERVICES])) - $serviceCount;
		$functionCount = count($this->FunctionList(true)) - $functionCount;		
		
		
		$this->debug(DEBUG_BUILD,"$descriptionCount new Descriptions imported",200);
		$this->debug(DEBUG_BUILD,"$serviceCount new Services imported",200);
		$this->debug(DEBUG_BUILD,"$functionCount new Functions imported",200);
		
		$this->debug(DEBUG_BUILD,"Detected props: ".implode(",",GetPropNames($props,true)),505);
		
// 		var_export($this->_device[DEVICE_DESCRIPTIONS])	;	
		$this->loaded();
	}
	
	private function _convertValue($DataType, $value){
		switch($DataType){
			case DATATYPE_BOOL	: $value=boolval($value);break;
			case DATATYPE_BYTE 	:
			case DATATYPE_INT 	:
			case DATATYPE_UINT	: $value=intval($value);break;
			case DATATYPE_FLOAT	: $value=flotval($value);break;
			case DATATYPE_STRING: $value = strval($value);break; 
			default				:
				if(is_array($value) && isset($value['default'])){
// var_dump($DataType, $value);					
					$value = $value['default'];
									
				}
		}
		return $value;
	}
	private function _getPREDefined($manu_id, $model_id){
		if(empty(PRE_Defines[$manu_id][PREDEF_ITEMS]))return null;
		foreach (PRE_Defines[$manu_id][PREDEF_ITEMS] as $item){
			if(($found=array_search($model_id, $item[PREDEF_MODELS]))!==false)return $item;
		}
		return null;		
	}

	private function _addDescription($Description, $ID=null){
		if (is_null($Description)){
			$this->debug(DEBUG_BUILD,'Empty description found'.(is_null($ID)?'':" for ID $ID"),-100);
			return null;
		}
		if(is_null($ID)){
			if(empty($this->_device[DEVICE_DESCRIPTIONS])) $ID=0;
			else foreach($this->_device[DEVICE_DESCRIPTIONS] as $index=>$desc)if(strcasecmp($desc, $Description)===0){return $ID;}
			if(is_null($ID))$ID=max(array_keys($this->_device[DEVICE_DESCRIPTIONS]))+1;
		}
		if(empty($this->_device[DEVICE_DESCRIPTIONS][$ID])){
			$this->_device[DEVICE_DESCRIPTIONS][$ID]=$Description;
			return $ID;
		}
		if(strcasecmp($this->_device[DEVICE_DESCRIPTIONS][$ID], $Description)==0)return $ID;
		$ID=max(array_keys($this->_device[DEVICE_DESCRIPTIONS]))+1;
		$this->_device[DEVICE_DESCRIPTIONS][$ID]=$Description;
		return $ID;
	}
	private function _addDescByArray(array &$data){
		if(!isset($data[VALUE_DESC_ID]))return;
		if(is_string($data[VALUE_DESC_ID]))$data[VALUE_DESC_ID]= $this->_addDescription($data[VALUE_DESC_ID]);
		else if(empty($this->_device[DEVICE_DESCRIPTIONS][$data[VALUE_DESC_ID]])){
			$this->debug(DEBUG_BUILD, 'invalid Message ID '.$data[VALUE_DESC_ID],-100);
			unset($data[VALUE_DESC_ID]);
		}
	}
	private function _importDescriptions(SimpleXMLElement $xml){
 		if(!($this->_options & OPT_INCLUDE_DESC))return;
		$count=count($xml->{DESCRIPTION_XML_IDENT});
		$descs=null;
		for($j=0;$j<$count;$j++){
			$ID=(int)$xml->{DESCRIPTION_XML_IDENT}[$j]->attributes()['id'];
			$desc=(string)$xml->{DESCRIPTION_XML_IDENT}[$j];
			$descs[$ID]=(string)$xml->{DESCRIPTION_XML_IDENT}[$j];
		}
		return $descs;
	}
	private function _importDeviceInfo(SimpleXMLElement $xml){
		static $convertInfo=['deviceType'=>INFO_TYPE,'friendlyName'=>INFO_NAME,'manufacturer'=> INFO_MANU_FACT,'manufacturerURL'=>INFO_MANU_URL,'modelDescription'=>INFO_DESC, 'modelName'=> INFO_MODEL_NAME,'modelNumber'=>INFO_MODEL_NR, 'modelURL'=>INFO_MODEL_URL,'serialNumber'=>INFO_SERIAL,'UDN'=>INFO_UDN];
		$_info=static::ExtractFromXMLItem($xml, $convertInfo);
		if(!empty($_info[INFO_TYPE]))$_info[INFO_TYPE]=static::LastNameFromString($_info[INFO_TYPE]);
		if($_info){
			if(!$this->_infoComplete){
				foreach([INFO_NAME,INFO_MANU_FACT,INFO_MODEL_NAME,INFO_MODEL_NR] as $id){
					$mi=static::DetectManufaturerInfo($_info[$id]);
					if(empty($this->_device[DEVICE_INFO][INFO_MANU_ID]))$this->_device[DEVICE_INFO][INFO_MANU_ID]=$mi['MANU'];
					if(empty($this->_device[DEVICE_INFO][INFO_MODEL_ID]))$this->_device[DEVICE_INFO][INFO_MODEL_ID]=$mi['MODEL'];
					if($this->_infoComplete=!(empty($this->_device[DEVICE_INFO][INFO_MANU_ID])||empty($this->_device[DEVICE_INFO][INFO_MODEL_ID])))break;
				}
			}
			if(!empty($this->_device[DEVICE_INFO][INFO_TYPE]))
				$this->_device[DEVICE_INFO][INFO_TYPE].=','.$_info[INFO_TYPE];
			else
				$this->_device[DEVICE_INFO] += $_info;
		}
	}
	private function _importDeviceIcons(SimpleXMLElement $xml){
		static $convertIcon=['mimetype'=>ICON_MIME,'width'=>ICON_WIDTH,'height'=>ICON_HEIGHT,'depth'=>ICON_DEPTH,'url'=>ICON_URL];
		if(empty($this->_device[DEVICE_ICONS]))$this->_device[DEVICE_ICONS]=[];
		foreach($xml->icon as $iconNr=>$icon){
			$_icon=static::ExtractFromXMLItem($icon, $convertIcon);
			if($_icon && in_array($_icon, $this->_device[DEVICE_ICONS])===false){
				if($this->_options  & OPT_CACHE_ICONS){
					$url=$this->_importConfig[CONFIG_SCHEME]."://".$this->_importConfig[CONFIG_HOST];
					if($this->_importConfig[CONFIG_PORT])$url.=":".$this->_importConfig[CONFIG_PORT];
					if($_icon[ICON_URL][0]!='/')$url.='/';
					$pathinfo=pathinfo($_icon[ICON_URL]);
					$fn=$pathinfo['filename'].'_'.crc32($url.$_icon[ICON_URL]).'.'.$pathinfo['extension'];
					if(!file_exists(RPC_ICON_CACHE_DIR.'/'.$fn)){
						if($data=file_get_contents($url.$_icon[ICON_URL])){
							if(file_put_contents(RPC_ICON_CACHE_DIR.'/'.$fn, $data)){
								$this->debug(DEBUG_BUILD,"Cache icon ".$pathinfo['basename']." to $fn",100);
								$_icon[ICON_URL]=$fn;
							}	
						}
					}
				}
				$this->_device[DEVICE_ICONS][]=$_icon;
			}
		}
	}
	private function _importDeviceList(SimpleXMLElement $xml){
		foreach($xml->device as $device){
			$this->_importDeviceInfo($device);
			if(!empty($device->iconList))$this->_importDeviceIcons($device->iconList);
			if(!empty($device->serviceList))$this->_importDeviceServices($device->serviceList);
			if(!empty($device->deviceList))$this->_importDeviceList($device->deviceList);
		}
	}
	private function _importDeviceServices(SimpleXMLElement $xml){
		static $convertService=['serviceType'=>SERVICE_ID,'controlURL'=>SERVICE_CTRL_URL,'eventSubURL'=>SERVICE_EVENT_URL,'lowerNames'=>SERVICE_LOWER_NAMES, 'connectionType'=>SERVICE_CONNECTION_TYPE,DESCRIPTION_XML_IDENT=>SERVICE_DESC_ID];
		foreach($xml->service as $service){
			$_service=$add_functions=null;
			$name=static::LastNameFromString((string)$service->serviceType);
			foreach($this->_serviceDefaults as $default){
				if(strcasecmp($default[SERVICE_NAME],$name)==0){
					$this->debug(DEBUG_BUILD,"Predefined Service defaults for $name loaded",554);
					$add_functions=@$default[SERVICE_FUNCTIONS];
					unset($default[SERVICE_FUNCTIONS]);
					$_service=$default;
					break;
				}
			}
			if(is_null($_service))$_service=[];
			$this->_lastServiceName=$name;
			$_service=array_merge([SERVICE_NAME=>$name, SERVICE_PORT=>$this->_importConfig[CONFIG_PORT]], $_service, static::ExtractFromXMLItem($service, $convertService)) ;// $this->_importConfig[CONFIG_PORT]];
			if(!isset($_service[SERVICE_CONNECTION_TYPE]))$_service[SERVICE_CONNECTION_TYPE]=isset($service->connectionType)?(int)$service->connectionType:$this->_device[DEVICE_CONFIG][CONFIG_CONNECTION_TYPE];
			if($this->_importConfig[CONFIG_PORT]>0 && !empty($SCPDURL=(string)$service->SCPDURL)){
				$url=$this->_importConfig[CONFIG_SCHEME]."://".$this->_importConfig[CONFIG_HOST];
				if($this->_importConfig[CONFIG_PORT])$url.=":".$this->_importConfig[CONFIG_PORT];
				if($SCPDURL[0]!='/' && $this->_importConfig[CONFIG_PORT])$url.='/';
				$_service[SERVICE_FUNCTIONS]=$this->_loadSCPDURL($url.$SCPDURL, !empty($_service[SERVICE_LOWER_NAMES]));
			}
			if($add_functions){
				foreach($add_functions as $function){
					$this->_lastFunction=&$function;
					$params =&$function[FUNCTION_PARAMS];
					if(!empty($params[PARAM_IN]))foreach($params[PARAM_IN] as &$param){
						$this->_addDescByArray($param);
						$param[VALUE_TYPE]=$this->_dataTpeXMLtoRCP($param[VALUE_TYPE]);
					}
					if(!empty($params[PARAM_OUT]))foreach($params[PARAM_OUT] as &$param){
						$this->_addDescByArray($param);
						$param[VALUE_TYPE]=$this->_dataTpeXMLtoRCP($param[VALUE_TYPE]);
					}
					$fn=$function[FUNCTION_NAME];
					$lowerNames=!empty($this->_serviceDefaults[SERVICE_LOWER_NAMES]);
					if(isset($function[SERVICE_LOWER_NAMES])){
						$lowerNames=$function[SERVICE_LOWER_NAMES];
						unset($function[SERVICE_LOWER_NAMES]);
					}
					if($lowerNames)$function[FUNCTION_NAME]=strtolower($function[FUNCTION_NAME]);

					$this->_checkProps($fn,$function);
					$_service[SERVICE_FUNCTIONS][$fn]=$function;
					$this->debug(DEBUG_BUILD,"Add user function ".$function[FUNCTION_NAME]." to service $name : ".debug::export($function),510);
				}
			}
	
			unset($_service[SERVICE_LOWER_NAMES]);
			$this->_device[DEVICE_SERVICES][$name]=$_service;
			unset($_service[SERVICE_FUNCTIONS]);
			$this->debug(DEBUG_BUILD,"Import service: ".debug::export($_service),100);
		}
	}
	private function _dataTpeXMLtoRCP(string $DataType){
		static $dataTypeConvert=['boolean'=>DATATYPE_BOOL,'string'=>DATATYPE_STRING,'ui1'=>DATATYPE_BYTE, 'ui2'=>DATATYPE_UINT,'ui4'=>DATATYPE_UINT,'i2'=>DATATYPE_INT,'i4'=>DATATYPE_INT,'floot'=>DATATYPE_FLOAT,'double'=>DATATYPE_FLOAT,'array'=>DATATYPE_ARRAY,'mixed'=>DATATYPE_MIXED,'??'=>DATATYPE_UNKNOWN];
		return empty($dataTypeConvert[$DataType])?DATATYPE_UNKNOWN:$dataTypeConvert[$DataType];
	}
	private function _loadSCPDURL(string $SCPDURL, bool $Lowernames){
		if(!$xml=simplexml_load_string($this->_loadContent($SCPDURL)))return null;
		if($descriptions=empty($xml->descriptions)?null:$this->_importDescriptions($xml->descriptions)){
			
// 			if(empty($this->_device[DEVICE_DESCRIPTIONS]))
// 				$this->_device[DEVICE_DESCRIPTIONS]=$descriptions;
// 			else foreach($descriptions as $id=>$desc){
// 				if(empty($this->_device[DEVICE_DESCRIPTIONS][$id]))
// 					$this->_device[DEVICE_DESCRIPTIONS][$id]=$desc;
// 				else if(in_array($desc, $this->_device[DEVICE_DESCRIPTIONS])===false)
// 					$this->_device[DEVICE_DESCRIPTIONS][]=$desc;
// 			}
// var_export($descriptions);			
		}
		$addDescID=function(array &$data, $DescID) use ($descriptions){
			if(!isset($data[$DescID]))return;
			if(!is_numeric($data[$DescID]))	$data[$DescID]= $this->_addDescription($data[$DescID]);
			elseif(is_numeric($data[$DescID]) &&  !empty($descriptions[$data[$DescID]]))$data[$DescID]= $this->_addDescription($descriptions[$data[$DescID]]);
			else if(empty($this->_device[DEVICE_DESCRIPTIONS][$data[$DescID]])){
				$this->debug(DEBUG_BUILD, 'invalid Message ID '.$data[$DescID],-100);
				unset($data[$DescID]);
			}
		};
		
		$statevars = $eventvars=[];
		foreach($xml->serviceStateTable->stateVariable as $item){
			$vname=(string)$item->name;
			$attr=$item->attributes();
			if(!empty($attr['sendEvents']))if(strcasecmp($attr['sendEvents'], 'yes')==0 && in_array($vname,$eventvars)===false)$eventvars[]=$vname;	
			$value=null;
			$typ=$this->_dataTpeXMLtoRCP((string)$item->dataType);
			if($typ==DATATYPE_UNKNOWN){
				$chk='A_ARG_TYPE_'.strtoupper((string)$item->dataType);
				if(!empty($statevars[$chk])){
					$statevars[$vname]=$statevars[$chk];
					continue;
				}
			}
			
			$statevars[$vname]=[VALUE_TYPE=>$typ];
			if(!empty($item->defaultValue))$value=(string)$item->defaultValue;
			if(is_null($value) && isset($this->_defaultValues[$vname]))$value=$this->_defaultValues[$vname];
			if(!empty($item->allowedValueRange)){
				$statevars[$vname][VALUE_MIN]=(int)$item->allowedValueRange->minimum;
				$statevars[$vname][VALUE_MAX]=(int)$item->allowedValueRange->maximum;
				$statevars[$vname][VALUE_STEP]=(int)$item->allowedValueRange->step;
				if(!is_null($value) && ($value<$statevars[$vname][VALUE_MIN] || $value>$statevars[$vname][VALUE_MAX]))$value=$statevars[$vname][VALUE_MIN];
			}
			if(!empty($item->allowedValueList)){
				$list=[];$ok=false;
				foreach($item->allowedValueList->allowedValue as $v){
					if(!is_null($value)&& $value == (string)$v) $ok=true;
					$list[]=(string)$v;
				}
				if(!is_null($value)&&!$ok)$value=null;
				if(is_null($value))$value=@$list[0];
				$statevars[$vname][VALUE_LIST]=$list;
			}
			if(!is_null($value))$statevars[$vname][VALUE_DEFAULT]=$this->_convertValue($statevars[$vname][VALUE_TYPE], $value);
			if(isset($item->{DESCRIPTION_XML_IDENT})){
				$statevars[$vname][VALUE_DESC_ID]= (string)$item->{DESCRIPTION_XML_IDENT};
				$addDescID($statevars[$vname],VALUE_DESC_ID);
			}
		}
		$items=[];
			
		
// debug::export($this->_serviceDefaults);
		foreach($xml->actionList->action as $item){


			$data_in=$data_out=$data_events=null;
			$fname=(string)$item->name;
			if(!empty($item->argumentList->argument )){
				foreach ($item->argumentList->argument as $arg){
					$aname=(string)$arg->name;
					$mode = (string)$arg->direction=='in'? PARAM_IN:PARAM_OUT;
					
					$data=[VALUE_NAME=>$aname];
					$statevar=empty((string)$arg->relatedStateVariable)?$aname:(string)$arg->relatedStateVariable;
					if(!empty($statevars[$statevar])){
						$data += $statevars[$statevar];
					}
					if(empty($data[VALUE_DEFAULT]) && !empty($this->_defaultValues[$aname]))$data += $this->_defaultValues[$aname];
					
					if(in_array($statevar, $eventvars)!==false){
						if(empty($data_events) || in_array($aname, $data_events)===false)$data_events[]=$aname;
					}
	
					if($this->_options & OPT_INCLUDE_DESC){
						if(!empty($arg->{DESCRIPTION_XML_IDENT}))				$data[VALUE_DESC_ID]=(string)$arg->{DESCRIPTION_XML_IDENT};
						elseif(isset($statevars[$statevar][VALUE_DESC_ID]))		$data[VALUE_DESC_ID]=$statevars[$statevar][VALUE_DESC_ID];	
						$addDescID($data,VALUE_DESC_ID);
					}
					
					if($mode==PARAM_IN)$data_in[]=$data;else{
						if(isset($data[VALUE_DEFAULT]))unset($data[VALUE_DEFAULT]);
						$data_out[]=$data;
					}
				}
			}
			
			if(isset($item->{DESCRIPTION_XML_IDENT})&& ($this->_options & OPT_INCLUDE_DESC)){
//var_export($item);
$items[$fname][FUNCTION_DESC_ID]=(string)$item->{DESCRIPTION_XML_IDENT};
				$addDescID($items[$fname],FUNCTION_DESC_ID);
			}
			
			$items[$fname][FUNCTION_NAME]=  $Lowernames	? strtolower($fname):$fname;
			$items[$fname][FUNCTION_PARAMS]=[PARAM_IN=>$data_in,PARAM_OUT=>$data_out];
			if($data_events){
				$this->debug(DEBUG_BUILD,"Function $fname has dataevents [" . implode(',',$data_events). "]",100 );
				$items[$fname][FUNCTION_EVENTS]=$data_events;
			}
			$this->_lastFunction=$items;
			$this->_checkProps($fname,$items[$fname]);
		}
		
		return $items;
	}
	private function _checkProps($name, $function=null){
		static $propPunctions=null;
		if(!$propPunctions)$propPunctions=GetPropFunctionNames(PROP_ALL_PROPS);
		$props=&$this->_device[DEVICE_CONFIG][CONFIG_PROPS_OPTIONS];
		$groups=&$this->_device[DEVICE_CONFIG][CONFIG_PROPS_GROUPS];
		foreach($propPunctions as $prop=>$functionNames){
			if(in_array($name,$functionNames)){
				$props = $props | $prop;
				switch($prop){
					case PROP_PLAY_CONTROL 			:	$groups = $groups | PROP_GROUP_PLAYER; break;
					case PROP_BRIGHTNESS_CONTROL 	:	
					case PROP_CONTRAST_CONTROL 		:
					case PROP_SHARPNESS_CONTROL		:
						if($groups & PROP_GROUP_AMPLIFIER)$groups-=PROP_GROUP_AMPLIFIER;
						$groups= $groups | PROP_GROUP_TV;
						break;
						
					case PROP_VOLUME_CONTROL 		:
							if(!empty($function[FUNCTION_PARAMS][PARAM_IN])){
								foreach($function[FUNCTION_PARAMS][PARAM_IN] as $param){
									if(empty($param[VALUE_LIST]))continue;
									$haystack = implode(',',$param[VALUE_LIST]);
									if(stripos($haystack,'lf')!==false && stripos($haystack,'lf')!==false){
										$props = $props | PROP_BALANCE_CONTROL;		
									}
								}
							}
					case PROP_BASS_CONTROL			:
					case PROP_TREBLE_CONTROL		:
					case PROP_LOUDNESS_CONTROL		:
					case PROP_MUTE_CONTROL 			: $groups= $groups | PROP_GROUP_SOUND; break;
					case PROP_SOURCE_CONTROL		: if(!($groups & PROP_GROUP_TV))$groups= $groups | PROP_GROUP_AMPLIFIER; break;
				}
			}
		}
		if(!empty($function[FUNCTION_EVENTS]))	$props = $props | PROP_EVENTS;
		// echo "Name: $name Prop: $props, Group: $groups\n";
		// 		elseif(in_array($name, ['getParamsetDescription','putParamset'])!==false)	$props = $props | PROP_SMARTHOME;
		
		//		elseif(in_array($name, ['GetSorce','SetSource'])!==false)	$props = $props | PROP_TREBLE_CONTROL;
	}
	private function _parseUrl(string $url){
		$r=parse_url($url);
		if($p=strpos($r['path'], '/'))$r['host']=substr($r['path'],0, $p);elseif(empty($r['host']) && !empty($r['path']))$r['host']=$r['path'];
		if(empty($r['user']))$r['user']=empty($this->_device)?$this->_creditials[CREDIT_USER]:$this->_device[DEVICE_CONFIG][CONFIG_CREDITIALS][CREDIT_USER];
		if(empty($r['pass']))$r['pass']=empty($this->_device)?$this->_creditials[CREDIT_PASS]:$this->_device[DEVICE_CONFIG][CONFIG_CREDITIALS][CREDIT_PASS];
		if(empty($r['scheme']))$r['scheme']=$r['port']==433?'https':'http';
		return [CONFIG_SCHEME=>$r['scheme'],
				CONFIG_HOST=>$r['host'],
				CONFIG_PORT=>empty($r['port'])?0 :$r['port'],
				CONFIG_OPTIONS=>$this->_options,
				CONFIG_CREDITIALS=>[CREDIT_USER=>$r['user'],CREDIT_PASS=>$r['pass']],
				CONFIG_CONNECTION_TYPE=>$this->_connectionType,
				CONFIG_PROPS_OPTIONS=>0,
				CONFIG_PROPS_GROUPS=>0
		];
	}
}

/*
public function Test(){
	// 		$conType=@GENDefine['userdefaults']['service']['connectionType'];
	// 		if(is_null($conType))$conType=@GENDefine['userdefaults']['device']['connectionType'];
	// 		$conType=is_null($conType)?'':"\n$s3<".DEF_CONNEC_TYPE_XML.">$conType</".DEF_CONNEC_TYPE_XML.'>';
	
	static $convertDataType=[DATATYPE_BOOL=>'boolean',DATATYPE_INT=>'i4',DATATYPE_UINT=>'ui4',DATATYPE_FLOAT=>'i4',DATATYPE_STRING=>'string',DATATYPE_ARRAY=>'array',DATATYPE_OBJECT=>'object',DATATYPE_MIXED=>'',DATATYPE_UNKNOWN=>''];
	foreach($this->_device[DEVICE_SERVICES] as $sn=>$service){
		foreach($service[SERVICE_FUNCTIONS] as $fn=>$function){
			$in=$out=null;
			if(!empty($function[FUNCTION_PARAMS][PARAM_IN])){
				foreach($function[FUNCTION_PARAMS][PARAM_IN] as $param){
					$c=count($in);
					$in[$c]=['name'=>$param[VALUE_NAME],'type'=>$convertDataType[$param[VALUE_TYPE]]];
					
					if(!empty($param[VALUE_DEFAULT])&&!is_null($param[VALUE_DEFAULT]))
						$in['default']=$param[VALUE_DEFAULT];
						if(!empty($param[VALUE_MIN])||!empty($param[VALUE_MAX])||!empty($param[VALUE_MAX]))
							$in['range']=[$param[VALUE_MIN],$param[VALUE_MAX],$param[VALUE_STEP]];
							elseif(!empty($param[VALUE_LIST])&&is_array($param[VALUE_LIST]))
							$in['list']=implode(',',$param[VALUE_LIST]);
				}
			}
			if(!empty($function[FUNCTION_PARAMS][PARAM_OUT])){
				foreach($function[FUNCTION_PARAMS][PARAM_OUT] as $param){
					$out[]=['name'=>$param[VALUE_NAME],'type'=>$convertDataType[$param[VALUE_TYPE]]];
				}
			}
			$result=['in'=>$in,'out'=>$out];
			if(!empty($function[FUNCTION_DESC_ID]))$result['desc']=$function[FUNCTION_DESC_ID];
			$defs[$fn] = str_replace(["\n",' ',',)'],['','',')'], var_export($result,true));
		}
		$info=&$this->_device[DEVICE_INFO];
		$out=["\$".$info[INFO_MANU_ID].$info[INFO_TYPE].'=['];
		$out[]="  'device'=>[";
		$out[]="    'deviceType'=>'urn:schemas-upnp-org:device:{$info[INFO_TYPE]}:1',";
		$out[]="    'friendlyName'=>'{$info[INFO_NAME]}',";
		$out[]="    'manufacturer'=>'{$info[INFO_MANU_FACT]}',";
		$out[]="    'manufacturerURL'=>'{$info[INFO_MANU_URL]}',";
		$out[]="    'modelName'=>'{$info[INFO_MODEL_NAME]}',";
		$out[]="    'modelNumber'=>'{$info[INFO_MODEL_NR]}',";
		$out[]="    'modelURL'=>'{$info[INFO_MODEL_URL]}',";
		$out[]="    'serialNumber'=>'{$info[INFO_SERIAL]}',";
		$out[]="    'UDN'=>'{$info[INFO_UDN]}',";
		foreach($this->_device[DEVICE_SERVICES] as $sn=>$service){
			$s[]="'%1<service>%2<connectionType>{$service[SERVICE_CONNECTION_TYPTE]}</connectionType>%2<serviceType>{$service[SERVICE_ID]}</serviceType>%2<serviceId>urn:upnp-org:serviceId:{$info[INFO_TYPE]}</serviceId>%2<controlURL>/</controlURL>%2<eventSubURL></eventSubURL>%2<SCPDURL>/homematicSCPD.xml</SCPDURL>%1</service>'";
		}
		$out[]="    'serviceList'=>".implode('',$s);
		$out[]="    ],";
		$out[]="    'userdefaults'=>[";
		$out[]="        'device'=>[";
		$out[]="    	  'defaultPort'=>".$this->_device[DEVICE_CONFIG][CONFIG_PORT].',';
		$out[]="    	  'connectionType'=>".$this->_device[DEVICE_CONFIG][CONFIG_CONNECTION_TYPE].'  //*0=soap,1=jsonrpc,2=web ';
		$out[]="    	],";
		$out[]="    	'service'=>[";
		$out[]="    	  'connectionType'=>".$this->_device[DEVICE_CONFIG][CONFIG_CONNECTION_TYPE];
		$out[]="    	]";
		$out[]="    ],";
		
		$out[]="    'functions '=>[";
		foreach($defs as $name => $def)$out[]="     '$name'=>$def,";
		$out[]="    ]";
		$out[]="];\n";
		echo implode("\n", $out);
	}
}
*/

?>