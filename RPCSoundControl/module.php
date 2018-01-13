<?php
require_once(IPS_GetKernelDir().'\modules\ProRpc\libs\loader.php');
class RPCSoundControl extends RPCModule {
	function Create(){
		parent::Create();
		ips::CreateProfile_Associations('_Balance', [-100=>'left',100=>'right'],'Music');
	}
	function RequestAction ( $Ident, $Wert ){
		if($v=parent::RequestAction($Ident, $Wert))return $v;
		if($this->getStatus()!=102)return false;		
		switch(GetPropsByNames([$Ident])[0]){
			case PROP_VOLUME_CONTROL : if(($ok=$this->forwardRequest('SetVolume', ['DesiredVolume'=>(int)$Wert]))!==false)$this->setValue($Ident, $Wert); break;
			case PROP_BALANCE_CONTROL: if(($ok=$this->forwardRequest('SetBalance', ['DesiredBalance'=>$Wert]))!==false)$this->setValue($Ident, $Wert);break;
			case PROP_MUTE_CONTROL	  : if(($ok=$this->forwardRequest('SetMute'	, ['DesiredMute'=>(bool)$Wert]))!==false)$this->setValue($Ident, $Wert);break;
			case PROP_BASS_CONTROL	  : if(($ok=$this->forwardRequest('SetBass', ['DesiredBass'=>$Wert]))!==false)$this->setValue($Ident, $Wert);break;
			case PROP_TREBLE_CONTROL : if(($ok=$this->forwardRequest('SetTreble', ['DesiredTreble'=>$Wert]))!==false)$this->setValue($Ident, $Wert);break;
			case PROP_LOUDNESS_CONTROL:if(($ok=$this->forwardRequest('SetLoudness', ['DesiredLoudness'=>$Wert]))!==false)$this->setValue($Ident, $Wert);break;
			
			default: $ok=false;IPS_LogMessage(__FUNCTION__,get_class($this). " invalid Request Action ! Ident: $Ident, Value: $Wert");
		}
// 		switch($Ident){
// 			case 'VOLUME' : if(($ok=$this->forwardRequest('SetVolume', ['DesiredVolume'=>(int)$Wert]))!==false)$this->setValue($Ident, $Wert); break;
// 			case 'BALANCE': if(($ok=$this->forwardRequest('SetBalance', ['DesiredBalance'=>$Wert]))!==false)$this->setValue($Ident, $Wert);break;
// 			case 'MUTE'	  : if(($ok=$this->forwardRequest('SetMute'	, ['DesiredMute'=>(bool)$Wert]))!==false)$this->setValue($Ident, $Wert);break;
// 			case 'BASS'	  : if(($ok=$this->forwardRequest('SetBass', ['DesiredBass'=>$Wert]))!==false)$this->setValue($Ident, $Wert);break;
// 			case 'TREBLE' : if(($ok=$this->forwardRequest('SetTreble', ['DesiredTreble'=>$Wert]))!==false)$this->setValue($Ident, $Wert);break;
// 			case 'LOUDNESS':if(($ok=$this->forwardRequest('SetLoudness', ['DesiredLoudness'=>$Wert]))!==false)$this->setValue($Ident, $Wert);break;
// 			default: $ok=false;IPS_LogMessage(__FUNCTION__,get_class($this). " invalid Request Action ! Ident: $Ident, Value: $Wert");
// 		}
		return $ok?null:false;
	}
	protected function getPropDef($prop){
		if($prop==PROP_VOLUME_CONTROL) return ['VOLUME','Volume','~Intensity.100',$type=1,$pos=1];
		if($prop==PROP_BASS_CONTROL) return ['BASS','Bass','~Intensity.100',1,2];
		if($prop==PROP_TREBLE_CONTROL) return ['TREBLE','Treble','~Intensity.100',1,3];
		if($prop==PROP_MUTE_CONTROL)return ['MUTE','Mute','~Switch',0,4];
		if($prop==PROP_BALANCE_CONTROL)return ['BALANCE','Balance','_Balance',1,5];
		if($prop==PROP_LOUDNESS_CONTROL)return ['LOUDNESS','Loudness','~Switch',0,6];
	}
	protected function getProps(){
 		return [PROP_VOLUME_CONTROL,PROP_BALANCE_CONTROL,PROP_BASS_CONTROL,PROP_TREBLE_CONTROL,PROP_MUTE_CONTROL,PROP_LOUDNESS_CONTROL];
 	}
	protected function aboutModule(){
		return 'RpcPro Sound Control';
	}
 	
	public function SetVolume(int $Volume){
		return $this->RequestAction(GetPropNames(PROP_VOLUME_CONTROL)[PROP_VOLUME_CONTROL], $Volume)!==false;	
	}
	public function SetBass(int $Bass){
		return $this->RequestAction(GetPropNames(PROP_BASS_CONTROL)[PROP_BASS_CONTROL], $Bass)!==false;
	}
	public function SetTreble(int $Treble){
		return $this->RequestAction(GetPropNames(PROP_TREBLE_CONTROL)[PROP_TREBLE_CONTROL], $Treble)!==false;
	}
	public function SetLoudness(bool $Loudness){
		return $this->RequestAction(GetPropNames(PROP_LOUDNESS_CONTROL)[PROP_LOUDNESS_CONTROL], $Loudness)!==false;
	}
	public function SetMute(bool $Mute){
		return $this->RequestAction(GetPropNames(PROP_MUTE_CONTROL)[PROP_MUTE_CONTROL], $Mute)!==false;
	}
	public function SetBalance(int $Balance){
		return $this->RequestAction(GetPropNames(PROP_BALANCE_CONTROL)[PROP_BALANCE_CONTROL], $Balance)!==false;
	}
	
}
?>