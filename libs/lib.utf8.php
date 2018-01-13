<?php
class utf8 {
	public static function encode_array(array &$toArray){
		$strencode = function(&$item, $key) {if ( is_string($item) )$item = utf8_encode($item); else if ( is_array($item) )	array_walk_recursive($item, $strencode);};
		array_walk_recursive($toArray, $strencode);
	}
	public static function decode_array(array &$fromArray){
		$strdecode = function(&$item, $key) {if ( is_string($item) )$item = utf8_decode($item);	else if ( is_array($item) )	array_walk_recursive($item, $strdecode);};
		array_walk_recursive($fromArray, $strdecode);
	}
	
}
?>