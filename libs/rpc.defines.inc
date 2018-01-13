<?
define ('RPC_DEBUG','.deb');
const
PROP_VOLUME_CONTROL 	= 1,
PROP_BALANCE_CONTROL	= 2,
PROP_CONTENT_BROWSER	= 4,
PROP_PLAY_CONTROL  		= 8,
PROP_BRIGHTNESS_CONTROL = 16,
PROP_CONTRAST_CONTROL   = 32,
PROP_SHARPNESS_CONTROL  = 64,
PROP_MUTE_CONTROL  		= 128,
PROP_BASS_CONTROL		= 256,
PROP_TREBLE_CONTROL		= 512,
PROP_SOURCE_CONTROL		= 1024,
PROP_LOUDNESS_CONTROL   = 2048,
// PROP_POWER_ON_OF		= 4096,
// PROP_DIM_UP_DOWN		= 8192,
PROP_REMOTE				= 16384,
PROP_EVENTS     		= 131072,
PROP_ALL_PROPS			= PROP_VOLUME_CONTROL+PROP_BALANCE_CONTROL+PROP_CONTENT_BROWSER+PROP_PLAY_CONTROL+PROP_BRIGHTNESS_CONTROL+PROP_CONTRAST_CONTROL+ PROP_SHARPNESS_CONTROL+PROP_MUTE_CONTROL+PROP_BASS_CONTROL+PROP_TREBLE_CONTROL+PROP_SOURCE_CONTROL+PROP_LOUDNESS_CONTROL+PROP_REMOTE+PROP_EVENTS;

function GetPropNames(int $props, $AllProps=false){
	static $convert = [
		PROP_VOLUME_CONTROL 	=>'VOLUME',
		PROP_BALANCE_CONTROL	=>'BALANCE',
		PROP_CONTENT_BROWSER	=>'CONTENT',
		PROP_PLAY_CONTROL		=>'PLAYSTATE',
		PROP_BRIGHTNESS_CONTROL	=>'BRIGHTNESS',
		PROP_CONTRAST_CONTROL   =>'CONTRAST',
		PROP_SHARPNESS_CONTROL  =>'SHARPNESS',
		PROP_MUTE_CONTROL  		=>'MUTE',
		PROP_BASS_CONTROL		=>'BASS',
		PROP_TREBLE_CONTROL		=>'TREBLE',
		PROP_SOURCE_CONTROL		=>'SOURCE',
		PROP_LOUDNESS_CONTROL	=>'LOUDNESS',	
		
		PROP_REMOTE				=>'Remote',
		PROP_EVENTS				=>'Events',
// 		PROP_CALLMON			=>'CallMon',
// 		PROP_PHONEBOOK			=>'PhoneBook'	
	];
	if($props==0)return [];
	$names=[];
	for($j=0;$j<23;$j++){
		$prop= 1 << $j;
		if(!$AllProps && $prop >= PROP_REMOTE)break;
		if(!($props&$prop))continue;
		$names[$prop]=$convert[$prop];
	}
	return $names;	
}
function GetPropsByNames(array $names, $AllProps=false){
	static $convert = [
		 	'VOLUME'=>PROP_VOLUME_CONTROL,
			'BALANCE'=>PROP_BALANCE_CONTROL,
			'CONTENT'=>PROP_CONTENT_BROWSER,
			'PLAYSTATE'=>PROP_PLAY_CONTROL,
			'BRIGHTNESS'=>PROP_BRIGHTNESS_CONTROL,
			'CONTRAST'=>PROP_CONTRAST_CONTROL,
			'SHARPNESS'=>PROP_SHARPNESS_CONTROL,
			'MUTE'=>PROP_MUTE_CONTROL,
			'BASS'=>PROP_BASS_CONTROL,
			'TREBLE'=>PROP_TREBLE_CONTROL,
			'LOUDNESS'=>PROP_LOUDNESS_CONTROL,
			'SOURCE'=>PROP_SOURCE_CONTROL,
			'Remote'=>PROP_REMOTE,
			'Events'=>PROP_EVENTS,
// 			'CallMon'=>PROP_CALLMON,
// 			'PhoneBook'=>PROP_PHONEBOOK	
	];
	$props=0;
	if(count($names)>0)foreach($names as $name){
		$name=strtoupper($name);
		if(isset($convert[$name]))$props = $props | $convert[$name];
	}
	return $props;	
}

