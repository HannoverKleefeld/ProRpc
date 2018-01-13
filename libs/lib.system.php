<?php
class system {
	public static function array_update(array &$Array1, array $Array2, array $CompareKeys=null){
		if(!is_numeric(key($Array1)))$Array1=[$Array1];
		if(!is_numeric(key($Array2)))$Array2=[$Array2];
		$result=false;
		$cmp=function($a1,$a2) use ($CompareKeys){
			$found=0; $dif=count($CompareKeys);
			foreach ($CompareKeys as $key){
				if(!isset($a1[$key]) || !isset($a2[$key]))continue;
				if($a1[$key]==$a2[$key])$found++;
			}
			return ($found==$dif);
		};
		foreach($Array2 as $k2=>$a2){   // hinzu wenn Array2item nicht in array1item
			$found=false;
			foreach($Array1 as $k1=>$a1){
				if($CompareKeys && $found=$cmp($a1,$a2))break;
				elseif(!$CompareKeys && $found=($a2 == $a1))break;
			}
			if(!$found){ $Array1[]=$a2; $result=true;}		
		}
		foreach($Array1 as $k1=>$a1){ 	// entfernen wenn Array1item nicht in array2item
			$found=true;
			foreach($Array2 as $k2=>$a2){
				if($CompareKeys && !($found= !$cmp($a2, $a1)))break;
				else if(!$CompareKeys && !$found=($a2 != $a1))break;
			}
			if($found){ unset($Array1[$k1]);$result=true;}		
		}
		if($result)$Array1=array_values($Array1);		
		return $result;
	}
}
?>