<?php
$PRE_DefinedInstanceParam= [VALUE_NAME => 'InstanceID',VALUE_TYPE => DATATYPE_UINT, VALUE_DEFAULT => 0];
$PRE_DefinedReturnParam  = [VALUE_NAME => 'Result',	VALUE_TYPE => DATATYPE_BOOL];
$RPC_Inject= [
	[	
		INJECT_MODELS  => ['STR-DN1050'],
		INJECT_SERVICES => [
			'ConnectionManager'=>[
				SERVICE_NAME => 'ConnectionManager',
				SERVICE_EXPORT=>[
					'Key_Codes' => [ 
						KEY_0 		=> ['key'=>"AAAAAgAAADAAAAAJAQ==",	'name'=>'0',			'delay'=>0],
						KEY_1 		=> ['key'=>"AAAAAgAAADAAAAAAAQ==",	'name'=>'1',			'delay'=>0],
						KEY_2		=> ['key'=>"AAAAAgAAADAAAAABAQ==",	'name'=>'2',			'delay'=>0],
						KEY_3 		=> ['key'=>"AAAAAgAAADAAAAACAQ==",	'name'=>'3',			'delay'=>0],
						KEY_4 		=> ['key'=>"AAAAAgAAADAAAAADAQ==",	'name'=>'4',			'delay'=>0],
						KEY_5 		=> ['key'=>"AAAAAgAAADAAAAAEAQ==",	'name'=>'5',			'delay'=>0],
						KEY_6 		=> ['key'=>"AAAAAgAAADAAAAAFAQ==",	'name'=>'6',			'delay'=>0],
						KEY_7 		=> ['key'=>"AAAAAgAAADAAAAAGAQ==",	'name'=>'7',			'delay'=>0],
						KEY_8 		=> ['key'=>"AAAAAgAAADAAAAAHAQ==",	'name'=>'8',			'delay'=>0],
						KEY_9 		=> ['key'=>"AAAAAgAAADAAAAAIAQ==",	'name'=>'9',			'delay'=>0],
						KEY_POWER 	=> ['key'=>"AAAAAgAAADAAAAAVAQ==",	'name'=>'Power',		'delay'=>0],
						KEY_MUTE 	=> ['key'=>"AAAAAgAAADAAAAAUAQ==",	'name'=>'Mute',			'delay'=>0], // Toogle
			// 					'MuteOn'=> "AAAAAwAADRAAAAAgAQ==",
			// 					'MuteOff'=> "AAAAAwAADRAAAAAhAQ==",
						KEY_OK 		=> ['key'=>"AAAAAgAAADAAAAAMAQ==",	'name'=>'Ok',			'delay'=>0],
						KEY_MENU 	=> ['key'=>"AAAAAgAAADAAAABTAQ==",	'name'=>'Menu',			'delay'=>0],
						KEY_INFO 	=> ['key'=>"AAAAAgAAADAAAABLAQ==",	'name'=>'Info',			'delay'=>0],
						KEY_RETURN 	=> ['key'=>"AAAAAwAAARAAAAB9AQ==",	'name'=>'Return',		'delay'=>0],
						KEY_OPTIONS => ['key'=>"AAAAAwAAARAAAABzAQ==",	'name'=>'Options',		'delay'=>0],
						KEY_SRCUP 	=> ['key'=>"AAAAAgAAALAAAABqAQ==",	'name'=>'Source up',	'delay'=>0],
						KEY_SRCDOWN => ['key'=>"AAAAAgAAALAAAABpAQ==",	'name'=>'Source down',	'delay'=>0],
						KEY_PLAY 	=> ['key'=>"AAAAAwAAARAAAAAyAQ==",	'name'=>'Play',			'delay'=>0],
						KEY_PAUSE 	=> ['key'=>"AAAAAwAAARAAAAA5AQ==",	'name'=>'Pause',		'delay'=>0],
						KEY_STOP 	=> ['key'=>"AAAAAwAAARAAAAA4AQ==",	'name'=>'Stop',			'delay'=>0],
						KEY_NEXT 	=> ['key'=>"AAAAAwAAARAAAAAxAQ==",	'name'=>'Next',			'delay'=>0],
						KEY_PREV 	=> ['key'=>"AAAAAwAAARAAAAAwAQ==",	'name'=>'Prev',			'delay'=>0],
						KEY_SHUFFLE => ['key'=>"AAAAAwAAARAAAAAqAQ==",	'name'=>'Shuffle',		'delay'=>0],
						KEY_REPEAT 	=> ['key'=>"AAAAAwAAARAAAAAsAQ==",	'name'=>'Repeat',		'delay'=>0],
						KEY_FF 		=> ['key'=>"AAAAAwAAARAAAAA0AQ==",	'name'=>'Fast forward',	'delay'=>0],
						KEY_FR 		=> ['key'=>"AAAAAwAAARAAAAAzAQ==",	'name'=>'Fast revert',	'delay'=>0],
						KEY_VOLUP 	=> ['key'=>"AAAAAgAAADAAAAASAQ==",	'name'=>'Volume up',	'delay'=>0],
						KEY_VOLDOWN => ['key'=>"AAAAAgAAADAAAAATAQ==",	'name'=>'Volume down',	'delay'=>0],
						KEY_UP 		=> ['key'=>"AAAAAgAAALAAAAB4AQ==",	'name'=>'Up',			'delay'=>0],
						KEY_DOWN 	=> ['key'=>"AAAAAgAAALAAAAB5AQ==",	'name'=>'Down',			'delay'=>0],
						KEY_LEFT 	=> ['key'=>"AAAAAgAAALAAAAB6AQ==",	'name'=>'Left',			'delay'=>0],
						KEY_RIGHT 	=> ['key'=>"AAAAAgAAALAAAAB7AQ==",	'name'=>'Right',		'delay'=>0]
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
if(!$code=@$Key_Codes->{$KeyCode})return $this->error(ERR_KeyCodeNotExists,$KeyCode); 
if(($ok=$this->X_SendIRCC($code->key)) && ($SendDelayMS || $SendDelayMS=$code->delay))usleep($SendDelayMS*1000);
return $ok;'
					],
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