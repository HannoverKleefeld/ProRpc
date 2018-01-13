<?php
class debug {
	public static function export($var, $LineBreak=''){
		return str_replace(["\t",'  ',"\n",'( ',') ',' => ',', ',',)',"\r"],[' ',' ',$LineBreak,'(',')','=>',',',')',$LineBreak], var_export($var,true));
	}
	public static function as_array($array){
		$out=[];
		foreach($array as $key=>$item){
			if (is_array($item)){
				$out[]='['.static::as_array($item).']';
			}else $out[]=$item;
		}
		return implode(',',$out);
	}
	public static function as_string(string $s, $Rows = 2){
		$count=strlen($s);
		$row = $Rows; $spacer=($row>1)?'    ':'';
		for($j=0; $j<$count;$j++){
			$c=$s[$j];	$ord=ord($c);$hex=dechex($c);
			if($c=="\n"||$c=="\r"||$c=="\f")$c='CR';
			
			$spacer=($Rows>1&&$row<>$Rows)?'  | ':'';
			
			echo sprintf("$spacer S[%3d] = %-2s : %03s : %02x",$j,$c,$ord,$hex);
			if($row-- < 1){ 
				if($Rows>1)echo " Text: ".substr($s,$j-$Rows, $Rows+1);
				echo "\n";$row = $Rows;}
		}		
	}
}
?>