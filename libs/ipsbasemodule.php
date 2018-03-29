<?php

const
	API_PROPS_IDENT = 'API_PROPS';

class IPSBaseModule extends IPSModule {
	function RequestAction($Ident,$Value){
		if($Ident=='GetInstanceID'){
			return $this->InstanceID;
		}
		return false;
	}
	/**
	 * @method getStatus
	 * @return int
	 */
	protected function getStatus(){
		return IPS_GetInstance($this->InstanceID)['InstanceStatus'];
	}
	protected function readProperty($PropName){
		return IPS_GetProperty($this->InstanceID,$PropName);
	}
	protected function setProperty($PropName, $PropValue, $Apply=false){
		if($this->ReadProperty($PropName)!=$PropValue){
			IPS_SetProperty($this->InstanceID,$PropName,$PropValue);
			if($Apply)IPS_ApplyChanges($this->InstanceID);
		}
	}

	protected function CreateProfile_Associations ($Name, $Associations, $Icon="TV", $Color=-1, $DeleteProfile=true, $type=1, $prefix='', $sufix='', $digits=0) {
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
	
}

?>