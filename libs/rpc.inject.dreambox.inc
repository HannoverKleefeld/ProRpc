<?php
$PRE_DefinedInstanceParam= [VALUE_NAME => 'InstanceID',VALUE_TYPE => DATATYPE_UINT, VALUE_DEFAULT => 0];
$PRE_DefinedReturnParam  = [VALUE_NAME => 'Result',	VALUE_TYPE => DATATYPE_BOOL];
$RPC_Inject= [
	[	
		INJECT_MODELS  => null,
		INJECT_SERVICES => [
			'WebApi'=>[
				SERVICE_NAME => 'WebApi',
				SERVICE_PORT => 80,
				SERVICE_CONNTYPE => CONNECTION_TYPE_URL,
				SERVICE_ID	 => '',
				SERVICE_CTRLURL => '/web',
				SERVICE_EVENTURL=> '',	
				SERVICE_FILTER => 'e2',
				SERVICE_EVENTS => null,	
	 			SERVICE_EXPORT => [
	 				'Key_Codes'=>[ 
						KEY_POWER 	=> ['key'=>116,	'name'=>'Power',		'delay'=>0],
						KEY_0 	  	=> ['key'=>11,	'name'=>'0',			'delay'=>0],
						KEY_1 		=> ['key'=>2,	'name'=>'1',			'delay'=>0],
						KEY_2 		=> ['key'=>3,	'name'=>'2',			'delay'=>0],
						KEY_3 		=> ['key'=>4,	'name'=>'3',			'delay'=>0],
						KEY_4 		=> ['key'=>5,	'name'=>'4',			'delay'=>0],
						KEY_5 		=> ['key'=>6,	'name'=>'5',			'delay'=>0],
						KEY_6 		=> ['key'=>7,	'name'=>'6',			'delay'=>0],
						KEY_7 		=> ['key'=>8,	'name'=>'7',			'delay'=>0],
						KEY_8 		=> ['key'=>9,	'name'=>'8',			'delay'=>0],
						KEY_9 		=> ['key'=>10,	'name'=>'9',			'delay'=>0],
						KEY_PREV 	=> ['key'=>412,	'name'=>'Prev',			'delay'=>0],
						KEY_NEXT 	=> ['key'=>407,	'name'=>'Next',			'delay'=>0],
						KEY_VOLUP 	=> ['key'=>115,	'name'=>'Volume up',	'delay'=>0],
						KEY_VOLDOWN => ['key'=>114,	'name'=>'Volume down',	'delay'=>0],
						KEY_MUTE 	=> ['key'=>113,	'name'=>'Mute',			'delay'=>0],
				// 						'BouquetUp' => 402,
				// 						'BouquetDown' => 403,
						KEY_ESC		 =>['key'=>174,	'name'=>'Exit',			'delay'=>0],
						KEY_INFO 	=> ['key'=>358,	'name'=>'Info',			'delay'=>0],
						KEY_UP 		=> ['key'=>103,	'name'=>'Up',			'delay'=>0],
						KEY_DOWN 	=> ['key'=>108,	'name'=>'Down',			'delay'=>0],
						KEY_LEFT 	=> ['key'=>105,	'name'=>'Left',			'delay'=>0],
						KEY_RIGHT 	=> ['key'=>106,	'name'=>'Right',		'delay'=>0],
						KEY_MENU 	=> ['key'=>139,	'name'=>'Menu',			'delay'=>0],
						KEY_OK 		=> ['key'=>352,	'name'=>'Ok',			'delay'=>0],
						KEY_SOURCE0 => ['key'=>377,	'name'=>'TV',			'delay'=>0], // TV
						KEY_SOURCE1 => ['key'=>392,	'name'=>'Audio',		'delay'=>0], // 'Audio'
						KEY_SOURCE2 => ['key'=>393,	'name'=>'Video',		'delay'=>0], // 'Video'
						KEY_SOURCE3 => ['key'=>385,	'name'=>'Radio',		'delay'=>0], // 'Radio'
						KEY_SOURCE4 => ['key'=>388,	'name'=>'Text',			'delay'=>0], // 'Text'
						KEY_RED 	=> ['key'=>398,	'name'=>'Red',			'delay'=>0],
						KEY_GREEN 	=> ['key'=>399,	'name'=>'Green',		'delay'=>0],
						KEY_YELLOW 	=> ['key'=>400,	'name'=>'Yellow',		'delay'=>0],
						KEY_BLUE 	=> ['key'=>401,	'name'=>'Blue',			'delay'=>0],
						KEY_HELP 	=> ['key'=>138,	'name'=>'Help',			'delay'=>0] 
					] 
	 			],
				SERVICE_FUNCTIONS => [
					'About'=>[
						FUNCTION_NAME=>'about',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>[
								[VALUE_NAME=>'enigmaversion',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'imageversion',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'webifversion',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'fpversion',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Model',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'lanMac',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'lanDHCP',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'lanIP',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'lanMask',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'lanGW',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Capacity',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Free',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Name',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Type',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'servicename',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'servicenamespace',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'serviceaspect',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'serviceprovider',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'videowidth',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'videoheight',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'servicevideosize',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'apid',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'vpid',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'pcrpid',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'pmtpid',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'txtpid',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'tsid',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'onid',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'sid',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'AddLocation'=>[
						FUNCTION_NAME=>'addlocation',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'dirname',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'createFolder',VALUE_TYPE=>DATATYPE_BOOL]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'AutoTimerList'=>[
						FUNCTION_NAME=>'autotimerlist',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'gl',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'Name',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Match',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Enabled',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'ExcludedTitle',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ExcludedShort',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ExcludedDescription',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ExcludedDays',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'IncludedTitle',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'IncludedShort',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'IncludedDescription',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'IncludedDays',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Services',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Bouquets',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'TimespanBegin',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'TimespanEnd',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Duration',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Counter',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'CounterLeft',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'CounterLimit',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Destination',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'CounterFormatString',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'LastBegin',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Justplay',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'AvoidDuplicateDescription',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Tags',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'AfterEvent',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'toggleDisabledIMG',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'BackUp'=>[
						FUNCTION_NAME=>'backup',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'Filename',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'CurrentTime'=>[
						FUNCTION_NAME=>'currenttime',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>null
						]
					],
					'DeviceInfo'=>[
						FUNCTION_NAME=>'deviceinfo',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>[
								[VALUE_NAME=>'enigmaversion',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'imageversion',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'webifversion',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'fpversion',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'devicename',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Name',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Model',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Mac',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Dhcp',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Ip',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Gateway',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Netmask',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Method6',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Ip6',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Gateway6',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Netmask6',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Capacity',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Free',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'DownMix'=>[
						FUNCTION_NAME=>'downmix',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'enable',VALUE_TYPE=>DATATYPE_BOOL]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'EpgBouquet'=>[
						FUNCTION_NAME=>'epgbouquet',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'bRef',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'time',VALUE_TYPE=>DATATYPE_UINT]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'EventID',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'TimeStart',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Duration',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'CurrentTime',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Title',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Description',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'DescriptionExtended',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceReference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceName',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'EpgMulti'=>[
						FUNCTION_NAME=>'epgmulti',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'bRef',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'time',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'endTime',VALUE_TYPE=>DATATYPE_UINT]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'EventID',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'TimeStart',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Duration',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'CurrentTime',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Title',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Description',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'DescriptionExtended',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceReference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceName',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'EpgNext'=>[
						FUNCTION_NAME=>'epgnext',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'bRef',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'EventID',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'TimeStart',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Duration',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'CurrentTime',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Title',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Description',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'DescriptionExtended',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceReference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceName',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'EpgNow'=>[
						FUNCTION_NAME=>'epgnow',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'bRef',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'EventID',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'TimeStart',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Duration',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'CurrentTime',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Title',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Description',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'DescriptionExtended',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceReference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceName',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'EpgNowNext'=>[
						FUNCTION_NAME=>'epgnownext',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'bRef',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'EventID',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'TimeStart',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Duration',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'CurrentTime',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Title',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Description',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'DescriptionExtended',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceReference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceName',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'EpgSearch'=>[
						FUNCTION_NAME=>'epgsearch',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'search',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'EventID',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'TimeStart',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Duration',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'eventcurrenttime',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Title',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Description',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'DescriptionExtended',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceReference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceName',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'EpgService'=>[
						FUNCTION_NAME=>'epgservice',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'sRef',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'time',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'endTime',VALUE_TYPE=>DATATYPE_UINT]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'EventID',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'TimeStart',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Duration',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'CurrentTime',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Title',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Description',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'DescriptionExtended',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceReference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceName',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'EpgServiceNext'=>[
						FUNCTION_NAME=>'epgservicenext',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'sRef',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'EventID',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'TimeStart',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Duration',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'CurrentTime',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Title',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Description',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'DescriptionExtended',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceReference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceName',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'EpgServiceNow'=>[
						FUNCTION_NAME=>'epgservicenow',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'sRef',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'EventID',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'TimeStart',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Duration',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'CurrentTime',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Title',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Description',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'DescriptionExtended',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceReference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceName',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'EpgSimilar'=>[
						FUNCTION_NAME=>'epgsimilar',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'sRef',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'eventid',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'EventID',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'TimeStart',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Duration',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'CurrentTime',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Title',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Description',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'DescriptionExtended',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceReference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceName',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'External'=>[
						FUNCTION_NAME=>'external',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>[
								[VALUE_NAME=>'Path',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Name',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Version',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'HasGUI',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'GUITarget',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'GetallServices'=>[
						FUNCTION_NAME=>'getallservices',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'sRef',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>null
						]
					],
					'GetAudioTracks'=>[
						FUNCTION_NAME=>'getaudiotracks',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>[
								[VALUE_NAME=>'Description',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Id',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Pid',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Active',VALUE_TYPE=>DATATYPE_BOOL]
							]
						]
					],
					'GetCurrent'=>[
						FUNCTION_NAME=>'getcurrent',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>[
								[VALUE_NAME=>'servicereference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'servicename',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'providername',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'videowidth',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'videoheight',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'servicevideosize',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'iswidescreen',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'apid',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'vpid',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'pcrpid',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'pmtpid',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'txtpid',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'tsid',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'onid',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'sid',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'eventservicereference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'eventservicename',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'eventprovidername',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'eventid',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'eventname',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'eventtitle',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'eventdescription',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'eventstart',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'eventduration',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'eventremaining',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'eventcurrenttime',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'eventdescriptionextended',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'result',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'resulttext',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'current',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ismuted',VALUE_TYPE=>DATATYPE_BOOL]
							]
						]
					],
					'GetCurrLocation'=>[
						FUNCTION_NAME=>'getcurrlocation',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>[
								[VALUE_NAME=>'location',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'GetLocations'=>[
						FUNCTION_NAME=>'getlocations',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>[
								[VALUE_NAME=>'location',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'GetPid'=>[
						FUNCTION_NAME=>'getpid',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>null
						]
					],
					'GetServices'=>[
						FUNCTION_NAME=>'getservices',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'sRef',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'Reference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Name',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'GetTags'=>[
						FUNCTION_NAME=>'gettags',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>[
								[VALUE_NAME=>'tag',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'MediaPlayerAdd'=>[
						FUNCTION_NAME=>'mediaplayeradd',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'file',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'MediaPlayerCmd'=>[
						FUNCTION_NAME=>'mediaplayercmd',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'command',VALUE_TYPE=>DATATYPE_UINT]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'MediaPlayerCurrent'=>[
						FUNCTION_NAME=>'mediaplayercurrent',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>[
								[VALUE_NAME=>'Artist',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Title',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Album',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Year',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Genre',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'CoverFilename',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'MediaPlayerList'=>[
						FUNCTION_NAME=>'mediaplayerlist',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'path',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'types',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'ServiceReference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'IsDirectory',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'Root',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'MediaPlayerLoad'=>[
						FUNCTION_NAME=>'mediaplayerload',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'filename',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'MediaPlayerPlay'=>[
						FUNCTION_NAME=>'mediaplayerplay',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'root',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'file',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'MediaPlayerRemove'=>[
						FUNCTION_NAME=>'mediaplayerremove',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'file',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'MediaPlayerWrite'=>[
						FUNCTION_NAME=>'mediaplayerwrite',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'filename',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'Message'=>[
						FUNCTION_NAME=>'message',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'text',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'type',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'timeout',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'default',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'MessageAnswer'=>[
						FUNCTION_NAME=>'messageanswer',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'getanswer',VALUE_TYPE=>DATATYPE_BOOL]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'MovieDelete'=>[
						FUNCTION_NAME=>'moviedelete',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'sRef',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'MovieList'=>[
						FUNCTION_NAME=>'movielist',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'dirname',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'tag',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'ServiceReference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Title',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Description',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'DescriptionExtended',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceName',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Time',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Length',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Tags',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Filename',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Filesize',VALUE_TYPE=>DATATYPE_UINT]
							]
						]
					],
					'Moviemove'=>[
						FUNCTION_NAME=>'moviemove',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'sRef',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'dirname',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'force',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'background',VALUE_TYPE=>DATATYPE_BOOL]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'MovieTags'=>[
						FUNCTION_NAME=>'movietags',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>[
								[VALUE_NAME=>'tag',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'ParentControlList'=>[
						FUNCTION_NAME=>'parentcontrollist',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>[
								[VALUE_NAME=>'ServiceReference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceName',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'PluginListread'=>[
						FUNCTION_NAME=>'pluginlistread',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'PowerState'=>[
						FUNCTION_NAME=>'powerstate',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'newstate',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'instandby',VALUE_TYPE=>DATATYPE_BOOL]
							]
						]
					],
					'RecordNow'=>[
						FUNCTION_NAME=>'recordnow',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'recordnow',VALUE_TYPE=>DATATYPE_BOOL]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'RemoteControl'=>[
						FUNCTION_NAME=>'remotecontrol',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'command',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'type',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'rcu',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'result',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'resulttext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'RemoveLocation'=>[
						FUNCTION_NAME=>'removelocation',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'dirname',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'RestartTwisted'=>[
						FUNCTION_NAME=>'restarttwisted',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>null
						]
					],
					'Restore'=>[
						FUNCTION_NAME=>'restore',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'Filename',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'SelectAudioTrack'=>[
						FUNCTION_NAME=>'selectaudiotrack',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_UINT]
							],
							PARAMS_OUT=>null
						]
					],
					'ServiceListPlayable'=>[
						FUNCTION_NAME=>'servicelistplayable',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'sRef',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'sRefPlaying',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'ServiceReference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServicePlayable',VALUE_TYPE=>DATATYPE_BOOL]
							]
						]
					],
					'ServiceListreLoad'=>[
						FUNCTION_NAME=>'servicelistreload',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'mode',VALUE_TYPE=>DATATYPE_UINT]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'ServicePlayable'=>[
						FUNCTION_NAME=>'serviceplayable',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'sRef',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'sRefPlaying',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'ServiceReference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServicePlayable',VALUE_TYPE=>DATATYPE_BOOL]
							]
						]
					],
					'Session'=>[
						FUNCTION_NAME=>'session',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>null
						]
					],
					'Settings'=>[
						FUNCTION_NAME=>'settings',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>[
								[VALUE_NAME=>'Name',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Value',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'Signal'=>[
						FUNCTION_NAME=>'signal',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'AGC',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'snrdb',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'snr',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ber',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'acg',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'SleepTimer'=>[
						FUNCTION_NAME=>'sleeptimer',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'cmd',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'time',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'action',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'enabled',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'confirmed',VALUE_TYPE=>DATATYPE_BOOL]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'enabled',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'minutes',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'action',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'confirmed',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'text',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'Stream'=>[
						FUNCTION_NAME=>'stream',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>null
						]
					],
					'StreamSubServices'=>[
						FUNCTION_NAME=>'streamsubservices',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'sRef',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'ServiceReference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Name',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'SubServices'=>[
						FUNCTION_NAME=>'subservices',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>[
								[VALUE_NAME=>'ServiceReference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Name',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'TimerAdd'=>[
						FUNCTION_NAME=>'timeradd',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'sRef',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'repeated',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'begin',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'end',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'name',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'description',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'dirname',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'tags',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'eit',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'disabled',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'justplay',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'afterevent',VALUE_TYPE=>DATATYPE_BOOL]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'TimerAddByEventid'=>[
						FUNCTION_NAME=>'timeraddbyeventid',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'sRef',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'eventid',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'justplay',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'dirname',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'tags',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'Timerchange'=>[
						FUNCTION_NAME=>'timerchange',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'sRef',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'begin',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'end',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'name',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'description',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'dirname',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'tags',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'eit',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'disabled',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'justplay',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'afterevent',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'repeated',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'channelOld',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'beginOld',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'endOld',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'deleteOldOnSave',VALUE_TYPE=>DATATYPE_BOOL]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'TimercleanUp'=>[
						FUNCTION_NAME=>'timercleanup',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'cleanup',VALUE_TYPE=>DATATYPE_BOOL]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'TimerDelete'=>[
						FUNCTION_NAME=>'timerdelete',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'sRef',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'begin',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'end',VALUE_TYPE=>DATATYPE_UINT]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'TimerList'=>[
						FUNCTION_NAME=>'timerlist',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>[
								[VALUE_NAME=>'ServiceReference',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ServiceName',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'EIT',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Name',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Description',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'DescriptionExtended',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Disabled',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'TimeBegin',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'TimeEnd',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'Duration',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'startPrepare',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'justPlay',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'afterEvent',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'Location',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Tags',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'LogEntries',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Filename',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Backoff',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'nextActivation',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'firstTryPrepare',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'State',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Repeated',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'dontSave',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Cancled',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'toggleDisabled',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'toggleDisabledIMG',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'TimerListWrite'=>[
						FUNCTION_NAME=>'timerlistwrite',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'write',VALUE_TYPE=>DATATYPE_BOOL]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'Tpm'=>[
						FUNCTION_NAME=>'tpm',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'cmd',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'random',VALUE_TYPE=>DATATYPE_BOOL]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'level2cert',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'level3cert',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'challengevalue',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'result',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'resulttext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'TvBrowser'=>[
						FUNCTION_NAME=>'tvbrowser',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'sRef',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'name',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'description',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'dirname',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'tags',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'eit',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'disabled',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'justplay',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'afterevent',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'command',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'year',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'month',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'day',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'shour',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'smin',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'ehour',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'emin',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'repeated',VALUE_TYPE=>DATATYPE_BOOL],
								[VALUE_NAME=>'syear',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'smonth',VALUE_TYPE=>DATATYPE_UINT],
								[VALUE_NAME=>'sday',VALUE_TYPE=>DATATYPE_UINT]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
					'Vol'=>[
						FUNCTION_NAME=>'vol',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'set',VALUE_TYPE=>DATATYPE_STRING, VALUE_DEFAULT=>'<CLEAR>']
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'result',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'resulttext',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'current',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ismuted',VALUE_TYPE=>DATATYPE_BOOL]
							]
						]
					],
					'Zap'=>[
						FUNCTION_NAME=>'zap',
						FUNCTION_PARAMS=>[
							PARAMS_IN=>[
								$PRE_DefinedInstanceParam,
								[VALUE_NAME=>'sRef',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'title',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'state',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'statetext',VALUE_TYPE=>DATATYPE_STRING]
							]
						]
					],
						
// 					'volume'=>[	
// 						FUNCTION_NAME => 'volume',
// 						FUNCTION_PARAMS => [
// 							PARAMS_IN =>[
// 								[VALUE_NAME => 'VolumeMode',VALUE_TYPE => DATATYPE_STRING, VALUE_LIST=>['set','up','down','mute','get'],VALUE_DEFAULT => 'mute'],
// 								[VALUE_NAME => 'DesiredVolume',	VALUE_TYPE => DATATYPE_UINT]
// 							],
// 							PARAMS_OUT=>[
									
// 							]
// 						],
// 						FUNCTION_SOURCE => 'switch($VolumeMode){
// 	case "set" : $arg=["set"=>"set".$DesiredVolume];break;
// 	case "get" : $arg=[];break;	
// 	case "up"  : case "down": $arg=["set"=>$VolumeMode];break;
// 	case "mute": 
// 		if($result=$that->volume("get")){
// 			if($result["ismuted"]!==(bool)$DesiredVolume){
// 				$arg=["set"=>$VolumeMode];
// 			}else{
// 				return true;
// 			}
// 		}else $arg=null;
// 		break;
// 	default    : $arg=$that->error("Invalid volume mode: ".$VolumeMode,-100); break;
// }
// $r=$that->Vol($arg);
// if($r && $r["result"]===false){
// 	return $that->error("Command error {$r["resulttext"]}",-101);
// }else {
// 	unset($r["result"]);
// 	unset($r["resulttext"]);
// }
// return $r;'
// 					],
					'SetVolume'=>[
						FUNCTION_NAME => 'SetVolume',
						FUNCTION_PARAMS => [
							PARAMS_IN =>[$PRE_DefinedInstanceParam,	[VALUE_NAME => 'DesiredVolume',	VALUE_TYPE => DATATYPE_UINT, VALUE_MIN=>0, VALUE_MAX=>100]],
							PARAMS_OUT=>[$PRE_DefinedReturnParam]
						],
						FUNCTION_SOURCE => '
if(!empty($STATIC["last_vol"]) && $STATIC["last_vol"]["current"]==$DesiredVolume)return true;
if(!$STATIC["last_vol"]=$this->Vol($InstanceID,"set$DesiredVolume"))return false;
return true;'
					],
					'GetVolume'=>[
						FUNCTION_NAME => 'GetVolume',
						FUNCTION_PARAMS => [
							PARAMS_IN=>[ $PRE_DefinedInstanceParam],
							PARAMS_OUT =>[ [VALUE_NAME => 'DesiredVolume',	VALUE_TYPE => DATATYPE_UINT ]]
						],
						FUNCTION_SOURCE => '
if(!empty($STATIC["last_vol"]))return $STATIC["last_vol"]["current"];
if(!$STATIC["last_vol"]=$this->Vol($InstanceID))return false;
return $STATIC["last_vol"]["current"];
'
					],
					'SetMute'=>[
						FUNCTION_NAME => 'SetMute',
						FUNCTION_PARAMS => [
							PARAMS_IN =>[$PRE_DefinedInstanceParam,	[VALUE_NAME => 'DesiredMute',VALUE_TYPE => DATATYPE_BOOL]],
							PARAMS_OUT=>[ $PRE_DefinedReturnParam]
						],
						FUNCTION_SOURCE => '
if(!empty($STATIC["last_vol"]) && $STATIC["last_vol"]["ismuted"]==$DesiredMute)return true;
return ($STATIC["last_vol"]=$this->Vol($InstanceID,"mute"));'
					],
					'GetMute'=>[
						FUNCTION_NAME => 'GetMute',
						FUNCTION_PARAMS => [
							PARAMS_IN=>[ $PRE_DefinedInstanceParam ],
							PARAMS_OUT =>[	[VALUE_NAME => 'DesiredMute',VALUE_TYPE => DATATYPE_BOOL]]
						],
						FUNCTION_SOURCE => '
if(!empty($STATIC["last_vol"]))return $STATIC["last_vol"]["ismuted"];
return ($STATIC["last_vol"]=$this->Vol($InstanceID))?$STATIC["last_vol"]["ismuted"]:false;'
					],
					'SendKeyCode'=>[
						FUNCTION_NAME => 'SendKeyCode',
						FUNCTION_EXPORT => ['Key_Codes'],	
						FUNCTION_PARAMS => [
							PARAMS_IN=>[ $PRE_DefinedInstanceParam,	
								[ VALUE_NAME => 'KeyCode',	VALUE_TYPE => DATATYPE_BYTE],
 								[VALUE_NAME => 'SendDelayMS',	VALUE_TYPE => DATATYPE_UINT, VALUE_DEFAULT => 0]		
							],
							PARAMS_OUT =>[$PRE_DefinedReturnParam]
						],
						FUNCTION_SOURCE => '
if(!$code=@$Key_Codes->{$KeyCode})return $this->error(ERR_KeyCodeNotExists,$KeyCode); 
if(($ok=$this->RemoteControl($InstanceID,$code->key,0,"")) && ($SendDelayMS || $SendDelayMS=$code->delay))usleep($SendDelayMS *1000);
return $ok;'
					],
					'GetKeyCodes'=>[
						FUNCTION_NAME => 'GetKeyCodes',
						FUNCTION_EXPORT => ['Key_Codes'],	
						FUNCTION_PARAMS => [
							PARAMS_IN=>  [ $PRE_DefinedInstanceParam],
							PARAMS_OUT =>[	[VALUE_NAME => 'KeyCodes',	VALUE_TYPE => DATATYPE_ARRAY] ]
						],
						FUNCTION_SOURCE => 'return clone $Key_Codes;'
					]
				]
			]		
		]
	]		
];

?>