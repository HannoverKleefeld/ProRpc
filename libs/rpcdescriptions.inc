<?php
/** @file rpcdescriptions.inc
 * @brief Handle descriptions
 *
 * Functions for processing device functions, service and argument descriptions
 * @author Xaver Bauer
 * @date 25.03.2018 11:37:48
 * @version 1.0.0
 * - Created
 * @package rpcdescriptions
 * @copydoc rpcdescriptions.inc
 */


$RpcDescriptions = ['changed'=>false,'items'=>null];

function RpcDescriptionsLoad(){
global $RpcDescriptions;
	if(!$RpcDescriptions['items'])$RpcDescriptions['items']=json_decode(@file_get_contents(RPC_CONFIG_DIR.'/descriptions.desc'),true);
}
function RpcDescriptionsSave(){
global $RpcDescriptions;	
	if($RpcDescriptions && $RpcDescriptions['changed'] && $d=json_encode($RpcDescriptions['items'])){
		$RpcDescriptions['changed']=false;
		return (bool)file_put_contents(RPC_CONFIG_DIR.'/descriptions.desc',$d);
	}
}
function RpcDescriptionGet(int $DescriptionID){
global $RpcDescriptions;
	RpcDescriptionsLoad();
	return empty($RpcDescriptions['items'][$DescriptionID])?$DescriptionID:$RpcDescriptions['items'][$DescriptionID];
}
function RpcDescriptionAdd(string $Description){
global $RpcDescriptions;
	RpcDescriptionsLoad();
	if(empty($RpcDescriptions['items']) || ($pos=array_search($Description, $RpcDescriptions['items']))===false){
		$pos=count($RpcDescriptions['items']);
		$RpcDescriptions['items'][]=$Description;
		$RpcDescriptions['changed']=true;
	}
	return $pos;
}

function RpcDescriptionDeviceSet(stdClass $device, $Description, $ServiceName, $FunctionName=null, $ParamName=null){
	$changed=false;
	$s_set=function($service)use($Description,&$changed){
// 		$changed=empty($service->{SERVICE_DESC} || $service->{SERVICE_DESC} != $Description);
		$service->{SERVICE_DESC}=RPC_EXTERNAL_DESCRIPTIONS?RpcDescriptionAdd($Description):$Description;
		return $changed=true;
	};
	$f_set=function($function)use($Description,&$changed){
		$function->{FUNCTION_DESC}=RPC_EXTERNAL_DESCRIPTIONS?RpcDescriptionAdd($Description):$Description;
		return $changed=true;
	};
	$p_set=function($params)use($Description,$ParamName, &$changed){
		foreach($params as $param){
			if($param->{VALUE_NAME}==$ParamName){
				$param->{VALUE_DESC}=RPC_EXTERNAL_DESCRIPTIONS?RpcDescriptionAdd($Description):$Description;
				return $changed=true;
			}
		}
		return false;
	};
	foreach($device->{DEVICE_SERVICES} as $sn=>$service){
		if(empty($ServiceName) || $ServiceName==$sn){
			if(is_null($FunctionName)){
				$s_set($service);
				if(empty($ServiceName))continue; else break;
			}
			foreach($service->{SERVICE_FUNCTIONS} as $fn=>$function){
				if(empty($FunctionName) || $FunctionName==$fn){
					if(is_null($ParamName)){
						$f_set($function);
						if(empty($FunctionName))continue;else break;
					}
				}
				if($function->{FUNCTION_PARAMS}->{PARAMS_IN} && $p_set($function->{FUNCTION_PARAMS}->{PARAMS_IN}))continue;
				if($function->{FUNCTION_PARAMS}->{PARAMS_OUT} && $p_set($function->{FUNCTION_PARAMS}->{PARAMS_OUT}))continue;
			}
		}
	}
	return $changed;
}
 ?>