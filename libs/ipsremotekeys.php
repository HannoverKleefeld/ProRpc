<?php
/* 
	@author Xaver Bauer
	@version 1.00.1
	Created 19.02.2018 - 14:54:13
	
*/
	
class IPSRemoteKeys {
	static $numPads=[KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9];
	static $movePads=[KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT];
	static $menuPads=[KEY_MENU,KEY_HELP,KEY_INFO,KEY_OPTIONS,KEY_OK,KEY_ESC,KEY_RETURN];
	static $playPads=[KEY_PLAY,KEY_PAUSE,KEY_STOP,KEY_PREV,KEY_NEXT,KEY_FF,KEY_FR,KEY_RECORD,KEY_SHUFFLE,KEY_REPEAT];
	static $sourcePads=[KEY_SOURCE,KEY_SOURCE0,KEY_SOURCE1,KEY_SOURCE2,KEY_SOURCE3,KEY_SOURCE4,KEY_SRCUP,KEY_SRCDOWN];
	static $controlPads=[KEY_VOLUP,KEY_VOLDOWN,KEY_MUTE,KEY_CHUP,KEY_CHDOWN,KEY_POWER];
	static $colorPads=[KEY_RED,KEY_GREEN,KEY_YELLOW,KEY_BLUE];
	
	public static function defaultGroups(){
		return [['NAME'=>'Numpad','ID'=>1,'ICON'=>'Keyboard'],['NAME'=>'Cursor','ID'=>2,'ICON'=>'Cross'],['NAME'=>'Menu','ID'=>3,'ICON'=>'Database'],['NAME'=>'Player','ID'=>4,'ICON'=>'Melody'],['NAME'=>'Source','ID'=>5,'ICON'=>'XBMC'],['NAME'=>'Control','ID'=>6,'ICON'=>'Wave'],['NAME'=>'Buttons','ID'=>7,'ICON'=>''],['NAME'=>'Custom1','ID'=>8,'ICON'=>''],['NAME'=>'Custom2','ID'=>9,'ICON'=>'']];	
	}
	public static function getKeyGroupID($key){
		if(in_array($key,static::$numPads))return 1;
		if(in_array($key,static::$movePads))return 2;
		if(in_array($key,static::$menuPads))return 3;
		if(in_array($key,static::$playPads))return 4;
		if(in_array($key,static::$sourcePads))return 5;
		if(in_array($key,static::$controlPads))return 6;
		if(in_array($key,static::$colorPads))return 7; 
		return 8;
	}
	public static function getKeyMap($key , array $map=null){
		if(empty(NAMES_KEYS[$key]))return null;
		if(empty($map))return ['NAME'=>NAMES_KEYS[$key],'KEY'=>$key,'GROUPID'=>static::getKeyGroupID($key)];
		foreach($map as $m)if($m['KEY']==$key)return $m;
		return null;
	}
	public static function defaultKeyMap(array $RefKeys = null){
		foreach(ALL_KEYS as $key){
			if($RefKeys && !in_array($key, $RefKeys))continue;
			$map[]=['NAME'=>NAMES_KEYS[$key],'KEY'=>$key,'GROUPID'=>static::getKeyGroupID($key)];
		}
		return $map;
	}
	
}
?>