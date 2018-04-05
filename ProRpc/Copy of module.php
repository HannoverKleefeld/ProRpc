<?php
/** @file module.php
@brief ProRpc Module for IP-Symcon

@author Xaver Bauer
@date 01.04.2018
@version 2.1.5
*/

/** @page module_version_history Module Version History
@version 2.1.5
- Fix display input selector
@date 29.03.2018 21:33:59
@version 2.1.4
- created

*/

error_reporting(E_ALL);

require_once __DIR__ .'/../libs/loader.php';

const 
	ACTIONS_INFO	= 0,
	ACTIONS_CREATE = 1,
	ACTIONS_DEBUG	= 2,
	ACTIONS_PLAYER = 10,
	ACTIONS_TITLER = 12,
	ACTIONS_SOUND  = 20,
	ACTIONS_VIDEO  = 30,
	ACTIONS_REMOTE = 40,
	ACTIONS_EVENTS = 50;
	

const
	SELECTOR_NONE 	= 0,
	SELECTOR_ALL 	= 1,
	SELECTOR_CONFIG = 10,
	SELECTOR_TITLER = 20,
	SELECTOR_REMOTE = 60,
	SELECTOR_EVENTS = 70;
	

const 
// 	MACRO_UPDATE	= 1,
	MACRO_CHECK		= 2,
	MACRO_EXECUTE	= 3;
const 
	RPC_STOP = 0,
	RPC_PLAY = 1,
	RPC_PAUSE = 2,
	RPC_NEXT = 3,
	RPC_PREV = 4;

class ProRpcModule extends IPSModule{
	protected $api=null,$logger = null;
	/**
	 * {@inheritDoc}
	 * @see IPSModule::Create()
	 */
	function Create() {
		parent::Create();
		// General
		$this->registerPropertyString('Host','');
		$this->registerPropertyInteger('Port',0);
		$this->registerPropertyString('User','');
		$this->registerPropertyString('Pass','');
		$this->registerPropertyString('ConfigFile','');
		$this->registerPropertyInteger('LogOptions',DEBUG_ERRORS);
		$this->setBuffer("ConfigFile",$this->ReadPropertyString('ConfigFile'));
		$this->registerPropertyInteger("InstanceID",0);
	}	
	function ApplyChanges() {
		$this->stopTimer();
		$new_file=$this->ReadPropertyString('ConfigFile')!=$this->GetBuffer('ConfigFile');
		
		$changed=$this->ApplyGlobals();
		$this->SetBuffer('ConfigFile',$this->ReadPropertyString('ConfigFile'));
		if(!$changed){
			$changed=$this->UpdateGlobals();		
		}
		if($changed){
// 			IPS_LogMessage(__FUNCTION__, "Config changed");
			IPS_ApplyChanges($this->InstanceID);
			if($new_file){
				$this->StatusUpdateRequest();
				if(substr($myname=IPS_GetName($this->InstanceID),0,6)==self::my_module_name){
					$def=json_decode($this->GetBuffer('ApiDef'));
					if($def)IPS_SetName($this->InstanceID,substr($myname,0,10)." ".$def->{DEF_MANU}." ".$def->{DEF_MODEL});
				}
			}
		}else{
			$this->startTimer();
		}
	}
	protected function UpdateGlobals(){}
	protected function ApplyGlobals(){ 
		
		return false;
	}
	protected function stopTimer(){}
	protected function startTimer(){}
	protected function apiHasProps(int $props){}
	protected function getLogger():IPSRpcLogger{
		if(!is_null($this->logger))return $this->logger;
		if(!$opts=$this->ReadPropertyInteger('LogOptions'))return $this->logger;
		$this->logger=new IPSRpcLogger($this, $opts);
		return $this->logger;	
	}
	protected function getApi($configFile=''){
		if(empty($configFile))$configFile=$this->GetBuffer('ConfigFile');
		if(!is_null($this->api) && $this->api->GetFilename()==$configFile)return $this->api;
// IPS_LogMessage(__FUNCTION__ . ':'.__LINE__, "Load $configFile");
		$api=null;
		$this->api = new RpcApi($configFile, $this->_getLogger());
 		if($this->api->HasError()){
 			echo "API error! For more information, see the message log";
 			return $this->api=null;
 		}
 		elseif($c=$this->api->GetConfig()){
 			if($c->{CONFIG_OPTIONS} & OPTIONS_NEED_HOST){
 				if(empty($c->{CONFIG_HOST}))$c->{CONFIG_HOST}=$this->ReadPropertyString('Host');
 			}
 			if($c->{CONFIG_OPTIONS} & OPTIONS_NEED_PORT){
 				if(empty($c->{CONFIG_PORT}))$c->{CONFIG_PORT}=$this->ReadPropertyInteger('Port');
 			}
 			if($c->{CONFIG_OPTIONS} & OPTIONS_NEED_USER_PASS){
 				if(empty($c->{CONFIG_LOGIN_U}))$c->{CONFIG_LOGIN_U}=$this->readPropertyString('User');
 				$c->{CONFIG_LOGIN_P}=$this->readPropertyString('Pass');
 			}
 		}
		return $this->api;
	}	
	
}

/** @copybrief module.php
 * 
 */
