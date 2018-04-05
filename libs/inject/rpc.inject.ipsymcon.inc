<?php 
$PRE_DefinedInstanceParam= [VALUE_NAME => 'InstanceID',VALUE_TYPE => DATATYPE_UINT, VALUE_DEFAULT => 0];
$PRE_DefinedReturnParam  = [VALUE_NAME => 'Result',	VALUE_TYPE => DATATYPE_BOOL];
$RPC_Inject= [
	[	
		INJECT_MODELS  => null,
		INJECT_SERVICES => [
			'JsonApi'=>[
				SERVICE_NAME => 'JsonApi',
				SERVICE_PORT => 3777,
				SERVICE_CONNTYPE => CONNECTION_TYPE_JSON,
				SERVICE_ID	 => '',
				SERVICE_CTRLURL => '/api/',
				SERVICE_EVENTURL=> '',	
				SERVICE_FILTER => '',
				SERVICE_EVENTS => null,
				SERVICE_FUNCTIONS=>[]
			]
		]
	]
];
$GLOBALS['RPC_Inject']=&$RPC_Inject;
function IPS_CreateFunctions(RPCConnection $Connection, String $Url){
	$convertIpsType=[DATATYPE_BOOL,DATATYPE_INT,DATATYPE_FLOAT,DATATYPE_STRING,DATATYPE_MIXED,DATATYPE_ARRAY];
	$functions=[];
	if($functionNames=$Connection->execute($Url, '', 'IPS_GetFunctionList', [0])) {
		foreach($functionNames as $functionName) {
			if($functionParams=$Connection->execute($Url, '', 'IPS_GetFunction', [$functionName])){
				$params_in=$params_out=null;
				if($functionParams['Parameters'])foreach($functionParams['Parameters'] as $param){
					$p=[VALUE_NAME=>$param['Description'] ,VALUE_TYPE=>$convertIpsType[$param['Type_']]];
					if($max=count($param['Enumeration'])){
						if($p[VALUE_TYPE]!=DATATYPE_STRING && is_string($param['Enumeration'][0])){
							$p+=[VALUE_MIN=>0,VALUE_MAX=>$max,VALUE_STEP=>1];
						}else $p[VALUE_LIST]=$param['Enumeration'];
					}
					$params_in[]=$p;
				}
				$params_out=[[VALUE_NAME=>$functionParams['Result']['Description'] ,VALUE_TYPE=>$convertIpsType[$functionParams['Result']['Type_']]]];
				$functions[$functionName]=[FUNCTION_NAME=>$functionName,FUNCTION_PARAMS=>[PARAMS_IN=>$params_in,PARAMS_OUT=>$params_out]];	
			}
		}	
	}
	return $functions;
}

function INIT_ipsymcon(array $Detection, RpcLogger $Logger=null){
global $RPC_Inject;
	if(empty($Detection[DETECTION_PORT]))$Detection[DETECTION_PORT]=$RPC_Inject[0][INJECT_SERVICES]['JsonApi'][SERVICE_PORT];
	if(empty($Detection[DETECTION_PORT]))return $Logger?$Logger->Error(ERR_NeedPort,ERR_NeedPort):false; 	
	if(empty($Detection[DETECTION_USER]))return $Logger?$Logger->Error(ERR_NeedCreditials,ERR_NeedCreditials):false; 	
	$con = new RPCJsonConnection([$Detection[DETECTION_USER],$Detection[DETECTION_PASS],''], $Logger);
	$url="http://{$Detection[DETECTION_URL]}:{$Detection[DETECTION_PORT]}/api/";
	$RPC_Inject[0][INJECT_INFO]=['ipsymcon',$con->execute($url, '', 'IPS_GetKernelVersion', []),'http',$Detection[DETECTION_PORT],CONNECTION_TYPE_JSON];
	$RPC_Inject[0][INJECT_SERVICES]['JsonApi'][SERVICE_FUNCTIONS] += IPS_CreateFunctions($con,$url);
	$GLOBALS['RPC_Inject'];
	return true;
}

?>