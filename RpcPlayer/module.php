<?php
require_once __DIR__.'/../libs/loader.php';
/** 
 * @author Xavier
 * 
 */
const 
RPC_STOP  = 0,
RPC_PLAY  = 1,
RPC_PAUSE = 2,
RPC_NEXT  = 3,
RPC_PREV  = 4;

class RpcPlayControl extends IPSControlModule {
	function Create(){
		parent::Create();
		$this->CreateProfile_Associations('RPC_PLAY',[RPC_STOP=>$this->Translate('Stop'),RPC_PLAY=>$this->Translate('Play'),RPC_PAUSE=>$this->Translate('Pause'),RPC_NEXT=>$this->Translate('Next'),RPC_PREV=>$this->Translate('Prev')]);
	}

	function Destroy(){
		parent::Destroy();
		if(count(IPS_GetInstanceListByModuleID('{19650302-XABA-MAJA-PLAY-20180101XLIB}'))==0){
 			IPS_DeleteVariableProfile('RPC_PLAY');
		}
	}
	function RequestAction($Ident,$Value){
		if(parent::RequestAction($Ident, $Value))return;
		if($Ident == NAMES_PROPS[PROP_PLAY_CONTROL])
			$this->setValueByProp(PROP_PLAY_CONTROL,$Value);
		else
			IPS_LogMessage(__CLASS__,"Invalid request action $Ident !  value: $Value");
	}
	
	public function UpdateStatus(bool $Force){
		if(!parent::UpdateStatus($Force))return false;
		foreach($this->getProps() as $prop=>$def)
			if($this->apiHasProp($prop))$this->getValueByProp($prop, $Force);
		return true;
	}	
	/**
	 * {@inheritDoc}
	 * @see IPSControlModule::GetConfigurationForm()
	 */
	public function GetConfigurationForm() {
		$form=json_decode(parent::GetConfigurationForm (),true);
  		$form["actions"][]=["type"=> "Button", "label"=>$this->Translate("Stop"),"onClick"=>"RPLAY_STOP(\$id);"];
  		$form["actions"][]=["type"=> "Button", "label"=>$this->Translate("Play"),"onClick"=>"RPLAY_PLAY(\$id);"];
  		$form["actions"][]=["type"=> "Button", "label"=>$this->Translate("Pause"),"onClick"=>"RPLAY_PAUSE(\$id);"];
   		return json_encode($form);
	}

	public function Play(){
		return $this->apiHasProp(PROP_PLAY_CONTROL)?$this->setValueByProp(PROP_PLAY_CONTROL,RPC_PLAY,true):null;
	}
	public function Pause(){
		return $this->apiHasProp(PROP_PLAY_CONTROL)?$this->setValueByProp(PROP_PLAY_CONTROL, RPC_PAUSE,true):null;
	}
	public function Stop(){
		return $this->apiHasProp(PROP_PLAY_CONTROL)?$this->setValueByProp(PROP_PLAY_CONTROL, RPC_STOP,true):null;
	}
	public function Next(){
		return $this->apiHasProp(PROP_PLAY_CONTROL)?$this->setValueByProp(PROP_PLAY_CONTROL, RPC_NEXT,true):null;
	}
	public function Previous(){
		return $this->apiHasProp(PROP_PLAY_CONTROL)?$this->setValueByProp(PROP_PLAY_CONTROL, RPC_PREV,true):null;
	}
	
	
	protected function getProps(){ //:array{
		// array( VariableType, Profilename, Position [, icon ] )
		return [
			PROP_PLAY_CONTROL=>[1,'RPC_PLAY',20, 'Melody'],
		];
	}
	protected function setValueByProp(int $Prop, $Value){
		if($ok=$Prop==PROP_PLAY_CONTROL)switch($Value){
			case RPC_STOP	: $ok=$this->forwardRequest('Stop', []); break;
			case RPC_PLAY	: $ok=$this->forwardRequest('Play', []); break;
			case RPC_PAUSE	: $ok=$this->forwardRequest('Pause', []); break;
			case RPC_NEXT	: $ok=$this->forwardRequest('Next', []); break;
			case RPC_PREV	: $ok=$this->forwardRequest('Previous', []); break;
		}
		if($ok && $this->setValueByIdent(NAMES_PROPS[$Prop],$Value)){
			$this->forwardRequest('DataChanged',[ [NAMES_PROPS[$Prop]=>$Value]]);
		}
	}
	protected function getValueByProp(int $Prop, $Force=false){
		if(!is_null($value=parent::getValueByProp($Prop,$Force)))return $value;
		if ($Prop!=PROP_PLAY_CONTROL) return $value;
		if(is_null($value=$this->forwardRequest('GetTransportInfo', [])))return $value;
		$value=$value['CurrentTransportState'];
		if(stripos($value,' PAUSE'))$value=RPC_PAUSE;
		elseif(stripos($value,' STOP'))$value=RPC_STOP;
		elseif(stripos($value,' PLAY'))$value=RPC_PLAY;
		else $value=RPC_STOP;
		if(!is_null($value)){
			if($this->setValueByIdent(NAMES_PROPS[$Prop], $value))
				$this->forwardRequest('DataChanged',[ [NAMES_PROPS[$Prop]=>$Value]]);
		}
		return $value;
	}
	
}

?>