<?php
// ['NAME' => 'Sony', 'MANUREG'=>'/(sony)/i', 'MODELREG'=>'/(\w+[-]\w+\d+)/i'], // STR-DN1050
// ['NAME' => 'Samsung', 'MANUREG'=>'/(samsung)/i', 'MODELREG'=>'/(\w+\d+\w\d+)/i'], // UE55F6400
// ['NAME' => 'Dream', 'MANUREG'=>'/(dream)/i', 'MODELREG'=>''],
// ['NAME' => 'Avm', 'MANUREG'=>'/(fritz!box|avm)/i','MODELREG'=>'/(\d+)/'],
// ['NAME' => 'Homematic', 'MANUREG'=>'/(homematic)/i', 'MODELREG'=>'/(homematic)/']


const 
PRE_DefinedReturnParam 		= [VALUE_NAME => 'Result', 		VALUE_TYPE => 'boolean'],
PRE_DefinedInstanceParam 	= [VALUE_NAME => 'InstanceID',	VALUE_TYPE => 'ui4', 	VALUE_DEFAULT => 0];	

const 
PRE_DefinesFunctions = [
	DEVICE_SERVICES => [
		[
			SERVICE_NAME => 'AVTransport',
			SERVICE_FUNCTIONS => [
				'GetBalance' => [
					FUNCTION_NAME => 'GetBalance',
					FUNCTION_PARAMS => [
						PARAM_IN =>[ PRE_DefinedInstanceParam],
						PARAM_OUT=>[ [VALUE_NAME => 'DesiredBalance',	VALUE_TYPE => 'i4']]
					],
					FUNCTION_SOURCE => '
if($param=$that->GetFunctionParam("SetVolume","Channel")){
	list($master,$lf,$rf)=$param[VALUE_LIST];
 	if(is_null($vl =  $that->GetVolume($InstanceID,$lf)))
 		$result=null;
 	elseif(is_null($vr=$that->GetVolume($InstanceID,$rf))) 
 		$result=null;
 	elseif($vl==$vr)$result=0;
 	elseif($vl>$vr)	$result=-($vl - $vr);
 	else $result=abs($vl - $vr);
	// $result=null;
}else $result=null;
return is_null($result)?null:(int)($result);'
				],
				'SetBalance' => [
					FUNCTION_NAME => 'SetBalance',
					FUNCTION_PARAMS => [
						PARAM_IN =>[PRE_DefinedInstanceParam,
								[VALUE_NAME => 'DesiredBalance',	VALUE_TYPE => 'i4',	VALUE_MIN  => -100,VALUE_MAX  => 100,VALUE_STEP => 1,VALUE_DEFAULT => 0]
						],
						PARAM_OUT=>[PRE_DefinedReturnParam],
					],
					FUNCTION_SOURCE => '
if(!$param=$that->GetFunctionParam("SetVolume","Channel"))return null;
list($master,$lf,$rf)=$param[VALUE_LIST];
if($DesiredBalance > 0){
	if($ok=$that->SetVolume($InstanceID,$rf, 100 - $DesiredBalance)) $ok=$that->SetVolume($InstanceID,$lf,100);
}elseif($DesiredBalance < 0){
	if($ok=$that->SetVolume($InstanceID,$lf, 100 - abs($DesiredBalance))) $ok=$that->SetVolume($InstanceID,$rf,100);
}else {
	$ok=$that->SetVolume($InstanceID,$lf,100);
	if($ok)$ok=$that->SetVolume($InstanceID,$rf,100);
}
return $ok;
'
				]
			]		
		]
	] 			
];
			

