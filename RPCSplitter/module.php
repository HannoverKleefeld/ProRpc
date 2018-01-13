<?php
define('LIB_PHP_VERSION','_5x');
require_once(IPS_GetKernelDir().'/modules/ProRpc/libs/loader.php');
require_once(LIB_INCLUDE_DIR.'/rpc.defines.inc');
DEFINE('ERROR_HANDLER_FUNCTION','Splitter_Error_Handler') ;

function Splitter_Error_Handler($msg,$code,$class){
	IPS_LogMessage($class,"ERROR: ($code) $msg");
	return true;
}

class RPCSplitterControl extends IPSBaseModule {
	static $VERSION = 1.6;
	const eventControl_hook = "eventControl";
	const refresh_life_time_sec   = 300;
	const min_interval_secs		= 10;
	const buffer_api_info_name	='ApiInfo';
	const buffer_client_name	='RuntimeClients';
	
	private $_api = null;
	private $_debugLevel=0;
	function __construct($InstanceID){
		parent::__construct($InstanceID);
		$this->_debugLevel=@$this->ReadPropertyInteger('DebugLevel');
	}
	function Create(){
		parent::Create();
		$this->registerPropertyBoolean('Open',false);
		$this->registerPropertyBoolean('UseEvents',false);
		$this->registerPropertyInteger('DebugLevel',0);
		$this->registerPropertyString('File','');
		$this->registerPropertyInteger('Props',0);
		$this->registerPropertyInteger('Groups',0);
		$this->registerPropertyString('Info','');
		$this->registerPropertyString('RemoteUser','');
		$this->registerPropertyString('RemotePass','');
		$this->registerPropertyInteger('UpdateInterval',0);
 		$this->registerPropertyString('Events','[]');
		
		$this->RegisterTimer("UpdateTimer", 0, "IPS_RequestAction($this->InstanceID,'UpdateTimer','');");
		$this->RegisterTimer("EventTimer",  0, "IPS_RequestAction($this->InstanceID,'EventsTimer','');");
 		$this->SetBuffer(self::buffer_api_info_name,'[]');
		$this->SetBuffer(self::buffer_client_name,'[]');
		$this->SetBuffer('RegisteredClientProps',0);
		$this->SetBuffer('EventRefreshInterval',0);
	}
	function Destroy(){
		parent::Destroy();
		$this->_api=null;
		if($this->_debugLevel & DEBUG_ALL)IPS_LogMessage(__CLASS__,'Destroy and UnRegister Hook: '.$this->_registerHook(false));
	}
	function ApplyChanges(){
		parent::ApplyChanges();
		$this->_stopTimer(1);
		$this->RegisterVariableBoolean('ONLINE','Device Online','');
		$this->RegisterVariableString('LAST_ERROR','Last Error','');
		$changed=0;
		$isOpen=$this->ReadPropertyBoolean('Open');
		if(($test=$this->ReadPropertyInteger('UpdateInterval'))<0 || ($test>0 && $test < self::min_interval_secs )){
// 			if($test>0)$this->setProperty('UpdateInterval',self::min_interval_secs);
// 			else $this->setProperty('UpdateInterval',0);
			$changed=301;
		}elseif($fn=$this->ReadPropertyString('File')) {
			if(!file_exists(RPC_CONFIG_DIR."/$fn")){
// 				$this->setProperty('File','');
// 				$this->setProperty('Props',0);
// 				$this->setProperty('Open',false);
				$this->SetBuffer('RegisteredClientProps','0');
				$this->_setOnline(false);
				$changed=201;
			}elseif($this->GetBuffer('ApiFileName')!=$fn){
				$this->SetBuffer('ApiFileName',$fn);
				if(!$api=$this->_getApi()){
					$this->_setOnline(false);
//  					$this->setProperty('File','');
// 					$this->setProperty('Props',0);
					$this->_handleEventList('clear');
					$this->SetBuffer('RegisteredClientProps','0');
					$this->setProperty('Open',false);
					$changed=200;
				}else{
					$apiProps['PROPS']=$api->GetConfigParam(CONFIG_PROPS_OPTIONS);
					if($this->ReadPropertyInteger('Props')!=$apiProps['PROPS']){
						$this->setProperty('Props',$apiProps['PROPS']);
						$changed=102;
					}
					$apiProps['GROUPS']=$api->GetConfigParam(CONFIG_PROPS_GROUPS);
					if($this->ReadPropertyInteger('Groups')!=$apiProps['GROUPS']){
						$this->setProperty('Groups',$apiProps['GROUPS']);
						$changed=102;
					}
					$this->SetBuffer(self::buffer_api_info_name,json_encode([INFO_MANU_ID=>$api->GetInfoParam(INFO_MANU_ID),INFO_MODEL_ID=>$api->GetInfoParam(INFO_MODEL_ID),INFO_VERSION=>$api->GetVersion()]));
					if($changed==102)$this->_sendApiPropsToClients($apiProps);
					$name = $api->GetInfoParam(INFO_MANU_ID) .' ['.$this->_api->GetConfigParam(CONFIG_HOST).':'.$this->_api->GetConfigParam(CONFIG_PORT).']';
					if($this->ReadPropertyString('Info')!=$name){
						IPS_SetName($this->InstanceID,$name);
						$this->setProperty('Info',$name);
						$changed=102;
					}
					$hasEvents=$api->HasEvents();
					if($this->ReadPropertyBoolean('UseEvents') && !$hasEvents){
						$this->setProperty('UseEvents',false);
						$changed=102;
					}
 					
 					$this->_handleEventList($hasEvents?'rebuild':'clear',null,!$changed);
					
 					if($isOpen)$this->_setOnline($api->IsOnline());
				}
			}
			
		}elseif($isOpen||$this->ReadPropertyInteger('Props')){
// 			$this->setProperty('Props',0);
			$this->SetBuffer('RegisteredClientProps','0');
// 			$this->setProperty('Open',false);
			$this->_setOnline(false);
			$isOpen=False;
			$changed=201;
		}
		if($changed){
			if($changed!=200)$this->error('');
			IPS_ApplyChanges($this->InstanceID);
			$this->SetStatus($changed);
		}else $changed=102;
		if( $changed==102 && !$isOpen){
			$changed=104;
		}
		$this->SetStatus($changed);
		if ($changed==102){
			$this->_updateRegisteredClientProps();
// 			$this->_handleEventList('rebuild');
// 			if($this->ReadPropertyBoolean('UseEvents'))	
// 				$this->_registerEvents();
// 			else{
// 				$this->_unregisterEvents();
// 				$this->_updateClientData();
// 			}	
		}else {
			$this->_setOnline(false);
			$this->_stopTimer(2);		
		}	
	}
	
