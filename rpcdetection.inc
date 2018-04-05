<?php
/** @file rpcdetection.inc 
 * @brief Detect injetions for predefined devices 
 *
 * All needed constants and helper functions to detect predefined devices 
 * @author Xaver Bauer
 * @date 21.01.2018
 * @version 2.0.1
 * Added PhpDoc Tags
 * @package rpcinject
 * @brief API constants and injetions for predefined devices
 * @copydetails rpcdetection.inc
 */
CONST
DETECT_DREAMBOX		= 1,
DETECT_AVM			= 2,
DETECT_SYNOLOGY		= 3,
DETECT_SONOS 		= 4,
DETECT_SONY			= 5,
DETECT_SAMSUNG		= 6,
DETECT_IPSYMCON		= 7,
DETECT_PLEX			= 8,
DETECT_HOMEMATIC	= 9,

DETECT_GENERIC		= 99;


const
DETECTION_IDENT			= 1,
DETECTION_MANU 			= 2,
DETECTION_MODEL			= 3, 
DETECTION_PORT			= 4,
DETECTION_URL			= 5,
DETECTION_INJECT 		= 6,
DETECTION_MORE_PER_IP	= 7,
DETECTION_PATTERN   	= 8,
DETECTION_VALID_MODELS	= 9,
DETECTION_USER			= 10,
DETECTION_PASS			= 11,
DETECTION_TYPE    		= 12,
// DETECTION_PROPS			= 12,


RPC_Detections = [
	DETECT_DREAMBOX=>[
		DETECTION_IDENT		=> DETECT_DREAMBOX,	
		DETECTION_PATTERN	=> '/(MediaServer) (\w{1,3}\d{1,3}) /i',	
		DETECTION_VALID_MODELS	=> ['dm8000','dm800','dm7000'],	
		DETECTION_PORT 		=> [8200,80],	
		DETECTION_URL  		=> ['rootDesc.xml',''],
		DETECTION_MORE_PER_IP=> false,
		DETECTION_INJECT	 => ['rpc.inject.dreambox.inc']
	],
	DETECT_AVM=>[
		DETECTION_IDENT		=> DETECT_AVM,
		DETECTION_PATTERN	=> '/(fritz!box)[\s\w]+lan (\d{3,4})/i',
		DETECTION_VALID_MODELS	=> ['3490','4010','4020','5490','5491','6430','6490','6591','6820','6890','7360','7362','7369','7390','7412','7430','7490','7560','7580','7581','7582','7590'],
		DETECTION_PORT 		=> [49000],
		DETECTION_URL 		=> ['tr64desc.xml'],
		DETECTION_USER		=> true,	
		DETECTION_MORE_PER_IP=> false,
		DETECTION_INJECT	 => ['rpc.inject.fritzbox.inc'],
		DETECTION_TYPE		=> 'WANConnectionDevice'
	],
	DETECT_SYNOLOGY=>[
		DETECTION_IDENT 	=> DETECT_SYNOLOGY,
		DETECTION_PATTERN  	=> '/(Synology) \w+ (\w{1,3}\d{1,4}[+ ])/i',
		DETECTION_VALID_MODELS	=> [],
		DETECTION_PORT 		=> [0],
		DETECTION_URL 		=> ['ssdp/desc-DSM-eth0.xml'],
		DETECTION_MORE_PER_IP=> false,
		DETECTION_INJECT	 => null,
		DETECTION_TYPE		=> 'NetworkStorage'	
	],
	DETECT_SONOS=>[
		DETECTION_IDENT 	=> DETECT_SONOS,
		DETECTION_PATTERN	=> '/(Sonos) (\w+:\d) \w\d/i',
		DETECTION_VALID_MODELS	=> [],
		DETECTION_PORT 		=> [1400],	
		DETECTION_URL 		=> ['xml/device_description.xml'],
		DETECTION_MORE_PER_IP=> false,
		DETECTION_INJECT	 => ['rpc.inject.sonos.inc'],	
	],
	DETECT_SONY=>[
		DETECTION_IDENT 	=> DETECT_SONY,
		DETECTION_PATTERN	=> '/(sony).+\s(\w{1,3}-\w{1,3}\d{1,4})/i',
		DETECTION_VALID_MODELS	=> ['STR-DN1050'],
		DETECTION_PORT 		=> [8080,8000],	
		DETECTION_URL 		=> ['description.xml','serverxml.xml'],
		DETECTION_MORE_PER_IP=> false,
		DETECTION_INJECT	 => ['rpc.inject.sony.inc'],		
	],
	DETECT_SAMSUNG=>[
		DETECTION_IDENT 	=> DETECT_SAMSUNG,
		DETECTION_PATTERN	=> '/(Samsung)[\w\s]+ (\w{1,3}\d{1,2}\w\d{1,4})/i',
		DETECTION_VALID_MODELS	=> ['UE55F6400'],
		DETECTION_PORT 		=> [7676],
		DETECTION_URL 		=> ['smp_16_'],
		DETECTION_MORE_PER_IP=> false,
		DETECTION_INJECT	 => ['rpc.inject.samsung.inc'],		
	],
	DETECT_IPSYMCON=>[
		DETECTION_IDENT 	=> DETECT_IPSYMCON,
		DETECTION_PATTERN	=> 'IP-Symcon',
		DETECTION_VALID_MODELS	=> null,
		DETECTION_PORT 		=> [3777],	
		DETECTION_URL 		=> 'api',
		DETECTION_MORE_PER_IP=> false,
		DETECTION_INJECT	 => ['rpc.inject.ipsymcon.inc'],	
		DETECTION_TYPE		=> 'SmarthomeServer'		
	],
	DETECT_PLEX=>[
		DETECTION_IDENT 	=> DETECT_PLEX,
		DETECTION_PATTERN	=> '/(Plex) Media Server (\d{1,2}.\d{1,2}.\d{1,2}.\d{1,4})/i',
		DETECTION_VALID_MODELS	=> null,
		DETECTION_PORT 		=> [32469],
		DETECTION_URL 		=> ['DeviceDescription.xml'],
		DETECTION_MORE_PER_IP=> false,
		DETECTION_INJECT	 => null,
		DETECTION_TYPE		=> 'MultimediaStorage',
		DETECTION_MODEL		=> 'all',	
	],
	DETECT_HOMEMATIC=>[
		DETECTION_IDENT 	=> DETECT_HOMEMATIC,
		DETECTION_PATTERN	=> 'Homematic',
		DETECTION_VALID_MODELS	=> null,
		DETECTION_PORT 		=> [2001],
		DETECTION_URL 		=> null,	
		DETECTION_MORE_PER_IP=>false,
		DETECTION_INJECT	=> ['rpc.inject.homematic.inc'],
		DETECTION_TYPE		=> 'SmarthomeServer'	
	],
	DETECT_GENERIC => [
		DETECTION_IDENT		=> DETECT_GENERIC,
		DETECTION_MANU		=> 'Generic',
		DETECTION_MODEL		=> 'all',
		DETECTION_PATTERN	=> null,	
		DETECTION_PORT 		=> null,	
		DETECTION_URL  		=> null,
		DETECTION_MORE_PER_IP=>false,
		DETECTION_INJECT	=> null,
		DETECTION_TYPE		=> 'RPCGenericDevice'	
	]	

];



