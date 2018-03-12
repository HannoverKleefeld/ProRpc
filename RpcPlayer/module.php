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
		$this->CreateProfile_Associations('RPC_PLAY',[RPC_STOP=>'Stop',RPC_PLAY=>'Play',RPC_PAUSE=>'Pause',RPC_NEXT=>'Next',RPC_PREV=>'Prev']);
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
			$this->setValue(PROP_PLAY_CONTROL,$Value);
		else
			IPS_LogMessage(__CLASS__,"Invalid request action $Ident !  value: $Value");
	}
	public function Play(){
		return $this->apiHasProp(PROP_PLAY_CONTROL)?$this->setValue(PROP_PLAY_CONTROL,RPC_PLAY,true):null;
	}
	public function Pause(){
		return $this->apiHasProp(PROP_PLAY_CONTROL)?$this->setValue(PROP_PLAY_CONTROL, RPC_PAUSE,true):null;
	}
	public function Stop(){
		return $this->apiHasProp(PROP_PLAY_CONTROL)?$this->setValue(PROP_PLAY_CONTROL, RPC_STOP,true):null;
	}
	public function Next(){
		return $this->apiHasProp(PROP_PLAY_CONTROL)?$this->setValue(PROP_PLAY_CONTROL, RPC_NEXT,true):null;
	}
	public function Previous(){
		return $this->apiHasProp(PROP_PLAY_CONTROL)?$this->setValue(PROP_PLAY_CONTROL, RPC_PREV,true):null;
	}

	protected function getProps(){ //:array{
		// array( VariableType, Profilename, Position [, icon ] )
		return [
			PROP_PLAY_CONTROL=>[1,'RPC_PLAY',0, 'Melody'],
		];
	}
	protected function setValue(int $Prop, $Value, $Force=false){
		if(!is_null($ok=parent::setValue($Prop,$Value,$Force)))return $ok;
		if($Prop==PROP_PLAY_CONTROL)switch($Value){
			case PROP_STOP	: $ok=$this->forwardRequest('Stop', []); break;
			case PROP_PLAY	: $ok=$this->forwardRequest('Play', []); break;
			case PROP_PAUSE	: $ok=$this->forwardRequest('Pause', []); break;
			case PROP_NEXT	: $ok=$this->forwardRequest('Next', []); break;
			case PROP_PREV	: $ok=$this->forwardRequest('Previous', []); break;
		}
		if($ok)parent::setValue($Prop, $Value, true);
	}
	protected function getValue(int $Prop, $Force=false){
		if(!is_null($value=parent::getValue($Prop,$Force)))return $value;
		if ($Prop!=PROP_PLAY_CONTROL) return $value;
		if(is_null($value=$this->forwardRequest('GetTransportInfo', [])))return $value;
		$value=$value['CurrentTransportState'];
		if(stripos($value,' PAUSE'))$value=RPC_PAUSE;
		elseif(stripos($value,' STOP'))$value=RPC_STOP;
		elseif(stripos($value,' PLAY'))$value=RPC_PLAY;
		else $value=RPC_STOP;
		if(!is_null($value))$this->setValue($Prop, $value,false);
		return $value;
	}
	
}

?>