	function RequestAction($ident, $value){
		switch($ident){
			case 'UpdateTimer': $this->_updateClientData(); break;
			case 'EventsTimer': $this->_refrehEvents(); break;
			case 'debug'	  : $this->debug($Message=$value[0],$ErrorCode=$value[1],$CalledClass=$value[2]);break;	
			case 'ABOUT_ME'	  : echo "RpcPro API Switch"; break;
			default:$this->error('Invalid request action %s !  value: %s',$Ident,$Value);
		}
	}
	function MessageSink ( $Zeitstempel, $SenderID, $NachrichtID, $Daten ){
// 		$this->SendDebug(__FUNCTION__,"SenderID: $SenderID, MsgID: $NachrichtID Data:".implode(',', $Daten),0);	
		if($NachrichtID==11102){ //Child object disconnected
			$this->_unregisterClient($SenderID);
		}
	}
	
	function GetConfigurationForm(){
// 	$this->SetBuffer(self::buffer_client_name,'[]');
		$options=[];
		if($files=scandir(RPC_CONFIG_DIR))foreach ($files as $file){
			if($file[0]=='.')continue;
			$file=pathinfo($file);
			if(!empty($file['extension'])&&$file['extension']=='json' && stripos($file['basename'], 'desc')===false )$options[]=["label"=>$file['filename'], "value"=> $file['basename']];
		}	
		$form["elements"][]=["type"=> "CheckBox", "name"=>"Open", "caption"=> "Open Connection"];
		$idOfEventCheckbox=count($form["elements"]);		
		$form["elements"][$idOfEventCheckbox]=["type"=> "CheckBox", "name"=>"UseEvents", "caption"=> "Activate Events"];
		$form["elements"][]=["type"=> "Select", "name"=>"File", "caption"=> "Config","options"=> $options];
		$form["elements"][]=["type"=>"ValidationTextBox","name"=>"RemoteUser", "caption"=> "Remote User" ];
		$form["elements"][]=["type"=>"PasswordTextBox","name"=>"RemotePass", "caption"=> "Remote Password" ];
		$form["elements"][]=["type"=>"IntervalBox","name"=>"UpdateInterval", "caption"=> "Seconds" ];
		$form["elements"][]=["type"=> "Select", "name"=>"DebugLevel", "caption"=> "Debug Level","options"=> [
				["label"=>'Off',   "value"=> DEBUG_NONE],	["label"=>'Info',  "value"=> DEBUG_INFO],
				["label"=>'Calls', "value"=> DEBUG_CALL],	["label"=>'Details',  "value"=> DEBUG_DETAIL], ["label"=>'All',   "value"=> DEBUG_ALL]
		]];
 		$file = $this->ReadPropertyString('File');
 		$info = $this->ReadPropertyString('Info');
 		$propInfo = ''; 
 		if($file && file_exists(RPC_CONFIG_DIR."/$file")){
 			$groups = $this->ReadPropertyInteger('Groups');
 			$myprops = $this->ReadPropertyInteger('Props');
 			foreach($props=GetPropNames(PROP_CONTENT_BROWSER+PROP_REMOTE+PROP_EVENTS,true) as $prop=>$name){if(!($myprops & $prop)) unset($props[$prop]);}
 			if(empty($props[PROP_EVENTS]))unset($form['elements'][$idOfEventCheckbox]);
 			$propInfo=implode(', ',array_merge($props, GetGroupNames($groups)));
 		}
 		$formatNextTime=function(array &$item){
 		 	if(isset($item['ACTIVE'])&&!$item['ACTIVE']){ $item['rowColor']="#FFFFC0"; return $item['LIFETIME']='inactive'; }
 			if(empty($item['LIFETIME'])){ $item['rowColor']="#DFDFDF"; return $item['LIFETIME']='disabled';	}
 		 	$dif= floor((time() - $item['LASTUPDATE'])/1000); 
  			if($dif >= $item['LIFETIME']){$item['rowColor']="#C0C0FF"; return $item['LIFETIME']='now';	}
  			$seconds=$item['LIFETIME']-$dif; $item['rowColor']='#C0FFC0';
			$days=$hours=$minutes=0;
			while ($seconds >= 86400) {$days++;$seconds-=86400;}
			while ($seconds >= 3600) {$hours++;$seconds-=3600;}
			while ($seconds >= 60) {$minutes++;$seconds-=60;}
  			return 	$item['LIFETIME']=trim(($days?$days.'D:' : ''). ($hours?$hours.'H:':'') . ($minutes?$minutes.'M:':'') . ($seconds?$seconds.'S':''));
 		}; 		
 		
 		if($this->ReadPropertyBoolean('UseEvents')){
 			

	 		$events=json_decode($this->ReadPropertyString('Events'),true);
// var_export($events);	 		
	 		foreach($events as &$event){
	 			$formatNextTime($event);

	 		}
	 		$form["elements"][]=["type"=>"List","name"=>"Events","caption"=>"Availbe Events","rowCount"=>5,"add"=>false,"delete"=>false, "columns"=> [
	 				["label"=>"", "name"=>"SID","width"=>"0", "visible"=>false, "save"=>true],
	 				["label"=>"", "name"=>"LASTUPDATE","width"=>"0", "visible"=>false, "save"=>true],
	 				["label"=>"Reg.", "name"=>"ACTIVE","width"=>"40px","edit"=>["type"=>"CheckBox","caption"=>"Register Event"]],
	 				["label"=>"Service", "name"=>"SERVICE","width"=>"110px", "save"=>true],
	 				["label"=>"Refresh", "name"=>"LIFETIME","width"=>"60px", "save"=>true],
  	 				["label"=>"Propertys", "name"=>"VARS","width"=>"auto", "edit"=> ["type"=>"ValidationTextBox","caption"=>"Avaible Status Variables"]],
	 			],'values' => $events
	 		];
 		}
 		$nointerval=$this->ReadPropertyInteger('UpdateInterval')<1;
		if(!$clients=json_decode($this->GetBuffer(self::buffer_client_name),true))$clients=[];
  		foreach($clients as &$client){	
  			$client['PROPS']=implode(',',GetPropNames($client['PROPS'],true));
  			if($nointerval)$client['LIFETIME']=0;
  			$formatNextTime($client);
  		}
		$form["actions"][]=["type"=>"List","name"=>"CLIENTS","caption"=>"Registered Clients","rowCount"=>5,"add"=>false,"delete"=>false, "save"=>false, "columns"=> [
 				["label"=>"ObjectID","name"=>"ID","width"=>"55px"],
 				["label"=>"Propertys", "name"=>"PROPS","width"=>"auto"],
				["label"=>"AutoUpdate", "name"=>"LIFETIME","width"=>"70px"]
 			], 'values' => array_values($clients)
 		];
// 		$form["actions"][]=["type"=>"Button","label"=>"TEST", "onClick"=>"IPS_RequestAction(\$id,'TEST',0);"];
		if($propInfo)$propInfo=" ".$this->Translate("Propertys").": $propInfo";
 		if($info)$info=" ".$this->Translate("to")." $info$propInfo";
 		$form["status"]=[
 			["code"=>102, "icon"=>"active",  "caption"=> $this->Translate("Connection open").$info],
 			["code"=>104, "icon"=>"inactive","caption"=> $this->Translate("Connection closed").$info],
 			["code"=>200, "icon"=>"error",   "caption"=> "Connection is in error state. Please check message log for more information."],
 			["code"=>201, "icon"=>"error",   "caption"=> "Connection config missing. Please select a config from listbox or create one with RPCBuilder."],
 			["code"=>301, "icon"=>"error",   "caption"=> sprintf($this->Translate("Invalid Updateinterval. Please select 0 for non (best joice when events enabled) or use Value between %s to xxxxxx seconds"),self::min_interval_secs)],
 		];
 		
		return json_encode($form);
	}
	function ForwardData($JSONString){
		$this->SendDebug(__FUNCTION__,utf8_decode($JSONString),0);
		if(!$this->ReadPropertyBoolean('Open'))return $this->_returnError('Splitter instance closed',1001);
		if(!$data = json_decode($JSONString,true))return $this->_returnError('Invalid Forward Data' , 1004);
		utf8::decode_array($data);
		if(empty($data['ObjectID']))return $this->_returnError('Invalid or no ObjectID received', 1005);
		if(empty($data['Buffer']['Function']))return $this->_returnError('No function given', 1002);
		$function=$data['Buffer']['Function'];
		$arguments=empty($data['Buffer']['Arguments'])?[]:$data['Buffer']['Arguments'];
		
		if($this->_handleInternalFunction($function, $arguments, $data['ObjectID'], $result))	return $result;
		if(!$api=$this->_getApi())return  $this->_returnError('API Creation error', 1003);
		$r = is_numeric(key($arguments))? call_user_func_array([$api,$function], $arguments) : $api->$function($arguments);
		if($api->HasError())return $this->_returnError($api->LastError(), $api->LastErrorCode());
		return $this->_returnResult($r);
	}
	
// 	function ReceiveData($JSONString){
// 		$this->SendDebug(__FUNCTION__,'Message::'.$JSONString,0);
// 		$data = json_decode($JSONString);
// 		$data->DataID = "{165092DF-CONT-4980-XB00-20171212XLIB}";
// 		if(empty($data->Buffer))return;
// 		//$this->SendDataToChildren(json_encode($data));
// 	}	 
	
	
	public function UpdateClients(){
		return $this->_updateClientData();
	}
	
