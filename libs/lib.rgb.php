<?php 
const 
	RGB_RED = 0,
	RGB_GREEN=1,
	RGB_BLUE=2;

class rgb {
	public static function fromInt(int $Color){
		list($rgb[RGB_RED], $rgb[RGB_GREEN], $rgb[RGB_BLUE]) = sscanf(substr('000000'.dechex($Color),-6), "%02x%02x%02x");
		return $rgb;
	}
	public static function fromHex(string $HexColor){
		if($HexColor && $HexColor[0]=='#')$HexColor=substr($HexColor,1);
		list($rgb[RGB_RED], $rgb[RGB_GREEN], $rgb[RGB_BLUE]) = sscanf(substr('000000'.$HexColor,-6), "%02x%02x%02x");
		return $rgb;
	}
	public static function toHex(int $Red, int $Green , int $Blue){
		return sprintf('%02x%02x%02x',$Red,$Green,$Blue);	
	}
	public static function toHexA(array $rgb){
		return sprintf('%02x%02x%02x',$rgb[RGB_RED], $rgb[RGB_GREEN], $rgb[RGB_BLUE]);	
	}
	public static function toInt(int $Red, int $Green , int $Blue){
		return hexdec(static::ToHex($Red, $Green, $Blue));		
	}
	public static function toIntA(array $rgb){
		return hexdec(static::ToHexA($rgb));		
	}
	public static function setLevel(array &$rgb, $NewLevel, $OldLevel=null){
		if(is_null($OldLevel))$OldLevel=round(max($rgb)/2.55);
		if($OldLevel==$NewLevel)return false;
		foreach($rgb as &$v){
			$v=round(($v/$OldLevel)*$NewLevel);
			if($v>255)$v=255;elseif($v>0)$v--;
		}	
		return true;
	}	
}

?>