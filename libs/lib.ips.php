<?php
class ips {
	public static function CreateProfile_Associations ($Name, $Associations, $Icon="TV", $Color=-1, $DeleteProfile=true, $type=1, $prefix='', $sufix='', $digits=0) {
		if ($DeleteProfile)@IPS_DeleteVariableProfile($Name);
		@IPS_CreateVariableProfile($Name, $type);
		IPS_SetVariableProfileText($Name, $prefix, $sufix);
		IPS_SetVariableProfileDigits($Name, $digits);
		IPS_SetVariableProfileIcon($Name, $Icon);
		$min=65000;$max=0;
		if($Associations){
			foreach($Associations as $Idx => $IdxName) {
				$min=min($min,$Idx);
				$max=max($max,$Idx);
				if ($IdxName == "") {
				  // Ignore
				} elseif (is_array($Color)) 
					IPS_SetVariableProfileAssociation($Name, $Idx, $IdxName, "", $Color[$Idx]);
				else 
					IPS_SetVariableProfileAssociation($Name, $Idx, $IdxName, "", $Color);
			}
		}
//echo "Min: $min, max : $max";		
		if($max=0)$max=$type==0?1:(strpos($sufix,'%')!==false?100:0);
		if($min < $max)
			IPS_SetVariableProfileValues($Name, $min, $max, 1);
		else 			
			IPS_SetVariableProfileValues($Name, 0, 0, 0);
	
	}
	public static function CreateProfile_Switch ($Name, $DisplayFalse, $DisplayTrue, $Icon="", $ColorOff=-1, $ColorOn=0x00ff00, $IconOff="", $IconOn="", $DeleteProfile=false) {
		if ($DeleteProfile)	@IPS_DeleteVariableProfile($Name);
		@IPS_CreateVariableProfile($Name, 0);
		IPS_SetVariableProfileText($Name, "", "");
		IPS_SetVariableProfileValues($Name, 0, 1, 0);
		IPS_SetVariableProfileDigits($Name, 0);
		IPS_SetVariableProfileIcon($Name, $Icon);
		IPS_SetVariableProfileAssociation($Name, 0, $DisplayFalse, $IconOff, $ColorOff);
		IPS_SetVariableProfileAssociation($Name, 1, $DisplayTrue, $IconOn, $ColorOn);
	}
}
?>