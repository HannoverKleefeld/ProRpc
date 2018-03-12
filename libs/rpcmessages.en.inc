<?php
const 
RPCMessages=[
	// RPC Connection	
	
	ERR_OpenSoketTo 			=> 'Error opening socket to %s at %s : %s [%s]',
	ERR_CantGetConnection 		=> 'Cant get connection',
	ERR_EmptyResponse			=> 'Empty Response',
	ERR_InvalidResposeCode		=> 'Call returns http error (%s) %s',
	ERR_InvalidResponceFormat	=> 'Request error: Invalid response type . Only %s allowed',
	ERR_MethodNoScala			=> 'Method name has no scalar value',
	ERR_FormatArray				=> 'Params must be given as array',
	ERR_NoResponseID			=> 'No response id',
	ERR_InvalidResponseID		=> 'Incorrect response id (request id: %s response id: %s )',
	ERR_HostNameNotSet			=> 'No hostname set',

	// Error ValidateValue	
	ERR_ParamIsEmpty			=> "Required param '%s' is empty!",
	ERR_InvalidParamTypeBool	=> "Invalid paramtype for ->%s<- ! require type boolean",
	ERR_InvalidParamTypeUint	=> "Invalid paramtype for ->%s<- ! require type int! [0-xxxxx]",
	ERR_InvalidParamTypeNum		=> "Invalid paramtype for  ->%s<- ! require type numeric",
	ERR_ValueToSmal				=> "Value %s for param ->%s<- is to lees! Allowed %s to %s",
	ERR_ValueToBig				=> "Value %s for param ->%s<-' is to great! Allowed %s to %s",
	ERR_ValueNotAllowed			=> "Value %s for param ->%s<- is invalid! Allowed only [ %s ]",

// RPC _call
	ERR_DeviceNotLoaded			=> 'Device configuration not loaded!',
	ERR_DeviceNotOnline			=> 'Device not online!',
	ERR_FunctionNotFound		=> 'Function "%s" not found!',
	ERR_InvalidParamFound		=> 'Invalid parameter %s found'	,
	ERR_SourceExportFailed		=> 'Failed to export %s variable %s',
	// in source functions
	ERR_KeyCodeNotExists		=> 'KeyCode %s not exist',  // From / For source function SendKeyCode;
	ERR_CantGetLocalAddress		=> 'Cant get local address', 
	ERR_ConnectSocketTo			=> 'Error opening socket to %s at port %s',

// rpcimport
	ERR_UnknownFileName			=> 'Unknown Filename %s',
	ERR_NoServicesFound			=> 'No services found',	
	ERR_NeedHostName			=> 'Hostname missing => call method with "[username [:password] @ ] hostname[:port]/name"',
	ERR_NeedPort				=> 'Hostname or port missing => call method with "[username [:password] @ ] hostname:port/name"',
	ERR_NeedCreditials			=> 'Login data missing => call method with "username[:password]@hostname:port/name"',
	ERR_InjectInitFailed		=> 'Injection init function %s failed',
		
// RPC GetService ...  Events register,refresh,unregister
	ERR_ServiceNotFound 		=> 'Service %s not found',
	ERR_NoEventServiceFound		=> 'No events found',
	ERR_ServiceHasNoEvents 		=> 'Service %s has no events',
	ERR_ServiceHasNoEventUrl	=> 'Service %s has no event control url',
	ERR_NoResposeSID			=> 'Empty event SID received',
	ERR_InvalidTimeouResponse	=> 'Invalid or no timeout received',
	ERR_NotMySIDReceived		=> 'Received sid %s is not my sid %s',
	ERR_InvalidCallbackUrl		=> 'Invalid callback url "%s"',

	// Detection in rpc.inject.inc
	ERR_FoundManuModelNotMatch	=> 'Found manufacturer %s but model %s not in detection matchlist',
	// Load/Save Config
	ERR_FileNotFound			=> 'File %s not found',
	ERR_NoRpcConfigFile			=> 'File %s is no valid Rpc config',
	ERR_ConfigFileVersionMismatch=>'Invalid config version: %s !! Need version: %s',
	ERR_NoDataToSave			=> 'Device data empty. Save aborted!',	
		
// rpchelp	
	MSG_FUNCTION 				=> 'Function',
	MSG_SERVICE					=> 'Service',
	MSG_PARAMS					=> 'Arguments',	
	MSG_IN						=> 'Input',
	MSG_OUT						=> 'Output',
	MSG_DESCRIPTION 			=> 'Description',

		
		
];

?>