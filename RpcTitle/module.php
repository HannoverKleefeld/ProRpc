<?php
require_once __DIR__.'/../libs/loader.php';
/** 
 * @author Xavier
 * 
 */

class RpcTitleView extends IPSControlModule {
	function Create(){
		parent::Create();
		$this->RegisterPropertyBoolean('TitleInfoAsHtml', false);
		$this->RegisterPropertyString('HtmlTemplate', 'default.template');
	}
	function ApplyChanges(){
		parent::ApplyChanges();
		if($this->maintainVariables()) $this->_updateInfo();
	}

	function GetConfigurationForm() {
		$form=json_decode(parent::GetConfigurationForm (),true);
		$form["elements"][]=["type"=> "CheckBox", "name"=>"TitleInfoAsHtml", "caption"=> "Show Title as HTML"];	
		$options=[];
		if($files=scandir(__DIR__))foreach ($files as $file){
			if($file[0]=='.')continue;
			$file=pathinfo($file);
			if(!empty($file['extension'])&&$file['extension']=='template')$options[]=["label"=>$file['filename'], "value"=> $file['basename']];
		}	
		$form["elements"][]=["type"=> "Select", "name"=>"HtmlTemplate", "caption"=> "Html template","options"=> $options];
		return json_encode($form);
	}
	
	protected function getProps(){ //:array{
		// array( VariableType, Profilename, Position [, icon ] )
		return [ ];
	}
	protected function updateVariablesByProps($NewProps){
		parent::updateVariablesByProps($NewProps);
		$Status=$this->maintainVariables($NewProps);
		$this->SendDebug(__FUNCTION__,'maintainVariables: '.$Status ,0);
		$this->SetStatus($Status);
		if($Status==102)$this->_updateInfo();
	}
	protected function setValueByProp(int $Prop, $Value){
		return true;
	}
	protected function getValueByProp(int $Prop, $Force=false){
		if(!is_null($value=parent::getValueByProp($Prop,$Force)))return $value;
		return $this->getValueByIdent('TITLE');
	}

	protected function dataChanged(array &$PropValues){
		static $validKeysCodes = [KEY_NEXT,KEY_PREV,KEY_CHDOWN,KEY_CHUP,KEY_LEFT,KEY_RIGHT];
		foreach($PropValues as $Key=>$Value){
			$Prop=$Key;
			if(!is_numeric($Prop)){
				if(strtoupper($Prop)==NAMES_PROPS[PROP_PLAY_CONTROL])
					$Prop=PROP_PLAY_CONTROL;
				elseif(strtoupper($Prop)==NAMES_PROPS[PROP_REMOTE_CONTROL])
					$Prop=PROP_REMOTE_CONTROL;
				else continue;  
			}elseif($Prop!=PROP_PLAY_CONTROL && $Prop!=PROP_REMOTE_CONTROL)continue;
			if($Prop==PROP_REMOTE_CONTROL){
				if(!in_array($Value, $validKeysCodes))continue;
				usleep(500000);
				$this->SetBuffer('last_playstate',0);
				$this->_updateInfo();
			}elseif($Prop==PROP_PLAY_CONTROL){
				if($Value==2)continue; // PAUSE pressed
				if($this->GetBuffer('last_playstate')!=$Value){
					$this->SetBuffer('last_playstate',$Value);
					usleep(500000);
					$this->_updateInfo();
				}
			}
			unset($PropValues[$Prop]);
		}
		return count($PropValues)==0;
	}
	private function maintainVariables($ApiProps=null){
		$names=['ARTIST','ALBUM','CREATOR','DESCRIPTION','DURATION','RELTIME'];
		$TitleInfoAsHtml=$this->ReadPropertyBoolean('TitleInfoAsHtml');
		if(is_null($ApiProps))$ApiProps=intval($this->GetBuffer('ApiProps'));
		$hide=$ApiProps==0;$Status=102;
		if($ApiProps!=0){
			if(! ($ApiProps & PROP_PLAY_CONTROL)){
				if(!$this->forwardRequest('FunctionExists', ['GetTransportInfo'])){
					$Status=201;
					$hide=true;
				}
			}
		}
		if(!$hide){
			$id=$this->RegisterVariableString('TITLE', $this->Translate('Title'), $TitleInfoAsHtml?'~HTMLBox':'', 0);				
			IPS_SetHidden($id,false);
			if($TitleInfoAsHtml){
				foreach($names as $name)@$this->UnregisterVariable($name);
			}else foreach($names as $index=>$name){
				$id=$this->RegisterVariableString($name, $this->Translate(ucfirst(strtolower($name))), $name=='DESCRIPTION'?'~HTMLBox':'', $index);
				IPS_SetHidden($id,false);
			}
		}else{
			if($id=@$this->GetIDForIdent('TITLE'))IPS_SetHidden($id,true);
			if($id)foreach($names as $name)if($id=@$this->GetIDForIdent($name))IPS_SetHidden($id,true);else break;
		}
		return $Status;
	}
	private function _updateInfo(){
		$ok=($info=$this->forwardRequest('GetCurrentInfo', []));
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
 			if($content=file_get_contents(__DIR__.'/'.$template)){
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
	
}
?>