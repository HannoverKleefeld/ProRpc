<?php



/**
 *
 * @author Xaver Bauer
 *        
 */
class IPSRpcGateway extends IPSBaseModule  {
	protected $api=null,$logger = null;
	function Create(){
		parent::Create();
		$this->registerPropertyString('Host','');
		$this->registerPropertyInteger('Port',0);
		$this->registerPropertyString('User','');
		$this->registerPropertyString('Pass','');
// 		$this->registerPropertyBoolean('EnableEvents',false);
		$this->registerPropertyString('ConfigFile','');
		$this->registerPropertyString('ApiDef','');
		$this->registerPropertyInteger('ActionOptions',0);
		$this->registerPropertyInteger('LogOptions',DEBUG_ERRORS);
//  		$this->registerPropertyString('Events','[]');
		$this->registerPropertyInteger('UpdateInterval',0);
		$this->registerPropertyInteger('EventsInterval',0);
		$this->registerTimer('UpdateTimer',0,"IPS_RequestAction($this->InstanceID,'PROCESS_UPDATE',0);");
//  		$this->registerTimer('EventsTimer',0,"IPS_RequestAction($this->InstanceID,'PROCESS_EVENTS',0);");
 		$this->setBuffer('ConfigFile',$this->ReadPropertyString('ConfigFile'));
	}
	function ApplyChanges(){
		parent::ApplyChanges();
		$this->_loadConfigFile($this->ReadPropertyString('ConfigFile'));
	}
	function RequestAction($Ident, $Value){
		if($Ident=='PROCESS_UPDATE'){
			$this->sendDataToClients('UpdateStatus', true);
			return true;
		}else false;
	}
	function ForwardData($JSONString){
		$this->SendDebug(__FUNCTION__,$JSONString,0);
// 		if(!$this->ReadPropertyBoolean('Open'))return $this->_returnError('Splitter instance closed',1001);
 		if(!$data = json_decode($JSONString,true))return $this->_returnError('Invalid Forward Data' , 1004);
// 		utf8::decode_array($data);
 		if(empty($data['ObjectID']))return $this->_returnError('Invalid or no ObjectID received', 1005);
 		if(empty($data['Buffer']['Function']))return $this->_returnError('No function given', 1002);
 		$function=$data['Buffer']['Function'];
 		$arguments=empty($data['Buffer']['Arguments'])?[]:$data['Buffer']['Arguments'];
 		if($this->_handleInternalFunction($function, $arguments,$result))	return $result;
 		if(!$api=$this->_getApi())return $this->_returnError('API Creation error', 1003);
 		$result = is_numeric(key($arguments))? call_user_func_array([$api,$function], $arguments) : $api->$function($arguments);
 		if($api->HasError())return $this->_returnError($this->logger->LastErrorMessage(), $this->logger->LastErrorCode());
 		return $this->_returnResult($result);
	}	
	function GetConfigurationForm(){
		$options=[];
		if($files=scandir(RPC_CONFIG_DIR))foreach ($files as $file){
			if($file[0]=='.')continue;
			$file=pathinfo($file);
			if(!empty($file['extension'])&&$file['extension']=='json' && stripos($file['basename'], 'desc')===false )$options[]=["label"=>$file['filename'], "value"=> $file['basename']];
		}	
// 		$form["elements"][]=["type"=> "CheckBox", "name"=>"Open", "caption"=> "Open Connection"];

		$form["elements"][]=["type"=> "Select", "name"=>"ConfigFile", "caption"=> "RPC Config","options"=> $options];
		$form["elements"][]=["type"=>"NumberSpinner","name"=>"UpdateInterval", "caption"=> "Update Interval Seconds" ];
		if($apiConfig=json_decode($this->GetBuffer('ApiConfig'),true)){
			if($apiConfig[CONFIG_OPTIONS] & OPTIONS_NEED_HOST){
				$form["elements"][]=["type"=>"ValidationTextBox","name"=>"Host", "caption"=> "Host" ];
			}
			if($apiConfig[CONFIG_OPTIONS] & OPTIONS_NEED_PORT){
				$form["elements"][]=["type"=>"NumberSpinner","name"=>"Port", "caption"=> "Port" ];
				
			}
			if($apiConfig[CONFIG_OPTIONS] & OPTIONS_NEED_USER_PASS){
				$form["elements"][]=["type"=>"ValidationTextBox","name"=>"User", "caption"=> "User" ];
				$form["elements"][]=["type"=>"PasswordTextBox","name"=>"Pass", "caption"=> "Password" ];
			}
		}
		if($apidef=json_decode($this->ReadPropertyString('ApiDef'),true)){
			$form["elements"][]=["type"=> "Label", "label"=> sprintf('Model: %s - %s API-Version %s',$apidef[DEF_MANU],$apidef[DEF_MODEL],$apidef[DEF_VERSION])];
			if(!$apidef[DEF_PROPS])$t=['None']; else foreach(ALL_PROPS as $prop)if($apidef[DEF_PROPS] & $prop)$t[]=NAMES_PROPS[$prop];
			$form["elements"][]=["type"=> "Label", "label"=> 'Props: '.implode(', ',$t)];
		}
		$dops=$this->readPropertyInteger('LogOptions');
		if(!$dops)$l=['None']; else{
			foreach(ALL_DEBUG as $opt)if($dops & $opt)$l[]=NAMES_DEBUG[$opt];
		}
 		$form["elements"][]=["type"=> "Label", "label"=> "Logging: ".implode(', ',$l)];
 		
		$form["elements"][]=["type"=> "Select", "name"=>"ActionOptions", "caption"=> "Action Options","options"=> [
				['label'=>'-> Nothing <-','value'=>0],
				['label'=>'Log Settings','value'=>1],
				['label'=>'Create Device','value'=>2],
		]];
		switch($this->readPropertyInteger('ActionOptions')){
			case 1 :
		 		$form["actions"][]=["type"=> "Label", "label"=> "Settings for logging "];
				$form["actions"][]=["type"=> "CheckBox", "name"=>"info", 	"caption"=> "Debug informations"];
		 		$form["actions"][]=["type"=> "CheckBox", "name"=>"call", 	"caption"=> "Debug function calls"];
		 		$form["actions"][]=["type"=> "CheckBox", "name"=>"build", 	"caption"=> "Debug build details"];
		 		$form["actions"][]=["type"=> "CheckBox", "name"=>"detect", 	"caption"=> "Debug device detections"];
		 		$form["actions"][]=["type"=> "CheckBox", "name"=>"detail", 	"caption"=> "Debug details"];
		 		$form["actions"][]=["type"=> "CheckBox", "name"=>"errors", 	"caption"=> "Log errors"];
		 		$form["actions"][]=["type"=> "Button", "label"=> "Set Log Options", "onClick"=>'$o=$info?1:0;if($call)$o=$o|2;if($build)$o=$o|4; if($detect)$o=$o|8;if($detail)$o=$o|'.DEBUG_DETAIL.'; if($errors)$o=$o|'.DEBUG_ERRORS.'; RGATE_SetLogOptions($id,$o);' ];
				break;
			case 2 :;
		 		$form["actions"][]=["type"=> "Label", "label"=> "Create a new device config "];
				$form["actions"][]=["type"=>"ValidationTextBox","name"=>"url", "caption"=> "Url to import" ];
		 		$form["actions"][]=["type"=> "Button", "label"=> "Import", "onClick"=>'echo "import $url";' ];
				break;
		}
		
		$form["status"]=[
				["code"=>102, "icon"=>"active",  "caption"=> "Instance ready"],
				["code"=>201, "icon"=>"error",   "caption"=> "Host/Url needed"],
				["code"=>202, "icon"=>"error",   "caption"=> "Port needed"],
				["code"=>203, "icon"=>"error",   "caption"=> "Loaded api require User and/or Password to make calls"]
		];
		
		return json_encode($form);
	}	
	public function SetLogOptions(int $Options){
		$this->setProperty('LogOptions', $Options,true);
	}

	
	protected function sendDataToClients(string $Command, $Data){
		$data['Buffer']=['Command'=>$Command,'Data'=>$Data];
		$data['DataID']='{19650302-CONT-MAJA-PRPC-20180101XLIB}';
		$data=json_encode($data);
		$this->SendDebug(__FUNCTION__,"Send To Childrens: $data",0);
		$this->SendDataToChildren($data);
	}
	protected function _handleInternalFunction($function, $arguments,&$result ){
		if($function=='RequestProps'){
			$props=($def = json_decode($this->ReadPropertyString('ApiDef'),true))?$def[DEF_PROPS]:0;			
			$result=$this->_returnResult($props);
			return true;
		}	
		if($function=='RequestInfo'){
			$result=$this->_returnResult(json_decode($this->ReadPropertyString('ApiDef'),true));
			return true;
		}	
		return false;
	}
	protected function _applyConfig($Props=null, $Options=null){
		if(is_null($Props))$Props=($p=json_decode($this->readPropertyString('ApiDef') ,true))?$p[DEF_PROPS]:0;		
		if(is_null($Options))$Options=($p=json_decode($this->GetBuffer('ApiConfig') ,true))?$p[CONFIG_OPTIONS]:0;		
// 		if($this->readPropertyBoolean('EnableEvents')){
// 			if($Props&PROP_EVENTS)
// 				$this->_registerEvents();
// 		}else {
// 			$this->_unregisterEvents();
// 		}
		if($Options & OPTIONS_NEED_HOST && empty($this->ReadPropertyString('Host'))){
			$this->SetStatus(201);			
		}elseif($Options & OPTIONS_NEED_PORT && empty($this->ReadPropertyInteger('Port'))){
			$this->SetStatus(202);			
		}elseif($Options & OPTIONS_NEED_USER_PASS  && empty($this->ReadPropertyString('User')) && empty($this->ReadPropertyString('Pass'))){
			$this->SetStatus(203);			
		}else $this->SetStatus(102);	
		return true;
	}
	protected function _loadConfigFile($Filename){
		static $spacer=' - ';
		if($isImport=preg_match('/\.json/i',$Filename))$Filename=RPC_CONFIG_DIR."/$Filename";
		if($Filename==$this->GetBuffer('ConfigFile')){
			return $this->_applyConfig();
		}
		$saveName=$this->GetBuffer('ConfigFile');
		$info=IPS_GetName($this->InstanceID);
		if(substr($info,0,6)=='ProRpc'){
			if(($pos=strpos($info,$spacer))!==false)$info=substr($info,0,$pos);
		}else $info=null; 
// 		if($this->readPropertyBoolean('EnableEvents'))$this->_unregisterEvents();		
		$this->SetBuffer('ConfigFile',$Filename);
		$apiProps=$apiOpts=0;
		if($api=$this->_getApi(true)){
			$def=$api->GetModelDef();
			$apiProps=$def->{DEF_PROPS};
			$apiConfig=$api->GetConfig();
			$this->SetBuffer('ApiConfig',json_encode($apiConfig));
			if($info)$info.=$spacer.$def->{DEF_MANU}.' ['.$def->{DEF_MODEL}.']';
			$this->setProperty('ApiDef', json_encode($def),true);
// 			$this->_updateEvents();
			if($api->DeviceImported()){
				$this->SetBuffer('ConfigFile',RPC_CONFIG_DIR.'/'.$api->GetFilename());
				$this->setProperty('ConfigFile', $api->GetFilename(),true);
			}
		}elseif($isImport){
			$this->SetBuffer('ConfigFile',$saveName);
			$this->SetBuffer('ApiConfig','');
			return false;
		}else $this->SetBuffer('ApiConfig','');
		if($info)IPS_SetName($this->InstanceID,$info);			
		$this->sendDataToClients(API_PROPS_IDENT, $apiProps);	
		return (bool)$api && $this->_applyConfig($apiProps,$apiOpts);
	}
	protected function & _getLogger(){
		if(!is_null($this->logger))return $this->logger;
		if(!$opts=$this->ReadPropertyInteger('LogOptions'))return $this->logger;
		$this->logger=new IPSRpcLogger($this, $opts);
		return $this->logger;	
	}
	protected function & _getApi($Force=false){
		if(!is_null($this->api) && !$Force)return $this->api;
		$this->api=new RPC($this->GetBuffer('ConfigFile'), $this->_getLogger());
 		if($this->api->HasError()){
 			$this->api=null;
 		}elseif($c=$this->api->GetConfig()){
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
	
	
	protected function _startTimer(){
		if($sec=$this->ReadPropertyInteger('UpdateInterval'))$this->SetTimerInterval('UpdateTimer',$sec); 
	}
	protected function _stopTimer(){
		$this->SetTimerInterval('UpdateTimer',0); 
	}
	protected function _returnError($Message,$ErrorCode){
		return json_encode(['Error'=>['message'=>$Message,'code'=>$ErrorCode]]);
	}
	protected function _returnResult($Result){
		return json_encode(['Result'=>$Result]);
	}
	
	
}

