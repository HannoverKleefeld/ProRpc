<?php
require_once __DIR__. '/../libs/loader.php';

	
/**
 *
 * @author Xaver Bauer
 *        
 */
class RpcEventControl extends IPSControlModule {
	static $SecureSeconds = 60; 
	/**
	 * {@inheritDoc}
	 * @see IPSControlModule::ApplyChanges()
	 */
	public function ApplyChanges() {
		// TODO Auto-generated method stub
		parent::ApplyChanges();
		if($this->ReadPropertyBoolean('EnableEvent')){
			if($this->ReadPropertyString('EventService')){
				$this->SetStatus(102);
				$this->_registerEvent();
			}else $this->SetStatus(501);
		} else {
			$this->SetStatus(102);
			$this->_unregisterEvent();
		}
	}

	/**
	 * {@inheritDoc}
	 * @see IPSControlModule::Create()
	 */
	public function Create(){
		parent::Create();
		$this->registerPropertyBoolean('EnableEvent',false);
		$this->registerPropertyString('EventService','');
		$this->RegisterPropertyInteger('MyPort',3777);
 		$this->registerTimer('EventTimer',0,"IPS_RequestAction($this->InstanceID,'PROCESS_EVENT',0);");
	}
	

	/**
	 * {@inheritDoc}
	 * @see IPSModule::Destroy()
	 */
	public function Destroy() {
		$this->_registerEventHook(false);
		parent::Destroy();
	}

	/**
	 * {@inheritDoc}
	 * @see IPSControlModule::GetConfigurationForm()
	 */
	function GetConfigurationForm() {
		$form=json_decode(parent::GetConfigurationForm(),true);
		$ports=[3777];
		$ids = IPS_GetInstanceListByModuleID("{D83E9CCF-9869-420F-8306-2B043E9BA180}"); // WebServer
		if(count($ids) > 0) {
			foreach($ids as $id){
				if($c=json_decode(IPS_GetConfiguration($id))){
					$ports[]=$c->Port;
				}
			}
		}
		foreach($ports as &$port)$port=['label'=>$port,'value'=>$port];
		$form["elements"][]=["type"=> "Select", "name"=>"MyPort", "caption"=> "Eventhook Port", "options"=>$ports];
  		$form["elements"][]=["type"=> "CheckBox", "name"=>"EnableEvent", "caption"=> "Enable Event"];

  		$options =[  ['label'=>'-> Select <-','value'=>''] ]; 
  		if($events=json_decode($this->GetBuffer('event_list')))
  			foreach($events as $eventService)$options[]=['label'=>$eventService,'value'=>$eventService];
 		$form["elements"][]=["type"=> "Select", "name"=>"EventService", "caption"=> "Event Service", "options"=>$options];
		if($this->getStatus()>109)
 			$form['status'][]=["code"=>501, "icon"=>"error",   "caption"=> "No event service selected"];
		return json_encode($form);
	}
	function RequestAction($Ident,$Value){
		if($Ident=='PROCESS_EVENT'){
			$this->_refreshEvent();
			return true;
		} else return parent::RequestAction($Ident, $Value);
	}
	
	/**
	 * {@inheritDoc}
	 * @see IPSControlModule::onInterfaceChanged()
	 */
	protected function onInterfaceChanged(bool $connected,int $InterfaceID) {
		parent::onInterfaceChanged($connected, $InterfaceID);
		if($connected ){ // && $this->GetBuffer('LastInterface')!=$InterfaceID
			$this->_readEvent();
		}
	}
	/**
	 * (non-PHPdoc)
	 *
	 * @see IPSControlModule::getProps()
	 *
	 */
	protected function getProps() {
		return [
				PROP_EVENTS=>null
		];
	}
	protected function _startTimer(){
		if(!$event=json_decode($this->GetBuffer('current_event')))return;
		if(!$this->ReadPropertyBoolean('EnableEvent'))return;
		$safe=round(($event->LIVETIME /100 ) * 70);
		$sec=($event->NEXTUPDATE + $safe) - time();
		if($sec>0)	$this->SetTimerInterval('EventTimer',$sec);
	}
	protected function _stopTimer(){
		$this->SetTimerInterval('EventTimer',0); 
	}
	
/*
 * Event Handling
 */	
	
	protected function ProcessHookData() {
		require_once RPC_LIB_DIR . '/rpcparseevents.inc';
		if(!$this->ReadPropertyBoolean('EnableEvent')){
			return RpcParseSendError($this->SendDebug(__FUNCTION__, "INFO => Events are disabeld.. sending Error back to disable event on remote device", 0));
		}
		if(!$output=RpcParseEventFromInput()) return; 
		if(is_string($output))return $this->SendDebug(__FUNCTION__,$output,0);
		foreach($output as $instanceID=>$data){
 			$this->forwardRequest('DataChanged', [$data, 'InstanceID'=>$instanceID]);	
 		}
	}
	