const 
PROP_GROUP_PLAYER 	= 1,
PROP_GROUP_SOUND 	= 2,
PROP_GROUP_TV 		= 4,
PROP_GROUP_INTERNET = 8,
PROP_GROUP_AMPLIFIER= 16;

function GetGroupNames(int $groups, $AllGroups=false){
	static $convert = [
		PROP_GROUP_PLAYER		=>'Player',
		PROP_GROUP_SOUND		=>'Sound',
		PROP_GROUP_TV   		=>'Sat/Tv',
		PROP_GROUP_INTERNET		=>'Internet',
		PROP_GROUP_AMPLIFIER 	=>'Amplifier'
	];
	if($groups==0)return [];
	for($j=0;$j<23;$j++){
		$prop= 1 << $j;
		if($prop > PROP_GROUP_AMPLIFIER)break;
		if(!($groups&$prop))continue;
		$names[$prop]=$convert[$prop];
	}
	return $names;	
}

function GetPropFunctionNames(int $props){
	if($props==0)return [];
	$names=[];
	if($props & PROP_VOLUME_CONTROL)	$names[PROP_VOLUME_CONTROL]		=['GetVolume','SetVolume'];
	if($props & PROP_BALANCE_CONTROL)	$names[PROP_BALANCE_CONTROL]	=['GetBalance','SetBalance']; 
	if($props & PROP_CONTENT_BROWSER)	$names[PROP_CONTENT_BROWSER]	=['Browse','Search'];
	if($props & PROP_PLAY_CONTROL) 		$names[PROP_PLAY_CONTROL]		=['Play','Stop','Next','Previous'];
	if($props & PROP_BRIGHTNESS_CONTROL)$names[PROP_BRIGHTNESS_CONTROL]	=['GetBrightness','SetBrightness'];
	if($props & PROP_CONTRAST_CONTROL) 	$names[PROP_CONTRAST_CONTROL]	=['GetContrast','SetContrast'];
	if($props & PROP_SHARPNESS_CONTROL)	$names[PROP_SHARPNESS_CONTROL]	=['GetSharpness','SetSharpness'];
	if($props & PROP_MUTE_CONTROL)		$names[PROP_MUTE_CONTROL]		=['GetMute','SetMute'];
	if($props & PROP_BASS_CONTROL)		$names[PROP_BASS_CONTROL]		=['GetBass','SetBass'];
	if($props & PROP_TREBLE_CONTROL)	$names[PROP_TREBLE_CONTROL]		=['GetTreble','SetTreble'];
	if($props & PROP_SOURCE_CONTROL)	$names[PROP_SOURCE_CONTROL]		=['GetSource','SetSource'];
	if($props & PROP_LOUDNESS_CONTROL)	$names[PROP_LOUDNESS_CONTROL]	=['GetLoudness','SetLoudness'];
	if($props & PROP_REMOTE)			$names[PROP_REMOTE]				=['SendKeyCodes'];
	return $names;
}

const
PLAYMODE_STOP = 0,
PLAYMODE_PAUSE= 1,
PLAYMODE_PLAY = 2,
PLAYMODE_NEXT = 3,
PLAYMODE_PREV = 4;

const
OPT_CHECK_PARAMS  		= 1,
OPT_RESULT_FILTER 		= 2,
OPT_DEFAULTS_TO_END 	= 4,
OPT_CACHE_ICONS			= 8,
OPT_INCLUDE_DESC  		= 16;

const
CONNECTION_TYPE_SOAP = 0,
CONNECTION_TYPE_JSON = 1,
CONNECTION_TYPE_URL  = 2,
CONNECTION_TYPE_XML  = 3;

const
DEBUG_NONE	 = 0,
DEBUG_INFO	 = 1,
DEBUG_CALL	 = 2,
DEBUG_BUILD  = 4,
DEBUG_DETAIL = 8,
DEBUG_ALL	 = 15;
const
HELP_SHORT	= 0,
HELP_NORMAL = 1,
HELP_FULL   = 2;

