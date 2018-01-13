<?php
require_once(IPS_GetKernelDir().'\modules\ProRpc\libs\loader.php');

class RPCRemoteControl extends RPCModule {
	const new_macro_name = "NewMacro";
	const max_macro_keys = 10;
	static $profiles = [
			'NUMBERS'=>['Number','Eyes',[KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9]],
			'KEYS'	=> ['Keys','Move',[KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT,KEY_OK,KEY_ESC,KEY_RETURN,KEY_CHUP,KEY_CHDOWN]],
			'MENUS'	=> ['Menu','Database',[KEY_MENU,KEY_HELP,KEY_INFO,KEY_OPTIONS]],
			'BUTTONS'=>['Buttons','Flower',[KEY_RED,KEY_GREEN,KEY_YELLOW,KEY_BLUE]],
			'CONTROL'=>['Control','Music',[KEY_SHUFFLE,KEY_REPEAT,KEY_PLAY,KEY_STOP,KEY_PAUSE,KEY_NEXT,KEY_PREV,KEY_FF,KEY_FR,KEY_RECORD]],
			'SOURCE' =>['Source','TV',[KEY_SOURCE,KEY_SOURCE0,KEY_SOURCE1,KEY_SOURCE2,KEY_SOURCE3,KEY_SOURCE4,KEY_SRCUP,KEY_SRCDOWN]],
			'VOLUME' =>['Volume','',[KEY_MUTE,KEY_VOLDOWN,KEY_VOLUP]]
	];
	
	function Create(){
		parent::Create();
		$this->registerPropertyString('KEYCODES','[]'); 
		$this->registerPropertyString('MACROS','[]');
	}
	function Destroy(){
		parent::Destroy();
		foreach(self::$profiles as $name=>$t)@IPS_DeleteVariableProfile($this->_makeProfileName($name));
	}
	function ApplyChanges(){
		parent::ApplyChanges();
		$this->_handleMacroTable('update');
	}
	function GetConfigurationForm(){
		$form=json_decode(parent::GetConfigurationForm(),true);
		$mycodes=json_decode($this->ReadPropertyString('KEYCODES'),true);
		foreach($mycodes as &$code)	$code['KEY']=KEYCODE_NAMES[$code['CODE']][0];
// var_export(array_values($mycodes));
		
		
		$form["elements"][]=["type"=>"List","name"=>"KEYCODES","caption"=>"Avaible KeyCodes","rowCount"=>5,"add"=>false,"delete"=>false,"columns"=> [
				["label"=>"Constant","name"=>"KEY","width"=>"100","save"=>false ],
				["label"=>"KeyCode", "name"=>"CODE","width"=>"60px"],
				["label"=>"Name","name"=>"NAME","width"=>"auto", "edit"=>["type"=>"ValidationTextBox"]]
			],"values"=>$mycodes	
		];
		if(count($mycodes)>0){
			$macros=json_decode($this->ReadPropertyString('MACROS'),true);
			$optmacros[]=["label"=>"<-None->", "value"=> '0'];
			foreach($macros as &$macro){
				$optmacros[]=["label"=>$macro['NAME'], "value"=> $macro['UUID']];
				if(!empty($macro['ERROR']))$macro['rowColor']=($macro['ERROR']=='NK') ? '#C0C0FF' : '#FFC0C0'; 
			}
	
			$options[]=["label"=>"<-Select->", "value"=> 0];
			foreach($mycodes as $code)$options[]=["label"=>$code['NAME'], "value"=> $code['CODE']];
			sort($options);
			$columns=[
				["label"=>"",		"name"=>"UUID","width"=>"0px","add"=>"0", "visible"=>false, "save"=>true],
				["label"=>"",		"name"=>"ERROR","width"=>"0px","add"=>"", "visible"=>false, "save"=>true],
				["label"=>"Name",	"name"=>"NAME","width"=>"80px","add"=>self::new_macro_name,"edit"=>["type"=>"ValidationTextBox"]],
				["label"=>"Delay", 	"name"=>"DELAY","width"=>"50px","add"=>"250000","edit"=>["type"=>"NumberSpinner", "caption"=> "Milli Seconds"]],
			];
			for($key=0;$key<self::max_macro_keys;$key++)$columns[]=["label"=>"KeyCode $key","name"=>"KEY$key","width"=>"70px", "add"=>"0", "edit"=>["type"=>"Select","caption"=>"Select KeyCode $key", "options"=> $options]];
			$columns[]=["label"=>"Next Macro","name"=>"MACRO","width"=>"80px", "add"=>"0", "edit"=>["type"=>"Select","options"=> $optmacros]];				
			$form["elements"][]=["type"=>"List","name"=>"MACROS","caption"=>"KeyCode Macros","rowCount"=>5,"add"=>true,"delete"=>true,"columns"=> $columns,'values'=>$macros];
			
			
			$form["actions"][]=["type"=> "Select", "name"=>"KeyCode", "caption"=> "Select KeyCode","options"=> $options];
			$form["actions"][]=["type"=> "Button", "label"=> "Send KeyCode", "onClick"=>"if(\$KeyCode)REMOTE_SendKey(\$id, \$KeyCode);else echo 'please select a KeyCode first!';"];
			if(count($optmacros)>0){
				$optmacros[0]["label"]="<-Select->";
				$form["actions"][]=["type"=> "Select", "name"=>"MacroName", "caption"=> "Select Macro","options"=> $optmacros];
				$form["actions"][]=["type"=> "Button", "label"=> "Send Macro", "onClick"=>"if(\$MacroName)REMOTE_SendMacro(\$id, \$MacroName);else echo 'please select a macro first!';"];
			}
		}		
	
		return json_encode($form);
	}
	function RequestAction ( $Ident, $Value ){
		if($v=parent::RequestAction($Ident, $Value))return $v;
		
		if($this->getStatus()!=102)return false;
		switch($Ident){
			case 'SENDKEY' 	: $this->SendKey($Value);break;
			case 'SENDMACRO': $this->SendMacro($Value);break;
			default	:  if(!empty(self::$profiles[$Ident]))$this->SendKey($Value);else $this->error('Invalid request action %s !  value: %s',$Ident,$Value);
 		}
	}
	public function SendKey(int $KeyCode){
		if($ok=$this->forwardRequest('SendKeyCodes', [[$KeyCode],''])){
			if($updateProps=$this->_findKeyCodeProp($KeyCode))
				sleep(1);
				$ok=$this->forwardRequest('ClientDataChanged', [$updateProps]);
		}		
		return $ok;
	}
	public function SendMacro(string $MacroName){
		return $this->_handleMacroTable('execute',$MacroName);
	}
	protected function getPropDef($prop){
		return null;
	}
	protected function getProps(){
 		return [PROP_REMOTE];
 	}
 	protected function onInterfaceChanged(bool $connected){
		parent::onInterfaceChanged($connected);
		if($connected && $this->getStatus()==102){
			if(!$codes=$this->forwardRequest('GetKeyCodes', []))$codes=[];
			$this->_updateKeyCodeTable($codes);
			$this->_updateVariables();
		}
	}
	protected function aboutModule(){
		return 'RpcPro Remote Command';
	}
	
