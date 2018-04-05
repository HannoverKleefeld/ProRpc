<?php
/** @file rpchelp.inc 
 * @brief Generate help output for RpcApi 
 *
 * @author Xaver Bauer
 * @date 24.03.2018
 * @version 2.0.2
 * - add CreateInternalHelp
 * @date 21.01.2018
 * @version 2.0.1
 * - add PhpDoc Tags
 * @package rpchelp
 * @brief Generate help output for RpcApi 
 * @copydetails rpchelp.inc
 */

/** @brief Generate help output
 * 
 * Generate help output from given @ref function_class
 * @param object $Function then @ref function_class
 * @param string $FunctionDisplayName Name to output as function name
 * @param array $Values Values to output
 * @param int $HelpMode Helpmode to output
 * @param string $serviceName Displayname of service to output
 * @param number $HelpWidht With of the output. Used if mode > HELP_SHORT
 * @param int|string the index or text for service description
 * @return array String List of output
 */
function CreateHelp(stdClass $Function, string $FunctionDisplayName, array $Values=null,int $HelpMode=HELP_SHORT, $serviceName='', $HelpWidht = 80, $ServiceDescription=null){
	$paramdesc=null;
	$dataTypeToPHPtype=function($DataType){
	 	static $convert=[DATATYPE_BOOL=>'bool',DATATYPE_INT=>'int',DATATYPE_UINT=>'int',DATATYPE_BYTE=>'int', DATATYPE_FLOAT=>'float',DATATYPE_STRING=>'string',DATATYPE_ARRAY=>'array',DATATYPE_OBJECT=>'object',DATATYPE_MIXED=>'',DATATYPE_UNKNOWN=>'unknown'];
		return empty($convert[$DataType])?'':$convert[$DataType];
	};
		
	if(!empty($Function->{FUNCTION_PARAMS}->{PARAMS_IN})) {
		$maxWidth=1;
		foreach ($Function->{FUNCTION_PARAMS}->{PARAMS_IN} as $param)$maxWidth=max($maxWidth,strlen($param->{VALUE_NAME})+2);
		foreach ($Function->{FUNCTION_PARAMS}->{PARAMS_IN} as $param){
			$typ=$dataTypeToPHPtype($param->{VALUE_TYPE});
			if(isset($param->{VALUE_DESC})){
				$desc=$param->{VALUE_DESC};
				if(is_numeric($desc)){
					if(!function_exists('RpcDescriptionGet'))require_once 'rpcdescriptions.inc';
					$desc=RpcDescriptionGet($desc);
				}
			}else $desc='';
			if(isset($param->{VALUE_DEFAULT}))$desc=sprintf('[%s] ',$param->{VALUE_DEFAULT}).$desc;
			$paramdesc['in'][$param->{VALUE_NAME}]=sprintf("   %{$maxWidth}s %-6s %s",'$'.$param->{VALUE_NAME},$typ,$desc);
			if(!isset($Values[$param->{VALUE_NAME}])|| is_null($Values[$param->{VALUE_NAME}]))$Values[$param->{VALUE_NAME}]='';
			$value=&$Values[$param->{VALUE_NAME}];
			$value=empty($value)?"$typ \$".$param->{VALUE_NAME}:"$typ \$".$param->{VALUE_NAME}."=".($typ=='string'?"\"$value\"":$value);
			if(isset($param->{VALUE_DEFAULT})){
				$v=$param->{VALUE_DEFAULT};
				if(is_null($v))	$v='null';
				else if(is_bool($v))$v=$v?'true':'false';
				elseif(is_string($v))$v="\"$v\"";
				$value.="=$v";
			}
			if(isset($param->{VALUE_MIN}))$value.=" [".$param->{VALUE_MIN}."-".$param->{VALUE_MAX}."]";
			elseif(isset($param->{VALUE_LIST}))$value.=" [".implode('|', $param->{VALUE_LIST})."]";
		}
		$in = implode(', ',$Values);if($in)$in=" $in ";
	}else $in='';
	if(!empty($Function->{FUNCTION_PARAMS}->{PARAMS_OUT})) {
		$out=[];$maxWidth=1;
		foreach ($Function->{FUNCTION_PARAMS}->{PARAMS_OUT} as $param)$maxWidth=max($maxWidth,strlen($param->{VALUE_NAME})+2);
		
		foreach( $Function->{FUNCTION_PARAMS}->{PARAMS_OUT} as $param){
			$typ=isset($param->{VALUE_TYPE})?$dataTypeToPHPtype($param->{VALUE_TYPE}):'';
			if(isset($param->{VALUE_DESC})){
				$desc=$param->{VALUE_DESC};
				if(is_numeric($desc)){
					if(!function_exists('RpcDescriptionGet'))require_once 'rpcdescriptions.inc';
					$desc=RpcDescriptionGet($desc);
				}
			}else $desc='';
			if(isset($param->{VALUE_LIST})){
				$rdesc=' once of '. implode('|',$param->{VALUE_LIST});
			}else $rdesc='';
			$paramdesc['out'][$param->{VALUE_NAME}]=sprintf("   %{$maxWidth}s %-6s %s%s", $param->{VALUE_NAME},$typ,$desc,$rdesc);
			$out[]="$typ ".$param->{VALUE_NAME};
		}
		$out=implode(', ',$out);
		if($out)if(count($Function->{FUNCTION_PARAMS}->{PARAMS_OUT}) > 1)$out=" => array[ $out ]";else $out=" => $out";
	}else $out='';
	if(isset($Function->{FUNCTION_DESC})){
		$desc=$Function->{FUNCTION_DESC};
		if(is_numeric($desc)){
			if(!function_exists('RpcDescriptionGet'))require_once 'rpcdescriptions.inc';
			$desc=RpcDescriptionGet($desc);
		}
	}else $desc=false;
	
	if($HelpMode > HELP_SHORT && ( $desc!==false||$paramdesc)){
		$fmt='  | %-'.($HelpWidht-7).'s |';
		$formatDesc=function($str, $maxwidht=0) use($fmt,$HelpWidht){
			static $from=['ä','Ä','ö','Ö','ü','Ü','ß','·','°','„','“'];
			$maxwidht=$maxwidht?$maxwidht:$HelpWidht - 7;
			$to=[chr(200),chr(201),chr(202),chr(203),chr(204),chr(205),chr(206),chr(207),chr(208),chr(209),chr(210)];
			$string=str_replace($from, $to, str_replace(["\R","\t",'–'], ['','','-'], $str));
			if(strlen($string)<=$maxwidht){
				$return=[$string];
			}else foreach(explode("\n",$string) as $string){
				$string=rtrim($string);
				if(strlen($string)<=$maxwidht && strpos($string,"\n")===false){
					$return[]=$string;continue;
				}
				foreach(explode("\n",wordwrap($string,$maxwidht)) as $substring){
					$return[]=$substring;
				}
			}
			foreach($return as &$v)	$v=str_replace($to,$from,sprintf($fmt,$v));
			return $return;
		};
	
		$help[]='\*+'.str_repeat('-', $HelpWidht-5).'+';
		if(!empty($serviceName)){
			$help[]=sprintf($fmt,"Service : $serviceName");
			if(!is_null($ServiceDescription)){
				if(is_numeric($ServiceDescription)){
					if(!function_exists('RpcDescriptionGet'))require_once 'rpcdescriptions.inc';
					$ServiceDescription=RpcDescriptionGet($ServiceDescription);
				}
 				$help[]=sprintf($fmt,'Description:');
				$help=array_merge($help,$formatDesc($ServiceDescription));
			}
		}
		
		if($HelpMode > HELP_NORMAL && $paramdesc){
			$helpStart=count($help);$help[]='';	$lineParams=[];
// 			$help[]=sprintf($fmt,' Argument:');
			if(isset($paramdesc['in'])){
				$help[]=sprintf($fmt,sprintf("  %5s:",'require'));
				foreach($paramdesc['in'] as $k=>$v)	{
					$help=array_merge($help,$formatDesc($v));
					$lineParams[]="\$$k";
				}
			}	
			if(isset($paramdesc['out'])){
				$adesc=$isArray=count($paramdesc['out'])>1?' array (':'';
				$help[]=sprintf($fmt,sprintf("  %5s:%s",'return',$adesc));
				foreach($paramdesc['out'] as $k=>$v)$help=array_merge($help,$formatDesc($v));
				if($isArray)$help[]=sprintf($fmt,sprintf("  %s",')'));
			}
			$maxWidth=60 - strlen($FunctionDisplayName);
			while(strlen($params=implode(',',$lineParams))>$maxWidth){
				if(end($lineParams)=='...')array_pop($lineParams);
				$count=count($lineParams);
				if($count<1){
					$lineParams[$count]=substr($lineParams[$count],0,$maxWidth-3).'...';
					break;
				}
				$lineParams[$count-1]='...';
			}
			if(strlen($params)>$maxWidth)$params=substr($params,0,$maxWidth-3).'...';
			
			$help[$helpStart]=sprintf($fmt,sprintf('%s: %s(%s)','Function',$FunctionDisplayName,$params));
		}
		if($desc){
			$help[]=sprintf($fmt,'Description');
			$help=array_merge($help,$formatDesc($desc));
		}
		$help[]='  +'.str_repeat('-', $HelpWidht-5).'+*/';
	}else $help[]="$FunctionDisplayName ($in)$out";
	return $help;
}
function CreateInternalHelp($FunctionName='', $ClassName='', int $HelpMode=HELP_SHORT, $HelpWidht = 80){
	require_once 'rpchelp.int.inc';
	$output=[];
	if($ClassName){
		if(empty(APIfunctions[$ClassName]))return null;
		if($FunctionName){
			if(empty(APIfunctions[$ClassName][$FunctionName]))return null;
			$ff=json_decode(json_encode(APIfunctions[$ClassName][$FunctionName]));
			return CreateHelp($ff , $FunctionName,null,$HelpMode, $ClassName , $HelpWidht);
		}else foreach(APIfunctions[$ClassName] as $fn=>$ff)
			if($fn[0]!='_')$output=array_merge($output,CreateHelp(json_decode(json_encode($ff)) , $fn, null,$HelpMode,$ClassName, $HelpWidht));
	}else foreach(APIfunctions as $sn=>$f){
		if($FunctionName){
			if(empty($f[$FunctionName]))continue;
			$ff=json_decode(json_encode($f[$FunctionName]));
			return CreateHelp($ff , $FunctionName,null,$HelpMode, $sn , $HelpWidht);
		}else foreach($f as $fn=>$ff)
			if($fn[0]!='_')$output=array_merge($output,CreateHelp(json_decode(json_encode($ff)) , $fn, null,$HelpMode,$sn, $HelpWidht));
	}
	return $output;
// 	$f=file_get_contents('rpcclass.php');
// 	preg_match_all('/public function ([_\w\d]+)((?:\(| ).*\))((?:\:[ \w]+| \:[ \w]+|))/',$f,$m);
}


?>