	protected function error($Message, $ErrorCode = NULL){
		parent::error($Message, $ErrorCode);
		$this->setValue('LAST_ERROR',$Message);
	}
	
	protected function ProcessHookData() {
		$accept=function(){
			header('HTTP/1.1 202 Accepted');
			echo "HTTP/1.1 202 Accepted\n\n";
			//	header("HTTP/1.0 200 OK");
			//	echo "HTTP/1.0 200 OK\n\n";
		};
		$error=function(){
			header('HTTP/1.1 404 ERROR');
			echo "HTTP/1.1 404 ERROR\n\n";
		};
		$registeredProps=intval($this->GetBuffer('RegisteredClientProps'));
		if($registeredProps<1) return $error($this->error('%s No Registered props found',__FUNCTION__));
		if(!$this->ReadPropertyBoolean('UseEvents'))return $error($this->error('%s Events are disabeld',__FUNCTION__));
		$Request = trim(file_get_contents('php://input'));
		$Request=htmlspecialchars_decode($Request);
		if(empty($Request)){
			if($this->_debugLevel&DEBUG_DETAIL)$this->debug('%s Empty data received',__FUNCTION__);
			return $error();
		}
		$eventType='';
		if(preg_match('/<LastChange>(.+)<\/LastChange>/', $Request, $m)){
 			$xml=simplexml_load_string('<?xml version="1.0" encoding="utf-8"?>'.$m[1]);
 			$values=$this->_valuesFromLastChangeXML($xml);
 			$eventType='change';
 		}else{
	 		$Request='<?xml version="1.0" encoding="utf-8"?>'.str_replace('e:','',$Request);	
			$xml=simplexml_load_string($Request);
 			$values=$this->_valuesFromPropertysXML($xml);
 			$eventType='property';
 		}

//  		if (!IPS_SemaphoreEnter(__FUNCTION__,1500))return $accept();
 	 		
 		if($this->_debugLevel&DEBUG_DETAIL)$this->SendDebug(__FUNCTION__, 'Process Changes => '.VarExport($values),0);
		$update=false;
		foreach($values as $instanceID=>$data){
			if($eventType=='change'){
				$propNames=GetPropNames($registeredProps,true);
				foreach($data as $k=>$v) if(in_array($k, $propNames)===false){
					unset($data[$k]);
				}
 				if(count($data)>0){
					$this->_sendValuesToClients($data);
					$update=true;
 				}
			}elseif($eventType=='property'){
				$this->_sendValuesToClients($data);
				$update=true;
			}else {
				
				$this->error('Unknown eventtype %s in %s',$eventType, __FUNCTION__);
			}
		}
 		if($update)$this->_handleClientList('update',0,$registeredProps);
// 		IPS_SemaphoreLeave(__FUNCTION__); 		
 		
		return $accept();
	}
	
// 	protected function SendDebug($class, $Message, $code){
// 		if(is_numeric($Message)){
// 			$Message="($Message) $class";
// 			$class=$code;				
// 		}
// 		parent::SendDebug($class,$Message,0);
// 	}
	
