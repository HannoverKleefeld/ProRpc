<?php
/* 
	@author Xaver Bauer
	Created 16.02.2018 - 23:59:30
*/
if(!defined('DEBUG_LOADER'))define('DEBUG_LOADER',false);
require_once __DIR__. '/rpcconstants.inc';
if(!function_exists('RPC_ClassAutoLoader')){
	
	DEFINE('RPC_LIB_DIR',__DIR__);
	DEFINE('RPC_CONFIG_DIR',__DIR__ . '/config');
	// DEFINE('RPC_PREDEF_DIR',__DIR__ . '/predefines');
	// DEFINE('RPC_CACHE_DIR',__DIR__ . '/cache');
	
	if(!file_exists(RPC_CONFIG_DIR))mkdir(RPC_CONFIG_DIR,755,true);

	function RPC_ClassAutoLoader($class){
		
		
		$class=strtolower($class);
		if($class=='net'||$class=='xml'||$class=='url'||$class=='ip'||$class=='utf8'||$class=='debug'||$class=='utils')
			$class='rpcclasses';
		
		if($class=="rpc")
			$file =RPC_LIB_DIR . "/rpcclass";
		else $file =RPC_LIB_DIR . "/$class";
	
		
		$log=function($classFile) use ($class){
			if(DEBUG_LOADER){
				$ok=is_file($classFile)?'Load':'Check';
				if (function_exists('IPS_LogMessage'))
					IPS_LogMessage('ClassAutoLoader',"$ok => $class :: $classFile");
				else 
					echo "ClassAutoLoader::$ok => $class :: $classFile\n";
			}
		};
		$classFile="$file.php";
		$log($classFile);
		if(is_file($classFile)&&!class_exists($class)){ include_once $classFile; return true;}
		$classFile="$file.class.php";
		$log($classFile);
		if(is_file($classFile)&&!class_exists($class)){ include_once $classFile; return true;}
		
// 		$classFile="$file.trait.php";
// 		$log($classFile);
// 		if(is_file($classFile)&&!class_exists($class)) { include $classFile; return true; }
		
// 		$classFile=LIB_INCLUDE_DIR . "/lib.$class.php";
// 		$log($classFile);
// 		if(is_file($classFile)&&!class_exists($class)) { include $classFile; return true; }
			
		return false;
	}
	spl_autoload_register('RPC_ClassAutoLoader');	 
}
if(!function_exists('boolstr')){
	function boolstr(bool $bool){
		return $bool?'true':'false';
	}
}
if(!function_exists('mixed2value')){
	function mixed2value($mixed){
		if(is_numeric($mixed))$mixed=is_float($mixed)?floatval($mixed):intval($mixed);
		elseif(strcasecmp($mixed,'true')==0) $mixed=TRUE;
		elseif(strcasecmp($mixed,'false')==0) $mixed=FALSE;
		return $mixed;
	}
}




?>