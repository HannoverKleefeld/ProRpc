<?php
require_once(IPS_GetKernelDir().'\modules\ProRpc\libs\loader.php');

class RPCPlayerControl extends RPCModule {
	function Create(){
		parent::Create();
		ips::CreateProfile_Associations('_PlayState', [PLAYMODE_STOP=>'Stop',PLAYMODE_PAUSE=>'Pause',PLAYMODE_PLAY=>'Play',PLAYMODE_NEXT=>'Next',PLAYMODE_PREV=>'Prev'],'Music');
	}
	function RequestAction ( $Ident, $Value ){
		if($v=parent::RequestAction($Ident, $Value))return $v;
		if(IPS_GetInstance($this->InstanceID)['InstanceStatus']!=102)return false;
		switch($Ident){
			case 'PLAYSTATE' : 
				switch($$Value){
					case PLAYMODE_STOP: $this->Stop();break;
					case PLAYMODE_PAUSE: $this->Pause();break;
					case PLAYMODE_PLAY: $this->Play();break;
					case PLAYMODE_NEXT: $this->Next();break;
					case PLAYMODE_PREV: $this->Previous();break;
				}
				break;
			default:$this->error('Invalid request action %s !  value: %s',$Ident,$Value);
		}
		return $ok?null:false;
	}
	protected function getPropDef($prop){
		if($prop==PROP_PLAY_CONTROL) return ['PLAYSTATE','Status','_PlayState',$type=1,$pos=0];
	}
	protected function getProps(){
		return [PROP_PLAY_CONTROL];
	}
	protected function aboutModule(){
		return 'RpcPro Player Control';
	}
	public function Play(){
		if(($ok=$this->forwardRequest('Play',[]))!==false)$this->setValue('PLAYSTATE', PLAYMODE_PLAY);
		return $ok;
	}
	public function Pause(){
		if(($ok=$this->forwardRequest('Pause',[]))!==false)$this->setValue('PLAYSTATE', PLAYMODE_PAUSE);
		return $ok;
	}
	public function Stop(){
		if(($ok=$this->forwardRequest('Stop',[]))!==false)$this->setValue('PLAYSTATE', PLAYMODE_STOP);
		return $ok;
	}
	public function Next(){
		if(($ok=$this->forwardRequest('Next',[]))!==false)$this->setValue('PLAYSTATE', PLAYMODE_PLAY);
		return $ok;
	}
	public function Previous(){
		if(($ok=$this->forwardRequest('Previous',[]))!==false)$this->setValue('PLAYSTATE', PLAYMODE_PLAY);
		return $ok;
	}
	
}
?>