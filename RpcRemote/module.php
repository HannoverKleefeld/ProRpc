<?php
require_once __DIR__.'/../libs/loader.php';
/** 
 * @author Xavier
 * 
 */
class RpcRemoteControl extends IPSControlModule {
	const new_macro_name = 'new macro';
	const max_macro_keys = 8;
	const min_seconds_before_create_timer = 5;
	/**
	 * {@inheritDoc}
	 * @see IPSControlModule::Create()
	 */
	function Create(){
		parent::Create();
		$this->registerPropertyString('GROUPS',json_encode(IPSRemoteKeys::defaultGroups()));
		$this->registerPropertyString('KEYS',json_encode(IPSRemoteKeys::defaultKeyMap()));
		$this->registerPropertyString('MACROS','[]');
	}
	function Destroy(){
		for($j=0;$j<10;$j++)@IPS_DeleteVariableProfile("RPC_GROUP_{$j}_{$this->InstanceID}");
		parent::Destroy();
	}
	function ApplyChanges(){
		parent::ApplyChanges();
		$this->_handleMacroTable('update');
	}
	function RequestAction($Ident,$Value){
		if(parent::RequestAction($Ident, $Value))return;
		$groups=json_decode($this->ReadPropertyString('GROUPS'));
		foreach($groups as $group)if($found=$Ident == 'GROUP_'.$group->ID)break;
		if($found)$this->setValueByProp(PROP_REMOTE_CONTROL, $Value);
		else IPS_LogMessage(__CLASS__,"Invalid request action $Ident !  value: $Value");
	}
	function GetConfigurationForm(){
		$form=json_decode(parent::GetConfigurationForm(),true);
		$form["elements"][]=["type"=>"List","name"=>"GROUPS","caption"=>"Avaible Groups","rowCount"=>5,"columns"=> [
				["label"=>"Name","name"=>"NAME", "width"=>"auto", "edit"=>["type"=>"ValidationTextBox","caption"=>"Groupname"]],
				["label"=>"Icon","name"=>"ICON", "width"=>"100", "edit"=>["type"=>"ValidationTextBox","caption"=>"Iconname"]],
				
				["label"=>"","name"=>"ID","width"=>"0","visible"=>false,"save"=>true]
			]	
		];
		$groups=json_decode($this->ReadPropertyString('GROUPS'));
		$options=[]; foreach($groups as $group)$options[]=['label'=>$group->NAME,'value'=>$group->ID];
 		$errors=[];
		$apikeys=json_decode($this->getBuffer('apiKeys'),true);
 		$keys=json_decode($this->ReadPropertyString('KEYS'));
 		foreach($keys as $k)$errors[]=['rowColor'=>empty($apikeys)?'#C0C0FF':(array_key_exists($k->KEY,$apikeys)?'':'#FFC0C0')];
  		$form["elements"][]=["type"=>"List","name"=>"KEYS","caption"=>"Key to Group","rowCount"=>6,"add"=>false,"delete"=>false,"columns"=> [
 				["label"=>"Code","name"=>"KEY","width"=>"50px"],
 				["label"=>"Key","name"=>"NAME","width"=>"150px","edit"=>["type"=>"ValidationTextBox","caption"=>"Keyname"]],
 				["label"=>"Group","name"=>"GROUPID","width"=>"auto","edit"=>["type"=>"Select","caption"=>"Groupname", "options"=>$options ]],
 			],'values'=>$errors	
 		];
		$errors=[];
 		$macros=json_decode($this->ReadPropertyString('MACROS'));
 		$optmacros=[["label"=>"<-None->", "value"=> '0']];
 		foreach($macros as $macro){
 			$optmacros[]=["label"=>$macro->NAME, "value"=> $macro->UUID];
 			$errors[]=['rowColor'=>!empty($macro->ERROR)?($macro->ERROR=='NK') ? '#C0C0FF' : '#FFC0C0':'']; 
 		}
 		foreach($keys as $k)$optkeys[]=["label"=>$k->NAME, "value"=>$k->KEY];
		sort($optkeys);
 		array_unshift($optkeys, ["label"=>"<-Select->", "value"=> 0]);
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
		$form["elements"][]=["type"=>"List","name"=>"MACROS","caption"=>"KeyCode Macros","rowCount"=>5,"add"=>true,"delete"=>true,"columns"=> $columns, 'values'=>$errors]; 				
		$form["elements"][]=["type"=> "Label", "label"=> "COLORS => YELLOW : Api has no KeyCodes, RED : KEY not supported from Api"];			

		if($apikeys){
			$optkeys=[];
 			foreach($keys as $k)if(array_key_exists($k->KEY, $apikeys))$optkeys[]=["label"=>$k->NAME, "value"=>$k->KEY];
			sort($optkeys);
			$form["actions"][]=["type"=> "Select", "name"=> "keycode", "caption"=>"Select Key to send","options"=>$optkeys];			
			$form["actions"][]=["type"=> "Button", "label"=> "Send Key", "onClick"=>'if(empty($keycode))echo "Please select a Key to send!";else RREMOTE_SendKey($id,$keycode)'];			
			
			if(count($macros)>0){
				array_shift($optmacros);	
				$form["actions"][]=["type"=> "Select", "name"=> "macro", "caption"=>"Select Macro to send","options"=>$optmacros];			
				$form["actions"][]=["type"=> "Button", "label"=> "Send Macro", "onClick"=>'if(empty($macro))echo "Please select a Macro to send!";else RREMOTE_SendMacro($id,$macro)'];			
			}
		}
		
		return json_encode($form);
	}
	public function UpdateStatus(bool $Force){
		if(!parent::UpdateStatus($Force))return false;
		return true;
	}
	public function SendKey(int $Key){
		if($ok=$this->apiHasProp(PROP_REMOTE_CONTROL))
			$ok=$this->setValueByProp(PROP_REMOTE_CONTROL, $Key,true);
		$this->SendDebug(__FUNCTION__,"Key: $Key => ".boolstr($ok),0);
		return $ok;
	}
	public function SendTwoKeys(int $Key_first, int $DelaySeconds, int $Key_second){
		if(empty($Key_first))return false;
		if(!$this->setValueByProp(PROP_REMOTE_CONTROL, $Key_first))return false;
		if(empty($Key_second))$Key_second=$Key_first;
		if($DelaySeconds < self::min_seconds_before_create_timer){
			sleep($DelaySeconds);
			return $this->setValueByProp(PROP_REMOTE_CONTROL, $Key_second);			
		}
		return $this->_createTimer($Key_second, $DelaySeconds);
	}
	public function SendMacro(string $MacroName){
		if($ok=$this->apiHasProp(PROP_REMOTE_CONTROL))$ok=$this->_handleMacroTable('execute',$MacroName);
		return $ok;
	}
	
