<?php
/** @file loader.php
 * @brief Register SPL Autoload function
 *   
 * @author Xaver Bauer
 * @date 21.01.2018
 * @version 2.0.1
 * Added PhpDoc Tags
 * @package loader
 * @brief Register SPL Autoload
 * @copydoc loader.php
*/
if(!defined('DEBUG_LOADER'))define('DEBUG_LOADER',false);
if(!function_exists('RPC_ClassAutoLoader')){
	if(!defined('RPC_LIB_DIR')) require_once 'rpcinit.inc';
	require_once RPC_LIB_DIR . '/rpcconstants.inc';
	function RPC_ClassAutoLoader($class){
		$class=strtolower($class);
		if($class=='net'||$class=='xml'||$class=='url'||$class=='ip'||$class=='utf8'||$class=='debug'||$class=='utils')
			$class='rpcclasses';
		$file =RPC_LIB_DIR . "/$class";
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
		return false;
	}
	spl_autoload_register('RPC_ClassAutoLoader');	 
}
?>