	private function _readEvent(){
		$this->_stopTimer();
		$events=array_keys( $this->forwardRequest('GetEventVars',['',true]));
		$autoStart=(bool)json_decode($this->GetBuffer('current_event'));
		$this->SetBuffer('current_event','');
		$this->SetBuffer('event_list', json_encode($events));
		if(!$events || !in_array($this->ReadPropertyString('EventService'),$events))
			$this->setProperty('EventService', '',true);
		elseif($this->ReadPropertyBoolean('EnableEvent') && $autoStart)
			$this->_registerEvent();
	}

	private function _unregisterEvent(){
		$this->_stopTimer();
		if($event=json_decode($this->GetBuffer('current_event'))){
			$now = time();
			if(!empty($event->SID) && $event->NEXTUPDATE < $now && !$this->forwardRequest('UnRegisterEvent',[$event->SID,$event->SERVICE]))
				IPS_LogMessage(__CLASS__,"ERROR: Unregister Event $event->SERVICE : $event->SID");
		}
		$this->SetBuffer('current_event','');		
	}
	private function _registerEvent(){
		if(!$myIp = NET::local_ip()){
			IPS_LogMessage(__CLASS__,'Cant get local IP to Register Events');
			return false;
		}
		if(!$hook = $this->_registerEventHook(true)){
			IPS_LogMessage(__CLASS__,"Failed to register webhook");
			return false;
		}
		if(!$service=$this->ReadPropertyString('EventService')) return $this->SetStatus(501);
		$now = time();
		if($event=json_decode($this->GetBuffer('current_event'))){
			if($event->SERVICE!=$service){
				$this->_unregisterEvent();
				$event=null;
			}
		}
		if(!$event){
			$event=new stdClass();
			$event->SERVICE=$service;
		}
		$myPort = $this->ReadPropertyInteger('MyPort');
 		$callback_url="http://$myIp:$myPort".$hook;
 		
		if(empty($event->SID) || $now > $event->NEXTUPDATE ){
			if($r=$this->forwardRequest('RegisterEvent',[$service, $callback_url])){
				$event->SID=$r[EVENT_SID];
				$event->NEXTUPDATE=$now + $r[EVENT_TIMEOUT];
				$event->LIFETIME=$r[EVENT_TIMEOUT];
			}else 
				$this->SendDebug(__FUNCTION__,"Register Event $event->SERVICE => ".boolstr((bool)$r),0);
		}
		$this->SetBuffer('current_event', json_encode($event));
	}
	private function _refreshEvent(){
		if(!$event=json_decode($this->GetBuffer('current_event')))return false;
		$this->_stopTimer();
		$now = time();
		if(!empty($event->SID) && $now >= $event->NEXTUPDATE ){
			if($r=$this->forwardRequest('RefreshEvent',[$event->SID, $event->SERVICE])){
				$event->NEXTUPDATE=$now + $r[EVENT_TIMEOUT];
				$event->LIFETIME=$r[EVENT_TIMEOUT];
			}else {
				$event->SID='';
				$event->NEXTUPDATE=0;
				$event->LIFETIME=0;
			}
			$this->SendDebug(__FUNCTION__," Refresh Event $event->SERVICE => ".boolstr((bool)$r),0);
			$this->SetBuffer('current_event', json_encode($event));
		}
		return true;
	}
	private function _registerEventHook(bool $Create) {
		$ids = IPS_GetInstanceListByModuleID("{015A6EB8-D6E5-4B93-B496-0D3F77AE9FE1}");
		if(sizeof($ids) > 0) {
			$hookname= "/hook/events{$this->InstanceID}";
			$hooks = json_decode(IPS_GetProperty($ids[0], "Hooks"), true);
			$found = false;
			foreach($hooks as $index => $hook) {
				if(trim($hook['Hook']) == $hookname) {
					if($hook['TargetID'] == $this->InstanceID){
						if($Create)	return $hookname;
						$found=$index;
						break;
					}
					$hooks[$index]['TargetID'] = $this->InstanceID;
					$found = $index;
					break;
				}
			}
			$changed = false;
			if(!$Create && $found!==false){
				$this->SendDebug(__FUNCTION__,"UnRegister Hook: $hookname",0);
				unset($hooks[$found]);
				$changed=true;
			}else if($found===false){
				$this->SendDebug(__FUNCTION__,"Register Hook: $hookname",0);
				$hooks[] = Array("Hook" => $hookname, "TargetID" => $this->InstanceID);
				$changed=true;
			}
			if($changed){
				IPS_SetProperty($ids[0], "Hooks", json_encode($hooks));
				IPS_ApplyChanges($ids[0]);
			}
			return $hookname;
		}else IPS_LogMessage(get_class($this),'ERROR Instance WebHook not found');
		return null;
	}
	
	
}

