<?php
/** @file rpcmessage.php
 * @brief Manage and translate RpcApi Messages
 * 
 * PHP Class to manage and translate RpcApi Messages
 * @author Xaver Bauer
 * @date 21.01.2018
 * @version 2.0.1
 * Added PhpDoc Tags
 * @package rpcmessage
 * @brief Manage and translate RpcApi Messages
 * @copydetails rpcmessage.php
 */

/** @brief Manage and translate RpcApi Messages
 * 
 * PHP Class to manage and translate RpcApi Messages\n
 * converts API error codes into readable form for output debug or error messages with RpcLogger\n
 * if a country code is specified when creating @ref RpcMessage::__construct , then this will be used to load the corresponding language file.\n
 * if no country code is specified or the language file is not found, the english language is used
 * @author Xaver Bauer
 * @date 21.01.2018
 * @version 2.0.1
 * Added PhpDoc Tags
 * 
 * @see @ref create_language_file RpcLogger
*/
class RpcMessage {
	/** Create a new RpcMessage object 
	 * @param string $lang The current languange code for translations. Default='en'
	 * @see @ref create_language_file
	 * @note If you specify a language other than English, you must have a referenced file in the defined \c RPC_MESSAGES_DIR
	 */
	public function __construct($lang=RPC_LANGUAGE) {
		$fn=RPC_MESSAGES_DIR."/rpcmessages.$lang.inc";
		if ($lang!='en' && !file_exists($fn))$fn=RPC_MESSAGES_DIR."/rpcmessages.en.inc";
		require_once $fn;
	}
	
	/**
	 * Find message with number $MessageNumber and replace then content with (sprintf) the given $Params
	 * @param int $MessageNumber Number of defined Message
	 * @param null|mixed[] $Params Params to replace in message. Default=NULL  
	 * @return string The translated message
	 */
	public function Get( $MessageNumber , $Params=null /* ... */){
		if(!$Message=$this->getMessage($MessageNumber)) return "Message number $MessageNumber not found!";
		if($cs=preg_match_all('/(%[0-9sdx\-]+)/', $Message) && !is_null($Params)){
			while(!$Params || count($Params)<$cs)$Params[]=NULL;
			array_unshift($Params, $Message);
			$Message=preg_replace('/(%[0-9sdx\-]+)/', '', call_user_func_array('sprintf', $Params));	
		}elseif($cs)$Message=preg_replace('/(%[0-9sdx\-]+)/', '??', $Message);
		return $Message;
	}
/// @cond PROTECTED
	/**
	 * @param int $MessageNumber MessageNumber to get
	 * @return string Content of message or empty if $MessageNumber not exists 
	 */
	protected function getMessage($MessageNumber){
		if(!empty(RPCMessages[$MessageNumber]))return RPCMessages[$MessageNumber];
// 		if(defined('RPCiMessages') && !empty(RPCiMessages[$MessageNumber]))return RPCiMessages[$MessageNumber];
		return '';
	}
/// @endcond	
}

