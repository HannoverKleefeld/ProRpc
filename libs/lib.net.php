<?php
class net {
	public static function get_local_address(){ 
		static $myip=null,$mymac=null;
		if(!$myip){
			ob_start();
			system("ipconfig /all");
			$s=str_replace(" ","",ob_get_contents());
			ob_clean();
			if(preg_match("|:([0-9a-fA-F]+)-([0-9a-fA-F]+)-([0-9a-fA-F]+)-([0-9a-fA-F]+)-([0-9a-fA-F]+)-([0-9a-fA-F]+)|",$s,$m)) $mymac=$m[0][0]==':'?substr($m[0],1):$m[0];
			if (preg_match("|:([0-9]+).([0-9]+).([0-9]+).([0-9]+)|",$s,$m))	$myip= $m[0][0]==':'?substr($m[0],1):$m[0];
		}
		return $myip||$mymac?[$myip, $mymac]:null;
	}
	public static function get_local_ip(){ 
		return ($i=static::get_local_address())?$i[0]:null;
	}
	public static function get_local_mac(){ 
		return ($i=static::get_local_address())?$i[1]:null;
	}
	public static function ping($host, $timeout=1){
		if(!$host)return false;
		ob_start();
		exec(sprintf('ping -n 1 -w %d %s',$timeout, escapeshellarg($host)), $res, $rval);
		ob_clean();
		return ($rval == 0);
	}
}
?>