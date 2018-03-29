<?php
/** Helper File to import RPC Devices  
 *   
 *  @author Xaver Bauer
 *  @date 21.01.2018 
 * 
 *  @version 1.2 
 *  Kommentare hinzugefügt (Doxygen).
 *  @version 1.1 
 *  Toten Code entfernt. 
 * 
 *  @version 1.0 
 *  code created  
 */
require_once 'rpc.inject.inc';

/**
 * @param string $UrlToDescXml Url to description xml
 * @param string $MyVersion Current Version 
 * @param RpcLogger $Logger Logger Object
 * @return null|stdClass Device config class 
 */
function ImportRpcDevice($UrlToDescXml, $MyVersion, RpcLogger $Logger=null){
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
	$oCache=$xml=$RPC_Inject=$info=$icons=$services=null;
	$def=[DEF_MANU=>'',DEF_MODEL=>'',DEF_VERSION=>$MyVersion,DEF_PROPS=>0,DEF_OPTIONS=>0];
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
 		$manuFile="rpc.inject.$manu";
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
 	$oCache=new Cache(__DIR__ . '/cache');
	if(!$xml=$oCache->Load($UrlToDescXml,true))return $error(ERR_UnknownFileName,$UrlToDescXml);
 		
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
		$services=[];
		foreach($xml->service as $xmlservice){
			$name=$lastNameFromString((string)$xmlservice->serviceType);
			$service=[SERVICE_NAME=>$name] + XML::extractFromItem($xmlservice, $convertService) ;// $this->_importConfig[CONFIG_PORT]];
			if(!isset($service[SERVICE_CONNTYPE]))$service[SERVICE_CONNTYPE]=$controlConfig[CONFIG_CONNTYPE];
			$service[SERVICE_FUNCTIONS]=(string)$xmlservice->SCPDURL;
			$services[$name]=$service;				
		}
		return $services;
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
	$services=$events=[];
	$parseServices($xml,$services,$parseServices);
	$detection=$injections=null;
	if( function_exists('DetectManufaturerInfo') & ($detection=DetectManufaturerInfo(implode(' ',$info),$Logger)) ){
		$def[DEF_MANU]=$detection[DETECTION_MANU];
		$def[DEF_MODEL]=$detection[DETECTION_MODEL];
		if(!empty($detection[DETECTION_USER])&&$detection[DETECTION_USER]===true)$controlConfig[CONFIG_OPTIONS]=$controlConfig[CONFIG_OPTIONS] | OPTIONS_NEED_USER_PASS;
		if(!empty($detection[DETECTION_INJECT]))foreach($detection[DETECTION_INJECT] as $injectFile){
			if($UrlToDescXml==$injectFile)continue;
			require_once $injectFile;
			$init='INIT_'.explode('.',$injectFile)[2];
			if(function_exists($init))$init($detection);
			if($RPC_Inject)foreach($RPC_Inject as $inject)if(empty($inject[INJECT_MODELS]) || (is_string($inject[INJECT_MODELS]) && $inject[INJECT_MODELS]==$detection[DETECTION_MODEL])|| (is_array($inject[INJECT_MODELS]) && in_array($detection[DETECTION_MODEL],$inject[INJECT_MODELS]))){
				if(empty($inject[INJECT_SERVICES]))$injections=null;else $injections=&$inject[INJECT_SERVICES];
				if(!empty($inject[INJECT_PROPS]))$def[DEF_PROPS]=$def[DEF_PROPS] | $inject[INJECT_PROPS];
				if(!empty($inject[INJECT_OPTIONS]))$controlConfig[CONFIG_OPTIONS]=$controlConfig[CONFIG_OPTIONS] | $inject[INJECT_OPTIONS];
				
				$debug(DEBUG_BUILD,'Found injection %s for model %s',$detection[DETECTION_MANU],$detection[DETECTION_MODEL]);
 				break;
			}
			$RPC_Inject=null;
		}
	}
	$updateServiceFunctions($services,$events);
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
	$device=json_decode(json_encode($device));
	utf8::decode($device);
	$debug(DEBUG_BUILD,'Import %s services, %s functions and %s events',count($services),$fcount,$ecount);
	$pn=null;foreach(ALL_PROPS as $prop)if($def[DEF_PROPS] & $prop)$pn[]=NAMES_PROPS[$prop];
	$debug(DEBUG_BUILD,'Imported props: %s',$pn?implode(',',$pn):'None');
	return $device;
}

?>