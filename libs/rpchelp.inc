<?php
/**
 * @param stdClass $Function
 * @param string $FunctionDisplayName
 * @param array $Values
 * @param int $HelpMode
 * @param string $serviceName
 * @param number $HelpWidht
 * @return array
 */
function CreateHelp(stdClass $Function, string $FunctionDisplayName, array $Values=null,int $HelpMode, $serviceName='', $HelpWidht = 80){
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
			$desc=isset($param->{VALUE_DESC})?$param->{VALUE_DESC}:'';
			if(isset($param->{VALUE_DEFAULT}))$desc=sprintf('[%s] ',$param->{VALUE_DEFAULT}).$desc;
			$paramdesc['in'][$param->{VALUE_NAME}]=sprintf("   %{$maxWidth}s %-6s %s",'$'.$param->{VALUE_NAME},$typ,$desc);
			if(is_null($Values[$param->{VALUE_NAME}]))$Values[$param->{VALUE_NAME}]='';
			$value=&$Values[$param->{VALUE_NAME}];
			$value=empty($value)?"$typ \$".$param->{VALUE_NAME}:"$typ \$".$param->{VALUE_NAME}."=".($typ=='string'?"\"$value\"":$value);
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
			$desc=isset($param->{VALUE_DESC})?$param->{VALUE_DESC}:'';
			if(isset($param->{VALUE_LIST})){
				$rdesc=' once of '. implode('|',$param->{VALUE_LIST});
			}else $rdesc='';
			$paramdesc['out'][$param->{VALUE_NAME}]=sprintf("   %{$maxWidth}s %-6s %s%s", $param->{VALUE_NAME},$typ,$desc,$rdesc);
			$out[]="$typ ".$param->{VALUE_NAME};
		}
		$out=implode(', ',$out);
		if($out)if(count($Function->{FUNCTION_PARAMS}->{PARAMS_OUT}) > 1)$out=" => array[ $out ]";else $out=" => $out";
	}else $out='';
	$desc=isset($Function->{FUNCTION_DESC})?$Function->{FUNCTION_DESC}:false;
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
			$help[]=sprintf($fmt," Service: $serviceName");
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
function CreateHelpFromRPCClass(RPC $Rpc, $HelpWidht = 80){
// 	$f=file_get_contents('rpcclass.php');
// 	preg_match_all('/public function ([_\w\d]+)((?:\(| ).*\))((?:\:[ \w]+| \:[ \w]+|))/',$f,$m);
$m=array (
  0 => 
  array (
    0 => 'public function IsOnline()',
    1 => 'public function AttachLogger(RpcLogger $Logger=null)',
    2 => 'public function DetachLogger(RpcLogger $Logger=null)',
    3 => 'public function Load($JsonConfigFileNameOrUrlToDescXml)',
    4 => 'public function Save()',
    5 => 'public function GetFilename()',
    6 => 'public function DeviceImported()',
    7 => 'public function HasError($ErrorNo=0)',
    8 => 'public function SetOptions($Options, $mode=\'set\')',
    9 => 'public function GetLogger()',
    10 => 'public function GetModelDef()',
    11 => 'public function GetConfig()',
    12 => 'public function GetServiceNames()',
    13 => 'public function GetService($ServiceName)',
    14 => 'public function GetFunctionNames($ServiceName=\'\', $IncludeServiceName=false)',
    15 => 'public function GetFunction($FunctionName, $ServiceName=null)',
    16 => 'public function GetEventVars($ServiceName, $IncludeName=false)',
    17 => 'public function RegisterEvent($ServiceName, $CallbackUrl, $RunTimeSec=0)',
    18 => 'public function RegisterEvents($ServiceName, $CallbackUrl, $RunTimeSec=0)',
    19 => 'public function RefreshEvent($SID, $Service, $RunTimeSec=0)',
    20 => 'public function RefreshEvents(array &$EventArray)',
    21 => 'public function UnregisterEvent($SID, $Service)',
    22 => 'public function UnRegisterEvents(&$EventArray)',
    23 => 'public function Help($FunctionName=\'\', $HelpMode= HELP_FULL, $ReturnResult=false)',
    24 => 'public function GetCurrentInfo($InstanceID=0)',
  ),
  1 => 
  array (
    0 => 'IsOnline',
    1 => 'AttachLogger',
    2 => 'DetachLogger',
    3 => 'Load',
    4 => 'Save',
    5 => 'GetFilename',
    6 => 'DeviceImported',
    7 => 'HasError',
    8 => 'SetOptions',
    9 => 'GetLogger',
    10 => 'GetModelDef',
    11 => 'GetConfig',
    12 => 'GetServiceNames',
    13 => 'GetService',
    14 => 'GetFunctionNames',
    15 => 'GetFunction',
    16 => 'GetEventVars',
    17 => 'RegisterEvent',
    18 => 'RegisterEvents',
    19 => 'RefreshEvent',
    20 => 'RefreshEvents',
    21 => 'UnregisterEvent',
    22 => 'UnRegisterEvents',
    23 => 'Help',
    24 => 'GetCurrentInfo',
  ),
  2 => 
  array (
    0 => '()',
    1 => '(RpcLogger $Logger=null)',
    2 => '(RpcLogger $Logger=null)',
    3 => '($JsonConfigFileNameOrUrlToDescXml)',
    4 => '()',
    5 => '()',
    6 => '()',
    7 => '($ErrorNo=0)',
    8 => '($Options, $mode=\'set\')',
    9 => '()',
    10 => '()',
    11 => '()',
    12 => '()',
    13 => '($ServiceName)',
    14 => '($ServiceName=\'\', $IncludeServiceName=false)',
    15 => '($FunctionName, $ServiceName=null)',
    16 => '($ServiceName, $IncludeName=false)',
    17 => '($ServiceName, $CallbackUrl, $RunTimeSec=0)',
    18 => '($ServiceName, $CallbackUrl, $RunTimeSec=0)',
    19 => '($SID, $Service, $RunTimeSec=0)',
    20 => '(array &$EventArray)',
    21 => '($SID, $Service)',
    22 => '(&$EventArray)',
    23 => '($FunctionName=\'\', $HelpMode= HELP_FULL, $ReturnResult=false)',
    24 => '($InstanceID=0)',
  ),
  3 => 
  array (
    0 => '',
    1 => '',
    2 => '',
    3 => '',
    4 => '',
    5 => '',
    6 => '',
    7 => '',
    8 => '',
    9 => '',
    10 => '',
    11 => '',
    12 => '',
    13 => '',
    14 => '',
    15 => '',
    16 => '',
    17 => '',
    18 => '',
    19 => '',
    20 => '',
    21 => '',
    22 => '',
    23 => '',
    24 => '',
  ),
);
$dataTypeToPHPtype=function($DataType){
	 	static $convert=[DATATYPE_BOOL=>'bool',DATATYPE_INT=>'int',DATATYPE_UINT=>'int',DATATYPE_BYTE=>'int', DATATYPE_FLOAT=>'float',DATATYPE_STRING=>'string',DATATYPE_ARRAY=>'array',DATATYPE_OBJECT=>'object',DATATYPE_MIXED=>'',DATATYPE_UNKNOWN=>'unknown'];
		return empty($convert[$DataType])?'':$convert[$DataType];
};

$createParamsIn = function ($ParamsIN, &$ParamDesc, &$AsString)use($dataTypeToPHPtype){
	$maxWidth=1;
	foreach ($ParamsIN as $param)$maxWidth=max($maxWidth,strlen($param->{VALUE_NAME})+2);
	foreach ($ParamsIN as $param){
		$typ=is_string($param->{VALUE_TYPE})?$param->{VALUE_TYPE}:$dataTypeToPHPtype($param->{VALUE_TYPE});
		$desc=isset($param->{VALUE_DESC})?$param->{VALUE_DESC}:'';
		if(isset($param->{VALUE_DEFAULT}))$desc=sprintf('[%s] ',$param->{VALUE_DEFAULT}).$desc;
		$ParamDesc['in'][$param->{VALUE_NAME}]=sprintf("   %{$maxWidth}s %-6s %s",'$'.$param->{VALUE_NAME},$typ,$desc);
		if(is_null($Values[$param->{VALUE_NAME}]))$Values[$param->{VALUE_NAME}]='';
		$value=&$Values[$param->{VALUE_NAME}];
		$value=empty($value)?"$typ \$".$param->{VALUE_NAME}:"$typ \$".$param->{VALUE_NAME}."=".($typ=='string'?"\"$value\"":$value);
		if(isset($param->{VALUE_MIN}))$value.=" [".$param->{VALUE_MIN}."-".$param->{VALUE_MAX}."]";
		elseif(isset($param->{VALUE_LIST}))$value.=" [".implode('|', $param->{VALUE_LIST})."]";
	}
	$AsString = implode(', ',$Values);if($AsString)$AsString=" $AsString ";
};
	


foreach($m[1] as $index=>$fn){
	$params=[PARAMS_IN=>null,PARAMS_OUT=>null];
	if($line=trim(str_replace(['(',')'],'',$m[2][$index]))){
		foreach(explode(',',$line) as $paramPair){
			$value=null;$type=DATATYPE_MIXED;
			if(strpos($paramPair,'=')!==false){
				$pair=explode('=',$paramPair);
				$paramPair=trim($pair[0]);
				$value=trim($pair[1]);
			}
			if(strpos($paramPair,' ')!==false){
				$pair=explode(' ',$paramPair);
				switch(strtolower($pair[0])){
					case 'array'	: $type=DATATYPE_ARRAY;break;
					case 'string'	: $type=DATATYPE_STRING;break;
					case 'bool'		: $type=DATATYPE_BOOL;break;
					case 'int'		: $type=DATATYPE_INT;break;
					case 'float'	: $type=DATATYPE_FLOAT;break;
					case 'object'	: $type=DATATYPE_OBJECT;break;
					default: $type=$pair[0];
				}
				$name=$pair[1];
			}else $name=$paramPair;
			$params[PARAMS_IN][]=[VALUE_NAME=>$name,VALUE_TYPE=> $type,VALUE_DEFAULT=>$value];		
		}
	}
	if(!empty($m[3][$index])){
		$params[PARAMS_OUT][]=[VALUE_NAME=>$m[3][$index],VALUE_TYPE=>$type,VALUE_DEFAULT=>$value];		
		
	}
	$functions[$fn]=[FUNCTION_NAME=>$fn,FUNCTION_PARAMS=>$params];
	
}
	exit(var_export($functions));
	
	if(!empty($Function->{FUNCTION_PARAMS}->{PARAMS_IN})) {
		$maxWidth=1;
		foreach ($Function->{FUNCTION_PARAMS}->{PARAMS_IN} as $param)$maxWidth=max($maxWidth,strlen($param->{VALUE_NAME})+2);
		foreach ($Function->{FUNCTION_PARAMS}->{PARAMS_IN} as $param){
			$typ=$dataTypeToPHPtype($param->{VALUE_TYPE});
			$desc=isset($param->{VALUE_DESC})?$param->{VALUE_DESC}:'';
			if(isset($param->{VALUE_DEFAULT}))$desc=sprintf('[%s] ',$param->{VALUE_DEFAULT}).$desc;
			$paramdesc['in'][$param->{VALUE_NAME}]=sprintf("   %{$maxWidth}s %-6s %s",'$'.$param->{VALUE_NAME},$typ,$desc);
			if(is_null($Values[$param->{VALUE_NAME}]))$Values[$param->{VALUE_NAME}]='';
			$value=&$Values[$param->{VALUE_NAME}];
			$value=empty($value)?"$typ \$".$param->{VALUE_NAME}:"$typ \$".$param->{VALUE_NAME}."=".($typ=='string'?"\"$value\"":$value);
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
			$desc=isset($param->{VALUE_DESC})?$param->{VALUE_DESC}:'';
			if(isset($param->{VALUE_LIST})){
				$rdesc=' once of '. implode('|',$param->{VALUE_LIST});
			}else $rdesc='';
			$paramdesc['out'][$param->{VALUE_NAME}]=sprintf("   %{$maxWidth}s %-6s %s%s", $param->{VALUE_NAME},$typ,$desc,$rdesc);
			$out[]="$typ ".$param->{VALUE_NAME};
		}
		$out=implode(', ',$out);
		if($out)if(count($Function->{FUNCTION_PARAMS}->{PARAMS_OUT}) > 1)$out=" => array[ $out ]";else $out=" => $out";
	}else $out='';
	
	
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
			$help[]=sprintf($fmt," Service: $serviceName");
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

?>