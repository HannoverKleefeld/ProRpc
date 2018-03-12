<?php
if(empty($RpcLanguange))$RpcLanguange='en';
const 
// RPC Connection
ERR_OpenSoketTo 			= 1001,
ERR_CantGetConnection 		= 1002,
ERR_EmptyResponse			= 1003,
ERR_InvalidResposeCode		= 1004,
ERR_InvalidResponceFormat	= 1005,
ERR_MethodNoScala			= 1006,
ERR_FormatArray				= 1007,
ERR_NoResponseID			= 1008,
ERR_InvalidResponseID		= 1009,
ERR_HostNameNotSet			= 1010,

// RPC ValidateValue
ERR_ParamIsEmpty			= 1101,
ERR_InvalidParamTypeBool	= 1102,
ERR_InvalidParamTypeUint	= 1103,
ERR_InvalidParamTypeNum		= 1104,
ERR_ValueToSmal				= 1105,
ERR_ValueToBig				= 1106,
ERR_ValueNotAllowed			= 1107,

// RPC _call
ERR_DeviceNotLoaded			= 1201,
ERR_DeviceNotOnline			= 1202,
ERR_FunctionNotFound		= 1203,
ERR_InvalidParamFound		= 1204,
ERR_SourceExportFailed		= 1205,
// in source functions
ERR_KeyCodeNotExists		= 1221,
ERR_CantGetLocalAddress		= 1222,
ERR_ConnectSocketTo			= 1223,
// rpcimport
ERR_UnknownFileName			= 1301,
ERR_NoServicesFound			= 1302,
ERR_NeedHostName			= 1303,
ERR_NeedPort				= 1304,
ERR_NeedCreditials			= 1305,
ERR_InjectInitFailed		= 1306,


// RPC Events register,refresh,unregister
ERR_ServiceNotFound			= 1401,
ERR_NoEventServiceFound		= 1402,
ERR_ServiceHasNoEvents		= 1403,
ERR_ServiceHasNoEventUrl	= 1404,	
ERR_NoResposeSID			= 1405,
ERR_InvalidTimeouResponse	= 1406,
ERR_NotMySIDReceived		= 1407,
ERR_InvalidCallbackUrl		= 1408,

// Detection in rpc.inject.inc
ERR_FoundManuModelNotMatch  = 1501,

// Load/Save Config
ERR_FileNotFound			= 1601,
ERR_NoRpcConfigFile			= 1602,
ERR_ConfigFileVersionMismatch=1603,
ERR_NoDataToSave			= 1604,

// rpchelp	
MSG_FUNCTION				= 2001,
MSG_PARAMS					= 2002,
MSG_IN						= 2003,
MSG_OUT						= 2004,
MSG_DESCRIPTION				= 2005,
MSG_SERVICE					= 2006
;


?>