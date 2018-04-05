<?php
/** @file rpcimport.inc
 * @brief Helper File to import RPC Devices  
 *   
 * @author Xaver Bauer
 * @date 26.03.2018
 * - add RpcDeviceMerge
 * @date 21.01.2018
 * @version 2.0.1
 * Added PhpDoc Tags
 * @package rpcimport
 * @brief Helper File to import RPC Devices
 * @copydetails rpcimport.inc
 */

require_once 'rpcmessages.inc';
require_once 'rpcdetection.inc';

/** @brief Import a device
 * 
 * @param string $UrlToDescXml Url to description xml
 * @param string $MyVersion Current Version 
 * @param RpcLogger $Logger Logger Object
 * @param bool Use Cache to load files from remote device 
 * @return null|stdClass $ref class_device
 * @see @link Device_Structure.php 
 */
function RpcImportDevice($UrlToDescXml, RpcLogger $Logger=null, $UseCache=true, $MyVersion=RPC_API_VERSION, $CopyToDevice = null ){
	static $convertUrl =[CONFIG_SCHEME,CONFIG_LOGIN_U,CONFIG_LOGIN_P,CONFIG_HOST,CONFIG_PORT,0,0,0];
	static $convertInfo=['deviceType','friendlyName','manufacturer','manufacturerURL','modelDescription','modelName','modelNumber', 'modelURL','serialNumber','UDN'];
	static $convertIcon=['mimetype'=>ICON_MIME,'width'=>ICON_WIDTH,'height'=>ICON_HEIGHT,'depth'=>ICON_DEPTH,'url'=>ICON_URL];
	static $convertService=['serviceType'=>SERVICE_ID,'controlURL'=>SERVICE_CTRLURL,'eventSubURL'=>SERVICE_EVENTURL,'options'=>SERVICE_OPTIONS, 'connectionType'=>SERVICE_CONNTYPE, 'description'=>SERVICE_DESC,'port'=>SERVICE_PORT,'export'=>SERVICE_EXPORT];
	static $convertDataType=['boolean'=>DATATYPE_BOOL,'string'=>DATATYPE_STRING,'ui1'=>DATATYPE_BYTE, 'ui2'=>DATATYPE_UINT,'ui4'=>DATATYPE_UINT,'i2'=>DATATYPE_INT,'i4'=>DATATYPE_INT,'floot'=>DATATYPE_FLOAT,'double'=>DATATYPE_FLOAT,'array'=>DATATYPE_ARRAY,'mixed'=>DATATYPE_MIXED,'??'=>DATATYPE_UNKNOWN];
	static $convertStateVar=['name'=>VALUE_NAME,'dataType'=>VALUE_TYPE ,'defaultValue'=>VALUE_DEFAULT,'description'=>VALUE_DESC];
	static $convertStateRange=['minimum'=>VALUE_MIN,'maximum'=>VALUE_MAX,'step'=>VALUE_STEP];
	static $convertActionArgument=['name'=>VALUE_NAME,'direction','relatedStateVariable'];
	static $propFunctions=[PROP_VOLUME_CONTROL=>['GetVolume','SetVolume'],PROP_MUTE_CONTROL=>['GetMute','SetMute'],PROP_BALANCE_CONTROL=>['GetBalance','SetBalance'],PROP_TREBLE_CONTROL=>['GetTreble','SetTreble'],PROP_BASS_CONTROL=>['GetBass','SetBass'],PROP_LOUDNESS_CONTROL=>['GetLoudness','SetLoudness'],PROP_BRIGHTNESS_CONTROL=>['GetBrightness','SetBrightness'],PROP_CONTRAST_CONTROL=>['GetContrast','SetContrast'],PROP_SHARPNESS_CONTROL=>['GetSharpness','SetSharpness'],PROP_SOURCE_CONTROL=>['GetSource','SetSource'],PROP_PLAY_CONTROL=>['Play','Stop','Next','Previous'],PROP_REMOTE_CONTROL=>['SendKeyCode','GetKeyCodes'],PROP_CONTENT_BROWSER=>['Browse','Search']];		
 	static $defaultValues=null;
 	if($Logger) $Logger->Debug(DEBUG_BUILD,"Begin build of $UrlToDescXml");
 	$myurl=URL::parse($UrlToDescXml);	
	$oCache=$xml=$RPC_Inject=$info=$icons=null;
	$services=[];
	$def=[DEF_MANU=>'',DEF_MODEL=>'',DEF_VERSION=>$MyVersion,DEF_PROPS=>0,DEF_OPTIONS=>0,DEF_PATH=>$myurl['path']];
	if($CopyToDevice){
		if(is_object($CopyToDevice))$CopyToDevice=json_decode(json_encode($CopyToDevice),true);
		if(!empty($CopyToDevice[DEVICE_CONFIG])){
			
		}
		if(!empty($CopyToDevice[DEVICE_DEF])){
			
			$def=[DEF_MANU=>'',DEF_MODEL=>'',DEF_VERSION=>$MyVersion,DEF_PROPS=>0,DEF_OPTIONS=>0,DEF_PATH=>$myurl['path']];	
		}
		if(!empty($CopyToDevice[DEVICE_ICONS])){
			
		}
		if(!empty($CopyToDevice[DEVICE_SERVICES])){
			$services = $CopyToDevice[DEVICE_SERVICES];
		}
	}
	
	
	$getProps=function ($service) use ($propFunctions){
		$props=0;
		if(!empty($service[SERVICE_EVENTS]))$props=$props | PROP_EVENTS;
		foreach($service[SERVICE_FUNCTIONS] as $name=>$v)
			foreach($propFunctions as $prop=>$functionNames)
				if(in_array($name,$functionNames)){ $props = $props | $prop;break;}
		return $props;
	};
 	$debug=function($Level,$Message,$Params=null)use($Logger){
 		if($Logger)
 			$Logger->Debug($Level, $Message,array_slice(func_get_args(), 2));
 	};
 	$error=function($Message, $Params=null)use($Logger){
 		if($Logger)$Logger->Error(0,$Message,array_slice(func_get_args(), 1));
 		return null;
 	};
 	$controlConfig= array_combine($convertUrl,$myurl);
 	$controlConfig[CONFIG_OPTIONS]=0;
 	$externalDescriptions=function(array $device){
 		if(!function_exists('RpcDescriptionGet'))require_once 'rpcdescriptions.inc';
 		foreach($device[DEVICE_SERVICES] as $sn=>$service){
 			if(isset($service[SERVICE_DESC]) && !is_numeric($service[SERVICE_DESC]))$service[SERVICE_DESC]=RpcDescriptionAdd($service[SERVICE_DESC]);
 			foreach($service[SERVICE_FUNCTIONS] as $fn=>$func){
 				if(isset($func[FUNCTION_DESC]) && !is_numeric($func[FUNCTION_DESC]))$func[FUNCTION_DESC]=RpcDescriptionAdd($func[FUNCTION_DESC]);
 				if(!empty($func[FUNCTION_PARAMS][PARAMS_IN]))foreach($func[FUNCTION_PARAMS][PARAMS_IN] as $param){
 					if(isset($param[VALUE_DESC]) && !is_numeric($param[VALUE_DESC]))$param[VALUE_DESC]=RpcDescriptionAdd($param[VALUE_DESC]);
 				}
 				if(!empty($func[FUNCTION_PARAMS][PARAMS_OUT]))foreach($func[FUNCTION_PARAMS][PARAMS_OUT] as $param){
 					if(isset($param[VALUE_DESC]) && !is_numeric($param[VALUE_DESC]))$param[VALUE_DESC]=RpcDescriptionAdd($param[VALUE_DESC]);
 				}
 			}
 		}
 		RpcDescriptionsSave();
 	};
 	
 	if($myurl['scheme']=='build'){	
		$manu=$myurl['path']?substr($myurl['path'],1):$myurl['host'];
		if($manu==$myurl['host'])$myurl['host']='';
		$detection=[
			DETECTION_URL=>$myurl['host'],
			DETECTION_PORT=>$myurl['port'],
			DETECTION_MANU=>$def[DEF_MANU]=$manu,
			DETECTION_USER=>$myurl['user'],
			DETECTION_PASS=>$myurl['pass']
		];
 		$manuFile=__DIR__."/inject/rpc.inject.$manu";
		if(!preg_match('/\.inc/i',$manuFile))$manuFile.='.inc';
		if(!file_exists($manuFile))return $error(ERR_UnknownFileName,$manuFile);
		require_once $manuFile;
		$manu=str_ireplace('.inc', '', $manu);
	  	$init="INIT_$manu";
		if(function_exists($init)){
 			if(empty($detection[DETECTION_URL]))return $error(ERR_NeedHostName);			
			if(!$init($detection,$Logger))return $error(ERR_InjectInitFailed, $init);	
		}
		$fcount=0;$ecount=0;
		if($RPC_Inject)foreach($RPC_Inject as $inject)if(empty($inject[INJECT_MODELS]) || (is_string($inject[INJECT_MODELS]) && $inject[INJECT_MODELS]=='*')|| (is_array($inject[INJECT_MODELS]) && in_array('*',$inject[INJECT_MODELS]))){
 			if(!empty($inject[INJECT_INFO]))
 				list($def[DEF_MANU],$def[DEF_MODEL],$controlConfig[CONFIG_SCHEME],$controlConfig[CONFIG_PORT],$controlConfig[CONFIG_CONNTYPE])=$inject[INJECT_INFO];
			$debug(DEBUG_BUILD,'Found injection %s for model %s',$def[DEF_MANU],$def[DEF_MODEL]);
 			if(!empty($inject[INJECT_PROPS]))$def[DEF_PROPS]=$inject[INJECT_PROPS];
 			if(!empty($inject[INJECT_OPTIONS]))$controlConfig[CONFIG_OPTIONS]=$controlConfig[CONFIG_OPTIONS] | $inject[INJECT_OPTIONS];
 			if(!empty($inject[INJECT_SERVICES]))foreach($inject[INJECT_SERVICES] as $sn=>$service){
				$debug(DEBUG_BUILD + DEBUG_DETAIL, "Inject service $sn");
				$services[$sn]=$service; $def[DEF_PROPS] =$def[DEF_PROPS] | $getProps($service);
				$fcount+= count($service[SERVICE_FUNCTIONS]);
				$ecount+= empty($service[SERVICE_EVENTS])?0:count($service[SERVICE_EVENTS]);
			}
			break;
		}
		if(empty($services))return $error(ERR_NoServicesFound);
		$controlConfig[CONFIG_SCHEME]='http';
		if(empty($controlConfig[CONFIG_HOST]))$controlConfig[CONFIG_OPTIONS]=$controlConfig[CONFIG_OPTIONS]|OPTIONS_NEED_HOST;
		if(empty($controlConfig[CONFIG_PORT]))$controlConfig[CONFIG_OPTIONS]=$controlConfig[CONFIG_OPTIONS]|OPTIONS_NEED_PORT;
		if(!empty($controlConfig[CONFIG_USER])||!empty($controlConfig[CONFIG_PASS]))$controlConfig[CONFIG_OPTIONS]|OPTIONS_NEED_USER_PASS;
		$device=[DEVICE_INFO=>$info,DEVICE_CONFIG=>$controlConfig,DEVICE_ICONS=>$icons,DEVICE_SERVICES=>$services,DEVICE_DEF=>$def];
		utf8::encode($device);
		if(RPC_EXTERNAL_DESCRIPTIONS===true)$externalDescriptions($device);
		$device=json_decode(json_encode($device));
		utf8::decode($device);
		$debug(DEBUG_BUILD,'Import %s services, %s functions and %s events',count($services),$fcount,$ecount);
		$pn=null;foreach(ALL_PROPS as $prop)if($device->{DEVICE_DEF}->{DEF_PROPS} & $prop)$pn[]=NAMES_PROPS[$prop];
		$debug(DEBUG_BUILD,'Imported props: %s',$pn?implode(',',$pn):'None');
		return $device;
	}
	unset($controlConfig[0]);
 	if($myurl['scheme']=='local'){
		exit(var_dump($myurl));
 		if(!$xml=@simplexml_load_file($UrlToDescXml))return $error(ERR_UnknownFileName,$UrlToDescXml);
  	}
  	if ($UseCache && !empty(RPC_CACHE_DIR)){
 		$oCache=new Cache(RPC_CACHE_DIR);
		if(!$xml=$oCache->Load($UrlToDescXml,true))return $error(ERR_UnknownFileName,$UrlToDescXml);
  	}else
  		$xml=simplexml_load_file($UrlToDescXml);
 	if(empty($controlConfig[CONFIG_PORT]))$controlConfig[CONFIG_PORT]=empty($xml->userDef->port)?80:(int)$xml->userDef->port;
	if(empty($controlConfig[CONFIG_CONNTYPE]))$controlConfig[CONFIG_CONNTYPE]=empty($xml->userDef->connectionType)?CONNECTION_TYPE_SOAP:(int)$xml->userDef->connectionType;
	if(empty($controlConfig[CONFIG_CONNTYPE]))$controlConfig[CONFIG_CONNTYPE]=CONNECTION_TYPE_SOAP;
	$lastNameFromString=function ($s){
		$t=explode(':', $s); while (($v=array_pop($t)) && is_numeric($v));	return $v?$v:$s;
	};
	$xmlTypeToRpcType=function( $DataType)use($convertDataType){
		return empty($convertDataType[$DataType])?DATATYPE_UNKNOWN:$convertDataType[$DataType];
	};
	$mixedToValue=function($DataType, $value){
		switch($DataType){
			case DATATYPE_BOOL	: $value=boolval($value);break;
			case DATATYPE_BYTE 	:
			case DATATYPE_INT 	:
			case DATATYPE_UINT	: $value=intval($value);break;
			case DATATYPE_FLOAT	: $value=flotval($value);break;
			case DATATYPE_STRING: $value = strval($value);break; 
			default				:
				if(is_array($value) && isset($value['default'])){
//					var_dump($DataType, $value);					
					$value = $value['default'];
				}
		}
		return $value;
	};
	$getDeviceInfo=function(SimpleXMLElement $xml)use($convertInfo,$lastNameFromString){
		$r=XML::extractFromItem(empty($xml->device[0])?$xml->device:$xml->device[0], $convertInfo);
		if(!empty($r['deviceType']))$r['deviceType']=$lastNameFromString($r['deviceType']);
		return $r;
	};
	$getDeviceIcons=function (SimpleXMLElement $xml)use($convertIcon){
		$icons=[];
		if(empty($xml->device->iconList->icon))return $icons;
		foreach($xml->device->iconList->icon as $icon)$icons[]=XML::extractFromItem($icon,$convertIcon);
		return $icons;
	};
	$getDeviceServices=function(SimpleXMLElement $xml)use($convertService,$controlConfig,$lastNameFromString){
		$s=[];
		foreach($xml->service as $xmlservice){
			$name=$lastNameFromString((string)$xmlservice->serviceType);
			$service=[SERVICE_NAME=>$name] + XML::extractFromItem($xmlservice, $convertService) ;// $this->_importConfig[CONFIG_PORT]];
			if(!isset($service[SERVICE_CONNTYPE]))$service[SERVICE_CONNTYPE]=$controlConfig[CONFIG_CONNTYPE];
			$service[SERVICE_FUNCTIONS]=(string)$xmlservice->SCPDURL;
			$s[$name]=$service;				
		}
		return $s;
	};
	$parseServices=function(SimpleXMLElement $xml,&$services, $self)use($getDeviceServices){
		foreach($xml->device as $device){
			if(!empty($device->serviceList)) $services=array_merge($services,$getDeviceServices($device->serviceList));
			if(!empty($device->deviceList))  $self($device->deviceList,$services, $self);
		}
	};
	$getServiceFunctions=function (SimpleXMLElement $xml, &$Events)use($defaultValues, $mixedToValue, $xmlTypeToRpcType,$convertStateVar,$convertStateRange,$convertActionArgument){
		$descriptions=empty($xml->descriptions)?null:$xml->descriptions;
		$statevars = $items=[];
		$foundStateVarItem=function ($StateVarName) use($xml){
			foreach($xml->serviceStateTable->stateVariable as $item)if($StateVarName==(string)$item->name)return $item;
			return null;
		};
		foreach($xml->serviceStateTable->stateVariable as $item){
			$typ=$xmlTypeToRpcType((string)$item->dataType);
			if($typ==DATATYPE_UNKNOWN){
				$chk='A_ARG_TYPE_'.strtoupper((string)$item->dataType);
				if($found=$foundStateVarItem($chk)){
					$item=$found; $typ=$xmlTypeToRpcType((string)$item->dataType);
				}
			}
			$statevar=XML::extractFromItem($item, $convertStateVar)	;
			$statevar[VALUE_TYPE]=$xmlTypeToRpcType($statevar[VALUE_TYPE]);
			if(!empty($item->allowedValueRange)){
				$value=isset($statevar[VALUE_DEFAULT])?$statevar[VALUE_DEFAULT]:null;
				$statevar += XML::extractFromItem($item->allowedValueRange, $convertStateRange);
				if(!is_null($value) && ($value<$statevar[VALUE_MIN] || $value>$statevar[VALUE_MAX]) )
					$statevar[VALUE_DEFAULT]=$statevar[VALUE_MIN];
			}
			if(!empty($item->allowedValueList)){
				foreach($item->allowedValueList->allowedValue as $vitem)$statevar[VALUE_LIST][]=(string)$vitem;
				if(!isset($statevar[VALUE_DEFAULT])||!in_array($statevar[VALUE_DEFAULT], $statevar[VALUE_LIST]))$statevar[VALUE_DEFAULT]=$statevar[VALUE_LIST][0];
			}
			if(!isset($statevar[VALUE_DEFAULT]) && stripos($statevar[VALUE_NAME], 'instanceid')!==false)$statevar[VALUE_DEFAULT]=0;
			$vname=$statevar[VALUE_NAME];
			unset($statevar[VALUE_NAME]);
			$attr=$item->attributes();
			if(!empty($attr['sendEvents']) && strcasecmp($attr['sendEvents'], 'yes')==0 && empty($Events[$vname]))$Events[$vname]=$typ;	
			$statevars[$vname]=$statevar;
		}
		foreach($xml->actionList->action as $item){
			$action=[FUNCTION_NAME=>(string)$item->name,FUNCTION_PARAMS=>[PARAMS_IN=>null,PARAMS_OUT=>null]];				
			if(isset($item->description))$action[FUNCTION_DESC]=(string)$item->description;
			if(!empty($item->argumentList->argument )){
				$data_in=$data_out=null;
				foreach ($item->argumentList->argument as $arg){
					$arg=XML::extractFromItem($arg,$convertActionArgument );
					$mode =$arg['direction']=='in'? PARAMS_IN:PARAMS_OUT;
					$statevar=empty($arg['relatedStateVariable'])?$arg[VALUE_NAME]:$arg['relatedStateVariable'];
					$data=[VALUE_NAME=>$arg[VALUE_NAME]];
					if(!empty($statevars[$statevar])) $data += $statevars[$statevar];
					if(!isset($data[VALUE_DEFAULT]) && isset($defaultValues[$arg[VALUE_NAME]]))$data[VALUE_DEFAULT]=$defaultValues[$arg[VALUE_NAME]];
					if(!empty($arg[VALUE_DESC]))$data[VALUE_DESC]=$arg[VALUE_DESC];
					
					if($mode==PARAMS_OUT){
						unset($data[VALUE_DEFAULT]);
						$data_out[]=$data;
					}else $data_in[]=$data;
				}
				$action[FUNCTION_PARAMS]=[PARAMS_IN=>$data_in,PARAMS_OUT=>$data_out];
			}
			$items[$action[FUNCTION_NAME]]=$action;
		}
		return $items;
	};
	$updateServiceFunctions=function(&$services, &$Events, $local=false) use($oCache,$controlConfig, $getServiceFunctions){
		foreach($services as $sname=>&$service){
 			if(!$local && $oCache){
				$url=$controlConfig[CONFIG_SCHEME]."://".$controlConfig[CONFIG_HOST];
				$port=empty($service[SERVICE_PORT])?$controlConfig[CONFIG_PORT]:$service[SERVICE_PORT];
				if($port)$url.=":$port";
				if($service[SERVICE_FUNCTIONS][0]!='/' && $port)$url.='/';
				$xml=$oCache->Load($url.$service[SERVICE_FUNCTIONS],true);
			}else $xml=simplexml_load_file($service[SERVICE_FUNCTIONS]);	
			$ev=[];
			$service[SERVICE_FUNCTIONS]=$getServiceFunctions($xml,$ev);
			if(!is_null($ev))$service[SERVICE_EVENTS]=$ev;
			$Events=array_merge($Events,$ev);
		}	
	};
	$isLocal=!($oCache);
	$info=$getDeviceInfo($xml);
	$icons=$getDeviceIcons($xml);
	$events=[];
	$parseServices($xml,$services,$parseServices);
	$updateServiceFunctions($services,$events);
	$detection=$injections=null;
	$infostr=implode(' ',$info);
	if($infostr && function_exists('DetectManufaturerInfo')){
		if(!$detection=DetectManufaturerInfo($infostr ,$Logger)) $detection=RPC_Detections[DETECT_GENERIC]; 
	
		$def[DEF_MANU]=$detection[DETECTION_MANU];
		$def[DEF_MODEL]=$detection[DETECTION_MODEL];
		if(!empty($detection[DETECTION_USER])&&$detection[DETECTION_USER]===true)$controlConfig[CONFIG_OPTIONS]=$controlConfig[CONFIG_OPTIONS] | OPTIONS_NEED_USER_PASS;
		if(!empty($detection[DETECTION_INJECT]))foreach($detection[DETECTION_INJECT] as $injectFile){
			if($UrlToDescXml==$injectFile)continue;
			require_once RPC_INJECT_DIR. '/' . $injectFile;
			$init='INIT_'.explode('.',$injectFile)[2];
			if(function_exists($init))$init($detection);
			
			if($RPC_Inject)foreach($RPC_Inject as $inject)if(empty($inject[INJECT_MODELS]) || (is_string($inject[INJECT_MODELS]) && $inject[INJECT_MODELS]==$detection[DETECTION_MODEL])|| (is_array($inject[INJECT_MODELS]) && in_array($detection[DETECTION_MODEL],$inject[INJECT_MODELS]))){
				if(empty($inject[INJECT_SERVICES]))$injections=null;else $injections=$inject[INJECT_SERVICES];
				if(!empty($inject[INJECT_CHECK])){
					foreach($inject[INJECT_CHECK] as $find=>$removes){
						foreach($services as $s)if($found=isset($s[SERVICE_FUNCTIONS][$find]))break;
						if($found) continue;
						$removes=explode(',',$removes);
						foreach($removes as $remove){
							foreach($injections as $sn=>&$service){
								if(isset($service[SERVICE_FUNCTIONS][$remove])){
									if($remove=='GetKeyCodes'){
										unset($service[SERVICE_EXPORT]['Key_Codes']);
										if(!empty($inject[INJECT_PROPS]) && ($inject[INJECT_PROPS] & PROP_REMOTE_CONTROL))
											$inject[INJECT_PROPS]=$inject[INJECT_PROPS] - PROP_REMOTE_CONTROL;
									}
									unset($service[SERVICE_FUNCTIONS][$remove]);
									if(count($service[SERVICE_FUNCTIONS])==0)unset($injections[$sn]);
									if(count($injections)==0){ $injections=null;break;}
								}
							}
							if(!$injections)break;
						}
					}
				}
				if($injections){
					if(!empty($inject[INJECT_PROPS]))$def[DEF_PROPS]=$def[DEF_PROPS] | $inject[INJECT_PROPS];
					if(!empty($inject[INJECT_OPTIONS]))$controlConfig[CONFIG_OPTIONS]=$controlConfig[CONFIG_OPTIONS] | $inject[INJECT_OPTIONS];
					$debug(DEBUG_BUILD,'Found injection %s for model %s',$detection[DETECTION_MANU],$detection[DETECTION_MODEL]);
				}
				break;
			}
			$RPC_Inject=null;
		}
	}
	
// exit(var_export($injections)); 		
	$fcount=0;$ecount=0;
	foreach($services as $sname=>&$service){
		if($injections && !empty($injections[$sname]) ){
			if($Logger)$Logger->Debug(DEBUG_BUILD + DEBUG_DETAIL, "Inject service $sname");
			$s=&$injections[$sname];
			if($s[SERVICE_EXPORT])$service[SERVICE_EXPORT]=empty($service[SERVICE_EXPORT])?$s[SERVICE_EXPORT]:array_merge($service[SERVICE_EXPORT],$s[SERVICE_EXPORT]);
			foreach($s[SERVICE_FUNCTIONS] as $fn=>$func){
				if(empty($service[SERVICE_FUNCTIONS][$fn]))
					$debug(DEBUG_BUILD + DEBUG_DETAIL, "Inject function $fn");
				else $debug(DEBUG_BUILD + DEBUG_DETAIL, "Inject override function $fn");
				$service[SERVICE_FUNCTIONS][$fn]=$func;
			}	
			unset($injections[$sname]);
		}
		$def[DEF_PROPS] =$def[DEF_PROPS] | $getProps($service);
		$fcount += count($service[SERVICE_FUNCTIONS]);
		$ecount += $service[SERVICE_EVENTS]?count($service[SERVICE_EVENTS]):0;
	}
	if($injections)foreach($injections as $sn=>$newservice){
		$services[$sn]=$newservice;
		$def[DEF_PROPS] =$def[DEF_PROPS] | $getProps($newservice);
		$fcount += count($service[SERVICE_FUNCTIONS]);
		$ecount += $service[SERVICE_EVENTS]?count($service[SERVICE_EVENTS]):0;
	}

	$device=[DEVICE_INFO=>$info,DEVICE_CONFIG=>$controlConfig,DEVICE_ICONS=>$icons,DEVICE_SERVICES=>$services,DEVICE_DEF=>$def];
	utf8::encode($device);
	if(RPC_EXTERNAL_DESCRIPTIONS===true)$externalDescriptions($device);
	$device=json_decode(json_encode($device));
	utf8::decode($device);
	$debug(DEBUG_BUILD,'Import %s services, %s functions and %s events',count($services),$fcount,$ecount);
	$pn=null;foreach(ALL_PROPS as $prop)if($def[DEF_PROPS] & $prop)$pn[]=NAMES_PROPS[$prop];
	$debug(DEBUG_BUILD,'Imported props: %s',$pn?implode(',',$pn):'None');
	if($oCache){
		$debug(DEBUG_BUILD,'Readings from => cache: %s remote: %s',$oCache->Hits['cache'],$oCache->Hits['remote']);
	}
	return $device;
}