	private function & _getApi(){
		if(!is_null($this->_api))return $this->_api;
		if(!($fn=$this->ReadPropertyString('File')) || !is_file(RPC_CONFIG_DIR."/$fn"))return $this->_api;
		$this->_api=new RPCDevice(RPC_CONFIG_DIR."/$fn");
		if($this->_api->HasError()){
			$this->_error($this->_api->GetError());
			$this->_setOnline(false);
			$this->_api=null;
		}else $this->_setOnline($this->_api->IsOnline());
		if($this->_api){
			$this->_api->SetDebugHandler($this,'RequestAction');
			$this->_api->SetDebugLevel($this->ReadPropertyInteger('DebugLevel'));
		}
		return $this->_api;
	}
	
	private function _handleInternalFunction($function, $arguments,$objectID, &$result ){
		if(strcasecmp($function,'RequestApiProps')==0){
			$this->_registerClient($objectID, $arguments[0]);
			$props=['PROPS'=>$this->ReadPropertyInteger('Props'), 'GROUPS'=>$this->ReadPropertyInteger('Groups')];
			if($p=json_decode($this->GetBuffer(self::buffer_api_info_name),true))$props=array_merge($props,$p);
			$result=$this->_returnResult($props);
			return true;
		}	
		if(strcasecmp($function,'RequestApiInfo')==0){
			//$this->_registerClient($objectID, $arguments[0]);
			$result=$this->_returnResult($info);
			return true;
		}	
		if(strcasecmp($function,'RequestDataUpdate')==0){
			$result= $this->_returnResult($this->_updateClientData($arguments[0],0,true));
			return true;
		}
		if(strcasecmp($function,'ClientDataChanged')==0){
			$result= $this->_returnResult($this->_updateClientData($arguments[0],0));
			return true;
		}
		if(strcasecmp($function,'DeleteClient')==0){
			$this->_unregisterClient($objectID);
			//$result= $this->_returnResult($this->_updateClientData($arguments[0]));
			return true;
		}
		
		return false;
	}
/*
 * Client List Handline
 */	
	private function _handleClientList($Command, $ObjectID=0, $Props=0){
		$clients=json_decode($this->GetBuffer(self::buffer_client_name),true);
		$changed=false;
		switch($Command){
			case 'calcprops':
				foreach($clients as $id=>$client)$Props=$Props|$client['PROPS']; 
				return $Props;
			case 'update'	:
				if(empty($ObjectID) && empty($Props)) break;
				if($this->_debugLevel&DEBUG_DETAIL)$this->debug('Update clients lastupdate from props %s',$Props);
				if(empty($ObjectID)){
					foreach($clients as $id=>$client){
						if($Props & $client['PROPS']){
							$changed=true;
							$clients[$id]['LASTUPDATE']=time();
						}
					}
				}else {
					$changed=true;
					$clients[$ObjectID]['LASTUPDATE']=time();
				}	
				break;
			case 'add'	    :
				if(!empty($clients[$ObjectID])) break;
				$changed=true;
				if($this->_debugLevel&DEBUG_DETAIL)$this->debug('Add client %s',$ObjectID);
				$this->RegisterMessage($ObjectID,11102); // Instanz wurde getrennt
				$clients[$ObjectID]=['ID'=>$ObjectID, 'PROPS'=>$Props, 'LASTUPDATE'=>time(), 'LIFETIME'=>self::refresh_life_time_sec ];
				break;
			case 'del'		:	
				if(empty($clients[$ObjectID])) break;
				if($this->_debugLevel&DEBUG_DETAIL)$this->debug('Delete client %s',$ObjectID);
				$changed=true;
				$this->UnRegisterMessage($ObjectID,11102); 
				unset($clients[$ObjectID]);
				break;
		}
// 		if (!IPS_SemaphoreEnter(__FUNCTION__,1500))return false;
		if($changed)$this->SetBuffer(self::buffer_client_name,json_encode($clients));
// 		IPS_SemaphoreLeave(__FUNCTION__);
		return $changed;
		
	}
	private function _updateRegisteredClientProps(){
		$this->SetBuffer('RegisteredClientProps', strval($this->_handleClientList('calcprops')));
	}
	private function _registerClient(int $ObjectID, int $Props){
		if($ok=$this->_handleClientList('add',$ObjectID,$Props)){
			$allProps=intval($this->GetBuffer('RegisteredClientProps'));
			$this->SetBuffer('RegisteredClientProps',strval($allProps | $Props));
		}	
		return $ok;
	}
	private function _unregisterClient(int $ObjectID){
		if($ok=$this->_handleClientList('del',$ObjectID)){
			$this->SetBuffer('RegisteredClientProps', strval($this->_handleClientList('calcprops')));
		}
		return $ok;
	}
/*
 * Event List Handling
 */
	private function _handleEventList($Command, $LiveTimeSec=null, $storeProp=true){
		if(!$api=$this->_getApi()) return $this->_error('API creation Error');
//		$events=json_decode($this->GetBuffer(self::buffer_eventids_name),true);
		$events=json_decode($this->ReadPropertyString('Events'),true);
		$unRegister=function(&$event){
			if(!$event['ACTIVE'] || empty($event['SID']))return false;
			$msg=$api->UnRegisterEvent($event['SID'], $event['SERVICE'])?'OK':'FAILED';
			$this->debug("Unregister %s events: %s" ,$event['SERVICE'], $msg);
			$event['SID']='';
			$event['LASTUPDATE']=$event['LIFETIME']=0;
			return true;
		};
		$changed= $active=false;
// $this->SendDebug(__FUNCTION__,$Command,0);		
		switch($Command){
			case 'register':
				$this->_stopTimer(2);
				if(!$myIp = net::get_local_ip()){$this->_error('Cant get local IP'); break;}
				if(!$hook = $this->_registerHook(true)){$this->_error("Failed register hook: ".self::eventControl_hook,-1);break;}
				$callbackUrl="http://$myIp:3777{$hook}";
				if(is_null($LiveTimeSec))$LiveTimeSec=self::refresh_life_time_sec;
				if(!empty($LiveTimeSec) && $LiveTimeSec < self::refresh_life_time_sec)$LiveTimeSec=self::refresh_life_time_sec;
				$minLiveTime=2678400;
				foreach ($events as $index=>&$event){
					if(!$event['ACTIVE'])continue;
					if($unRegister($event))$changed=true;
					if($result=$api->RegisterEvent($event['SERVICE'],$callbackUrl,$LiveTimeSec)){
						$msg='OK';
						$event['SID']=$result['SID']; $event['LASTUPDATE']=0;
						$event['LIFETIME']=(($result['TIMEOUT'] /100) * 50 ); // 50% of Original LIFETIME for secure 
						$minLiveTime=min($minLiveTime,$event['LIFETIME']);
						$changed=$active=true;
					} else $msg='FAILED';
					$this->debug("Register %s events: %s",$event['SERVICE'],$msg);
				}
				$this->SetBuffer('EventRefreshInterval',$minLiveTime);
				break;
			case 'refresh'	    :
				$this->_stopTimer(2);
				$minLiveTime=2678400;
				foreach ($events as &$event){
					if(!$event['ACTIVE'])continue;
					$active=true;
					if(floor((time() - $item['LASTUPDATE'])/1000) >= $event['LIFETIME']){
						if(!is_null($LiveTimeSec) && $LiveTimeSec < self::refresh_life_time_sec)$LiveTimeSec=self::refresh_life_time_sec;
						$active=false;
						if($result=$api->RefreshEvent($event['SID'], $event['SERVICE'], $LiveTimeSec)){
							$msg='OK';
							$event['LASTUPDATE']=time();
							$event['LIFETIME']=(($result['TIMEOUT'] /100) * 50 ); // 50% of Original LIFETIME for secure 
							$changed=$active=true;
						}else $msg='FAILED';
						$this->debug("Refresh %s events: %s",$event['SERVICE'], $msg);
					}
					$minLiveTime=min($minLiveTime,$event['LIFETIME']);
				}
				$this->SetBuffer('EventRefreshInterval',$minLiveTime);
				break;
			case 'unregister'	:
				$this->_stopTimer(2);
				foreach ($events as $index=>&$event){
					if($unRegister($event))$changed=true;
				}
				break;
			case 'rebuild'		:
				$newevents=[];
				$active=false;
				if(!$serviceNames = $api->GetEventServiceNames()){
					$this->_error("No Events found"); 
					foreach($events as &$event)$unRegister($event);
					$changed=true;
					$events=[];
					break;
				}
				foreach($serviceNames as $sname){
					$newevents[]=['ACTIVE'=>false, 'SID'=>'','SERVICE'=>$sname,'LASTUPDATE'=>0 ,'LIFETIME'=>0, 'VARS'=>implode(',', $api->GetEventVars($sname))];
				}
 				$changed=system::array_update($events, $newevents);
				break;
			case 'clear' 	:
				foreach($events as $event)$unRegister($event);
				$events=[];
				$changed=true;
				break;
			default: return $this->error('Call %s with unkonwn command %s',__FUNCTION__,$Command);	
		}
		if($changed)$this->setProperty('Events',json_encode($events),$storeProp);
		if($active)$this->_startTimer(2);else $this->SetBuffer('EventRefreshInterval',0);
$this->SendDebug(__FUNCTION__,$Command." : $changed",0);		
		return $changed;
	}
	private function _registerEvents(){
		return $this->_handleEventList('register');
	}
	private function _unregisterEvents(){
		return $this->_handleEventList('unregister');
	}
	private function _refrehEvents(){
		return $this->_handleEventList('refresh');
	}
	
/*
 * Timer Handling
 */	
	private function _stopTimer($What=0){
		if($What==0 || $What&1)$this->SetTimerInterval("UpdateTimer",0);
		if($What==0 || $What&2)$this->SetTimerInterval("EventTimer",0);
		if($this->_debugLevel & DEBUG_ALL)$this->debug('Stop timer! What: %s',$What);
	}
	private function _startTimer($What=0){
		if(($What==0 || $What&1) && $this->ReadPropertyBoolean('UseEvents')){
			if(!$time=intval($this->GetBuffer('EventRefreshInterval')))$time=200;
			$this->SetTimerInterval("EventTimer",$time);
			if($this->_debugLevel & DEBUG_ALL)$this->debug('Start event timer  with %s seconds',$time);
		}
		if(($What==0 || $What&2) && ($time=$this->ReadPropertyInteger('UpdateInterval'))>0){
			$this->SetTimerInterval("UpdateTimer",$time);
			if($this->_debugLevel & DEBUG_ALL)$this->debug('Start update timer  with %s seconds',$time);
		}	
	}

	
/*
 * Update Data Handling
 */	
	private function _updateClientData($byprops=0, $instanceId=null, $doReturn=false){
		$this->_stopTimer(1);
		if(!$api  =$this->_getApi())return  $this->_error('API creation Error');
		$registeredProps=intval($this->GetBuffer('RegisteredClientProps'));
		if($registeredProps < 1 )return true;
		$props=$this->ReadPropertyInteger('Props');
		if($byprops==0)$byprops=$props;
		$myinterval=$this->ReadPropertyInteger('UpdateInterval');
		$is_prop=function($prop) use ($registeredProps,$byprops,$props){
			return ( ($registeredProps & $prop) && ($props & $prop) && ($byprops & $prop));
		};
		$ok=true; $data=[];
		$params=is_null($instanceId)?null:[NAME_INSTANCE_ID=>$instanceId];
		if( $is_prop(PROP_PLAY_CONTROL)){
			$newState=PLAYMODE_STOP;
			if($state=$api->GetTransportInfo($params)){
				$state=$state['CurrentTransportState'];
				if($param=$api->GetFunctionParam('GetTransportInfo','')){
					if(!empty($param[VALUE_LIST])){
						$newState=array_search($state, $param[VALUE_LIST]);
						if($newState!==false){
							if($newState==3)$newState=PLAYMODE_PLAY;
							elseif($newState>3)$newState=PLAYMODE_STOP;
						}else $newState=PLAYMODE_STOP;
					}else {
						if(stripos($state, 'STOP')!==false)	 $newState=PLAYMODE_STOP;
						elseif(stripos($state, 'PLAY')!==false) $newState=PLAYMODE_PLAY;
						elseif(stripos($state, 'PAUSE')!==false)$newState=PLAYMODE_PAUSE;
					}
				}
			}	
			$ok=!$api->HasError();
			$data['PLAYSTATE']=$newState;
		}
		$propNames = GetPropNames($registeredProps);
		if($ok && $is_prop(PROP_SOURCE_CONTROL))    { $data[$propNames[PROP_SOURCE_CONTROL]]=$api->GetSource($params); $ok=!$api->HasError();}
		if($ok && $is_prop(PROP_VOLUME_CONTROL))	{ $data[$propNames[PROP_VOLUME_CONTROL]]=$api->GetVolume($params); $ok=!$api->HasError();}
		if($ok && $is_prop(PROP_MUTE_CONTROL))		{ $data[$propNames[PROP_MUTE_CONTROL]]=$api->GetMute($params);$ok=!$api->HasError();}
		if($ok && $is_prop(PROP_BASS_CONTROL))		{ $data[$propNames[PROP_BASS_CONTROL]]=$api->GetBass($params);$ok=!$api->HasError();}
		if($ok && $is_prop(PROP_TREBLE_CONTROL))	{ $data[$propNames[PROP_TREBLE_CONTROL]]=$api->GetTreble($params);$ok=!$api->HasError();}
		if($ok && $is_prop(PROP_BRIGHTNESS_CONTROL)){ $data[$propNames[PROP_BRIGHTNESS_CONTROL]]=$api->GetBrightness($params);$ok=!$api->HasError();}
		if($ok && $is_prop(PROP_SHARPNESS_CONTROL))	{ $data[$propNames[PROP_SHARPNESS_CONTROL]]=$api->GetSharpness($params);$ok=!$api->HasError();}
		if($ok && $is_prop(PROP_CONTRAST_CONTROL))	{ $data[$propNames[PROP_CONTRAST_CONTROL]]=$api->GetContrast($params);$ok=!$api->HasError();}
		if($ok && $is_prop(PROP_BALANCE_CONTROL))	{ $data[$propNames[PROP_BALANCE_CONTROL]]=$api->GetBalance($params);$ok=!$api->HasError();}
		if($ok && $is_prop(PROP_LOUDNESS_CONTROL))	{ $data[$propNames[PROP_LOUDNESS_CONTROL]]=$api->GetLoudness($params);$ok=!$api->HasError();}
		if($ok){
			if($doReturn)return $data; // client call update ... return result;
 			$this->_handleClientList('update',0,$registeredProps);
 			$data[NAME_INSTANCE_ID]=intval($instanceId);
 			$this->_sendValuesToClients($data);
			$this->_startTimer(1);
		}else 
			$this->_error($api->GetError());
		return $ok;
	}
	
/*
 * Update Handling from Event Data
 */	
	
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
			$data=[NAME_INSTANCE_ID=>$instanceID, $iname=>$attr];
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
	 					$data[GetPropNames(PROP_BALANCE_CONTROL)[PROP_BALANCE_CONTROL]]=$balance;
					}
				}else 
					echo "Var: $var\n";
			}
			$values[$instanceID]=$data;		
		}
		return $values;
	}
	private function _valuesFromPropertysXML($xml) {
		$data=json_decode(json_encode($xml),true)['property'];
		$values=[NAME_INSTANCE_ID=>EVENT_INSTANCE_ID];
		foreach($data as $prop){
			foreach($prop as $name=>$value){
		// 		if(is_array($value) && count($value)==0)$value=null;
				$values[$name]=$value;
			}
		}
		return [EVENT_INSTANCE_ID=>$values];
	}
	