	protected function getProps(){ //:array{
		// array( VariableType, Profilename, Position [, icon ] )
		return [
			PROP_REMOTE_CONTROL=>null,
		];
	}	
	protected function updateVariablesByProps($NewProps){
		parent::updateVariablesByProps($NewProps);
		if($NewProps & PROP_REMOTE_CONTROL){
			$apikeys=$this->forwardRequest('GetKeyCodes', []);
			$this->_updateVariablesByKeys($apikeys);
		}else{
			$this->_updateVariablesByKeys([]);			
		}
	}
	protected function setValueByProp(int $Prop, $Value){
		if($Prop!=PROP_REMOTE_CONTROL)return false;
		$apikeys=json_decode($this->getBuffer('apiKeys'),true);
		if(!array_key_exists($Value, $apikeys))return false;
		$ok=$this->forwardRequest('SendKeyCode', ['KeyCode'=>$Value, 'SendDelayMS'=>0]);	
		if($ok){
			
			$this->forwardRequest('DataChanged', [[NAMES_PROPS[$Prop]=>$Value]]);
		}
		return $ok;
	}
	protected function getValueByProp(int $Prop, $Force=false){
 		return null;
	}
	private function _updateVariablesByKeys(array $RefKeys){
		$groups=json_decode($this->ReadPropertyString('GROUPS'));
 		$keys=json_decode($this->ReadPropertyString('KEYS'));
// IPS_LogMessage(__FUNCTION__,var_export($RefKeys,true));
		$keys_changed=false;
		foreach($keys as &$key){
			if(!array_key_exists($key->KEY, $RefKeys))continue;
			if(empty($key->NAME) || $key->NAME==NAMES_KEYS[$key->KEY]){
				if($key->NAME!=$RefKeys[$key->KEY]['name']){
					$key->NAME=$RefKeys[$key->KEY]['name'];
					$keys_changed=true;
				}
			}
			$props[$key->GROUPID][$key->KEY]=$key->NAME;
		}
		$filter=[];
		foreach($groups as $group){
			$vname='GROUP_'.$group->ID;
			$id=@$this->GetIDForIdent($vname);
			if(empty($props[$group->ID])){
				if($id){
					IPS_SetHidden($id,true);
					$this->disableAction($vname);
				}
				
				continue;
			}
			$filter[]=".*$vname.*";
			$this->CreateProfile_Associations("RPC_GROUP_{$group->ID}_{$this->InstanceID}", $props[$group->ID],$group->ICON);
			if($id)
				IPS_SetHidden($id,false);
			else
				$id=$this->createVariable($vname ,1,"RPC_GROUP_{$group->ID}_{$this->InstanceID}");
 			IPS_SetName($id,$group->NAME);
			$this->enableAction($vname);
		}
		$this->updateReceiveFilter($filter);
		foreach($RefKeys as &$rkey)unset($rkey['name']);
		$this->setBuffer('apiKeys',json_encode($RefKeys));
		if($keys_changed)$this->setProperty('KEYS', json_encode($keys),true);
		$this->_handleMacroTable ('check');		
	}
	private function _handleMacroTable ($command, $data=null){
		static $check_recrusive_error = [];
		static $updateProps = 0;
		$macros=json_decode($this->ReadPropertyString('MACROS'),true);
		$macro_changed=false;$ok=true;
		foreach($macros as &$macro){
			switch ($command) {
				case 'update' :
					if($macro['UUID']==0){	$macro['UUID']=$this->_getUID(); $macro_changed=true;	}
					if($macro['MACRO']==$macro['UUID']){ $macro['MACRO']=0; 	$macro_changed=true;} // Refrence to self not allowed
					if($macro['NAME']==self::new_macro_name){ $macro['NAME']='Macro'.mt_rand(100,200);$macro_changed=true;}
					//break;
				case 'check' :
					$error=null;$hasKeys=false;
					$apikeys=json_decode($this->getBuffer('apiKeys'));
					for($key=0;$key<self::max_macro_keys;$key++){
						if(($keycode=$macro["KEY$key"])){
							$found=($apikeys);
							if($found)foreach($apikeys as $code=>$def)if($found=$code==$keycode)break;
							if(!$found)$error[]=$key; else $hasKeys=true;
						}
					}
					if($error){ $macro['ERROR']=implode(',', $error);$macro_changed=true;}
					elseif(!$hasKeys && empty($macro['ERROR'])){$macro['ERROR']="NK";$macro_changed=true;}
					elseif($hasKeys && !empty($macro['ERROR'])){unset($macro['ERROR']);$macro_changed=true;}	
					break;
				case 'execute':
					if(is_null($data))return $this->error('No macroname or id given');
					if(is_numeric($data)){if ($macro['UUID']!=$data)break;}
					elseif(strcasecmp($macro['NAME'], $data)!=0) break;
					if(!empty($macro['ERROR'])) return $this->error('Macro % has has error in Key(s): %s',$macro['NAME'],$macro['ERROR']);
// 					if(count($check_recrusive_error)==0)$updateProps=0;
					if(!empty($check_recrusive_error[$macro['UUID']])){
						IPS_LogMessage(__CLASS__,'Macro '.$macro['NAME'].' ! No Recrusions allowed');
						return null;
					}
					$this->SendDebug(__FUNCTION__,'Execute macro : '.$macro['NAME'],0);
					for($key=0; $key < self::max_macro_keys; $key++ ){
						if($keycode=$macro["KEY$key"]){
							if(!$ok=$this->forwardRequest('SendKeyCode', ['KeyCode'=>$keycode]))break;
							if($macro["DELAY$key"])usleep($macro["DELAY$key"]*1000);
						}
						
					}
					$this->SendDebug(__FUNCTION__,'Send Macro :'.boolstr($ok),0);
					if(!$ok)break;
					$check_recrusive_error[$macro['UUID']]=true;
					$senddelay=$macro["DELAY"]?$macro["DELAY"]:100;
					if($next=$macro["MACRO"]){
						usleep($senddelay*1000);
						$ok=$this->_handleMacroTable('execute',$next);
					}
					unset($check_recrusive_error[$macro['UUID']]);
					break;
			}
			if(!$ok)break;
		}
		
		if($macro_changed)	$this->setProperty('MACROS',json_encode($macros),true );
		return $ok;
		
	}
	private function _getUID(){
		return mt_rand()+1;
	}
	private function _createTimer($KeyToSend, $Seconds){
		$name="Timer $Seconds sec. for Key $KeyToSend";
		if($id=@IPS_GetObjectIDByName($name,$KeyToSend) )return true;
		$id=IPS_CreateEvent(1);
		IPS_SetHidden($id,true);
		IPS_SetName($id,"Timer $Seconds sec. for Key $KeyToSend");
		IPS_SetEventCyclic ($id,0,0,0,0, 1, $Seconds );
		IPS_SetEventScript($id, "if(RREMOTE_SendKey($this->InstanceID,$KeyToSend))IPS_DeleteEvent($id);");
		IPS_SetEventLimit($id,1);
		IPS_SetParent($id,$this->InstanceID);
		IPS_SetEventActive($id,true);
		return true;
	}
}