const 
DESCRIPTION_XML_IDENT = 'description';
const
KEY_0		= 1,
KEY_1	  	= 2,
KEY_2	  	= 3,
KEY_3	  	= 4,
KEY_4	  	= 5,
KEY_5	  	= 6,
KEY_6	  	= 7,
KEY_7	  	= 8,
KEY_8	  	= 9,
KEY_9	  	= 10,
KEY_UP	  	= 20,
KEY_DOWN  	= 21,
KEY_LEFT  	= 22,
KEY_RIGHT 	= 23,
KEY_OK	  	= 24,
KEY_ESC	  	= 25,
KEY_MENU  	= 26,
KEY_RED	  	= 27,
KEY_GREEN 	= 28,
KEY_YELLOW	= 29,
KEY_BLUE  	= 30,
KEY_POWER 	= 31,
KEY_HELP  	= 32,
KEY_INFO	= 33,
KEY_VOLUP	= 34,
KEY_VOLDOWN	= 35,
KEY_MUTE	= 36,
KEY_PLAY	= 37,
KEY_PAUSE	= 38,
KEY_STOP	= 39,
KEY_PREV	= 40,
KEY_NEXT	= 41,
KEY_SOURCE 	= 42,
KEY_SOURCE0 = 43,
KEY_SOURCE1 = 44,
KEY_SOURCE2	= 45,
KEY_SOURCE3	= 46,
KEY_SOURCE4 = 47,
KEY_RECORD	= 48,
KEY_RETURN	= 49,
KEY_SHUFFLE = 50,
KEY_REPEAT  = 51,
KEY_OPTIONS = 52,
KEY_FF		= 53,
KEY_FR		= 54,
KEY_CHUP 	= 90,
KEY_CHDOWN  = 91,
KEY_SRCUP 	= 92,
KEY_SRCDOWN = 93;

const 
KEYCODE_NAMES=[
	KEY_0=>['KEY_0','Num0'],
	KEY_1=>['KEY_1','Num1'],
	KEY_2=>['KEY_2','Num2'],
	KEY_3=>['KEY_3','Num3'],
	KEY_4=>['KEY_4','Num4'],
	KEY_5=>['KEY_5','Num5'],
	KEY_6=>['KEY_6','Num6'],
	KEY_7=>['KEY_7','Num7'],
	KEY_8=>['KEY_8','Num8'],
	KEY_9=>['KEY_9','Num9'],
	KEY_UP=>['KEY_UP','Up'],
	KEY_DOWN=>['KEY_DOWN','Down'],
	KEY_LEFT=>['KEY_LEFT','Left'],
	KEY_RIGHT=>['KEY_RIGHT','Right'],
	KEY_OK=>['KEY_OK','Ok'],
	KEY_ESC=>['KEY_ESC','Esc'],
	KEY_MENU=>['KEY_MENU','Menu'],
	KEY_RED=>['KEY_RED','Red'],
	KEY_GREEN=>['KEY_GREEN','Green'],
	KEY_YELLOW=>['KEY_YELLOW','Yellow'],
	KEY_BLUE=>['KEY_BLUE','Blue'],
	KEY_POWER=>['KEY_POWER','Power'],
	KEY_HELP=>['KEY_HELP','Help'],
	KEY_INFO=>['KEY_INFO','Info'],
	KEY_VOLUP=>['KEY_VOLUP','VolumeUp'],
	KEY_VOLDOWN=>['KEY_VOLDOWN','VolumeDown'],
	KEY_MUTE=>['KEY_MUTE','Mute'],
	KEY_PLAY=>['KEY_PLAY','Play'],
	KEY_PAUSE=>['KEY_PAUSE','Pause'],
	KEY_STOP=>['KEY_STOP','Stop'],
	KEY_PREV=>['KEY_PREV','Prev'],
	KEY_NEXT=>['KEY_NEXT','Next'],
	KEY_SOURCE=>['KEY_SOURCE','Source'],
	KEY_SOURCE0=>['KEY_SOURCE0','Source0'],
	KEY_SOURCE1=>['KEY_SOURCE1','Source1'],
	KEY_SOURCE2=>['KEY_SOURCE2','Source2'],
	KEY_SOURCE3=>['KEY_SOURCE3','Source3'],
	KEY_SOURCE4=>['KEY_SOURCE4','Source4'],
	KEY_RECORD=>['KEY_RECORD','Record'],
	KEY_RETURN=>['KEY_RETURN','Return'],
	KEY_SHUFFLE=>['KEY_SHUFFLE','Shuffle'],
	KEY_REPEAT=>['KEY_REPEAT','Repeat'],
	KEY_OPTIONS=>['KEY_OPTIONS','Options'],
	KEY_FF=>['KEY_FF','FastForward'],
	KEY_FR=>['KEY_FR','FastBack'],
	KEY_CHUP=>['KEY_CHUP','ChannelUp'],
	KEY_CHDOWN=>['KEY_CHDOWN','ChannelDown'],
	KEY_SRCUP=>['KEY_SRCUP','SourceUp'],
	KEY_SRCDOWN=>['KEY_SRCDOWN','SourceDown']
];

?>