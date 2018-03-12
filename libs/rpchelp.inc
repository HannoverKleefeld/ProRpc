<?php
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
			$paramdesc['out'][$param->{VALUE_NAME}]=sprintf("   %{$maxWidth}s %-6s %s",'$'.$param->{VALUE_NAME},$typ,$desc);
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
			$help[]=sprintf($fmt,' Argument:');
			if(isset($paramdesc['in'])){
				$help[]=sprintf($fmt,sprintf("  %5s:",'require'));
				foreach($paramdesc['in'] as $k=>$v)	{
					$help=array_merge($help,$formatDesc($v));
					$lineParams[]="\$$k";
				}
			}	
			if(isset($paramdesc['out'])){
				$help[]=sprintf($fmt,sprintf("  %5s:",'return'));
				foreach($paramdesc['out'] as $k=>$v)$help=array_merge($help,$formatDesc($v));
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