class ProRpc extends IPSModule {
/// @cond VARIABLES	
	// Global
	const my_module_name ='ProRpc';
	protected $api=null,$logger = null;
	// RemoteControl
	const new_macro_name = 'new macro';
	const max_macro_keys = 8;
	const min_seconds_before_create_timer = 5;
/// @endcond	
/// @cond PRIVATE
	/**
	 * {@inheritDoc}
	 * @see IPSModule::Create()
	 */
	function Create() {
		parent::Create();
		// General
		$this->registerPropertyString('Host','');
		$this->registerPropertyInteger('Port',0);
		$this->registerPropertyString('User','');
		$this->registerPropertyString('Pass','');
		$this->registerPropertyBoolean("DeleteNoUsed",false);
		$this->registerPropertyString('ConfigFile','');
		$this->registerPropertyInteger('LogOptions',DEBUG_ERRORS);
		$this->registerPropertyInteger('UpdateInterval',0);
		$this->registerPropertyInteger('EventsInterval',0);
		$this->registerTimer("UpdateTimer",0,"IPS_RequestAction($this->InstanceID,'PROCESS_UPDATE',0);");
		$this->registerPropertyInteger("ActionSelector",0);
		$this->registerPropertyInteger("ConfigSelector",0);
		// API
		$this->setBuffer("ConfigFile",$this->ReadPropertyString('ConfigFile'));
		$this->SetBuffer("ApiProps",0);
		$this->SetBuffer("ApiDef",'');
		$this->SetBuffer("ApiConfig",'');
		$this->SetBuffer("ApiActions",'[]');
		$this->registerPropertyInteger("InstanceID",0);
		// Title btw Track info
		$this->RegisterPropertyBoolean('ShowTitleInfo', false);
		$this->RegisterPropertyBoolean('TitleInfoAsHtml', false);
		$this->RegisterPropertyString('HtmlTemplate', 'default.template');
		// Remote Control
		$this->registerPropertyString('RemoteGroups',json_encode(IPSRemoteKeys::defaultGroups()));
		$this->registerPropertyString('RemoteKeys',json_encode(IPSRemoteKeys::defaultKeyMap()));
		$this->registerPropertyString('RemoteMacros','[]');
		
		// Events
		$this->registerPropertyBoolean('EnableEvent',false);
		$this->registerPropertyString('EventServices','[]');
		$this->RegisterPropertyInteger('EventScript',0);
		$this->RegisterPropertyInteger('EventPort',3777);
		$this->registerTimer("EventTimer",0,"IPS_RequestAction($this->InstanceID,'PROCESS_EVENTS',0);");
		$this->SetBuffer("ActiveEvents",'[]');
		$this->SetBuffer('NextEventRefresh',0);
	}	
	/**
	 * {@inheritDoc}
	 * @see IPSModule::Destroy()
	 */
	function Destroy() {
		$this->_registerEventHook(false);
		if(count(IPS_GetInstanceListByModuleID('{19650302-XABA-MAJA-PROA-20180330XLIB}'))==0){
 			@IPS_DeleteVariableProfile("RPC_PLAY_$this->InstanceID");
 			@IPS_DeleteVariableProfile('RPC_BALANCE');
 			for($j=0;$j< IPSRemoteKeys::max_groups ;$j++)
 				@IPS_DeleteVariableProfile("RPC_GROUP_{$j}_$this->InstanceID");
		}
		
	}
	/**
	 * {@inheritDoc}
	 * @see IPSModule::ApplyChanges()
	 */
	function ApplyChanges() {
		$this->stopTimer();
		$new_file=$this->ReadPropertyString('ConfigFile')!=$this->GetBuffer('ConfigFile');
		$changed=$this->ApplyGlobals();
		if(!$changed){
			if($this->_apiHasProps(PROP_REMOTE_CONTROL) && $this->_handleMacroTable (MACRO_CHECK))$changed=true;	
			if($this->_apiHasProps(PROP_EVENTS) && $this->_updateEvents(true))$changed=true;
			
		}
		if($changed){
// 			IPS_LogMessage(__FUNCTION__, "Config changed");
			IPS_ApplyChanges($this->InstanceID);
			if($new_file){
				$this->StatusUpdateRequest();
				if(substr($myname=IPS_GetName($this->InstanceID),0,6)==self::my_module_name){
					$def=json_decode($this->GetBuffer('ApiDef'));
					if($def)IPS_SetName($this->InstanceID,substr($myname,0,10)." ".$def->{DEF_MANU}." ".$def->{DEF_MODEL});
				}
			}
		}else{
			$this->startTimer();
		}
			
	}
	/**
	 * {@inheritDoc}
	 * @see IPSModule::GetConfigurationForm()
	 */
	function GetConfigurationForm() {
		$form=[];$keys=$macros=$optmacros=null;
		// Create Config select 
		$foptions=[];
		if($files=scandir(RPC_CONFIG_DIR))foreach ($files as $file){
			if($file[0]=='.')continue;
			$file=pathinfo($file);
			if(!empty($file['extension'])&&$file['extension']=='json' && stripos($file['basename'], 'desc')===false )$foptions[]=["label"=>$file['filename'], "value"=> $file['basename']];
		}	
		$form["elements"][]=["type"=> "Select", "name"=>"ConfigFile", "caption"=> "RPC Config","options"=> $foptions];
		if(count($foptions)==0){
			$actionSelector=ACTIONS_CREATE;
			goto ShowActionsSelector;
		}
		 $actionSelector=$this->readPropertyInteger('ActionSelector');
		// --- Building Selectors -----------------------------------
		$actionSelectors=[ ['label'=>'Information','value'=>ACTIONS_INFO],['label'=>'Log Settings','value'=>ACTIONS_DEBUG],['label'=>'Create RPC Config','value'=>ACTIONS_CREATE] ];
		$configSelectors=[['label'=>'-> None <-','value'=>SELECTOR_NONE],['label'=>'*ALL*','value'=>SELECTOR_ALL] ,['label'=>'Remote Connection','value'=>SELECTOR_CONFIG]];
		$myActions=json_decode($this->GetBuffer('ApiActions'),true);
		$myProps=intval($this->GetBuffer('ApiProps'));
		foreach($myActions as $actions_id=>$prop_list){
			$show=false;
			foreach($prop_list as $prop=>$info)if($show=(!$info || !$info['hidden']))break;
			if(!$show)continue;
			switch ($actions_id){
				case ACTIONS_PLAYER :
					$actionSelectorsID[]=['label'=>'Player test','value'=>ACTIONS_PLAYER];
					break;
				case ACTIONS_TITLER :
					$configSelectors[]=['label'=>'Title output','value'=>SELECTOR_TITLER];
// 					$validConfigSelectors[]=ACTIONS_TITLER;
					break;
				case ACTIONS_SOUND :
					$actionSelectors[]=['label'=>'Sound test','value'=>ACTIONS_SOUND];
					break;
				case ACTIONS_VIDEO :
					$actionSelectors[]=['label'=>'Video test','value'=>ACTIONS_VIDEO];
					break;	
				case ACTIONS_REMOTE :
					$actionSelectors[]=['label'=>'Send Keycode','value'=>ACTIONS_REMOTE];
					$configSelectors[]=['label'=>'Keycodes and Macros','value'=>SELECTOR_REMOTE];
// 					$validConfigSelectors[]=SELECTOR_REMOTE;
					break;
				case ACTIONS_EVENTS :
					$configSelectors[]=['label'=>'Remote Events','value'=>SELECTOR_EVENTS];
// 					$validConfigSelectors[]=SELECTOR_EVENTS;
					break;
			}
		}
		$form["elements"][]=["type"=> "Select", "name"=>"ConfigSelector", "caption"=> "Show config for","options"=>$configSelectors];
		$form["elements"][]=["type"=> "Select", "name"=>"ActionSelector", "caption"=> "Show Actions for","options"=>$actionSelectors];
		// Add Delete Mode
		$YesNo=[['label'=>"Delete",'value'=>1],['label'=>"Hide",'value'=>0]];
		$form["elements"][]=["type"=> "Select", "name"=>"DeleteNoUsed", "caption"=> "Unused vars","options"=>$YesNo];

		if(!$this->GetBuffer('ConfigFile')){ // No Config selected
			return json_encode($form);
		}
		// Create Global Props
		$config_selected=$this->readPropertyInteger('ConfigSelector');
		
		if($config_selected==SELECTOR_CONFIG || $config_selected==SELECTOR_ALL){
			$form["elements"][]=["type"=>"ValidationTextBox","name"=>"Host", "caption"=> "Host" ];
			$form["elements"][]=["type"=>"NumberSpinner","name"=>"Port", "caption"=> "Port" ];
			$form["elements"][]=["type"=>"ValidationTextBox","name"=>"User", "caption"=> "User" ];
			$form["elements"][]=["type"=>"PasswordTextBox","name"=>"Pass", "caption"=> "Password" ];
			$form["elements"][]=["type"=>"NumberSpinner","name"=>"UpdateInterval", "caption"=> "Update Interval Seconds" ];
		}
		
		if((!$def=$this->_getApiDef())|| !$props=$def->{DEF_PROPS}){
			goto ShowActionsSelector;
		}
		
// return json_encode($form);		
		if($config_selected==SELECTOR_TITLER || (!empty($myActions[ACTIONS_TITLER]) && $config_selected==SELECTOR_ALL)){
			$YesNo=[['label'=>"Yes",'value'=>1],['label'=>"No",'value'=>0]];
			$form["elements"][]=["type"=> "Select", "name"=>"ShowTitleInfo", "caption"=> "Show Titleinfo","options"=>$YesNo];
			$form["elements"][]=["type"=> "Select", "name"=>"TitleInfoAsHtml", "caption"=> "Show Info as Html","options"=>$YesNo];
			$options=[];
			if($files=scandir(RPC_CONFIG_DIR))foreach ($files as $file){
				if($file[0]=='.')continue;
				$file=pathinfo($file);
				if(!empty($file['extension'])&&$file['extension']=='template')$options[]=["label"=>$file['filename'], "value"=> $file['basename']];
			}	
			$form["elements"][]=["type"=> "Select", "name"=>"HtmlTemplate", "caption"=> "Html template","options"=> $options];
		}

		if($config_selected==SELECTOR_EVENTS || ($myProps & PROP_EVENTS && $config_selected==SELECTOR_ALL) ){
			$ports=[3777];
			$ids = IPS_GetInstanceListByModuleID("{D83E9CCF-9869-420F-8306-2B043E9BA180}"); // WebServer
			foreach($ids as $id)if($c=json_decode(IPS_GetConfiguration($id)))$ports[]=$c->Port;
			foreach($ports as &$port)$port=['label'=>$port,'value'=>$port];
			$form["elements"][]=["type"=> "SelectScript", "name"=>"EventScript", "caption"=> "Event user Script"];
			$form["elements"][]=["type"=> "Select", "name"=>"EventPort", "caption"=> "Eventhook Port", "options"=>$ports];
	  		$form["elements"][]=["type"=> "CheckBox", "name"=>"EnableEvents", "caption"=> "Enable Events"];
			$form["elements"][]=["type"=>"List","name"=>"EventServices","caption"=>"Avaible Events","rowCount"=>5,"columns"=> [
					["label"=>"Name","name"=>"SERVICE", "width"=>"100px","save"=>true],
					["label"=>"Enabled","name"=>"ENABLED", "width"=>"50px","edit"=>["type"=> "CheckBox", "caption"=> "Enable servie events"]],
					["label"=>"Values","name"=>"VARS", "width"=>"auto","save"=>true],
					["label"=>"","name"=>"SID", "width"=>"0px","visible"=>false,"save"=>true],
					["label"=>"","name"=>"LIFETIME", "width"=>"0px","save"=>true],
					["label"=>"","name"=>"NEXTUPDATE","width"=>"0px","visible"=>false,"save"=>true]
				]	
			];
		}
		
		if($config_selected==SELECTOR_REMOTE || ($myProps & PROP_REMOTE_CONTROL && $config_selected==SELECTOR_ALL)){
			
			$form["elements"][]=["type"=>"List","name"=>"RemoteGroups","caption"=>"Avaible Groups","rowCount"=>5,"columns"=> [
					["label"=>"Name","name"=>"NAME", "width"=>"auto", "edit"=>["type"=>"ValidationTextBox","caption"=>"Groupname"]],
					["label"=>"Icon","name"=>"ICON", "width"=>"100", "edit"=>["type"=>"ValidationTextBox","caption"=>"Iconname"]],
					
					["label"=>"","name"=>"ID","width"=>"0","visible"=>false,"save"=>true]
				]	
			];
			$groups=json_decode($this->ReadPropertyString('RemoteGroups'));
			$options=[]; foreach($groups as $group)$options[]=['label'=>$group->NAME,'value'=>$group->ID];
 	  		$form["elements"][]=["type"=>"List","name"=>"RemoteKeys","caption"=>"Key to Group","rowCount"=>6,"add"=>false,"delete"=>false,"columns"=> [
	 				["label"=>"Code","name"=>"KEY","width"=>"50px"],
	 				["label"=>"Key","name"=>"NAME","width"=>"150px","edit"=>["type"=>"ValidationTextBox","caption"=>"Keyname"]],
 	  				["label"=>"","name"=>"rowColor","width"=>"0","visible"=>false,"save"=>true],
	 				["label"=>"Group","name"=>"GROUPID","width"=>"auto","edit"=>["type"=>"Select","caption"=>"Groupname", "options"=>$options ]],
	 			]	
	 		];
			$keys=json_decode($this->ReadPropertyString('RemoteKeys'));
 	  		$macros=json_decode($this->ReadPropertyString('RemoteMacros'));
	 		$optmacros=[["label"=>"<-None->", "value"=> '0']];
	 		foreach($macros as $macro){
	 			$optmacros[]=["label"=>$macro->NAME, "value"=> $macro->UUID];
	 		}
	 		$optkeys=[["label"=>"<-Select->", "value"=> 0]];
	 		foreach($keys as $k)$optkeys[]=["label"=>$k->NAME, "value"=>$k->KEY];
			sort($optkeys);
	 		$columns=[
				["label"=>"",		"name"=>"UUID","width"=>"0px","add"=>"0", "visible"=>false, "save"=>true],
				["label"=>"",		"name"=>"ERROR","width"=>"0px","add"=>"", "visible"=>false, "save"=>true],
				["label"=>"Name",	"name"=>"NAME","width"=>"80px","add"=>self::new_macro_name,"edit"=>["type"=>"ValidationTextBox","caption"=> "Macroname"]],
				["label"=>"Delay", 	"name"=>"DELAY","width"=>"60px","add"=>"250","edit"=>["type"=>"NumberSpinner", "caption"=> "Milliseconds"]],
			];
			for($j=0;$j<self::max_macro_keys;$j++){
				$columns[]=["label"=>"Key$j","name"=>"KEY$j","width"=>"75px", "add"=>"0", "edit"=>["type"=>"Select","caption"=>"Select Key $j", "options"=> $optkeys]];
				$columns[]=["label"=>"Delay$j","name"=>"DELAY$j","width"=>"60px", "add"=>"250", "edit"=>["type"=>"NumberSpinner","caption"=>"Milliseconds", "options"=> $optkeys]];
			}
			$columns[]=["label"=>"Next Macro","name"=>"MACRO","width"=>"80px", "add"=>"0", "edit"=>["type"=>"Select","options"=> $optmacros]];				
			$form["elements"][]=["type"=>"List","name"=>"RemoteMacros","caption"=>"KeyCode Macros","rowCount"=>5,"add"=>true,"delete"=>true,"columns"=> $columns]; 				
			$form["elements"][]=["type"=> "Label", "label"=> "COLORS => YELLOW : Api has no KeyCodes, RED : KEY not supported from Api"];			

		}
ShowActionsSelector:		
		switch($actionSelector){
			case ACTIONS_INFO  :
				if($def){
					$form["actions"][]=["type"=> "Label", "label"=> sprintf('Model: %s - %s API-Version %s',$def->{DEF_MANU},$def->{DEF_MODEL},$def->{DEF_VERSION})];
					if(!$def->{DEF_PROPS})$t=['None']; else foreach(ALL_PROPS as $prop)if($def->{DEF_PROPS} & $prop)$t[]=NAMES_PROPS[$prop];
					$form["actions"][]=["type"=> "Label", "label"=> 'Props: '.implode(', ',$t)];
				}
				$dops=$this->readPropertyInteger('LogOptions');
				if(!$dops)$l=['None']; else{
					foreach(ALL_DEBUG as $opt)if($dops & $opt)$l[]=NAMES_DEBUG[$opt];
				}
				$form["actions"][]=["type"=> "Label", "label"=> "Logging: ".implode(', ',$l)];
				break;
			case ACTIONS_DEBUG :
		 		$form["actions"][]=["type"=> "Label", "label"=> "Settings for logging "];
				$form["actions"][]=["type"=> "CheckBox", "name"=>"info", 	"caption"=> "Debug informations"];
		 		$form["actions"][]=["type"=> "CheckBox", "name"=>"call", 	"caption"=> "Debug function calls"];
		 		$form["actions"][]=["type"=> "CheckBox", "name"=>"build", 	"caption"=> "Debug build details"];
		 		$form["actions"][]=["type"=> "CheckBox", "name"=>"detect", 	"caption"=> "Debug device detections"];
		 		$form["actions"][]=["type"=> "CheckBox", "name"=>"detail", 	"caption"=> "Debug details"];
		 		$form["actions"][]=["type"=> "CheckBox", "name"=>"errors", 	"caption"=> "Log errors"];
		 		$form["actions"][]=["type"=> "CheckBox", "name"=>"setlogfile", 	"caption"=> "Set logfile"];
		 		$form["actions"][]=["type"=> "SelectFile", "name"=>"logfile", 	"caption"=> "Select a logfile"];
		
		 		$form["actions"][]=["type"=> "Button", "label"=> "Set Log Options", "onClick"=>'$o=$info?1:0;if($call)$o=$o|2;if($build)$o=$o|4; if($detect)$o=$o|8;if($detail)$o=$o|'.DEBUG_DETAIL.'; if($errors)$o=$o|'.DEBUG_ERRORS.'; RPC_SetLogOptions($id,$o);' ];
				break;
			case ACTIONS_CREATE :
		 		$form["actions"][]=["type"=> "Label", "label"=> "Create a new API configuration"];
				if(!file_exists(RPC_CONFIG_DIR.'/discover.data')){
		 			$form["actions"][]=["type"=> "Label", "label"=> "No Devices discovered. Press Discover, wait of OK then reload this formular"];
					$form["actions"][]=["type"=> "Button", "label"=> "Discover", "onClick"=>'IPS_RequestAction($id,"DISCOVER",0);echo "OK";' ];
				}else {
					$discovered=json_decode(file_get_contents(RPC_CONFIG_DIR.'/discover.data'),true);
					foreach($discovered as &$v)$v['HOST']=URL::parse($v['URL'])['host'];
	  				$form["actions"][]=["type"=>"List","name"=>"discover","caption"=>"Discovered devices","rowCount"=>6,"add"=>false,"delete"=>false,"columns"=> [
	 						["label"=>"Manufacurer","name"=>"MANU","width"=>"80px"],
	  						["label"=>"Modelnumber","name"=>"MODEL","width"=>"80px"],
	 						["label"=>"Modeltype","name"=>"TYPE","width"=>"auto"],
	  						["label"=>"Host","name"=>"HOST","width"=>"110px"],
	  						["label"=>"","name"=>"URL","width"=>"0","visible"=>false]
	 					],'values'=>$discovered	
	 				];
	  				$form["actions"][]=["type"=> "Button", "label"=> "Import selected device", "onClick"=>'if(empty($discover))echo "Please select a device";else if(RPC_ImportDevice($id,$discover["URL"]))echo "OK";else echo "'.$this->Translate("Fail").'";' ];
	  				$form["actions"][]=["type"=> "Button", "label"=> "Search for new devices", "onClick"=>'IPS_RequestAction($id,"DISCOVER",0);echo "OK";' ];
				}
				$form["actions"][]=["type"=>"ValidationTextBox","name"=>"url", "caption"=> "Url to import" ];
		 		$form["actions"][]=["type"=> "Button", "label"=> "Import", "onClick"=>'echo "import $url";' ];
				break;
			case ACTIONS_PLAYER :
  				$form["actions"][]=["type"=> "Button", "label"=>$this->Translate("Stop"),"onClick"=>"RPC_STOP(\$id);"];
  				$form["actions"][]=["type"=> "Button", "label"=>$this->Translate("Play"),"onClick"=>"RPC_PLAY(\$id);"];
  				$form["actions"][]=["type"=> "Button", "label"=>$this->Translate("Pause"),"onClick"=>"RPC_PAUSE(\$id);"];
				break;
			case ACTIONS_SOUND : 
				$form["actions"][]=["type"=> "HorizontalSlider", "name"=>"vol","caption"=>"Volume","minimum"=>0,"maximum"=>100,"onClick"=>"RSOUND_SetVolume(\$id,\$vol);"];		
  				$form["actions"][]=["type"=> "Button", "label"=>"Mute On","onClick"=>"RPC_SetMute(\$id,true);"];
  				$form["actions"][]=["type"=> "Button", "label"=>"Mute Off","onClick"=>"RPC_SetMute(\$id,false);"];
				break;
			case ACTIONS_VIDEO : 
				break;
			case ACTIONS_EVENTS :
				break;
			case ACTIONS_REMOTE :
	 			$optkeys=[];
	 			if(empty($keys)){
	 				$keys=json_decode($this->ReadPropertyString('RemoteKeys'));
					$macros=json_decode($this->ReadPropertyString('RemoteMacros'));
	 				$optmacros=[["label"=>"<-None->", "value"=> '0']];
	 				foreach($macros as $macro)$optmacros[]=["label"=>$macro->NAME, "value"=> $macro->UUID];
	 			}
	 			foreach($keys as $k)if(empty($k->rowColor))$optkeys[]=["label"=>$k->NAME, "value"=>$k->KEY];
				sort($optkeys);
				array_unshift($optkeys,["label"=>"<-Select->", "value"=> 0] );
				$form["actions"][]=["type"=> "Select", "name"=> "keycode", "caption"=>"Select Key to send","options"=>$optkeys];			
				$form["actions"][]=["type"=> "Button", "label"=> "Send Key", "onClick"=>'if(empty($keycode))echo "Please select a Key to send!";else RPC_SendKey($id,$keycode)'];
				if(count($optmacros)>1){
					//array_shift($optmacros);
					$form["actions"][]=["type"=> "Select", "name"=> "macro", "caption"=>"Select Macro to send","options"=>$optmacros];			
					$form["actions"][]=["type"=> "Button", "label"=> "Send Macro", "onClick"=>'if(empty($macro))echo "Please select a Macro to send!";else RPC_SendMacro($id,$macro)'];			
				}
				break;
			
		}
		$form["status"]=[
				["code"=>102, "icon"=>"active",  "caption"=> "API is ready to work"],
				["code"=>201, "icon"=>"error",   "caption"=> "no config selected"],
				["code"=>202, "icon"=>"error",   "caption"=> "failed to load selected config"],
				["code"=>301, "icon"=>"error",   "caption"=> "this API need to configure a Hostname"],
				["code"=>302, "icon"=>"error",   "caption"=> "this API need to configure a Port"],
				["code"=>303, "icon"=>"error",   "caption"=> "this API need to configure a User or Password"]
		];
		return json_encode($form);
		
		
		
	}
	/**
	 * {@inheritDoc}
	 * @see IPSModule::RequestAction()
	 */
	public function RequestAction( $Ident,  $Value) {
		if($Ident=='DEBUG_API'){
			list($class,$message)=json_decode($Value);
			$this->SendDebug($class,$message,0);
		}elseif($Ident=='PROCESS_UPDATE'){
			$this->StatusUpdateRequest();
		}elseif($Ident=='PROCESS_EVENTS'){
			$this->_updateEvents();
		}elseif($Ident=='DISCOVER'){
			require_once(RPC_LIB_DIR.'/rpcimport.inc');
			$fn=RPC_CONFIG_DIR.'/discover.data';
			$discovered=file_exists($fn)? json_decode(file_get_contents($fn),true):[];
 			$new=RpcDiscoverNetwork($this->_getLogger());
 			foreach($new as $discover)if(!in_array($discover, $discovered))$discovered[]=$discover;
 			file_put_contents($fn,json_encode($discovered));
		}elseif(($prop=array_search($Ident, NAMES_PROPS))!==false){
			$this->setValueByProp($prop, $Value);
		}else{
			$myActions=json_decode($this->GetBuffer("ApiActions"),true);
			foreach($myActions as $action_id => $props){
				if(array_key_exists($Ident,$props)){
					if($action_id==ACTIONS_REMOTE){
						$this->setValueByProp(PROP_REMOTE_CONTROL, [$Ident,$Value]);
					}
				}
			}
		}
	}
///@endcond
	
