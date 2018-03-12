<?php

/** 
 * @author Xaver Bauer
 * 
 */
class RpcMessage {
	/**
	 */
	public function __construct($lang='en') {
		if($lang!='en' && !file_exists("rpcmessages.$lang.inc"))$lang='en';
		require_once "rpcmessages.$lang.inc";
	}
	
	public function Get( $MessageNumber , $Params=null /* ... */){
		if(!$Message=$this->getMessage($MessageNumber)) return "Message number $MessageNumber not found!";
		if($cs=preg_match_all('/(%[0-9sdx\-]+)/', $Message) && !is_null($Params)){
			while(!$Params || count($Params)<$cs)$Params[]=NULL;
			array_unshift($Params, $Message);
			$Message=preg_replace('/(%[0-9sdx\-]+)/', '', call_user_func_array('sprintf', $Params));	
		}elseif($cs)$Message=preg_replace('/(%[0-9sdx\-]+)/', '??', $Message);
		return $Message;
	}
	protected function getMessage($MessageNumber){
		if(!empty(RPCMessages[$MessageNumber]))return RPCMessages[$MessageNumber];
// 		if(defined('RPCiMessages') && !empty(RPCiMessages[$MessageNumber]))return RPCiMessages[$MessageNumber];
		return '';
	}
	
}