/*
 * Sending Data to Clients
 */	
	private function _sendValuesToClients(array $values){
		if(empty($values[NAME_INSTANCE_ID]))$values[NAME_INSTANCE_ID]=0;
		utf8::encode_array($values);
		$send['Buffer']['Values']=$values;
		$send['DataID']='{165092DF-CONT-4980-XB00-20171212XLIB}';
		$send=json_encode($send);
		if($this->_debugLevel)$this->SendDebug(__CLASS__,"Send Values: ".$send,0);
		$this->SendDataToChildren($send);
	}
	
	private function _sendApiPropsToClients(array $ApiProps){
		$data['Buffer']['Command']='API_PROPS';
		$data['Buffer']['Value']=$ApiProps;
		$data['DataID']='{165092DF-CONT-4980-XB00-20171212XLIB}';
		$data=json_encode($data);
		$this->SendDebug(__FUNCTION__,$data,0);
		$this->SendDataToChildren($data);
	}
	
/*
 * Internal functions
 */	
	
	private function _setOnline(bool $online){
		$id=$this->GetIDForIdent('ONLINE');
		if(GetValueBoolean($id)!==$online)SetValueBoolean($id,$online);
	}
	private function _returnError($Message,$ErrorCode){
		$this->setValue('LAST_ERROR',$Message);
		return json_encode(['Error'=>['msg'=>$Message,'code'=>$ErrorCode]]);
	}
	private function _returnResult($Result){
// 		$this->SendDebug(__FUNCTION__,VarExport($Result),0);
		return json_encode(['Result'=>$Result]);
	}
	
	private function _registerHook(bool $Create) {
		$ids = IPS_GetInstanceListByModuleID("{015A6EB8-D6E5-4B93-B496-0D3F77AE9FE1}");
		if(sizeof($ids) > 0) {
			$hookname= "/hook/{$this->InstanceID}/".self::eventControl_hook;
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
?>