	public function SetLogOptions(int $Options){
		$this->setProperty('LogOptions', $Options,true);
		return true;
	}
	public function GetApi(){
		if($api=$this->_getApi()){
			$api->GetLogger()->SetParent(null);
			return $api;
		}
	}
	public function StatusUpdateRequest(){
		$myActions=json_decode($this->GetBuffer("ApiActions"),true);
		foreach($myActions as $action_id => $props){
			if($action_id==ACTIONS_TITLER){
				$this->_updateTitleInfo();
			}else
			foreach($props as $prop=>$var_id){
				if(!empty($var_id) && is_numeric($prop)){
					$this->getValueByProp((int)$prop,true);
				}
			}
		}
		return true;
	}
	
	public function Play(){
		return $this->_apiHasProps(PROP_PLAY_CONTROL)?$this->setValueByProp(PROP_PLAY_CONTROL,RPC_PLAY,true):false;
	}
	public function Pause(){
		return $this->_apiHasProps(PROP_PLAY_CONTROL)?$this->setValueByProp(PROP_PLAY_CONTROL, RPC_PAUSE,true):false;
	}
	public function Stop(){
		return $this->_apiHasProps(PROP_PLAY_CONTROL)?$this->setValueByProp(PROP_PLAY_CONTROL, RPC_STOP,true):false;
	}
	public function Next(){
		return $this->_apiHasProps(PROP_PLAY_CONTROL)?$this->setValueByProp(PROP_PLAY_CONTROL, RPC_NEXT,true):false;
	}
	public function Previous(){
		return $this->_apiHasProps(PROP_PLAY_CONTROL)?$this->setValueByProp(PROP_PLAY_CONTROL, RPC_PREV,true):false;
	}
	