function RpcDiscoverNetwork(RpcLogger $Logger=null, $UseCache=true, $MyVersion=RPC_API_VERSION){
 	$debug=function($Level,$Message,$Params=null)use($Logger){
 		if($Logger)$Logger->Debug($Level, $Message,array_slice(func_get_args(), 2));
 		else echo call_user_func_array(['sprintf'],array_slice(func_get_args(), 1))."\n";
 	};
 	$error=function($Message, $Params=null)use($Logger){
 		if($Logger)$Logger->Error(0,$Message,array_slice(func_get_args(), 1));
 		else echo call_user_func_array(['sprintf'],array_slice(func_get_args(), 1))."\n";
 	};
 	$discovered=[];
 	if(!$f=NET::DiscoverSsdpDevices())return [];
//  	require_once 'discover.inc';
// 	$f=$discover;// NET::DiscoverSsdpDevices();
	$debug(DEBUG_BUILD,"begin scan Network for deviced");
	foreach($f as $idiscover=>$discover){
		list(,,,$host,$port,$path)=array_values(URL::parse($url=$discover['URL'],['query','fragment']));
		if(!empty($discover['INFO'])){
			if(!$detect=DetectManufaturerInfo(implode(' ',$discover['INFO']),$Logger,false))continue;
			$manu=$detect[DETECTION_MANU];
			if(empty($discovered[$manu][$port])){
				$discovered[$manu][$port]=['type'=>empty($discover['INFO']['deviceType'])?'':$discover['INFO']['deviceType'],'detect'=>$detect,'info'=>$discover['INFO'] ];
			}
			$d=&$discovered[$detect[DETECTION_MANU]][$port];
			$d['models'][]=$detect[DETECTION_MODEL];
			if(empty($d['urls']))$d['urls'][]=$url;
			else if (!in_array($url, $d['urls'] ))$d['urls'][]=$url;
			unset($f[$idiscover]);
		}
	}
// 	var_dump($f);
	foreach($f as $idiscover=>$discover){
		if(empty($discover['INFO']))continue;
		list(,,,$host,$port,$path)=array_values(URL::parse($url=$discover['URL'],['query','fragment']));
		$info=&$discover['INFO'];
		foreach($discovered as $manu=>&$fdiscover){
			if(empty($fdiscover[$port])){
				continue;
			}
			$finfo=&$fdiscover[$port];
			$found=false;
			foreach($finfo['urls'] as $furl){
				$furl=URL::parse($furl);
				if($found=$furl['host']==$host){
					if(!in_array($url, $finfo['urls']))$finfo['urls'][]=$url;
					unset($f[$idiscover]);
					break;
				}
			}
			if($found)break;
			foreach($finfo['info'] as $key=>$value){
				if($found= (!empty($info[$key]) && $info[$key]==$value)){
					if(!in_array($url, $finfo['urls']))$finfo['urls'][]=$url;
					unset($f[$idiscover]);
					break;
				}
			}
			if($found)break;
			$allinfos = implode(' ',$info);
			if($found=stripos($allinfos,$manu)!==false){
				if(!in_array($url, $finfo['urls']))$finfo['urls'][]=$url;
				unset($f[$idiscover]);
			}
		}
		if(!$found){
			$debug(DEBUG_BUILD + DEBUG_DETAIL, "No found %s add generic device",$url);
			$detect=RPC_Detections[DETECT_GENERIC];
			$manu=$detect[DETECTION_MANU];
			if(empty($discovered[$manu][$port])){
				$discovered[$manu][$port]=['type'=>empty($discover['INFO']['deviceType'])?'':$discover['INFO']['deviceType'],'detect'=>$detect,'info'=>$discover['INFO'] ];
			}
			$d=&$discovered[$detect[DETECTION_MANU]][$port];
			$d['models'][]='All';
			if(empty($d['urls']))$d['urls'][]=$discover['URL'];
			else if (!in_array($discover['URL'], $d['urls'] ))$d['urls'][]=$discover['URL'];
		}
	}
// 	exit(var_export($discovered));
	foreach($discovered as $manu=>$discovers){
		$detect=null;
		foreach($discovers as $dport=>$discover){
			if(is_null($detect))$detect=$discover['detect'];
			unset($discovered[$manu][$dport]['detect'],$discovered[$manu][$dport]['info']);
		}
		$discovered[$manu]=['detect'=>$detect,'ports'=>$discovers];
	}
//  exit(var_export($discovered));
	$result = [];
	$addResult=function($manu,$model, $url, $type)use(&$result){
		$result[]=['MANU'=>$manu,'MODEL'=>$model,'TYPE'=>ucfirst($type),'URL'=>$url];
	};
	foreach($discovered as $manu=>&$discovers){
		$detect=&$discovers['detect'];
		if(!empty($detect[DETECTION_URL])){
			$port=key($discovers['ports']);
			foreach($detect[DETECTION_URL] as $did=>$durl){
				if(!empty($detect[DETECTION_PORT][$did]))$port=$detect[DETECTION_PORT][$did];	
// echo "suche $manu an port $port\n";
				foreach($discovers['ports'] as $dport=>$discover){
					if($dport!=$port)continue;
// echo "gefunden $manu an port $port type {$discover['type']}\n";
					$last_mid=0;
					foreach($discover['urls'] as $mid=>$murl){
// echo "vergleiche $durl mit $murl\n";
						if($found=stripos($murl, "/$durl")!==false){
							$debug(DEBUG_BUILD+DEBUG_DETAIL,"found MASTER for %s",$manu);
							if(empty($discovers['masters']))$discovers['masters'][]=$murl;
							elseif(!in_array($murl, $discovers['masters']))$discovers['masters'][]=$murl;
							$addResult($manu,$discover['models'][$mid],$murl,empty($detect[DETECTION_TYPE])?$discover['type']:$detect[DETECTION_TYPE]);
							$last_mid=$mid;
						}
					}
					if($found)continue;
					$testUrl=URL::parse($discover['urls'][0]);
					$testUrl=$testUrl['scheme'].'://'.$testUrl['host'].':'.$dport.'/'.$durl;
					if(file_get_contents($testUrl)){
						$addResult($manu,$discover['models'][$last_mid],$testUrl,empty($detect[DETECTION_TYPE])?$discover['type']:$detect[DETECTION_TYPE]);
						continue;
					}
					$debug(DEBUG_BUILD+DEBUG_DETAIL,"Not found! Port %s Path %s => add Generic",$port,$durl);
				}
			}
		}
 	}
	$debug(DEBUG_BUILD,"found %s Network devices",count($result));
 	return $result;
}
function RpcDevicePost(stdClass $device, $postUrl='http://www.tier-freunde.org/rpcapi'){
	$saveConfig=$device->{DEVICE_CONFIG};
	$device->{DEVICE_CONFIG}->{CONFIG_LOGIN_U}='';
	$device->{DEVICE_CONFIG}->{CONFIG_LOGIN_P}='';
	$device->{DEVICE_CONFIG}->{CONFIG_HOST}='';
	$options = array(
	    'http' => array(
	        'header'  => "Content-type: application/x-www-form-urlencoded\r\n",
	        'method'  => 'POST',
	        'content' => json_encode($device)
	    )
	);
	$context  = stream_context_create($options);
	$result = file_get_contents($postUrl, false, $context);
	if ($result === FALSE) { /* Handle error */ }
	return $result;
}