	private function _handleMacroTable ($command, $data=null){
		static $check_recrusive_error = [];
		static $updateProps = 0;
		$mycodes=json_decode($this->ReadPropertyString('KEYCODES'),true);
		$macros=json_decode($this->ReadPropertyString('MACROS'),true);
		$changed=false;
		foreach($macros as &$macro){
			switch ($command) {
				case 'update' :
					if($macro['UUID']==0){	$macro['UUID']=$this->_getUID(); $changed=true;	}
					if($macro['MACRO']==$macro['UUID']){ $macro['MACRO']=0; 	$changed=true;} // Refrence to self not allowed
					if($macro['NAME']==self::new_macro_name){ $macro['NAME']='Macro'.mt_rand(100,200);$changed=true;}
					//break;
				case 'check' :
					$error=null;$hasKeys=false;
					for($key=0;$key<self::max_macro_keys;$key++){
						if(($keycode=$macro["KEY$key"])){
							$found=false;
							foreach($mycodes as $code)if($code['CODE']==$keycode){$found=true;break;};
							if(!$found)$error[]=$key; else $hasKeys=true;
						}
					}
					if($error){ $macro['ERROR']=implode(',', $error);$changed=true;}
					elseif(!$hasKeys && empty($macro['ERROR'])){$macro['ERROR']="NK";$changed=true;}
					elseif($hasKeys && !empty($macro['ERROR'])){unset($macro['ERROR']);$changed=true;}	
					break;
				case 'execute':

					if(is_null($data))return $this->error('No macroname or id given');
					if(is_numeric($data)){if ($macro['UUID']!=$data)break;}
					elseif(strcasecmp($macro['NAME'], $data)!=0) break;
					if(!empty($macro['ERROR'])) return $this->error('Macro % has has error in Key(s): %s',$macro['NAME'],$macro['ERROR']);
					
					if(count($check_recrusive_error)==0)$updateProps=0;
					if(!empty($check_recrusive_error[$macro['UUID']])){
						return $this->error('Macro %s ! No Recrusions allowed',$macro['NAME']);
					}
					$this->debug('Execute macro %s',$macro['NAME']);
					for($key=0;$key<self::max_macro_keys;$key++){
						if($keycode=$macro["KEY$key"]){
							$send[]= empty($macro["DELAY$key"])?$keycode:['code'=>$keycode,'delay'=> intval($macro["DELAY$key"])];
							// Building Updateprops.. sending to Switch to refresh clients with changed Props   
							$updateProps = $updateProps | $this->_findKeyCodeProp($keycode);
						}
					}
					$senddelay=$macro["DELAY"]?100000:$macro["DELAY"];
					
					$check_recrusive_error[$macro['UUID']]=true;
					if( ($ok=$this->forwardRequest('SendKeyCodes', [$send,'',intval($senddelay)])) !==false ){
						if($next=$macro["MACRO"]){
							usleep($senddelay);
							$ok=$this->_handleMacroTable('execute',$next, $updateProps);
						}
					}
					unset($check_recrusive_error[$macro['UUID']]);
					if(count($check_recrusive_error)==0){
						if ($updateProps >0 ){
							sleep(1);
							$ok=$this->forwardRequest('ClientDataChanged', [$updateProps]);
						}	
					}
					return $ok;
			}
		}
		if($changed){
			IPS_SetProperty($this->InstanceID,'MACROS',json_encode($macros) );
			IPS_ApplyChanges($this->InstanceID);
		}
		
		
	}
	private function _updateKeyCodeTable ($KeyCodes){
		$mycodes=json_decode($this->ReadPropertyString('KEYCODES'),true);
		$changed=false; $found=false;
		foreach($mycodes as $index=>$code)if(in_array($code['CODE'], $KeyCodes)==false){unset($mycodes[$index]);$changed=true;}
		foreach($KeyCodes as $keycode){
			if(empty(KEYCODE_NAMES[$keycode]))return $this->error('Invalid Key Code %s found in %s',$keycode,__FUNCTION__);
			$found=false;
			foreach($mycodes as $code)if($code['CODE']==$keycode){$found=true;break;};		
			if(!$found){$mycodes[]=['CODE'=>$keycode,'NAME'=>KEYCODE_NAMES[$keycode][1]];$changed=true;}
		}
		if($changed){
			IPS_SetProperty($this->InstanceID,'KEYCODES',json_encode($mycodes));
			IPS_ApplyChanges($this->InstanceID);
			$this->_handleMacroTable('check');
		}
		
		return $changed;
	}
	private function _updateVariables(){
		$mycodes=json_decode($this->ReadPropertyString('KEYCODES'),true);
		$myProfiles=[];
		$makeProfileName=function($name){return "_{$name}_for_".$this->InstanceID;};
		foreach($mycodes as $code){
			if($profile=$this->_findKeyCodeProfile($code['CODE']))$myProfiles[$profile][$code['CODE']]=$code['NAME'];
		}
		foreach(self::$profiles as $name=>$profile){
			if(empty($myProfiles[$name]) && @$this->GetIDForIdent($name)){
				@$this->DisableAction($name);
				@$this->UnregisterVariable($name);
				@IPS_DeleteVariableProfile($makeProfileName($name));
			}	
		}
		$pos=0;
		foreach($myProfiles as $name=>$assosiations){
			if(!$vname=self::$profiles[$name][0])$vname=$name;
			$icon=self::$profiles[$name][1];
			$profile=$makeProfileName($name);
			ips::CreateProfile_Associations($profile,$assosiations,$icon);
			$this->RegisterVariableInteger($name,$vname,$profile,$pos++);
			$this->EnableAction($name);
		}
		
	}
/*
 * Helper Function
 */	
	private function _findKeyCodeProfile(int $keyCode){
		foreach(self::$profiles as $name=>$profile)if(in_array($keyCode,$profile[2])!==false) return $name;
		return null;
	}
	private function _findKeyCodeProp(int $keyCode){
		switch($this->_findKeyCodeProfile($keyCode)){
			case 'CONTROL': return PROP_PLAY_CONTROL;
			case 'VOLUME' : return ($keyCode==KEY_MUTE) ? PROP_MUTE_CONTROL : PROP_VOLUME_CONTROL;
			case 'SOURCE' : return PROP_SOURCE_CONTROL;
		}
		return 0;
	}
	private function _getUID(){
		return mt_rand()+1;
	}

}
?>