	public function SetVolume(int $Volume){
		return $this->_apiHasProps(PROP_VOLUME_CONTROL)?$this->setValueByProp(PROP_VOLUME_CONTROL, $Volume,true):null;
	}
	public function SetTreble(int $Treble){
		return $this->_apiHasProps(PROP_TREBLE_CONTROL)?$this->setValueByProp(PROP_TREBLE_CONTROL, $Volume,true):null;
	}
	public function SetBass(int $Bass){
		return $this->_apiHasProps(PROP_BASS_CONTROL)?$this->setValueByProp(PROP_BASS_CONTROL, $Volume,true):null;
	}
	public function SetBalance(int $Balance){
		return $this->_apiHasProps(PROP_BALANCE_CONTROL)?$this->setValueByProp(PROP_BALANCE_CONTROL, $Volume,true):null;
	}
	public function SetMute(bool $Mute){
		return $this->_apiHasProps(PROP_MUTE_CONTROL)?$this->setValueByProp(PROP_MUTE_CONTROL, $Mute):null;
	}
	public function SetLoudness(bool $Loudness){
		return $this->_apiHasProps(PROP_LOUDNESS_CONTROL)?$this->setValueByProp(PROP_LOUDNESS_CONTROL, $Volume,true):null;
	}
	
	public function SetBrightness(int $Brightness){
		return $this->_apiHasProps(PROP_BRIGHTNESS_CONTROL)?$this->setValueByProp(PROP_BRIGHTNESS_CONTROL, (int)$Brightness,true):null;
	}
	public function SetSharpness(int $Sharpness){
		return $this->_apiHasProps(PROP_SHARPNESS_CONTROL)?$this->setValueByProp(PROP_SHARPNESS_CONTROL, (int)$Sharpness,true):null;
	}
	public function SetContrast(int $Contrast){
		return $this->_apiHasProps(PROP_CONTRAST_CONTROL)?$this->setValueByProp(PROP_CONTRAST_CONTROL,(int) $Contrast,true):null;
	}
	public function SetColor(int $Color){
		return $this->_apiHasProps(PROP_COLOR_CONTROL)?$this->setValueByProp(PROP_COLOR_CONTROL, (int) $Color,true):null;
	}
	
	public function SendKey(int $Key){
		if($ok=$this->_apiHasProps(PROP_REMOTE_CONTROL)){
			$ok=$this->setValueByProp(PROP_REMOTE_CONTROL, $Key);
		}//else echo "No Prop";
		$this->SendDebug(__FUNCTION__,"Key: $Key => ".($ok?'true':'false'),0);
		return $ok;
	}
	public function SendTwoKeys(int $Key_first, int $DelaySeconds, int $Key_second){
		if($this->_apiHasProps(PROP_REMOTE_CONTROL))return false;
		if(empty($Key_first))return false;
		if(!$this->setValueByProp(PROP_REMOTE_CONTROL, $Key_first))return false;
		if(empty($Key_second))$Key_second=$Key_first;
		if($DelaySeconds < self::min_seconds_before_create_timer){
			sleep($DelaySeconds);
			return $this->setValueByProp(PROP_REMOTE_CONTROL, $Key_second);			
		}
		return $this->_createSendKeyTimer($Key_second, $DelaySeconds);
	}
	public function SendMacro(string $MacroName):bool{
		if($this->_apiHasProps(PROP_REMOTE_CONTROL))return false;
		$macros=json_decode($this->ReadPropertyString('RemoteMacros'));
		$found=false;
		foreach($macros as $macro){
			if(is_numeric($MacroName)){
				if($found=$macro->UUID==intval($MacroName)){
					$MacroName=$macro->NAME;
					break;
				}elseif($found=$macro->NAME==$MacroName) break;
			}elseif($found=$macro->NAME==$MacroName) break;
		}
		return $found?$this->_handleMacroTable('execute',$MacroName):false;
	}
	
