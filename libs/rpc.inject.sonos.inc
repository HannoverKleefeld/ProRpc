<?php
$PRE_DefinedInstanceParam= [VALUE_NAME => 'InstanceID',VALUE_TYPE => DATATYPE_UINT, VALUE_DEFAULT => 0];
$PRE_DefinedReturnParam  = [VALUE_NAME => 'Result',	VALUE_TYPE => DATATYPE_BOOL];
$RPC_Inject= [
	[	
		INJECT_MODELS  => null,
		INJECT_OPTIONS	=> OPTIONS_NEED_HOST,	
		INJECT_SERVICES => [
			'RenderingControl'=>[
				SERVICE_NAME => 'RenderingControl',
	 			SERVICE_EXPORT => null,
				SERVICE_FUNCTIONS => [
					'GetBalance'=>[
						FUNCTION_NAME => 'GetBalance',
						FUNCTION_PARAMS => [
							PARAMS_IN =>[$PRE_DefinedInstanceParam],
							PARAMS_OUT=>[ [VALUE_NAME => 'DesiredBalance',	VALUE_TYPE => DATATYPE_INT]]
						],
						FUNCTION_SOURCE => '
	if(!empty($STATIC["__balance"]))return $STATIC["__balance"][0];
	if(is_null($LF=$this->GetVolume($InstanceID,"LF")))return null;
	if(is_null($RF=$this->GetVolume($InstanceID,"RF")))return null;
	if($LF==100&&$RF==100)$b=0;	elseif($RF==100)$b= 100 - $LF;	elseif($LF==100)$b= -(100-$RF);	else $b=$LF-$RF;
	$STATIC["__balance"]=[$b,$LF,$RF];
	return $STATIC["__balance"][0];'			
					],
					'SetBalance'=>[
						FUNCTION_NAME => 'SetBalance',
						FUNCTION_PARAMS => [
							PARAMS_IN =>[ $PRE_DefinedInstanceParam, [VALUE_NAME => 'DesiredBalance',VALUE_TYPE => DATATYPE_INT,	 VALUE_MIN  => -100,VALUE_MAX  => 100,VALUE_STEP => 1,VALUE_DEFAULT => 0]],
							PARAMS_OUT=>[ $PRE_DefinedReturnParam ],
						],
						FUNCTION_SOURCE => '
	if(empty($STATIC["__balance"]))if(is_null($this->GetBalance($InstanceID)))return null;
	if($STATIC["__balance"][0]==$DesiredBalance)return true;
	if($DesiredBalance==0){$L=100;$R=100;}
	elseif($DesiredBalance0){$L=100 - $DesiredBalance; $R=100;}
	elseif($DesiredBalance){$R=100 + $DesiredBalance; $L=100;}
	if($STATIC["__balance"][1]!=$L && !$this->SetVolume($InstanceID,"LF",$L))return null;
	if($STATIC["__balance"][2]!=$R && !$this->SetVolume($InstanceID,"RF",$R))return null;
	$STATIC["__balance"][0]=$DesiredBalance;
	return true;'
					]
				]
			]
		]
	]		
];

?>