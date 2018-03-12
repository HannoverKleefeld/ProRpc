<?php
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
		DETECTION_INJECT	 => ['rpc.inject.fritzbox.inc']	
	],
	DETECT_SYNOLOGY=>[
		DETECTION_IDENT 	=> DETECT_SYNOLOGY,
		DETECTION_PATTERN  	=> '/(Synology) \w+ (\w{1,3}\d{1,4}[+ ])/i',
		DETECTION_VALID_MODELS	=> [],
		DETECTION_PORT 		=> [0],
		DETECTION_URL 		=> ['ssdp/desc-DSM-eth0.xml'],
		DETECTION_MORE_PER_IP=> false,
		DETECTION_INJECT	 => null	
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
		DETECTION_PORT 		=> [8080],	
		DETECTION_URL 		=> ['description.xml'],
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
		DETECTION_INJECT	 => ['rpc.inject.ipsymcon.inc']		
	],
	DETECT_PLEX=>[
		DETECTION_IDENT 	=> DETECT_PLEX,
		DETECTION_PATTERN	=> '/(Plex) Media Server (\d{1,2}.\d{1,2}.\d{1,2}.\d{1,4})/i',
		DETECTION_VALID_MODELS	=> null,
		DETECTION_PORT 		=> [32469],
		DETECTION_URL 		=> ['DeviceDescription.xml'],
		DETECTION_MORE_PER_IP=> false,
		DETECTION_INJECT	 => null		
	],
	DETECT_HOMEMATIC=>[
		DETECTION_IDENT 	=> DETECT_HOMEMATIC,
		DETECTION_PATTERN	=> 'Homematic',
		DETECTION_VALID_MODELS	=> null,
		DETECTION_PORT 		=> [2001],
		DETECTION_URL 		=> null,	
		DETECTION_MORE_PER_IP=>false,
		DETECTION_INJECT	=> ['rpc.inject.homematic.inc']	
	],
	DETECT_GENERIC => [
		DETECTION_IDENT		=> DETECT_GENERIC,	
		DETECTION_PATTERN	=> null,	
		DETECTION_VALID_MODELS	=> null,	
		DETECTION_PORT 		=> null,	
		DETECTION_URL  		=> null,
		DETECTION_MORE_PER_IP=>false,
		DETECTION_INJECT	=> null	
	]	

];

function DetectManufaturerInfo($DetectionString, RpcLogger $Logger=null){
	if(empty($DetectionString))return false;
// echo "Detect $DetectionString\n";
// $pattern='/Plex Media Server (\d{1,2}.\d{1,2}.\d{1,2}.\d{1,4})/i';// (\w{1,3\d{2}\w\d)/i'; //    .+[\||\s](\w{1,3}-\w{1,3}\d{1,4})/i';
// preg_match($pattern, $DetectionString,$m);
// exit(var_dump($m));


// MediaServer Plex Media Server: Multimedia Server Plex, Inc. http://www.plexapp.com/ Plex Media Server Plex Media Server 1.10.1.4602 http://www.plexapp.com/ uuid:1e9f9d14-f975-765b-bdd8-5e3e8deef9ae
// MediaRenderer [TV] Samsung Samsung Electronics http://www.samsung.com/sec Samsung TV DMR UE55F6400 AllShare1.0 http://www.samsung.com/sec 20110517DMR uuid:0ee6b280-00fa-1000-b849-0c891041f72d
// ZonePlayer 192.168.112.54 - Sonos PLAY:3 Sonos, Inc. http://www.sonos.com Sonos PLAY:3 Sonos PLAY:3 S3 http://www.sonos.com/products/zoneplayers/S3 uuid:RINCON_B8E9373DABCE01400
// Basic nas (DS412+) Synology http://www.synology.com Synology NAS DS412+ DS412+ 6.1-15217 http://www.synology.com 1132227683 uuid:73796E6F-6473-6D00-0000-001132227683//  InternetGatewayDevice|gateway|AVM|www.avm.de|FRITZ!Box Fon WLAN 7390|FRITZ!Box Fon WLAN 7390| - avm|www.avm.de|uuid:739f2409-bccb-40e7-8e6c-9CC7A6BC2DEB
// Detect MediaServer|dm8000 Mediaserver|Justin Maggard|http://www.netgear.com/|MiniDLNA on Linux|Windows Media Connect compatible (MiniDLNA)|1|http://www.netgear.com|12345678|uuid:4d696e69-444c-164e-9d41-0009341c5872
// MediaRenderer|Sony|Sony Corporation|http://www.sony.net/|MULTI CHANNEL AV RECEIVER|STR-DN1050|JB3.2|uuid:5f9ec1b3-ed59-1900-4530-d8d43cd2af47
	foreach(RPC_Detections as $detection){
		if(!$detection[DETECTION_PATTERN])continue;
		if($Logger)$Logger->Debug(DEBUG_DETECT,'Search for: '.$detection[DETECTION_PATTERN]);
		if(@preg_match($detection[DETECTION_PATTERN], $DetectionString,$m)){
			$manu=$m[1];$model=$m[2];
			if($Logger)$Logger->Debug(DEBUG_DETECT,sprintf('found: %s [%s]',$manu,$model));
			if($detection[DETECTION_VALID_MODELS] && !in_array($model,$detection[DETECTION_VALID_MODELS])){
				if($Logger)$Logger->Error(ERR_FoundManuModelNotMatch,$manu,$model);
				return null;
			}
			unset($detection[DETECTION_PATTERN],$detection[DETECTION_VALID_MODELS]);
			return [DETECTION_MANU=>$manu,DETECTION_MODEL=>$model]+$detection;
// 			$r=[DETECTION_MANU=>$manu,DETECTION_MODEL=>$model]+$detection;
// 			ksort($r);
// 			return $r;
		}
	}
	return false;
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