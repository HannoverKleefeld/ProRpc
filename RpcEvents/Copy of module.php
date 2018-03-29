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
		if($this->ReadPropertyBoolean('EnableEvents'))
			$this->_registerEvents();
		else $this->_unregisterEvents();
	}

	/**
	 * {@inheritDoc}
	 * @see IPSControlModule::Create()
	 */
	public function Create(){
		parent::Create();
		$this->registerPropertyBoolean('EnableEvents',false);
		$this->RegisterPropertyInteger('MyPort',3777);
		$this->registerPropertyString('Events','[]');
 		$this->registerTimer('EventsTimer',0,"IPS_RequestAction($this->InstanceID,'PROCESS_EVENTS',0);");
		$this->SetBuffer('EventsTimerSec',0);
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
	public function GetConfigurationForm() {
		// TODO Auto-generated method stub
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
 		$form["elements"][]=["type"=> "CheckBox", "name"=>"EnableEvents", "caption"=> "Enable Events"];

 		
 		if($events=json_decode($this->readPropertyString('Events')))foreach($events as $event){
			if(!empty($event->SID))$colors[]=['rowColor'=>'#00FF00'];
			elseif($event->NEXTUPDATE==-1)$colors[]=['rowColor'=>'#FF0000'];
			else $colors[]=['rowColor'=>'#FFFFFF'];
		}else $colors=[];
		$form["elements"][]=["type"=>"List","name"=>"Events","caption"=>"Avaible Events","rowCount"=>5,"columns"=> [
				["label"=>"Enabled",	"name"=>"REGISTER", "width"=>"50px","edit"=>["type"=>"CheckBox","caption"=>"Enable Service Events"]],
				["label"=>"Name",	"name"=>"SERVICE", "width"=>"auto","save"=>true],
				["label"=>"Time", 	"name"=>"NEXTUPDATE", "width"=>"60px","save"=>true],
				["label"=>"LiveTime","name"=>"LIFETIME", "width"=>"0","visible"=>false,"save"=>true],
				["label"=>"SID", 	"name"=>"SID", "width"=>"0","visible"=>false,"save"=>true]
			]
			,'values'=>$colors	
		];
		return json_encode($form);
	}
	function RequestAction($Ident,$Value){
		if(parent::RequestAction($Ident, $Value))return true;
		if($Ident=='PROCESS_EVENTS'){
			$this->_refreshEvents();
			return true;
		} else IPS_LogMessage(__CLASS__,"Invalid request action $Ident !  value: $Value");
		return false;
	}
	
	/**
	 * {@inheritDoc}
	 * @see IPSControlModule::onInterfaceChanged()
	 */
	protected function onInterfaceChanged(bool $connected,int $InterfaceID) {
		// TODO Auto-generated method stub
		
		parent::onInterfaceChanged($connected, $InterfaceID);
		if($connected ){ // && $this->GetBuffer('LastInterface')!=$InterfaceID
			$this->_readEvents();
// 			$this->setBuffer('LastInterface',$InterfaceID);
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
		if($sec=intval($this->GetBuffer('EventsTimerSec')))$this->SetTimerInterval('EventsTimer',$sec); 
	}
	protected function _stopTimer(){
		$this->SetTimerInterval('EventsTimer',0); 
	}
	
/*
 * Event Handling
 */	
	
	
	protected function ProcessHookData() {
		$accept=function(){
			header('HTTP/1.1 202 Accepted');
			echo "HTTP/1.1 202 Accepted\n\n";
			return true;
		};
		$error=function(){
			header('HTTP/1.1 404 ERROR');
			echo "HTTP/1.1 404 ERROR\n\n";
			return false;
		};
		
// 		$registeredProps=intval($this->GetBuffer('RegisteredClientProps'));
// 		if($registeredProps<1) return $error($this->error('%s No Registered props found',__FUNCTION__));
		if(!$this->ReadPropertyBoolean('EnableEvents')){
			return $error($this->SendDebug(__FUNCTION__, "ERROR => Events are disabeld", 0));
		}
		$Request = trim(file_get_contents('php://input'));
		$Request=htmlspecialchars_decode($Request);
		if(empty($Request)){
			return $error($this->SendDebug(__FUNCTION__,'ERROR => Empty data received',0));
		}
		if(preg_match('/<LastChange>(.+)<\/LastChange>/', $Request, $m)){
 			$xml=simplexml_load_string('<?xml version="1.0" encoding="utf-8"?>'.$m[1]);
 			$values=$this->_valuesFromLastChangeXML($xml);
 			$this->SendDebug(__FUNCTION__, 'Process Changes => '.var_export($values,true),0);
 			foreach($values as $instanceID=>$data){
 				$this->forwardRequest('DataChanged', [0=>$data,'InstanceID'=>$instanceID]);	
 			}
 		}else{
	 		$Request='<?xml version="1.0" encoding="utf-8"?>'.str_replace('e:','',$Request);	
			$xml=simplexml_load_string($Request);
 			$values=$this->_valuesFromPropertysXML($xml);
 			$this->SendDebug(__FUNCTION__, 'Process Changes => '.var_export($values,true),0);
			 			
 		}
		return $accept();
	}
	
	private function _valuesFromLastChangeXML($xml, $doReturn=false){
		$getAttributes=function($item ){
			$out=[];
			if(!empty($item['@attributes']))foreach ($item['@attributes'] as $name=>$value)$out[$name]=
				!is_numeric( $value)?( strcasecmp($value,'true')==0?boolval($value):$value):(is_float($value)?floatval($value):intval($value));
			return $out;
		};
		$a=json_decode(json_encode($xml),true);
		$values=[];
		foreach ($a as $iname=>$instance){
			if($iname=="@attributes")continue;
			$attr=$getAttributes($instance);
			$attr=implode(',', $attr);
			$instanceID=intval($attr);
			$data=['InstanceID'=>$instanceID, $iname=>$attr];
			foreach($instance as $vname=>$var){
				if($vname=="@attributes")continue;
				$vname=strtoupper($vname);
				if(empty($var[0])){
					$attr=$getAttributes($var);
					if(!empty($attr['channel'])){
						$ch=$attr['channel']; $val=$attr['val'];
						if(strtoupper($ch)=='MASTER')$data[$vname]=$val; else $data[$vname][$ch]=$val;
					}else{
						$attr=implode(',', $attr);
						$data[$vname]=$attr;				
					}
				}elseif(is_array($var)){
					$balance=null;	
					foreach($var as $index=>$props){
						$attr=$getAttributes($props);
						if(!empty($attr['channel'])){
							$ch=$attr['channel'];
							$val=$attr['val'];
							if(stripos($ch,'lf')!==false||stripos($ch,'rf')!==false){
								$balance[strtoupper($ch)]=$val;
							}elseif(strtoupper($ch)=='MASTER')
								$data[$vname]=$val;
							else $data[$vname][$ch]=$val;
						}else{
							$attr=implode(',', $attr);
							$data[$vname][$index]=$attr;				
						}
					}
					if($balance){
						if(!$vl=$balance['LF'])		$balance=$balance['RF'];
						elseif(!$vr=$balance['RF'])	$balance=-$balance['LF'];
	 					elseif($vl==$vr)			$balance=0;
	 					elseif($vl>$vr)				$balance=-($vl - $vr);
	 					else 						$balance=abs($vl - $vr);
	 					$data[NAMES_PROPS[PROP_BALANCE_CONTROL]]=$balance;
					}
				}else 
					$this->SendDebug(__FUNCTION__,"Unknown var => $var",0);
			}
			$values[$instanceID]=$data;		
		}
		return $values;
	}
	private function _valuesFromPropertysXML($xml) {
		$data=json_decode(json_encode($xml),true)['property'];
		$values=[];
		foreach($data as $prop){
			foreach($prop as $name=>$value){
		// 		if(is_array($value) && count($value)==0)$value=null;
				$values[$name]=$value;
			}
		}
		return [0=>$values];
	}
	
	
	private function _readEvents(){
// 		if($events=json_decode($this->readPropertyString('Events')))$this->_unregisterEvents($events);
		$this->_stopTimer();
		$events=[];
		$services=array_keys( $this->forwardRequest('GetEventVars',['',true]));
		foreach($services as $sn)$events[]=(object)['SERVICE'=>$sn,"REGISTER"=>$sn=='ConnectionManager',"LIFETIME"=>0, "NEXTUPDATE"=>'0',"SID"=>''];
 		$this->_calcEventTimerSec($events);
		$this->setProperty('Events', json_encode($events),true);
	}
	private function _calcEventTimerSec(array $events){
		$nextUpdate = 0;
		foreach($events as $event){
			if($event->REGISTER && $event->SID && $event->NEXTUPDATE!=-1){
				if(!$nextUpdate)$nextUpdate=$event->NEXTUPDATE;
				else $nextUpdate=min($nextUpdate, $event->NEXTUPDATE);	
			}
		}
		$now = time() + self::$SecureSeconds;
		if($nextUpdate){
			$seconds= $nextUpdate - $now;
			if($seconds < 2 )$seconds=2;
		}else $seconds=0;
		$this->SetBuffer('EventsTimerSec',$seconds);
		return ($seconds);
	}
	

	private function _unregisterEvents(){
		if(!$events=json_decode($this->readPropertyString('Events')))return
		$this->_stopTimer();
		$now = time();$changed=false;
		foreach ($events as $event){
			if(!empty($event->SID)){
// IPS_LogMessage(__CLASS__,'Unregister Event: '.$event->SID);
				if($event->NEXTUPDATE < $now && !$this->forwardRequest('UnRegisterEvent',[$event->SID,$event->SERVICE]))
					IPS_LogMessage(__CLASS__,"ERROR: Unregister Event $event->SERVICE : $event->SID");
				$changed=true;
				$event->SID='';
				$event->NEXTUPDATE=0;
				$event->LIFETIME=0;
			}
		}
		$this->_calcEventTimerSec($events);
		if($changed)$this->setProperty('Events', json_encode($events),true);
		return true;
	}
	private function _registerEvents(){
		if(!$events=json_decode($this->readPropertyString('Events')))return false;
		if(!$myIp = NET::local_ip()){
			IPS_LogMessage(__CLASS__,'Cant get local IP to Register Events');
			return false;
		}
		if(!$hook = $this->_registerEventHook(true)){
			IPS_LogMessage(__CLASS__,"Failed to register webhook");
			return false;
		}
		$myPort = $this->ReadPropertyInteger('MyPort');
 		$callback_url="http://$myIp:$myPort".$hook;
		$now = time();$changed=false;
 		foreach($events as $event){
 			if($event->REGISTER){
 				if(empty($event->SID) || $now > $event->NEXTUPDATE ){
 					if($r=$this->forwardRequest('RegisterEvent',[$event->SERVICE, $callback_url])){
 						$event->SID=$r[EVENT_SID];
 						$event->NEXTUPDATE=$now + $r[EVENT_TIMEOUT];
 						$event->LIFETIME=$r[EVENT_TIMEOUT];
 					}else {
 						$event->NEXTUPDATE=-1;
 						$event->LIFETIME=0;
 						$event->REGISTER=false;
 					}
 					$changed=true;
					$this->SendDebug(__FUNCTION__,"Register Event $event->SERVICE => ".boolstr((bool)$r),0);
 				}
 			}elseif(!empty($event->SID)){
 				if($now < $event->NEXTUPDATE){
					$this->forwardRequest('UnRegisterEvent',[$event->SID,$event->SERVICE] );
 				}
				$event->SID='';
				$event->NEXTUPDATE=0;
 				$event->LIFETIME=0;
				$changed=true;
 			}
 		}
		if($changed)$this->setProperty('Events', json_encode($events),true);
		if($this->_calcEventTimerSec($events))$this->_startTimer();
	}
	private function _refreshEvents(){
		if(!$events=json_decode($this->readPropertyString('Events')))return false;
		$this->_stopTimer();
		$now = time() + self::$SecureSeconds ;$changed=false;
		foreach ($events as $event){
 			if($event->REGISTER){
 				if(!empty($event->SID) && $now >= $event->NEXTUPDATE ){
 					if($r=$this->forwardRequest('RefreshEvent',[$event->SID, $event->SERVICE])){
 						$event->NEXTUPDATE=($now-self::$SecureSecond) + $r[EVENT_TIMEOUT];
 						$event->LIFETIME=$r[EVENT_TIMEOUT];
 					}else {
 						$event->SID='';
 						$event->NEXTUPDATE=-1;
 						$event->LIFETIME=0;
 						$event->REGISTER=false;
 					}
 					$changed=true;
 					$this->SendDebug(__FUNCTION__," Refresh Event $event->SERVICE => ".boolstr((bool)$r),0);
 				}
 			}
		}
		if($changed)$this->setProperty('Events', json_encode($events),true);
		if($this->_calcEventTimerSec($events))$this->_startTimer();
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

