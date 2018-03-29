<?php
require_once __DIR__.'/../libs/loader.php';
/** 
 * @author Xavier
 * 
 */
class RpcSoundControl extends IPSControlModule {
	function Create(){
		parent::Create();
		$this->CreateProfile_Associations('RPC_Balance',null);
 		IPS_SetVariableProfileValues('RPC_Balance',-100,100,1);
// 			$this->registerPropertyInteger('LastUpdate',0);
// 		$this->registerPropertyBoolean('EnableEvents',false);
// 		$this->registerPropertyString('ConfigFile','');
// 		$this->registerPropertyString('Host','');
// 		$this->registerPropertyString('User','');
// 		$this->registerPropertyString('Pass','');
// 		$this->registerPropertyInteger('PollInterval',0);
// 		$this->registerPropertyInteger('Props',0);
// 		$this->registerPropertyInteger('DebugOptions',0);
// 		// Only for Form Selection
// 		$this->registerPropertyBoolean('ExpertConfig',false);
// 		$this->registerPropertyBoolean('DebugConfig',false);
		
	}
	function Destroy(){
		parent::Destroy();
		if(count(IPS_GetInstanceListByModuleID('{19650302-XABA-MAJA-CONT-20180101XLIB}'))==0){
 			IPS_DeleteVariableProfile('RPC_Balance');
		}
	}
	function RequestAction($Ident,$Value){
		if(parent::RequestAction($Ident, $Value))return;
		switch($Ident){
			case NAMES_PROPS[PROP_VOLUME_CONTROL] 	: $this->SetVolume($Value); break;
			case NAMES_PROPS[PROP_BASS_CONTROL] 	: $this->SetBass($Value); break;
			case NAMES_PROPS[PROP_TREBLE_CONTROL] 	: $this->SetTreble($Value); break;
			case NAMES_PROPS[PROP_BALANCE_CONTROL] 	: $this->SetBalance($Value) ; break;
			case NAMES_PROPS[PROP_LOUDNESS_CONTROL]	: $this->SetLoudness($Value); break;
			case NAMES_PROPS[PROP_MUTE_CONTROL] 	: $this->SetMute($Value); break;
			default:IPS_LogMessage(__CLASS__,"Invalid request action $Ident !  value: $Value");
		}
	}

	/**
	 * {@inheritDoc}
	 * @see IPSControlModule::GetConfigurationForm()
	 */
	public function GetConfigurationForm() {
		$form=json_decode(parent::GetConfigurationForm (),true);
		$form["actions"][]=["type"=> "HorizontalSlider", "name"=>"vol","caption"=>"Volume","minimum"=>0,"maximum"=>100,"onClick"=>"RSOUND_SetVolume(\$id,\$vol);"];		
  		$form["actions"][]=["type"=> "Button", "label"=>"Mute On","onClick"=>"RSOUND_SetMute(\$id,true);"];
  		$form["actions"][]=["type"=> "Button", "label"=>"Mute Off","onClick"=>"RSOUND_SetMute(\$id,false);"];
		return json_encode($form);
	}

	public function SetVolume(int $Volume){
		return $this->apiHasProp(PROP_VOLUME_CONTROL)?$this->setValueByProp(PROP_VOLUME_CONTROL, $Volume,true):null;
	}
	public function SetTreble(int $Treble){
		return $this->apiHasProp(PROP_TREBLE_CONTROL)?$this->setValueByProp(PROP_TREBLE_CONTROL, $Volume,true):null;
	}
	public function SetBass(int $Bass){
		return $this->apiHasProp(PROP_BASS_CONTROL)?$this->setValueByProp(PROP_BASS_CONTROL, $Volume,true):null;
	}
	public function SetBalance(int $Balance){
		return $this->apiHasProp(PROP_BALANCE_CONTROL)?$this->setValueByProp(PROP_BALANCE_CONTROL, $Volume,true):null;
	}
	public function SetMute(bool $Mute){
		return $this->apiHasProp(PROP_MUTE_CONTROL)?$this->setValueByProp(PROP_MUTE_CONTROL, $Mute,true):null;
	}
	public function SetLoudness(bool $Loudness){
		return $this->apiHasProp(PROP_LOUDNESS_CONTROL)?$this->setValueByProp(PROP_LOUDNESS_CONTROL, $Volume,true):null;
	}
	public function UpdateStatus(bool $Force){
		if(!parent::UpdateStatus($Force))return false;
		foreach($this->getProps() as $prop=>$def)
			if($this->apiHasProp($prop))$this->getValueByProp($prop, $Force);
		return true;
	}
	protected function getProps(){ //:array{
		// array( VariableType, Profilename, Position [, icon ] )
		return [
			PROP_VOLUME_CONTROL=>[1,'~Intensity.100',0,'Intensity'],
			PROP_BASS_CONTROL=>[1,'~Intensity.100',1,'Intensity'],
			PROP_TREBLE_CONTROL=>[1,'~Intensity.100',2,'Intensity'],
			PROP_BALANCE_CONTROL=>[1,'RPC_Balance',3,'Intensity'],
			PROP_LOUDNESS_CONTROL=>[0,'~Switch',4,'Speedo'],
			PROP_MUTE_CONTROL=>[0,'~Switch',5,'Speaker']
		];
	}
	protected function setValueByProp(int $Prop, $Value){
		switch($Prop){
			case PROP_VOLUME_CONTROL	: $ok=$this->forwardRequest('SetVolume', ['DesiredVolume'=>(int)$Value]); break;
			case PROP_BALANCE_CONTROL	: $ok=$this->forwardRequest('SetBalance', ['DesiredBalance'=>(int)$Value]); break;
			case PROP_BASS_CONTROL 		: $ok=$this->forwardRequest('SetBass', ['DesiredBass'=>(int)$Value]); break;
			case PROP_TREBLE_CONTROL	: $ok=$this->forwardRequest('SetTreble', ['DesiredTreble'=>(int)$Value]); break;
			case PROP_LOUDNESS_CONTROL	: $ok=$this->forwardRequest('SetLoudness', ['DesiredLoudness'=>(bool)$Value]);break;
			case PROP_MUTE_CONTROL		: $ok=$this->forwardRequest('SetMute', ['DesiredMute'=>(bool)$Value]);break;
		}
		if($ok)return parent::setValueByIdent(NAMES_PROPS[$Prop], $Value);
	}
	protected function getValueByProp(int $Prop, $Force=false){
		if(!is_null($value=parent::getValueByProp($Prop,$Force)))return $value;
		switch ($Prop){
			case PROP_VOLUME_CONTROL	: $value=$this->forwardRequest('GetVolume', []); break;;
			case PROP_BALANCE_CONTROL	: $value=$this->forwardRequest('GetBalance',[]); break;
			case PROP_BASS_CONTROL 		: $value=$this->forwardRequest('GetBass', []); break;
			case PROP_TREBLE_CONTROL	: $value=$this->forwardRequest('GetTreble', []); break;
			case PROP_LOUDNESS_CONTROL	: $value=$this->forwardRequest('GetLoudness', []); break;
			case PROP_MUTE_CONTROL		: $value=$this->forwardRequest('GetMute', []);break;;
		}
		if(!is_null($value))$this->setValueByIdent(NAMES_PROPS[$Prop], $value);
		return $value;
	}
	
}