	public function CallFunction(string $FunctionName,string &$Error, string $Arguments=''){
		$Arguments=$Arguments?explode(',',$Arguments):[];
		foreach($Arguments as &$arg)$arg=mixed2value($arg);
		$r=($api=$this->_getApi())?$api->__call($FunctionName, $Arguments):null;
		if($api->HasError())$Error=$this->logger->LastErrorMessage();
		return $r;
	}
	public function ImportDevice(string $UrlToDescription){
		$api=new RpcApi($UrlToDescription,$this->_getLogger());
		return !$api->HasError()&& $api->IsImported();
	}
///@cond PROTECTED
	protected function setValueByProp(int $Prop, $Value){
		$api=$this->_getApi();
		if(!$api || !$api->HasProps($Prop)) return false;
		
		switch($Prop){
			case PROP_VOLUME_CONTROL	: $ok=$api->SetVolume(['DesiredVolume'=>(int)$Value]); break;
			case PROP_BALANCE_CONTROL	: $ok=$api->SetBalance(['DesiredBalance'=>(int)$Value]); break;
			case PROP_BASS_CONTROL 		: $ok=$api->SetBass(['DesiredBass'=>(int)$Value]); break;
			case PROP_TREBLE_CONTROL	: $ok=$api->SetTreble(['DesiredTreble'=>(int)$Value]); break;
			case PROP_LOUDNESS_CONTROL	: $ok=$api->SetLoudness(['DesiredLoudness'=>(bool)$Value]);break;
			case PROP_MUTE_CONTROL		: $ok=$api->SetMute(['DesiredMute'=>(bool)$Value]);break;
			case PROP_BRIGHTNESS_CONTROL: $ok=$api->SetBrightness(['DesiredBrightness'=>(int)$Value]); break;
			case PROP_SHARPNESS_CONTROL	: $ok=$api->SetSharpness(['DesiredSharpness'=>(int)$Value]);break;
			case PROP_CONTRAST_CONTROL	: $ok=$api->SetContrast(['DesiredContrast'=>(int)$Value]);break;
			case PROP_PLAY_CONTROL		: switch($Value){
					case RPC_STOP: $ok=$api->Stop(); if($ok)$ok=RPC_STOP;break;
					case RPC_PLAY: $ok=$api->Play(); if($ok)$ok=RPC_PLAY;break;
					case RPC_PAUSE:$ok=$api->Pause();if($ok)$ok=RPC_PAUSE; break;
					case RPC_NEXT: $ok=$api->Next(); if($ok)$Value=RPC_PLAY; break;
					case RPC_PREV: $ok=$api->Prev(); if($ok)$Value=RPC_PLAY; break;
					default: $ok=false;
				}
				break;
			case PROP_REMOTE_CONTROL	: 
				if(is_array($Value)){
					$ok=$api->SendKeyCode(0,$Value[1]);
					if($ok)$this->setValueByIdent($Value[0], $Value[1]);
					return $ok;
				}
				else return $api->SendKeyCode(['KeyCode'=>$Value]);
				
// 				$ok=false;	
			default: $ok=false;
		}
		if($ok)$this->setValueByIdent(NAMES_PROPS[$Prop], $Value);
		return (bool)$ok;
	}
	protected function getValueByProp(int $Prop, bool $Force=false){
		if($Force && !($api=$this->_getApi()) ||!$api->HasProps($Prop)) return false;
		
		if(!$Force)	return $this->getValue(NAMES_PROPS[$Prop]);
		switch ($Prop){
			case PROP_VOLUME_CONTROL	: $value=$api->GetVolume();	break;;
			case PROP_BALANCE_CONTROL	: $value=$api->GetBalance(); break;
			case PROP_BASS_CONTROL 		: $value=$api->GetBass(); break;
			case PROP_TREBLE_CONTROL	: $value=$api->GetTreble(); break;
			case PROP_LOUDNESS_CONTROL	: $value=$api->GetLoudness(); break;
			case PROP_MUTE_CONTROL		: $value=$api->GetMute();break;
			case PROP_BRIGHTNESS_CONTROL: $value=$api->GetBrightness(); break;
			case PROP_SHARPNESS_CONTROL	: $value=$api->GetSharpness();break;
			case PROP_CONTRAST_CONTROL	: $value=$api->GetContrast();break;
			case PROP_PLAY_CONTROL		: 
				if($value=$api->GetTransportInfo()){
					$value=$value['CurrentTransportState'];
					if(stripos($value,' PAUSE'))$value=RPC_PAUSE;
					elseif(stripos($value,' STOP'))$value=RPC_STOP;
					elseif(stripos($value,' PLAY'))$value=RPC_PLAY;
					else $value=RPC_STOP;
				}else $value=RPC_STOP;
				break;
			default:$value=null;
		}
		if(!is_null($value))$this->setValueByIdent(NAMES_PROPS[$Prop], $value);
		return $value;
	}
	protected function startTimer(){
		if($sec=$this->ReadPropertyInteger('UpdateInterval'))$this->SetTimerInterval('UpdateTimer',$sec); 
		if($sec=intval($this->GetBuffer('NextEventRefresh')))$this->SetTimerInterval('EventTimer', $Sec);
	}
	protected function stopTimer(){
		$this->SetTimerInterval('UpdateTimer',0);
		$this->SetTimerInterval('EventTimer', 0);
	}
	protected function readProperty(string $PropName){
		return IPS_GetProperty($this->InstanceID,$PropName);
	}
	protected function setProperty($PropName, $PropValue, $Apply=false):bool{
		if($this->ReadProperty($PropName)!=$PropValue){
			IPS_SetProperty($this->InstanceID,$PropName,$PropValue);
			if($Apply)IPS_ApplyChanges($this->InstanceID);
			return true;
		}
		return false;
	}
	protected function setValueByIdent($ValueIdOrName, $Value):bool{
		if(!is_numeric($ValueIdOrName))if(!$ValueIdOrName=@$this->GetIDForIdent($ValueIdOrName))return false;
		if(is_null($v=@GetValue($ValueIdOrName))|| $v==$Value)return false;
		return SetValue($ValueIdOrName,$Value);
	}
	protected function getValueByIdent($ValueIdOrName):bool{
		if(!is_numeric($ValueIdOrName))if(!$ValueIdOrName=@$this->GetIDForIdent($ValueIdOrName))return null;
		return @GetValue($ValueIdOrName);
		
	}
	/**
	 * @param string $ident
	 * @param int $type
	 * @param string $profile
	 * @param int $pos
	 * @return int
	 */
	protected function createVariable(string $ident,int $type, $profile='',$pos=0, $name='', $icon=''):int{
		
		if(empty($name))$name=$ident;
		if($type==0)$id=$this->RegisterVariableBoolean($ident,$name,$profile,$pos);
		elseif($type==1)$id=$this->RegisterVariableInteger($ident,$name,$profile,$pos);
		elseif($type==2)$id=$this->RegisterVariableFloat($ident,$name,$profile,$pos);
		elseif($type==3)$id=$this->RegisterVariableString($ident,$name,$profile,$pos);
		if($icon)IPS_SetIcon($id,$icon);
		return $id;
	}
	protected function createProfile_Associations ($Name, $Associations, $Icon="TV", $Color=-1, $DeleteProfile=true, $type=1, $prefix='', $sufix='', $digits=0) {
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
	
///@endcond
///@cond PRIVATE
	private function UpdateConfig($def=null,$config=null){
		if(is_null($def) || is_null($config)){
			if($api=$this->_getApi()){
				$def=$api->GetModelDef();
				$config=$api->GetConfig();
			}
		}
		$this->SetBuffer('ApiProps', $def ? $def->{DEF_PROPS}:0);
		$this->SetBuffer('ApiDef',   $def ? json_encode($def):'');
		$this->SetBuffer('ApiConfig',$config?json_encode($config):'');
	}
	
	private function ApplyGlobals(){
		$changed=false;
		$old_props=intval($this->GetBuffer('ApiProps'));
		$old_def=json_decode($this->GetBuffer("ApiDef"));
		$old_config=json_decode($this->GetBuffer("ApiConfig"));
		$config_file=$this->ReadPropertyString('ConfigFile');
		$status=102;$new_props=0;$new_def=$new_config= '';
		if($is_new_api=$config_file!=$this->GetBuffer('ConfigFile')){
			if(empty($config_file)){
				$status=201;
				if($this->ReadPropertyInteger("ActionSelector")>ACTIONS_DEBUG && $this->SetProperty("ActionSelector", ACTIONS_INFO))$changed=true;			
				if($this->SetProperty("ConfigSelector", SELECTOR_NONE))$changed=true;			
// 				$this->UpdateConfig('','');
				$this->SetBuffer("ApiProps",$new_props=0);
				$this->SetBuffer("ApiDef",$new_def='');
				$this->SetBuffer("ApiConfig",$new_config='');
				$this->SetBuffer('NextEventRefresh',0);
			}elseif(!$api=$this->_getApi($config_file)){
				$status=202;
				if($this->ReadPropertyInteger("ActionSelector")>ACTIONS_DEBUG && $this->SetProperty("ActionSelector", ACTIONS_INFO))$changed=true;			
				if($this->SetProperty("ConfigSelector", SELECTOR_NONE))$changed=true;			
// 				$this->UpdateConfig('','');
				$this->SetBuffer("ApiProps",$new_props=0);
				$this->SetBuffer("ApiDef",$new_def='');
				$this->SetBuffer("ApiConfig",$new_config='');
				$this->SetBuffer('NextEventRefresh',0);
			}else {
				$new_def=$api->GetModelDef();
				$new_config=$api->GetConfig();
				$new_props=$new_def->{DEF_PROPS};
				if($this->SetProperty("ConfigSelector", SELECTOR_CONFIG))$changed=true;			
			}
		}else{ // is_new_api
			$new_def=$old_def;
			$new_config=$old_config;
			$new_props=$old_props;
			
		}
		$myActions=json_decode($this->GetBuffer("ApiActions"),true);
		// Check Player
		if($is_new_api || !$new_props & PROP_PLAY_CONTROL || ($old_props & PROP_PLAY_CONTROL != $new_props & PROP_PLAY_CONTROL)){
			if($this->ApplyPlayer($myActions, $new_props))$changed=true;
		}
		// Check Titler
// 		if($this->ReadPropertyBoolean('ShowTitleInfo')){
  			if($this->ApplyTitler($myActions, $new_props,$is_new_api?$config_file:''))$changed=true;
// 		}else unset($myActions[ACTIONS_TITLER]);
		
		
		// Check Sound
		$prop_sum=array_sum(ALL_PROPS_SOUND);
		if($is_new_api || !$new_props & $prop_sum || $old_props & $prop_sum != $new_props & $prop_sum){
			if($this->ApplySound($myActions, $new_props))$changed=true;
		}
		
		// Check Video
		$prop_sum=array_sum(ALL_PROPS_VIDEO);
		if($is_new_api || !$new_props & $prop_sum || $old_props & $prop_sum != $new_props & $prop_sum){
			if($this->ApplyVideo($myActions, $new_props))$changed=true;
		}
		// Check Remote
		if($is_new_api ||!$new_props&PROP_REMOTE_CONTROL || $old_props & PROP_REMOTE_CONTROL != $new_props & PROP_REMOTE_CONTROL){
			if($this->ApplyRemote($myActions,$new_props, $is_new_api?$config_file:''))$changed=true;
		}
		// Check Events
		if($is_new_api ||!$new_props & PROP_EVENTS ||  $old_props & PROP_EVENTS != $new_props & PROP_EVENTS){
			if($this->_unregisterEvents($is_new_api,false))$changed=true;
			if($this->ApplyEvents($myActions,$new_props, $is_new_api?$config_file:''))$changed=true;
		}
			
		$this->SetBuffer('ConfigFile',$config_file);
		$selector=$newSelector=$this->ReadPropertyInteger("ConfigSelector");
		if($selector==SELECTOR_REMOTE  && !($new_props & PROP_REMOTE_CONTROL))$newSelector=SELECTOR_NONE;
		elseif($selector==SELECTOR_EVENTS && !($new_props & PROP_EVENTS))$newSelector=SELECTOR_NONE;
		if($selector!=$newSelector && $this->SetProperty("ConfigSelector", $newSelector))$changed=true;
// 		IPS_LogMessage(__FUNCTION__, var_export($myActions,true));			
		foreach(array_keys($myActions) as $k)$this->_deleteActions($myActions,$k,true);	
		
// 		foreach($myActions as $prop=>&$action){
// 			if($this->ReadPropertyBoolean("DeleteNoUsed"))foreach($action as $iid=>$info){
// 				if(!empty($info['id']) && $info['hidden']){
// 					$ident=IPS_GetObject($info['id'])['ObjectIdent'];
// 					$this->UnregisterVariable($ident);
// 					unset($action[$iid]);
// 				}
// 			}
// 			if(count($action)==0)unset($myActions[$prop]);
// 		}
		$this->SetBuffer('ApiActions', json_encode($myActions));
// 		$this->UpdateConfig($new_def,$new_config);
		$this->SetBuffer('ApiProps', $new_props);
		$this->SetBuffer('ApiDef', json_encode($new_def));
		$this->SetBuffer('ApiConfig', json_encode($new_config));
		if($status==102 && $new_config){
			if($new_config->{CONFIG_OPTIONS} & OPTIONS_NEED_HOST){
 				if(empty($this->ReadPropertyString('Host'))){
 					if(!empty($new_config->{CONFIG_HOST})){
 						if($this->SetProperty('Host',$new_config->{CONFIG_HOST}))$changed=true;
 					}else $status=301;
 				}
 			}
 			if($new_config->{CONFIG_OPTIONS} & OPTIONS_NEED_PORT){
 				if(empty($this->ReadPropertyString('Port'))){
					if(!empty($new_config->{CONFIG_PORT})){
 						if($this->SetProperty('Port',$new_config->{CONFIG_PORT}))$changed=true;
 					}elseif($status==102)$status=302;
 				}
 			}
 			if($new_config->{CONFIG_OPTIONS} & OPTIONS_NEED_USER_PASS){
 				if( empty($this->ReadPropertyString('User')) && empty($this->ReadPropertyString('Pass'))){
 					$ok=false;
 					if(!empty($new_config->{CONFIG_LOGIN_U})){
 						if($this->SetProperty('User',$new_config->{CONFIG_LOGIN_U}))$changed=true;
 						$ok=true;
 					}
 					if(!empty($new_config->{CONFIG_LOGIN_P})){
 						if($this->SetProperty('Pass',$new_config->{CONFIG_LOGIN_P}))$changed=true;
 						$ok=true;
 					}
 					if(!$ok && $status==102)$status=303;
 				}
 			}
 			if($status!=102 && $this->SetProperty("ConfigSelector", SELECTOR_CONFIG))$changed=true;
 		}
		$this->SetStatus($status);
		return $changed;
	}
	private function ApplyPlayer(&$myActions, int $new_props){
		$changed=false;
		if($new_props & PROP_PLAY_CONTROL){
			if(empty($myActions[ACTIONS_PLAYER][PROP_PLAY_CONTROL]['id'])){
				$profile="RPC_PLAY_$this->InstanceID";
				$this->CreateProfile_Associations($profile,[RPC_STOP=>$this->Translate('Stop'),RPC_PLAY=>$this->Translate('Play'),RPC_PAUSE=>$this->Translate('Pause'),RPC_NEXT=>$this->Translate('Next'),RPC_PREV=>$this->Translate('Prev')]);
				$myActions[ACTIONS_PLAYER][PROP_PLAY_CONTROL]['id']=$this->createVariable(NAMES_PROPS[PROP_PLAY_CONTROL], 1,$profile,1,$this->Translate(ucfirst(strtolower(NAMES_PROPS[PROP_PLAY_CONTROL]))));
				$myActions[ACTIONS_PLAYER][PROP_PLAY_CONTROL]['hidden']=false;
			}else{
				IPS_SetHidden($myActions[ACTIONS_PLAYER][PROP_PLAY_CONTROL]['id'], $myActions[ACTIONS_PLAYER][PROP_PLAY_CONTROL]['hidden']=false);
			}
			$this->MaintainAction(NAMES_PROPS[PROP_PLAY_CONTROL], true);
		}elseif(!empty($myActions[ACTIONS_PLAYER])){
			$delteIfEmpty=$this->ReadPropertyBoolean("DeleteNoUsed");
			foreach($myActions[ACTIONS_PLAYER] as $prop=>&$action){
				if($delteIfEmpty){
					$this->UnregisterVariable(NAMES_PROPS[$prop]);
					unset($myActions[ACTIONS_PLAYER]);
				}else{
					$this->MaintainAction(NAMES_PROPS[$prop], false);
					IPS_SetHidden($action['id'], $action['hidden']=true);
				}
			}
		}
		return $changed;
	}
	private function ApplySound(&$myActions,int $new_props){
		$changed=false;
		$delteIfEmpty=$this->ReadPropertyBoolean("DeleteNoUsed");
		foreach (ALL_PROPS_SOUND as $prop){
			$id=empty($myActions[ACTIONS_SOUND][$prop]['id'])?0:$myActions[ACTIONS_SOUND][$prop]['id'];
			if(($ok=$new_props & $prop) && !$id){
				switch($prop){
					case PROP_VOLUME_CONTROL : $id=$this->createVariable(NAMES_PROPS[$prop], 1,'~Intensity.100',10,$this->Translate(ucfirst(strtolower(NAMES_PROPS[$prop]))),'Intensity');break;
					case PROP_BASS_CONTROL	 : $id=$this->createVariable(NAMES_PROPS[$prop],1,'~Intensity.100',11,$this->Translate(ucfirst(strtolower(NAMES_PROPS[$prop]))),'Intensity');break;
					case PROP_TREBLE_CONTROL : $id=$this->createVariable(NAMES_PROPS[$prop],1,'~Intensity.100',12,$this->Translate(ucfirst(strtolower(NAMES_PROPS[$prop]))),'Intensity');break;
					case PROP_BALANCE_CONTROL:
						if(!IPS_VariableProfileExists('RPC_BALANCE')){
							$this->createProfile_Associations('RPC_BALANCE',null);
							IPS_SetVariableProfileValues('RPC_BALANCE',-100,100,1);
						}
						$id=$this->createVariable(NAMES_PROPS[$prop],1,'RPC_BALANCE',13,$this->Translate(ucfirst(strtolower(NAMES_PROPS[$prop]))),'Intensity');
						break;
					case PROP_LOUDNESS_CONTROL:$id=$this->createVariable(NAMES_PROPS[$prop],0,'~Switch',14,$this->Translate(ucfirst(strtolower(NAMES_PROPS[$prop]))),'Speedo');break;
					case PROP_MUTE_CONTROL   : $id=$this->createVariable(NAMES_PROPS[$prop],0,'~Switch',15,$this->Translate(ucfirst(strtolower(NAMES_PROPS[$prop]))),'Speaker');break;
				}
				$myActions[ACTIONS_SOUND][$prop]['id']=$id;
				$myActions[ACTIONS_SOUND][$prop]['hidden']=false;
				$this->MaintainAction(NAMES_PROPS[$prop],true);
			}elseif(!$ok && $id){
				if($delteIfEmpty){
					$this->UnregisterVariable(NAMES_PROPS[$prop]);
					unset($myActions[ACTIONS_SOUND][$prop]);
				}else{
					IPS_SetHidden($id, $myActions[ACTIONS_SOUND][$prop]['hidden']=true);
					$this->MaintainAction(NAMES_PROPS[$prop], false);
				}
			}elseif($id){
				IPS_SetHidden($id,$myActions[ACTIONS_SOUND][$prop]['hidden']=false);
				$this->MaintainAction(NAMES_PROPS[$prop], true);
			}
		}
		return $changed;
	}
	private function ApplyVideo(&$myActions,int $new_props){
		$changed=false;
		$delteIfEmpty=$this->ReadPropertyBoolean("DeleteNoUsed");
		foreach (ALL_PROPS_VIDEO as $prop){
			$id=empty($myActions[ACTIONS_VIDEO][$prop]['id'])?0:$myActions[ACTIONS_VIDEO][$prop]['id'];
			if(($ok=$new_props & $prop) && !$id){
				switch($prop){
					case PROP_BRIGHTNESS_CONTROL : $id=$this->createVariable(NAMES_PROPS[$prop], 1,'~Intensity.100',20,$this->Translate(ucfirst(strtolower(NAMES_PROPS[$prop]))),'Intensity');break;
					case PROP_SHARPNESS_CONTROL	 : $id=$this->createVariable(NAMES_PROPS[$prop],1,'~Intensity.100',21,$this->Translate(ucfirst(strtolower(NAMES_PROPS[$prop]))),'Intensity');break;
					case PROP_CONTRAST_CONTROL : $id=$this->createVariable(NAMES_PROPS[$prop],1,'~Intensity.100',22,$this->Translate(ucfirst(strtolower(NAMES_PROPS[$prop]))),'Intensity');break;
					case PROP_COLOR_CONTROL : $id=$this->createVariable(NAMES_PROPS[$prop],1,'~Intensity.100',23,$this->Translate(ucfirst(strtolower(NAMES_PROPS[$prop]))),'Intensity');break;
				}
				$myActions[ACTIONS_VIDEO][$prop]['id']=$id;
				$myActions[ACTIONS_VIDEO][$prop]['hidden']=false;
				$this->EnableAction(NAMES_PROPS[$prop]);
			}elseif(!$ok && $id){
				if($delteIfEmpty){
					$this->UnregisterVariable(NAMES_PROPS[$prop]);
					unset($myActions[ACTIONS_VIDEO][$prop]);
				}else{
					IPS_SetHidden($id, $myActions[ACTIONS_VIDEO][$prop]['hidden']=true);
					$this->MaintainAction(NAMES_PROPS[$prop], false);
				}
			}elseif($id){
				IPS_SetHidden($id, $myActions[ACTIONS_VIDEO][$prop]['hidden']=false);
				$this->MaintainAction(NAMES_PROPS[$prop], true);
			}
		}
		return $changed;
	}
	private function ApplyEvents(&$myActions, int $new_props, string $config_file=''){
		$changed=false;
		if($config_file){
// IPS_LogMessage(__FUNCTION__, "Load: ". $config_file);
			$this->_unregisterEvents(false);
 			$active_events=[];
 			if($api=$this->_getApi($config_file)){
 				if($e=$api->GetEventVars('',true)){
 					foreach($e as $sn=>$vars)
 						$active_events[]=(object)['SERVICE'=>$sn,'ENABLED'=>false, 'VARS'=>implode(',',$vars),'SID'=>'','NEXTUPDATE'=>0,'LIFETIME'=>0];
 					$myActions[ACTIONS_EVENTS][PROP_EVENTS]=null;			
 				}else unset($myActions[ACTIONS_EVENTS]);
			}else unset($myActions[ACTIONS_EVENTS]);
			if($this->SetProperty('EventServices', json_encode($active_events)))$changed=true;
 		}elseif($this->_updateEvents(true))$changed=true;
 		
 		
		return $changed;
	}
	private function ApplyRemote(&$myActions,int $new_props, string $config_file=''){
		$changed=false;
		if($new_props & PROP_REMOTE_CONTROL){
			if(!$api=$this->_getApi($config_file))return $changed;
// IPS_LogMessage(__FUNCTION__, "Load: ". $config_file);
			$RefKeys=($api->HasProps(PROP_REMOTE_CONTROL))? $api->GetKeyCodes():[];
// IPS_LogMessage(__FUNCTION__, "RefKeys: ". var_export($RefKeys,true));
		}else $RefKeys=[];
		
 		$keys=json_decode($this->ReadPropertyString('RemoteKeys'));
		$keys_changed=false;
		foreach($keys as $key){
			if(empty($RefKeys->{$key->KEY})) {
// 			if(!array_key_exists($key->KEY, $RefKeys)){
				if(empty($key->rowColor)){
					$key->rowColor='#FF0000';
					$keys_changed=true;
				}
				continue;
			}elseif(!empty($key->rowColor)){
				unset($key->rowColor);
				$keys_changed=true;
			}
			if(empty($key->NAME) || $key->NAME==NAMES_KEYS[$key->KEY]){
				
				if($key->NAME!= $RefKeys->{$key->KEY}->name){
					$key->NAME=$RefKeys->{$key->KEY}->name;
					$keys_changed=true;
				}
			}
			$props[$key->GROUPID][$key->KEY]=$key->NAME;
		}

		$groups=json_decode($this->ReadPropertyString('RemoteGroups'));
		$delteIfEmpty=$this->ReadPropertyBoolean("DeleteNoUsed");
		foreach($groups as $igroup=>$group){
			$vname='GROUP_'.$group->ID;
			
			if(empty($props[$group->ID])){
				if(!empty($myActions[ACTIONS_REMOTE][$vname]['id'])){
					if($delteIfEmpty){
						$this->UnregisterVariable($vname);						
 						IPS_DeleteVariableProfile("RPC_GROUP_{$group->ID}_{$this->InstanceID}");
 						unset($myActions[ACTIONS_REMOTE][$vname]);
					}else{
						$this->MaintainAction($vname, false);
						IPS_SetHidden($myActions[ACTIONS_REMOTE][$vname]['id'],$myActions[ACTIONS_REMOTE][$vname]['hidden']=true);
					}
 					
				}
			}elseif(empty($myActions[ACTIONS_REMOTE][$vname]['id'])){
				$this->CreateProfile_Associations("RPC_GROUP_{$group->ID}_{$this->InstanceID}", $props[$group->ID],$group->ICON);
				$id=$this->createVariable($vname ,1,"RPC_GROUP_{$group->ID}_{$this->InstanceID}", $igroup+40, $group->NAME);
				$myActions[ACTIONS_REMOTE][$vname]['id']=$id;
				$myActions[ACTIONS_REMOTE][$vname]['hidden']=false;
				$this->MaintainAction($vname, true);
			}else{
				IPS_SetHidden($myActions[ACTIONS_REMOTE][$vname]['id'],$myActions[ACTIONS_REMOTE][$vname]['hidden']=false);
				$this->MaintainAction($vname, true);
			}
		}
		if($keys_changed){
			if($this->setProperty('RemoteKeys', json_encode($keys)))$changed=true;
		}
		return $changed;
	}
	private function ApplyTitler(&$myActions, int $new_props, string $config_file=''){
		$changed=false;
		$delteIfEmpty=$this->ReadPropertyBoolean("DeleteNoUsed");
		if(!($new_props & PROP_PLAY_CONTROL)){
	 		$this->_hideActions($myActions,ACTIONS_TITLER);
			return $changed;
		}	
		if(!($api=$this->_getApi($config_file))){
			$this->_hideActions($myActions,ACTIONS_TITLER);
			return $changed;
		}
		if(!$api->FunctionExists('GetTransportInfo')){
			$this->_hideActions($myActions,ACTIONS_TITLER);
			return $changed;
		}
		$TitleInfoAsHtml=$this->ReadPropertyBoolean('TitleInfoAsHtml');
		if(empty($myActions[ACTIONS_TITLER]['TITLE']['id'])){
			$myActions[ACTIONS_TITLER]['TITLE']['id']=$id=$this->createVariable('TITLE', 3, $TitleInfoAsHtml?'~HTMLBox':'', 30,$this->Translate('Title'));				
			$myActions[ACTIONS_TITLER]['TITLE']['hidden']=false;
		}
		if(!$TitleInfoAsHtml){
			foreach(['ARTIST','ALBUM','CREATOR','DESCRIPTION','DURATION','RELTIME'] as $index=>$name){
				if(!empty($myActions[ACTIONS_TITLER][$name]['id']))continue;
				$myActions[ACTIONS_TITLER][$name]['id']=$this->createVariable($name, 3, '', $index+31,$this->Translate(ucfirst(strtolower($name))));
				$myActions[ACTIONS_TITLER][$name]['hidden']=false;		
			}
		}
		return $changed;		
	}

	private function _apiHasProps(int $Props):bool{
		return intval($this->GetBuffer('ApiProps')) & $Props;
	}
	private function _getApiDef(){
		return json_decode($this->GetBuffer('ApiDef'));
// 		return intval($this->GetBuffer('ApiProps')) & $Props;
	}
	private function _getLogger():IPSRpcLogger{
		if(!is_null($this->logger))return $this->logger;
		if(!$opts=$this->ReadPropertyInteger('LogOptions'))return $this->logger;
		$this->logger=new IPSRpcLogger($this, $opts);
		return $this->logger;	
	}
	private function _getApi($configFile=''){
		if(empty($configFile))$configFile=$this->GetBuffer('ConfigFile');
		if(!is_null($this->api) && $this->api->GetFilename()==$configFile)return $this->api;
// IPS_LogMessage(__FUNCTION__ . ':'.__LINE__, "Load $configFile");
		$api=null;
		$this->api = new RpcApi($configFile, $this->_getLogger());
 		if($this->api->HasError()){
 			echo "API error! For more information, see the message log";
 			return $this->api=null;
 		}
 		elseif($c=$this->api->GetConfig()){
 			if($c->{CONFIG_OPTIONS} & OPTIONS_NEED_HOST){
 				if(empty($c->{CONFIG_HOST}))$c->{CONFIG_HOST}=$this->ReadPropertyString('Host');
 			}
 			if($c->{CONFIG_OPTIONS} & OPTIONS_NEED_PORT){
 				if(empty($c->{CONFIG_PORT}))$c->{CONFIG_PORT}=$this->ReadPropertyInteger('Port');
 			}
 			if($c->{CONFIG_OPTIONS} & OPTIONS_NEED_USER_PASS){
 				if(empty($c->{CONFIG_LOGIN_U}))$c->{CONFIG_LOGIN_U}=$this->readPropertyString('User');
 				$c->{CONFIG_LOGIN_P}=$this->readPropertyString('Pass');
 			}
 		}
		return $this->api;
	}

	private function _deleteActions(&$myActions, int $actionId, bool $OnlyIfHidden=true){
		if(empty($myActions[$actionId]))return;
		foreach($myActions[$actionId] as $prop=>$action){
			if(is_array($action) && $action['id']){
				if(!$OnlyIfHidden || $action['hidden']){
					$ident=IPS_GetObject($action['id'])['ObjectIdent'];
					$this->UnregisterVariable($ident);
					unset($myActions[$actionId][$prop]);
				}
			}
		}
		if(count($myActions[$actionId])==0)unset($myActions[$actionId]);
	}
	private function _hideActions(&$myActions, int $actionId){
		$delteIfEmpty=$this->ReadPropertyBoolean("DeleteNoUsed");
		if(empty($myActions[$actionId]))return;
		foreach($myActions[$actionId] as $prop=>&$action){
			if(is_array($action) && $action['id']){
				$ident=IPS_GetObject($action['id'])['ObjectIdent'];
				if($delteIfEmpty){
					$this->UnregisterVariable($ident);
					unset($myActions[$actionId][$prop]);
				}else{
					IPS_SetHidden($actionId['id'],$action['hidden']=true);
					$this->MaintainAction($ident, false);
				}
			}
		}
		if(count($myActions[$actionId])==0)unset($myActions[$actionId]);
	}
	
	/*
	 * Title output
	 */
	private function _updateTitleInfo(){
		if(!$this->ReadPropertyBoolean('ShowTitleInfo'))return;
		if(!$api=$this->_getApi())return;
		$ok=($info=$api->GetCurrentInfo());
		if($this->ReadPropertyBoolean('TitleInfoAsHtml')){
			if(empty($info['albumArtist']))$info['albumArtist']='NA';
			if(empty($info['title']))$info['title']='NA';		
			if(empty($info['creator']))$info['creator']='NA';		
			if(empty($info['album']))$info['album']='NA';		
			if(empty($info['description']))$info['description']='';
			if(empty($info['duration']))$info['duration']='?';
			if(empty($info['relTime']))$info['relTime']='?';
			$info['artist']=$info['albumArtist'];
			unset($info['albumArtist']);
 			$template=$this->ReadPropertyString('HtmlTemplate');
 			if($content=file_get_contents(RPC_CONFIG_DIR.'/'.$template)){
				foreach($info as $key=>$value)$replacements['$'.$key]=&$info[$key];
				$content=str_ireplace(array_keys($replacements),array_values($replacements),$content);
				$this->setValueByIdent('TITLE', $content);
 			}else $this->setValueByIdent('TITLE', $content);
		}else{
			if(!empty($info['albumArtist']))$this->setValueByIdent('ARTIST', $info['albumArtist']);		
			if(!empty($info['title']))$this->setValueByIdent('TITLE', $info['title']);		
			if(!empty($info['creator']))$this->setValueByIdent('CREATOR', $info['creator']);		
			if(!empty($info['album']))$this->setValueByIdent('ALBUM', $info['album']);		
			if(!empty($info['description']))$this->setValueByIdent('DESCRIPTION', $info['description']);		
			if(!empty($info['duration']))$this->setValueByIdent('DURATION', $info['duration']);
			if(!empty($info['relTime']))$this->setValueByIdent('RELTIME', $info['relTime']);
		}
		return $ok;
	}
	
	
	/*
	 * Remote Commands and Macro Support
	 */
	private function _createSendKeyTimer($KeyToSend, $Seconds){
		$name="Timer $Seconds sec. for Key $KeyToSend";
		if($id=@IPS_GetObjectIDByName($name,$KeyToSend) )return true;
		$id=IPS_CreateEvent(1);
		IPS_SetHidden($id,true);
		IPS_SetName($id,"Timer $Seconds sec. for Key $KeyToSend");
		IPS_SetEventCyclic ($id,0,0,0,0, 1, $Seconds );
		IPS_SetEventScript($id, "if(RPC_SendKey($this->InstanceID,$KeyToSend))IPS_DeleteEvent($id);");
		IPS_SetEventLimit($id,1);
		IPS_SetParent($id,$this->InstanceID);
		IPS_SetEventActive($id,true);
		return true;
	}
	private function _handleMacroTable (int $command, $data=null){
		static $check_recrusive_error = [];
		static $updateProps = 0;
		if(!$api=$this->_getApi()){
			return false;
		}
		$macros=json_decode($this->ReadPropertyString('RemoteMacros'));
		$macro_changed=false;$ok=true;
		if($command!=MACRO_EXECUTE)$apikeys=$api->GetKeyCodes();
		foreach($macros as $macro){
			switch ($command) {
				case MACRO_CHECK :
					if($macro->UUID==0){$macro->UUID=mt_rand()+1; $macro_changed=true;	}
					if($macro->MACRO==$macro->UUID){$macro->MACRO=0; $macro_changed=true;} // Refrence to self not allowed
					if($macro->NAME==self::new_macro_name){ $macro->NAME='Macro'.mt_rand(100,200);$macro_changed=true;}
					//break;
					$error=null;$hasKeys=false;
					for($key=0;$key<self::max_macro_keys;$key++){
						if(($keycode=$macro->{"KEY$key"})){
							$found=(bool)$apikeys;
							if($found)foreach($apikeys as $code=>$def)if($found=$code==$keycode)break;
							if(!$found)$error[]=$key; else $hasKeys=true;
						}
					}
					if($error){ 
						$macro->ERROR=implode(',', $error);
 	 					$macro->rowColor='#C0C0FF'; 
						$macro_changed=true;
					}elseif(!$hasKeys && empty($macro->ERROR)){
						$macro->ERROR="Not found";
						$macro->rowColor='#FFC0C0';
						$macro_changed=true;
					}
					elseif($hasKeys && !empty($macro->ERROR)){
						unset($macro->ERROR);
						unset($macro->rowColor);
						$macro_changed=true;
					}	
					break;
				case MACRO_EXECUTE:
					if(is_null($data))return $this->error('No macroname or id given');
					if(is_numeric($data)){if ($macro->UUID!=$data)break;}
					elseif(strcasecmp($macro->NAME, $data)!=0) break;
					if(!empty($macro->ERROR)){
						IPS_LogMessage(__CLASS__, sprintf('Macro % has has error in Key(s): %s',$macro->NAME,$macro->ERROR));
						return null;
					}
// 					if(count($check_recrusive_error)==0)$updateProps=0;
					if(!empty($check_recrusive_error[$macro->UUID])){
						IPS_LogMessage(__CLASS__,'Macro '.$macro->NAME.' ! No Recrusions allowed');
						return null;
					}
					$this->SendDebug(__FUNCTION__,'Execute macro : '.$macro->NAME,0);
					for($key=0; $key < self::max_macro_keys; $key++ ){
						if($keycode=$macro->{"KEY$key"}){
							if(!$ok=$api->SendKeyCode(['KeyCode'=>$keycode])){
								break;
							}
							if($macro->{"DELAY$key"})usleep($macro->{"DELAY$key"}*1000);
						}
					}
					$this->SendDebug(__FUNCTION__,'Send Macro :'.($ok?'true':'false'),0);
					if(!$ok)break;
					$check_recrusive_error[$macro->UUID]=true;
					$senddelay=$macro->DELAY?$macro->DELAY:150;
					#TODO Generate Timer for long Macros 
					if($next=$macro->MACRO){
						usleep($senddelay*1000);
						$ok=$this->_handleMacroTable(MACRO_EXECUTE,$next);
					}
					unset($check_recrusive_error[$macro->UUID]);
					break;
			}
			if(!$ok)break;
		}
		
		if($macro_changed)return $this->setProperty('RemoteMacros',json_encode($macros));
		return $command==MACRO_EXECUTE?$ok:false;
	}	
	
	/*
	 * Event handling support
	 */
	protected function ProcessHookData() {
		require_once RPC_LIB_DIR . '/rpcparseevents.inc';
		if(!$this->ReadPropertyBoolean('EnableEvent')){
			return RpcParseSendError($this->SendDebug(__FUNCTION__, "INFO => Events are disabeld.. sending Error back to disable event on remote device", 0));
		}
		if(!$output=RpcParseEventFromInput()) return;
		if(is_string($output))return $this->SendDebug(__FUNCTION__,$output,0);

// 		$myActions=json_decode($this->GetBuffer("ApiActions"),true);
		foreach($output as $instanceID=>&$data){
			if($instanceID != $this->ReadPropertyInteger('InstanceID'))continue;
			foreach ($data as $ident=>$value){
				if(($prop=in_array($ident, NAMES_PROPS))!==false){
					$this->setValue(NAMES_PROPS[$prop], $Value);
					unset($data[$ident]);
				}
			}
			if(count($data)==0)unset($output[$instanceID]);
		}
		if(count($output)>0 && $script_id=$this->ReadPropertyInteger('EventScript')){
			IPS_RunScriptEx($script_id, ['VALUES'=>$output]);
		}
	}
	private function _unregisterEvents(bool $Apply=true){
		$activeEvents=json_decode($this->ReadPropertyString('EventServices'));
		$now = time();
		foreach($activeEvents as $eid=>$event){
			if(!empty($event->SID) && $event->NEXTUPDATE < $now && $api=$this->_getApi() && !$api->UnRegisterEvent($event->SID,$event->SERVICE))
				IPS_LogMessage(__CLASS__,"ERROR: Unregister Event $event->SERVICE : $event->SID");
			$event->SID='';
			$event->NEXTUPDATE=0;
			$event->LIFETIME=0;
		}
		return $Apply?$this->SetProperty('EventServices', json_encode($activeEvents), true):false; 		
	}
	private function _updateEvents($events = null){
		if($events && $events!==true) $active_events=$events; else $active_events=json_decode($this->ReadPropertyString('EventServices'));
		$this->stopTimer();
		$now=time(); $changed=false;
		$events_enabled=$this->ReadPropertyBoolean('EnableEvent');
		$CallbackUrl='';
		$next_event_refreh = 0;
		foreach($active_events as $eid=>$event){
			if(!$event->ENABLED || !$events_enabled){
				if($event->SID && $event->NEXTUPDATE < $now){
					if(!$api=$this->api && !$api=$this->_getApi())break;
					$api->UnregisterEvent($event->SID, $event->Service);
				}
				$event->SID='';
				$event->NETXUPDATE=0;
				$event->LIFETIME=0;
				$changed=true;
				continue;
			}
			if($event->ENABLED && $events_enabled){
				if(!$api=$this->api && !$api=$this->_getApi())break;
				if($event->SID && $event->NEXTUPDATE < $now){
					$r=$api->RefreshEvent($event->SID, $event->Service);
				}else{
					if(empty($CallbackUrl) && !$CallbackUrl=$this->_registerEventHook(true))break;
					$r=$api->RegisterEvent($event->SERVICE, $CallbackUrl);
				}
				if($r){
					$event->SID = $r[EVENT_SID];
					$event->LIFETIME=$r[EVENT_TIMEOUT];
					$event->NEXTUPDATE = $r[EVENT_TIMEOUT] + $now;
					$changed=true;
					$next_event_refreh=$next_event_refreh?min($next_event_refreh,$event->LIFETIME):$event->LIFETIME;
				}
			}
		}
		$this->SetBuffer('NextEventRefresh',$next_event_refreh);
		if($changed)$changed=$this->SetProperty('EventServices', json_encode($active_events),!empty($events));
		if(!$events)$this->startTimer();	
		return $changed;
	}
	private function _registerEventHook(bool $Create) {
		$ids = IPS_GetInstanceListByModuleID("{015A6EB8-D6E5-4B93-B496-0D3F77AE9FE1}");
		if(!$myIp = NET::local_ip()){
			IPS_LogMessage(__CLASS__,'Cant get local IP to Register Events');
			return null;
		}
		if(count($ids) > 0) {
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
			if($Create){
				$myPort = $this->ReadPropertyInteger('EventPort');
				return "http://$myIp:$myPort".$hookname;
			}else return null;
		}else IPS_LogMessage(get_class($this),'ERROR Instance WebHook not found');
		return null;
	}
	
///@endcond	
}
/** @brief Transfer Messages/Errors to IP-Symcon Output
 * @author Xaver Bauer
 * 
 */
class IPSRpcLogger extends RpcLogger {
	private $ipsModule;
// private $oMessage=null;
	/**
	 *
	 * @param int $LogOptions
	 * @param string $LogFileName
	 * @param RpcMessage $MessageObject
	 *
	 */
	public function __construct(ProRpc $IpsModule, $LogOptions, $LogFileName = '') {
		parent::__construct ( $LogOptions, $LogFileName, new RpcMessage());
		$this->SetParent($IpsModule);
	}
	function __wakeup(){
// 		echo __CLASS__ . " => WAKEUP\n";
		parent::__wakeup();
		$this->SetParent(null);
	}
	public function SetParent(ProRpc $IpsModule=null){
		$this->ipsModule=$IpsModule;
// 		if($this->oMessage)
// 			$this->oMessage->SetParent($IpsModule);
// 		elseif($ipsModule)
// 			$this->SetMessage(new IPSRpcMessage($IpsModule));
	}
	/**
	 * {@inheritDoc}
	 * @see RpcLogger::doOutput()
	 */
	protected function doOutput($Message, $Class, $AsError) {
		if($AsError){
			IPS_LogMessage($Class,$Message);
		}elseif($this->ipsModule){
			$this->ipsModule->RequestAction('DEBUG_API',json_encode([$Class,trim(str_replace("DEBUG: $Class",'',$Message))]));
		}else parent::doOutput($Message, $Class, $AsError);
	}

	
	
	

}

///@cond INTERNAL
class IPSRemoteKeys {
	const max_groups = 11;
	static $numPads=[KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9];
	static $movePads=[KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT];
	static $menuPads=[KEY_MENU,KEY_HELP,KEY_INFO,KEY_OPTIONS,KEY_OK,KEY_ESC,KEY_RETURN];
	static $playPads=[KEY_PLAY,KEY_PAUSE,KEY_STOP,KEY_PREV,KEY_NEXT,KEY_FF,KEY_FR,KEY_RECORD,KEY_SHUFFLE,KEY_REPEAT];
	static $sourcePads=[KEY_SOURCE,KEY_SOURCE0,KEY_SOURCE1,KEY_SOURCE2,KEY_SOURCE3,KEY_SOURCE4,KEY_SRCUP,KEY_SRCDOWN];
	static $controlPads=[KEY_VOLUP,KEY_VOLDOWN,KEY_MUTE,KEY_CHUP,KEY_CHDOWN,KEY_POWER];
	static $colorPads=[KEY_RED,KEY_GREEN,KEY_YELLOW,KEY_BLUE];
	