function DetectManufaturerInfo($DetectionString, RpcLogger $Logger=null, bool $GenericIfNotFound=false){
	if(empty($DetectionString))return false;
	foreach(RPC_Detections as $ident=>$detection){
		if($ident==DETECT_GENERIC || empty($detection[DETECTION_PATTERN]))continue;
		if($Logger)$Logger->Debug(DEBUG_DETECT + DEBUG_DETAIL,'Search for: '.$detection[DETECTION_PATTERN]);
		if(@preg_match($detection[DETECTION_PATTERN], $DetectionString,$m)){
			$manu=$m[1];$model=$m[2];
			if($Logger)$Logger->Debug(DEBUG_DETECT,sprintf('found: %s [%s]',$manu,$model));
			if(!empty($detection[DETECTION_VALID_MODELS]) && !in_array($model,$detection[DETECTION_VALID_MODELS])){
				if($Logger)$Logger->Error(ERR_FoundManuModelNotMatch,$manu,$model);
				continue;
			}
			unset($detection[DETECTION_PATTERN],$detection[DETECTION_VALID_MODELS]);
			if(!empty($detection[DETECTION_MANU]))$manu=$detection[DETECTION_MANU];
			return [DETECTION_MANU=>empty($detection[DETECTION_MANU])?$manu:empty($detection[DETECTION_MANU]),DETECTION_MODEL=>empty($detection[DETECTION_MODEL])?$model:$detection[DETECTION_MODEL]]+$detection;
		}
	}
	return $GenericIfNotFound?RPC_Detections[DETECT_GENERIC]:false;
}

function DetectDevices($Ip, $MultipleDevicesOnOneIp=false){
	$found=false;
	foreach(RPC_Detections as $detection){
		list($ident,$detect_manu,$detect_model,$models,$ports,$urls,$pre_defs_xml,$more_as_one_per_ip)=array_values($detection);
		$deviceFound=false;
// 		echo "$ident => ";
		foreach($ports as $index=>$port){
// 			echo "$port,";
			if($port && ip::exist($Ip,$port ) ) {
				$url= is_array($urls) && !empty($urls[$index])?'/'.$urls[$index]:(is_string($urls)?"/$urls":'');
				url::Set($url);
				if(empty(url::Scheme()))url::Scheme('http');
				url::Host($Ip);
				url::Port($port);
				$detect = $detection;
				$detect[DETECTION_PORT]=$port;
				$detect[DETECTION_URL]=url::Get(WHAT_URL + WHAT_PATH);;
				$deviceFound=true;
				if(!$more_as_one_per_ip){ $found[$ident]=$detect; break; }else $found[$ident][]=$detect;
			}	
		}
// 		echo "[$Ip] ".($deviceFound?" found\n":" Not Found\n");
		if($deviceFound && !$MultipleDevicesOnOneIp )break;
	}	
// 	if($found)ksort($found);
	return $found;
}



?>