function RpcDeviceSave(stdClass $Device,  $JsonConfigFileName='',RpcLogger $Logger=null){
	if(empty($Device))return $Logger? $Logger->Error(ERR_NoDataToSave,ERR_NoDataToSave):false;
	$file=pathinfo($JsonConfigFileName);
	if(empty($file['basename'])){
		if(!empty($Device->{DEVICE_DEF}->{DEF_MANU}))$fn=ucfirst($Device->{DEVICE_DEF}->{DEF_MANU});
		elseif(!empty($Device->{DEVICE_INFO}->manufacturer))$fn=ucfirst($Device->{DEVICE_INFO}->manufacturer);
		else $fn='Unknown';
		if(!empty($Device->{DEVICE_DEF}->{DEF_MODEL}))$fn.='['.$Device->{DEVICE_DEF}->{DEF_MODEL}.']';
		elseif(!empty($Device->{DEVICE_INFO}->friendlyName))$fn.='-'.$Device->{DEVICE_INFO}->friendlyName;
		if(!empty($Device->{DEVICE_INFO}->deviceType))$fn.='-'.ucfirst($Device->{DEVICE_INFO}->deviceType);
		$file['basename']=trim(str_ireplace([':',',','  ','www.','https:','http:','/','.'],['_','-',' ',' ',' ','','','','',''], $fn));
	}
	$file['dirname']=!empty($file['dirname'])?$file['dirname'].'/':(defined('RPC_CONFIG_DIR')?RPC_CONFIG_DIR.'/':'config/');
	if($file['dirname'])@mkdir($file['dirname'],755,true);
	$fileName=$file['dirname'].$file['basename'].'.json';
	if($Logger)$Logger->Debug(empty($JsonConfigFileName)?DEBUG_BUILD:DEBUG_INFO, "Save file to $fileName");
	$config=clone $Device;
	UTF8::encode($config);
	return file_put_contents($fileName, json_encode($config))?$fileName:false;
}
?>