	public static function defaultGroups(){
		return [['NAME'=>'Numpad','ID'=>1,'ICON'=>'Keyboard'],['NAME'=>'Cursor','ID'=>2,'ICON'=>'Cross'],['NAME'=>'Menu','ID'=>3,'ICON'=>'Database'],['NAME'=>'Player','ID'=>4,'ICON'=>'Melody'],['NAME'=>'Source','ID'=>5,'ICON'=>'XBMC'],['NAME'=>'Control','ID'=>6,'ICON'=>'Wave'],['NAME'=>'Buttons','ID'=>7,'ICON'=>''],['NAME'=>'Custom1','ID'=>8,'ICON'=>''],['NAME'=>'Custom2','ID'=>9,'ICON'=>'']];	
	}
	public static function getKeyGroupID($key){
		if(in_array($key,static::$numPads))return 1;
		if(in_array($key,static::$movePads))return 2;
		if(in_array($key,static::$menuPads))return 3;
		if(in_array($key,static::$playPads))return 4;
		if(in_array($key,static::$sourcePads))return 5;
		if(in_array($key,static::$controlPads))return 6;
		if(in_array($key,static::$colorPads))return 7; 
		return 8;
	}
	public static function getKeyMap($key , array $map=null){
		if(empty(NAMES_KEYS[$key]))return null;
		if(empty($map))return ['NAME'=>NAMES_KEYS[$key],'KEY'=>$key,'GROUPID'=>static::getKeyGroupID($key)];
		foreach($map as $m)if($m['KEY']==$key)return $m;
		return null;
	}
	public static function defaultKeyMap(array $RefKeys = null){
		foreach(ALL_KEYS as $key){
			if($RefKeys && !in_array($key, $RefKeys))continue;
			$map[]=['NAME'=>NAMES_KEYS[$key],'KEY'=>$key,'GROUPID'=>static::getKeyGroupID($key)];
		}
		return $map;
	}


	
	
}
///@endcond	

 ?>