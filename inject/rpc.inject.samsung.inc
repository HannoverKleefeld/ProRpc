<?php
$PRE_DefinedInstanceParam= [VALUE_NAME => 'InstanceID',VALUE_TYPE => DATATYPE_UINT, VALUE_DEFAULT => 0];
$PRE_DefinedReturnParam  = [VALUE_NAME => 'Result',	VALUE_TYPE => DATATYPE_BOOL];
$RPC_Inject= [
	[	
		INJECT_MODELS  => ['UE55F6400'],
		INJECT_SERVICES => [
			'ConnectionManager'=>[
				SERVICE_NAME => 'ConnectionManager',
				SERVICE_EXPORT=>[
					'Key_Codes' => [ 
						KEY_0 		=> ['key'=>"KEY_0",			'name'=>'0',			'delay'=>0],
						KEY_1 		=> ['key'=>"KEY_1",			'name'=>'1',			'delay'=>0],
						KEY_2		=> ['key'=>"KEY_2",			'name'=>'2',			'delay'=>0],
						KEY_3 		=> ['key'=>"KEY_3",			'name'=>'3',			'delay'=>0],
						KEY_4 		=> ['key'=>"KEY_4",			'name'=>'4',			'delay'=>0],
						KEY_5 		=> ['key'=>"KEY_5",			'name'=>'5',			'delay'=>0],
						KEY_6 		=> ['key'=>"KEY_6",			'name'=>'6',			'delay'=>0],
						KEY_7 		=> ['key'=>"KEY_7",			'name'=>'7',			'delay'=>0],
						KEY_8 		=> ['key'=>"KEY_8",			'name'=>'8',			'delay'=>0],
						KEY_9 		=> ['key'=>"KEY_9",			'name'=>'9',			'delay'=>0],
						KEY_POWER 	=> ['key'=>"KEY_POWEROFF",	'name'=>'Power',		'delay'=>0],
						KEY_MUTE 	=> ['key'=>"KEY_MUTE",		'name'=>'Mute',			'delay'=>0], // Toogle
			// 					'MuteOn'=> "AAAAAwAADRAAAAAgAQ==",
			// 					'MuteOff'=> "AAAAAwAADRAAAAAhAQ==",
						KEY_OK 		=> ['key'=>"KEY_ENTER",		'name'=>'Ok',			'delay'=>0],
						KEY_ESC		=> ['key'=>"KEY_EXIT",		'name'=>'Esc',			'delay'=>0],
						KEY_MENU 	=> ['key'=>"KEY_MENU",		'name'=>'Menu',			'delay'=>0],
						KEY_HELP 	=> ['key'=>"KEY_GUIDE",		'name'=>'Help',			'delay'=>0],
						KEY_INFO 	=> ['key'=>"KEY_INFO",		'name'=>'Info',			'delay'=>0],
						KEY_RETURN 	=> ['key'=>"KEY_RETURN",	'name'=>'Return',		'delay'=>0],
						KEY_SOURCE0 => ['key'=>"KEY_SOURCE",	'name'=>'Source select',			'delay'=>0], // TV
						KEY_SOURCE1 => ['key'=>"KEY_TV",		'name'=>'TV',		'delay'=>0], // 'Audio'
						KEY_SOURCE2 => ['key'=>"KEY_HDMI",		'name'=>'HDMI1',		'delay'=>0], // 'Video'
						KEY_SOURCE3 => ['key'=>"KEY_HDMI2",		'name'=>'HDMI1',		'delay'=>0], // 'Radio'
						KEY_RECORD  => ['key'=>"KEY_RECORD",	'name'=>'Record',		'delay'=>0], // 'Radio'
						KEY_OPTIONS => ['key'=>"KEY_TOOLS",		'name'=>'Options',		'delay'=>0],
						KEY_CHUP 	=> ['key'=>"KEY_CHUP",		'name'=>'Channel up',	'delay'=>0],
						KEY_CHDOWN =>  ['key'=>"KEY_CHDOWN",	'name'=>'Channel down',	'delay'=>0],
						KEY_PLAY 	=> ['key'=>"KEY_PLAY",		'name'=>'Play',			'delay'=>0],
						KEY_PAUSE 	=> ['key'=>"KEY_PAUSE",		'name'=>'Pause',		'delay'=>0],
						KEY_STOP 	=> ['key'=>"KEY_STOP==",	'name'=>'Stop',			'delay'=>0],
						KEY_NEXT 	=> ['key'=>"KEY_NEXT",		'name'=>'Next',			'delay'=>0],
						KEY_PREV 	=> ['key'=>"KEY_PREVIOUIS",	'name'=>'Prev',			'delay'=>0],
// 						KEY_SHUFFLE => ['key'=>"AAAAAwAAARAAAAAqAQ==",	'name'=>'Shuffle',		'delay'=>0],
// 						KEY_REPEAT 	=> ['key'=>"AAAAAwAAARAAAAAsAQ==",	'name'=>'Repeat',		'delay'=>0],
						KEY_FF 		=> ['key'=>"KEY_FF",		'name'=>'Fast forward',	'delay'=>0],
						KEY_FR 		=> ['key'=>"KEY_REWIND",	'name'=>'Fast revert',	'delay'=>0],
						KEY_VOLUP 	=> ['key'=>"KEY_VOLUP",		'name'=>'Volume up',	'delay'=>0],
						KEY_VOLDOWN => ['key'=>"KEY_VOLDOWN",	'name'=>'Volume down',	'delay'=>0],
						KEY_UP 		=> ['key'=>"KEY_UP",		'name'=>'Up',			'delay'=>0],
						KEY_DOWN 	=> ['key'=>"KEY_DOWN",		'name'=>'Down',			'delay'=>0],
						KEY_LEFT 	=> ['key'=>"KEY_LEFT",		'name'=>'Left',			'delay'=>0],
						KEY_RIGHT 	=> ['key'=>"KEY_RIGHT",		'name'=>'Right',		'delay'=>0]
					]		
				],	
				SERVICE_FUNCTIONS => [
					'SendKeyCode'=>[
						FUNCTION_NAME => 'SendKeyCode',
						FUNCTION_EXPORT => ['Key_Codes'],	
						FUNCTION_PARAMS => [
							PARAMS_IN=>[ $PRE_DefinedInstanceParam,
								[VALUE_NAME => 'KeyCode',	VALUE_TYPE => DATATYPE_BYTE],
 								[VALUE_NAME => 'SendDelayMS',	VALUE_TYPE => DATATYPE_UINT, VALUE_DEFAULT => 0]		
							],
							PARAMS_OUT =>[ $PRE_DefinedReturnParam ]
						],
						FUNCTION_SOURCE => '
if(!$code=@$Key_Codes->$KeyCode)return $this->error(ERR_KeyCodeNotExists,$KeyCode); 
if(empty($STATIC["my_address"]) && !$STATIC["my_address"]=NET::local_address()) return $this->error(ERR_CantGetLocalAddress);
if(!$sock=fsockopen($this->_device->{DEVICE_CONFIG}->{CONFIG_HOST}, $this->_device->{DEVICE_CONFIG}->{CONFIG_PORT}))return $this->error(ERR_ConnectSocketTo,$this->_device->{DEVICE_CONFIG}->{CONFIG_HOST}, $this->_device->{DEVICE_CONFIG}->{CONFIG_PORT}); 
stream_set_timeout ($sock,2);
$appstring = "iphone..iapp.samsung";
$tvappstring = "iphone.UE55C8000.iapp.samsung";
$remotename = base64_encode("Samsung Device Remote Control");
$ipencoded = base64_encode($STATIC["my_address"][0]);
$macencoded = base64_encode($STATIC["my_address"][1]);
$message = chr(0x64).chr(0x00).chr(strlen($ipencoded)).chr(0x00).$ipencoded.chr(strlen($macencoded)).chr(0x00).$macencoded.chr(strlen($remotename)).chr(0x00).$remotename;
$part = chr(0x00).chr(strlen($appstring)).chr(0x00).$appstring.chr(strlen($message)).chr(0x00).$message;
fwrite($sock,$part);
$message = chr(0xc8).chr(0x00);
$part = chr(0x00).chr(strlen($appstring)).chr(0x00).$appstring.chr(strlen($message)).chr(0x00).$message;
fwrite($sock,$part);
$key=base64_encode($code->key);
$message = chr(0x00).chr(0x00).chr(0x00).chr(strlen($key)).chr(0x00).$key;
$part = chr(0x00).chr(strlen($tvappstring)).chr(0x00).$tvappstring.chr(strlen($message)).chr(0x00).$message;
fwrite($sock,$part);
fclose($sock);
if($SendDelayMS||$SendDelayMS=$code->delay)usleep($SendDelayMS*1000);
return true;'		],
					'GetKeyCodes'=>[
						FUNCTION_NAME => 'GetKeyCodes',
						FUNCTION_EXPORT => ['Key_Codes'],	
						FUNCTION_PARAMS => [
							PARAMS_IN =>[ $PRE_DefinedInstanceParam ],
							PARAMS_OUT=>[ [VALUE_NAME => 'KeyCodes',	VALUE_TYPE => DATATYPE_ARRAY]]
						],
						FUNCTION_SOURCE => 'return clone $Key_Codes;'
					]
				]
			]
		]
	]
];



?>