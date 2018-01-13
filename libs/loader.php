<?
if(!defined('DEBUG_LOADER'))define('DEBUG_LOADER',false);
if(!defined('LIB_PHP_VERSION'))define('LIB_PHP_VERSION','');

if(!function_exists('LIB_ClassAutoLoader')){
	
	DEFINE('LIB_INCLUDE_DIR',__DIR__);
	DEFINE('RPC_CONFIG_DIR',__DIR__ . '/config/devices');
	DEFINE('RPC_CACHE_DIR',__DIR__ . '/config/cache');
	DEFINE('RPC_ICON_CACHE_DIR',__DIR__ . '/config/icons');
	if(!file_exists(RPC_CONFIG_DIR))mkdir(RPC_CONFIG_DIR,755,true);
	if(!file_exists(RPC_CACHE_DIR))mkdir(RPC_CACHE_DIR,755,true);
	if(!file_exists(RPC_ICON_CACHE_DIR))mkdir(RPC_ICON_CACHE_DIR,755,true);
	function LIB_ClassAutoLoader($class){
		
		$class=strtolower($class);
		$file =LIB_INCLUDE_DIR . "/$class";
		$log=function($classFile) use ($class){
			if(DEBUG_LOADER){
				$ok=is_file($classFile)?'Load':'Check';
				if (function_exists('IPS_LogMessage'))
					IPS_LogMessage('ClassAutoLoader',"$ok => $class :: $classFile");
				else 
					echo "ClassAutoLoader::$ok => $class :: $classFile\n";
			}
		};
		if(LIB_PHP_VERSION){
			$classFile="$file.class".LIB_PHP_VERSION.".php";
			$log($classFile);
			if(is_file($classFile)&&!class_exists($class)){ include $classFile; return true;}
			$classFile="$file.trait".LIB_PHP_VERSION.".php";
			$log($classFile);
			if(is_file($classFile)&&!class_exists($class)) { include $classFile; return true; }
		}	
		$classFile="$file.class.php";
		$log($classFile);
		if(is_file($classFile)&&!class_exists($class)){ include $classFile; return true;}

		$classFile="$file.trait.php";
		$log($classFile);
		if(is_file($classFile)&&!class_exists($class)) { include $classFile; return true; }
		
		$classFile=LIB_INCLUDE_DIR . "/lib.$class.php";
		$log($classFile);
		if(is_file($classFile)&&!class_exists($class)) { include $classFile; return true; }
			
		return false;
	}
	spl_autoload_register('LIB_ClassAutoLoader');	 
}
 
?>