const 
	DefinedSynology = [
		PREDEF_DETECTION => [DETECTION_MANU => '/(synology)/i',DETECTION_MODEL => '/(DS\w+)/i']
	],
	DefinedSonos = [
		PREDEF_DETECTION => [DETECTION_MANU => '/(sonos)/i',DETECTION_MODEL => '/(play.\w+)/i']
	],
	DefineSymcom = [
		PREDEF_DETECTION => [DETECTION_MANU => '/(ip-symcon)/i',DETECTION_MODEL => '/(IPS\w+)/i']	
	],
	DefinePlex = [
		PREDEF_DETECTION => [ DETECTION_MANU => '/(plex)/i', /*DETECTION_MODEL => '/(play.\w+)/i' */]	
	],
	DefineHomematic = [
		PREDEF_DETECTION => [ DETECTION_MANU => '/(homematic)/i', DETECTION_MODEL => '/(ccu\w+)/i' ]	
	],
	DefineAVM = [
		PREDEF_DETECTION => [ DETECTION_MANU => '/(fritz!box|avm)/i',DETECTION_MODEL => '/ (\d+)|(aha)/i'],
		PREDEF_ITEMS => [ 
			[	PREDEF_MODELS => ['7390','7490','7590'],
// 				DEVICE_CONFIG => [CONFIG_PROPS_OPTIONS => PROP_PHONEBOOK + PROP_CALLMON]
			],[ 
				PREDEF_MODELS => ['AHA']
			]
		] 	
	],
	DefineSony = [
 		PREDEF_DETECTION => [	DETECTION_MANU => '/(sony)/i',	DETECTION_MODEL => '/^(\w+[-]\w+\d+)/i'	],
		PREDEF_ITEMS => [ 
			[ 
				PREDEF_MODELS => ['STR-DN1050'],
				PREDEF_KEYCODES => [ 
					KEY_0 => "AAAAAgAAADAAAAAJAQ==",
					KEY_1 => "AAAAAgAAADAAAAAAAQ==",
					KEY_2 => "AAAAAgAAADAAAAABAQ==",
					KEY_3 => "AAAAAgAAADAAAAACAQ==",
					KEY_4 => "AAAAAgAAADAAAAADAQ==",
					KEY_5 => "AAAAAgAAADAAAAAEAQ==",
					KEY_6 => "AAAAAgAAADAAAAAFAQ==",
					KEY_7 => "AAAAAgAAADAAAAAGAQ==",
					KEY_8 => "AAAAAgAAADAAAAAHAQ==",
					KEY_9 => "AAAAAgAAADAAAAAIAQ==",
					KEY_POWER => "AAAAAgAAADAAAAAVAQ==",
					KEY_MUTE => "AAAAAgAAADAAAAAUAQ==", // Toogle
		// 					'MuteOn'=> "AAAAAwAADRAAAAAgAQ==",
		// 					'MuteOff'=> "AAAAAwAADRAAAAAhAQ==",
					KEY_OK => "AAAAAgAAADAAAAAMAQ==",
					KEY_MENU => "AAAAAgAAADAAAABTAQ==",
					KEY_INFO => "AAAAAgAAADAAAABLAQ==",
					KEY_RETURN => "AAAAAwAAARAAAAB9AQ==",
					KEY_OPTIONS => "AAAAAwAAARAAAABzAQ==",
					KEY_SRCDOWN => "AAAAAgAAALAAAABpAQ==",
					KEY_SRCUP => "AAAAAgAAALAAAABqAQ==",
					KEY_PLAY => "AAAAAwAAARAAAAAyAQ==",
					KEY_PAUSE => "AAAAAwAAARAAAAA5AQ==",
					KEY_STOP => "AAAAAwAAARAAAAA4AQ==",
					KEY_NEXT => "AAAAAwAAARAAAAAxAQ==",
					KEY_PREV => "AAAAAwAAARAAAAAwAQ==",
					KEY_SHUFFLE => "AAAAAwAAARAAAAAqAQ==",
					KEY_REPEAT => "AAAAAwAAARAAAAAsAQ==",
					KEY_FF => "AAAAAwAAARAAAAA0AQ==",
					KEY_FR => "AAAAAwAAARAAAAAzAQ==",
					KEY_VOLUP => "AAAAAgAAADAAAAASAQ==",
					KEY_VOLDOWN => "AAAAAgAAADAAAAATAQ==",
					KEY_UP => "AAAAAgAAALAAAAB4AQ==",
					KEY_DOWN => "AAAAAgAAALAAAAB5AQ==",
					KEY_LEFT => "AAAAAgAAALAAAAB6AQ==",
					KEY_RIGHT => "AAAAAgAAALAAAAB7AQ==",
		// 					'PureDirect' => "AAAAAwAABRAAAAB5AQ==" 
				],
				DEVICE_SERVICES => [
					[
						SERVICE_NAME => 'ConnectionManager',
						SERVICE_FUNCTIONS => [
							[
								FUNCTION_NAME => 'SendKeyCodes',
								SERVICE_LOWER_NAMES=>false,
								FUNCTION_PARAMS => [
									PARAM_IN=>[
										[	
											VALUE_NAME => 'KeyCodes',
											VALUE_TYPE => 'array',
										],[
											VALUE_NAME => 'Host',
											VALUE_TYPE => 'string',
										],[
											VALUE_NAME => 'SendDelay',
											VALUE_TYPE => 'ui4',
											VALUE_DEFAULT => 250000
										]		
									],
									PARAM_OUT =>[
										[
											VALUE_NAME => 'Status',
											VALUE_TYPE => 'boolean',
										]
									]
								],
								FUNCTION_SOURCE => '
$result=true; $doDelay=count($KeyCodes)>1;
foreach($KeyCodes as &$code){
	if(is_array($code))$result=$code["code"]=$that->getKeyCode($code["code"]);
	else $result=$code=$that->getKeyCode($code);
	if(!$result){$that->error(ERR_KeyCodeNotExists,$code);break;}
}	
if($result)foreach($KeyCodes as $code){ 
	if(is_array($code)){ $delay=$code["delay"]; $code=$code["code"]; } else $delay=$SendDelay;
	if(!$result=$that->X_SendIRCC($code))break;
 	if($doDelay && $delay)usleep($delay);
} 
return (bool)$result;'
							]
						]		
					]
				]		
			] 
		] 			
	],
	DefineDreamBox = [  
		PREDEF_DETECTION => [ DETECTION_MANU => '/(enigma2|dream|dm80)/i',	DETECTION_MODEL => '/(dm8000|dm800|enigma2)/i'],
		PREDEF_ITEMS => [ 
			[ 
				PREDEF_MODELS => ['DM8000','DM800','DM7290']
			],[
				PREDEF_MODELS => ['ENIGMA2'],
				PREDEF_KEYCODES => [ 
					KEY_POWER => 116,
					KEY_0 => 11,
					KEY_1 => 2,
					KEY_2 => 3,
					KEY_3 => 4,
					KEY_4 => 5,
					KEY_5 => 6,
					KEY_6 => 7,
					KEY_7 => 8,
					KEY_8 => 9,
					KEY_9 => 10,
					KEY_PREV => 412,
					KEY_NEXT => 407,
					KEY_VOLUP => 115,
					KEY_VOLDOWN => 114,
					KEY_MUTE => 113,
			// 						'BouquetUp' => 402,
			// 						'BouquetDown' => 403,
			// 						'Lame' => 174,
					KEY_INFO => 358,
					KEY_UP => 103,
					KEY_DOWN => 108,
					KEY_LEFT => 105,
					KEY_RIGHT => 106,
					KEY_MENU => 139,
					KEY_OK => 352,
					KEY_SOURCE0 => 377, // TV
					KEY_SOURCE1 => 392, // 'Audio'
					KEY_SOURCE2 => 393, // 'Video'
					KEY_SOURCE3 => 385, // 'Radio'
					KEY_SOURCE4 => 388, // 'Text'
					KEY_RED => 398,
					KEY_GREEN => 399,
					KEY_YELLOW => 400,
					KEY_BLUE => 401,
					KEY_HELP => 138 
				], 
				DEVICE_CONFIG => [CONFIG_CONNECTION_TYPE => CONNECTION_TYPE_URL],
				DEVICE_SERVICES => [
					[
						SERVICE_FILTER_DEF => '..%s',
						SERVICE_LOWER_NAMES=> true,
						SERVICE_NAME => 'WebApi',
						SERVICE_FUNCTIONS => [
// 							[
// 								FUNCTION_NAME => 'SetSource',
// 								FUNCTION_PARAMS => [
// 									PARAM_IN =>[
// 										[
// 											VALUE_NAME => 'Source',
// 											VALUE_TYPE => 'string',
// 											VALUE_DESC_ID => 'Source select',
// 											VALUE_LIST	  =>['tv','audio','video','radio','text'],
// 											VALUE_DEFAULT => 'tv'
// 										]
// 									],
// 									PARAM_OUT=>[],
// 								],
// 								FUNCTION_SOURCE => ''
// 							],	
								
							[
								FUNCTION_NAME => '_volume',
								FUNCTION_PARAMS => [
									PARAM_IN =>[
										[
											VALUE_NAME => 'VolumeMode',
											VALUE_TYPE => 'string',
											VALUE_DESC_ID => 'Volume mode',
											VALUE_LIST	  =>['set','up','down','mute','get'],
											VALUE_DEFAULT => 'mute'
										],[
												VALUE_NAME => 'DesiredVolume',
												VALUE_TYPE => 'ui2',
												VALUE_DEFAULT => 0
										]
									],
									PARAM_OUT=>[],
								],
								FUNCTION_SOURCE => "switch(\$VolumeMode){
	case 'set' : \$arg=['set'=>'set'.\$DesiredVolume];break;
	case 'get' : \$arg=[];break;	
	case 'up'  : case 'down':  \$arg=['set'=>\$VolumeMode];break;
	case 'mute': 
		if(\$result=\$that->_volume(\"get\")){
			if(\$result[\"ismuted\"]!==(bool)\$DesiredVolume){
				\$arg=['set'=>\$VolumeMode];
			}else{
				exit(true);
			}
		}else \$arg=null;
		break;
	default    : \$arg=\$that->error('Invalid volume mode: '.\$VolumeMode,-100); break;
}
\$filter=['result'=>'..result','resulttext'=>'..resulttext','current'=>'..current','ismuted'=>'..ismuted'];
\$r= is_null(\$arg)?null: \$that->callConnection('/web','vol',\$arg,\$filter,'WebApi');
if(\$r && \$r['result']===false){
	\$r=\$that->error(\"Command error {\$r['resulttext']}\",-101);
}else {
	unset(\$r['result']);
	unset(\$r['resulttext']);
}
return \$r;"
								],
								[
									FUNCTION_NAME => 'SetVolume',
									SERVICE_LOWER_NAMES=>false,
									FUNCTION_PARAMS => [
										PARAM_IN =>[ 
											[ PRE_DefinedInstanceParam, VALUE_NAME => 'DesiredVolume',	VALUE_TYPE => 'ui2']
										],
										PARAM_OUT=>[PRE_DefinedReturnParam],
									],
									FUNCTION_SOURCE => '$result=$that->_volume("set",$DesiredVolume);return (bool)$result;'
								],[
									FUNCTION_NAME => 'GetVolume',
									SERVICE_LOWER_NAMES=>false,
									FUNCTION_PARAMS => [
										PARAM_IN=>[PRE_DefinedInstanceParam],
										PARAM_OUT =>[
											[
												VALUE_NAME => 'DesiredVolume',
												VALUE_TYPE => 'ui2',
											]
										]
									],
									FUNCTION_SOURCE => '$result=$that->_volume("get",0);if($result)$result=$result["current"]; return $result;'
								],[
									FUNCTION_NAME => 'SetMute',
									SERVICE_LOWER_NAMES=>false,
									FUNCTION_PARAMS => [
										PARAM_IN =>[PRE_DefinedInstanceParam,
											[	VALUE_NAME => 'DesiredMute',VALUE_TYPE => 'boolean'	]
										],
										PARAM_OUT=>[PRE_DefinedReturnParam],
									],
									FUNCTION_SOURCE => 'return (bool)$that->_volume("mute",(int)$DesiredMute);'
								],[
									FUNCTION_NAME => 'GetMute',
									SERVICE_LOWER_NAMES=>false,	
									FUNCTION_PARAMS => [
										PARAM_IN=>[PRE_DefinedInstanceParam],
										PARAM_OUT =>[ [VALUE_NAME => 'DesiredMute',VALUE_TYPE => 'boolean']	]
									],
									FUNCTION_SOURCE => 'if($result=$that->_volume("get",0)){$result=$result["ismuted"];}return $result;'
								],[
									FUNCTION_NAME => 'SendKeyCodes',
									SERVICE_LOWER_NAMES=>false,	
									FUNCTION_PARAMS => [
										PARAM_IN=>[
											[	
												VALUE_NAME => 'KeyCodes',
												VALUE_TYPE => 'array',
											],[
												VALUE_NAME => 'Host',
												VALUE_TYPE => 'string',
 											],[
												VALUE_NAME => 'SendDelay',
												VALUE_TYPE => 'ui4',
												VALUE_DEFAULT => 250000
											]		
										],
										PARAM_OUT =>[PRE_DefinedReturnParam	]
									],
									FUNCTION_SOURCE => '
$result=true; $doDelay=count($KeyCodes)>1;
foreach($KeyCodes as &$code){
	if(is_array($code))$result=$code["code"]=$that->getKeyCode($code["code"]);
	else $result=$code=$that->getKeyCode($code);
	if(!$result){$that->error(ERR_KeyCodeNotExists,$code);break;}
}	
if($result)foreach($KeyCodes as $code){ 
	if(is_array($code)){ $delay=$code["delay"]; $code=$code["code"]; } else $delay=$SendDelay;

	if($result=$that->RemoteControl($code,0,"")){
		if(!empty($result["resulttext"]))
			$that->debug('.DEBUG_CALL.',$result["resulttext"],201);
	}else break;
 	if($doDelay && $delay)usleep($delay);
} 
return (bool)$result;'			]
							]
						]
				]
			] 
		] 
	],
	DefineSamsung = [
		PREDEF_DETECTION => [ DETECTION_MANU => '/(samsung)/i',	DETECTION_MODEL => '/^(\w+\d+\w\d+)/i'],
		PREDEF_ITEMS => [ 
			[ 
				PREDEF_MODELS => ['UE55F6400'],
				PREDEF_KEYCODES => [ 
					KEY_0 => 'KEY_0',
					KEY_1 => 'KEY_1',
					KEY_2 => 'KEY_2',
					KEY_3 => 'KEY_3',
					KEY_4 => 'KEY_4',
					KEY_5 => 'KEY_5',
					KEY_6 => 'KEY_6',
					KEY_7 => 'KEY_7',
					KEY_8 => 'KEY_8',
					KEY_9 => 'KEY_9',
					KEY_UP => 'KEY_UP',
					KEY_DOWN => 'KEY_DOWN',
					KEY_LEFT => 'KEY_LEFT',
					KEY_RIGHT => 'KEY_RIGHT',
					KEY_POWER => 'KEY_POWEROFF',
					KEY_MENU => 'KEY_MENU',
		// 			'Prech' => 'KEY_PRECH',
					KEY_HELP => 'KEY_GUIDE',
					KEY_INFO => 'KEY_INFO',
					KEY_RETURN => 'KEY_RETURN',
		// 			'Ch_List' => 'KEY_CH_LIST',
					KEY_ESC => 'KEY_EXIT',
					KEY_OK => 'KEY_ENTER',
					KEY_SOURCE => 'KEY_SOURCE',
					KEY_SOURCE0 => 'KEY_TV',
					KEY_SOURCE1 => 'KEY_HDMI',
					KEY_SOURCE2 => 'KEY_HDMI2',
					KEY_PLAY => 'KEY_PLAY',
					KEY_PAUSE => 'KEY_PAUSE',
					KEY_STOP => 'KEY_STOP',
					KEY_MUTE => 'KEY_MUTE',
					KEY_VOLUP => 'KEY_VOLUP',
					KEY_VOLDOWN => 'KEY_VOLDOWN',
					KEY_CHUP => 'KEY_CHUP',
					KEY_CHDOWN => 'KEY_CHDOWN',
					KEY_PREV => 'KEY_REWIND',
					KEY_NEXT => 'KEY_FF',
					KEY_RECORD => 'KEY_REC',
		// 			'Picture_Size' => 'KEY_PICTURE_SIZE',
					KEY_OPTIONS => 'KEY_TOOLS',
		// 			'Contents' => 'KEY_CONTENTS',
		// 			'W_Link' => 'KEY_W_LINK',
		// 			'Rss' => 'KEY_RSS',
		// 			'Mts' => 'KEY_MTS',
		// 			'Caption' => 'KEY_CAPTION',
		// 			'Ad' => 'KEY_AD',
					
		// 			'Pannel_ChDown' => 'KEY_PANNEL_CHDOWN',
		// 			'D_Off' => 'KEY_D_OFF' 
				], 
				DEVICE_SERVICES => [
					[
						SERVICE_NAME => 'ConnectionManager',
						SERVICE_FUNCTIONS => [
							[
								FUNCTION_NAME => 'SendKeyCodes',
								SERVICE_LOWER_NAMES=>false,
								FUNCTION_PARAMS => [
									PARAM_IN=>[
										[	
											VALUE_NAME => 'KeyCodes',
											VALUE_TYPE => 'array',
										],[
											VALUE_NAME => 'Host',
											VALUE_TYPE => 'string',
// 												VALUE_DEFAULT => ''
										],[
											VALUE_NAME => 'SendDelay',
											VALUE_TYPE => 'ui4',
											VALUE_DEFAULT => 250000
										]		
									],
									PARAM_OUT =>[PRE_DefinedReturnParam]
								],
								FUNCTION_SOURCE => '
$result=true; $doDelay=count($KeyCodes)>1;
foreach($KeyCodes as &$code){
	if(is_array($code))$result=$code["code"]=$that->getKeyCode($code["code"]);
	else $result=$code=$that->getKeyCode($code);
	if(!$result){$that->error(ERR_KeyCodeNotExists,$code);break;}
}	
if($result)while(true){
	if(!$ipinfo=net::get_local_address()){
		$result = $that->error(ERR_CantGetLocalIP); break;
	}
	if(!$sock=fsockopen($Host, 55000)){
		 $result= $that->error(ERR_ConnectTo,$Host); break;
	}
	stream_set_timeout ($sock,2);
	$appstring = "iphone..iapp.samsung";
	$tvappstring = "iphone.UE55C8000.iapp.samsung";
	$remotename = base64_encode("Samsung Device Remote Control");
	$ipencoded = base64_encode($ipinfo[0]);
	$macencoded = base64_encode($ipinfo[1]);
	$message = chr(0x64).chr(0x00).chr(strlen($ipencoded)).chr(0x00).$ipencoded.chr(strlen($macencoded)).chr(0x00).$macencoded.chr(strlen($remotename)).chr(0x00).$remotename;
	$part = chr(0x00).chr(strlen($appstring)).chr(0x00).$appstring.chr(strlen($message)).chr(0x00).$message;
	fwrite($sock,$part);
	$message = chr(0xc8).chr(0x00);
	$part = chr(0x00).chr(strlen($appstring)).chr(0x00).$appstring.chr(strlen($message)).chr(0x00).$message;
	fwrite($sock,$part);

	$doDelay=$SendDelay>0 && count($KeyCodes)>1;
	foreach($KeyCodes as $code){ 
	if(is_array($code)){ $delay=$code["delay"]; $code=$code["code"]; } else $delay=$SendDelay;
		$key=base64_encode($code);
		$message = chr(0x00).chr(0x00).chr(0x00).chr(strlen($key)).chr(0x00).$key;
		$part = chr(0x00).chr(strlen($tvappstring)).chr(0x00).$tvappstring.chr(strlen($message)).chr(0x00).$message;
		fwrite($sock,$part);
		if($doDelay && $delay)usleep($delay);
	}
	fclose($sock);
	$result=true;
	break;
} 
return (bool)$result;'
							]
						]		
					]
				]
		
			]
		]
];



	
const PRE_Defines = [
		'Sonos' 	=> DefinedSonos,
		'Synology'	=> DefinedSynology, 
 		'IPSymcon'	=> DefineSymcom,
 		'Plex' 		=> DefinePlex,
 		'Homematic'	=> DefineHomematic,
 		'AVM' 		=> DefineAVM, 
		'Sony'		=> DefineSony,
		'dreambox'  => DefineDreamBox,
		'Samsung'	=> DefineSamsung,
];

?>