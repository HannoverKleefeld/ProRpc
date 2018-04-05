<?php 
/** @file rpcinit.inc
@brief Pre defines the need Constants

initializes the necessary runtime constants and, if necessary, creates the used directories
\arg \c RPC_LIB_DIR directory where rpcapi.php located<br>
\arg \c RPC_INJECT_DIR if not defined then default is \c RPC_LIB_DIR/\b inject<br>
\arg \c RPC_MESSAGES_DIR directory where rpcmessages.en.inc located<br>default is \c RPC_LIB_DIR/\b messages<br>
\arg \c RPC_CONFIG_DIR directory where API json config files to saved and load from<br>default is \c RPC_LIB_DIR/\b config<br>
\arg \c RPC_CACHE_DIR directory where import remote device files are cached<br>default is \c RPC_LIB_DIR/\b cache<br>
\arg \c RPC_EXTERNAL_DESCRIPTIONS if true(default) then all descriptions<br>are stored in the external file \c RPC_CONFIG_DIR/\b descriptions.desc to reduce the size of the API configuration file
\arg \c RPC_LANGUAGE if not defined then default is \b 'en'

 * 
 * @author Xaver Bauer
 * @date 20.03.2018
 * @version 2.0.0
 * Created
 * @package rpcinit
 * @brief Pre define API constants
 * @copydetails rpcinit.inc
 */


///@cond INTERNAL
if(defined('RPC_LIB_DIR'))die('You can not redefine the base API constant RPC_LIB_DIR');
DEFINE('RPC_LIB_DIR',__DIR__);
DEFINE('RPC_API_VERSION','2.37.4');
if(!defined('RPC_LANGUAGE'))define('RPC_LANGUAGE','en');
if(!DEFINED('RPC_CONFIG_DIR')) DEFINE('RPC_CONFIG_DIR',__DIR__ . '/config');	
if(!DEFINED('RPC_INJECT_DIR')) DEFINE('RPC_INJECT_DIR',__DIR__ . '/inject');	
if(!DEFINED('RPC_MESSAGES_DIR')) DEFINE('RPC_MESSAGES_DIR',__DIR__ . '/messages');
if(!DEFINED('RPC_CACHE_DIR')) DEFINE('RPC_CACHE_DIR',__DIR__ . '/cache');
if(!is_dir(RPC_CONFIG_DIR))	mkdir(RPC_CONFIG_DIR,755,true);
if(!is_dir(RPC_INJECT_DIR))	mkdir(RPC_INJECT_DIR,755,true);
if(!is_dir(RPC_MESSAGES_DIR)) mkdir(RPC_MESSAGES_DIR,755,true);
if(!is_dir(RPC_CACHE_DIR)) mkdir(RPC_CACHE_DIR,755,true);
if(!defined('RPC_EXTERNAL_DESCRIPTIONS'))define('RPC_EXTERNAL_DESCRIPTIONS',true);

///@endcond
if(!function_exists('boolstr')){
	/**
	 * @param bool $bool
	 * @retval string
	 */
	function boolstr(bool $bool){
		return $bool?'true':'false';
	}
}
if(!function_exists('mixed2value')){
	/**
	 * @param mixed $mixed Value to be convert
	 * @retval mixed Converted value
	 */
	function mixed2value($mixed){
		if(is_numeric($mixed))$mixed=is_float($mixed)?floatval($mixed):intval($mixed);
		elseif(strcasecmp($mixed,'true')==0) $mixed=TRUE;
		elseif(strcasecmp($mixed,'false')==0) $mixed=FALSE;
		return $mixed;
	}
}


?>