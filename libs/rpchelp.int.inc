<?php
/** @file rpchelp.int.inc
 * @brief Internal API function def for Help
 *
 * @author Xaver Bauer
 * @date 24.03.2018 19:30:38
 * @version 1.0.0
 * @package rpchelpint
 * @brief Generate help output for internal methods
 * @copydoc rpchelp.int.inc
 */
 

const APIfunctions= [
	''		 => [
		"boolstr" => [
			FUNCTION_NAME => "boolstr",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"bool", VALUE_TYPE=>DATATYPE_BOOL ],
				],
				PARAMS_OUT => null
			]
		],
		"mixed2value" => [
			FUNCTION_NAME => "mixed2value",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"mixed", VALUE_TYPE=>DATATYPE_MIXED ],
				],
				PARAMS_OUT => null
			]
		]
	],
	'RPCErrorHandler' => [
		"CatchError" => [
			FUNCTION_NAME => "CatchError",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"ErrLevel", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"ErrMessage", VALUE_TYPE=>DATATYPE_MIXED ],
				],
				PARAMS_OUT => null
			]
		]
	],	
	'RpcApi' => [	
		"__construct" => [
			FUNCTION_NAME => "__construct",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"Source", VALUE_TYPE=>DATATYPE_STRING, VALUE_DEFAULT=>"" ],
					[ VALUE_NAME=>"Logger", VALUE_TYPE=>DATATYPE_OBJECT, VALUE_DEFAULT=>null ],
				],
				PARAMS_OUT => null
			]
		],
		"__destruct" => [
			FUNCTION_NAME => "__destruct",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => null,
				PARAMS_OUT => null
			]
		],
		"__call" => [
			FUNCTION_NAME => "__call",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"FunctionName", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"Arguments", VALUE_TYPE=>DATATYPE_MIXED ],
				],
				PARAMS_OUT => null
			]
		],
		"__wakeup" => [
			FUNCTION_NAME => "__wakeup",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => null,
				PARAMS_OUT => null
			]
		],
		"__sleep" => [
			FUNCTION_NAME => "__sleep",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => null,
				PARAMS_OUT => null
			]
		],
		"IsOnline" => [
			FUNCTION_NAME => "IsOnline",
			FUNCTION_DESC => "Returns true if device online",
			FUNCTION_PARAMS => [
				PARAMS_IN => null,
				PARAMS_OUT => [[VALUE_NAME=>'Return',VALUE_TYPE=>DATATYPE_BOOL]]
			]
		],
		"AttachLogger" => [
			FUNCTION_NAME => "AttachLogger",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"Logger", VALUE_TYPE=>DATATYPE_OBJECT, VALUE_DEFAULT=>null ],
				],
				PARAMS_OUT => null
			]
		],
		"DetachLogger" => [
			FUNCTION_NAME => "DetachLogger",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"Logger", VALUE_TYPE=>DATATYPE_OBJECT, VALUE_DEFAULT=>null ],
				],
				PARAMS_OUT => null
			]
		],
		"Load" => [
			FUNCTION_NAME => "Load",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"Source", VALUE_TYPE=>DATATYPE_MIXED ],
				],
				PARAMS_OUT => null
			]
		],
		"Save" => [
			FUNCTION_NAME => "Save",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => null,
				PARAMS_OUT => null
			]
		],
		"GetFilename" => [
			FUNCTION_NAME => "GetFilename",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => null,
				PARAMS_OUT => null
			]
		],
		"IsImported" => [
			FUNCTION_NAME => "IsImported",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => null,
				PARAMS_OUT => null
			]
		],
		"HasError" => [
			FUNCTION_NAME => "HasError",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"ErrorNo", VALUE_TYPE=>DATATYPE_INT, VALUE_DEFAULT=>0 ],
				],
				PARAMS_OUT => null
			]
		],
		"SetOptions" => [
			FUNCTION_NAME => "SetOptions",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"Options", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"mode", VALUE_TYPE=>DATATYPE_STRING, VALUE_DEFAULT=>"set" ],
				],
				PARAMS_OUT => null
			]
		],
		"SetApiOptions" => [
			FUNCTION_NAME => "SetApiOptions",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"Options", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"mode", VALUE_TYPE=>DATATYPE_STRING, VALUE_DEFAULT=>"set" ],
				],
				PARAMS_OUT => null
			]
		],
		"SetCreditials" => [
			FUNCTION_NAME => "SetCreditials",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"Username", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"Password", VALUE_TYPE=>DATATYPE_MIXED ],
				],
				PARAMS_OUT => null
			]
		],
		"FunctionExists" => [
			FUNCTION_NAME => "FunctionExists",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"FunctionName", VALUE_TYPE=>DATATYPE_MIXED ],
				],
				PARAMS_OUT => null
			]
		],
		"GetLogger" => [
			FUNCTION_NAME => "GetLogger",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => null,
				PARAMS_OUT => null
			]
		],
		"GetModelDef" => [
			FUNCTION_NAME => "GetModelDef",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => null,
				PARAMS_OUT => null
			]
		],
		"GetConfig" => [
			FUNCTION_NAME => "GetConfig",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => null,
				PARAMS_OUT => null
			]
		],
		"GetIcon" => [
			FUNCTION_NAME => "GetIcon",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"Index", VALUE_TYPE=>DATATYPE_INT, VALUE_DEFAULT=>null ],
				],
				PARAMS_OUT => null
			]
		],
		"GetIconImageTag" => [
			FUNCTION_NAME => "GetIconImageTag",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"Index", VALUE_TYPE=>DATATYPE_INT, VALUE_DEFAULT=>null ],
				],
				PARAMS_OUT => null
			]
		],
		"HasProps" => [
			FUNCTION_NAME => "HasProps",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"Props", VALUE_TYPE=>DATATYPE_INT ],
				],
				PARAMS_OUT => null
			]
		],
		"GetServiceNames" => [
			FUNCTION_NAME => "GetServiceNames",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => null,
				PARAMS_OUT => null
			]
		],
		"GetService" => [
			FUNCTION_NAME => "GetService",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"ServiceName", VALUE_TYPE=>DATATYPE_MIXED ],
				],
				PARAMS_OUT => null
			]
		],
		"GetFunctionNames" => [
			FUNCTION_NAME => "GetFunctionNames",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"ServiceName", VALUE_TYPE=>DATATYPE_STRING, VALUE_DEFAULT=>'' ],
					[ VALUE_NAME=>"IncludeServiceName", VALUE_TYPE=>DATATYPE_MIXED, VALUE_DEFAULT=>false ],
				],
				PARAMS_OUT => null
			]
		],
		"GetFunction" => [
			FUNCTION_NAME => "GetFunction",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"FunctionName", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"ServiceName", VALUE_TYPE=>DATATYPE_MIXED, VALUE_DEFAULT=>null ],
				],
				PARAMS_OUT => null
			]
		],
		"GetEventVars" => [
			FUNCTION_NAME => "GetEventVars",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"ServiceName", VALUE_TYPE=>DATATYPE_STRING, VALUE_DEFAULT=>'' ],
					[ VALUE_NAME=>"IncludeNames", VALUE_TYPE=>DATATYPE_MIXED, VALUE_DEFAULT=>false ],
				],
				PARAMS_OUT => null
			]
		],
		"RegisterEvent" => [
			FUNCTION_NAME => "RegisterEvent",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"ServiceName", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"CallbackUrl", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"RunTimeSec", VALUE_TYPE=>DATATYPE_INT, VALUE_DEFAULT=>0 ],
				],
				PARAMS_OUT => null
			]
		],
		"RegisterEvents" => [
			FUNCTION_NAME => "RegisterEvents",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"ServiceName", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"CallbackUrl", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"RunTimeSec", VALUE_TYPE=>DATATYPE_INT, VALUE_DEFAULT=>0 ],
				],
				PARAMS_OUT => null
			]
		],
		"RefreshEvent" => [
			FUNCTION_NAME => "RefreshEvent",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"SID", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"Service", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"RunTimeSec", VALUE_TYPE=>DATATYPE_INT, VALUE_DEFAULT=>0 ],
				],
				PARAMS_OUT => null
			]
		],
		"RefreshEvents" => [
			FUNCTION_NAME => "RefreshEvents",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"EventArray", VALUE_TYPE=>DATATYPE_MIXED ],
				],
				PARAMS_OUT => null
			]
		],
		"UnregisterEvent" => [
			FUNCTION_NAME => "UnregisterEvent",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"SID", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"Service", VALUE_TYPE=>DATATYPE_MIXED ],
				],
				PARAMS_OUT => null
			]
		],
		"UnregisterEvents" => [
			FUNCTION_NAME => "UnregisterEvents",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"EventArray", VALUE_TYPE=>DATATYPE_MIXED ],
				],
				PARAMS_OUT => null
			]
		],
		"Help" => [
			FUNCTION_NAME => "Help",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"FunctionName", VALUE_TYPE=>DATATYPE_STRING, VALUE_DEFAULT=>'' ],
					[ VALUE_NAME=>"HelpMode", VALUE_TYPE=>DATATYPE_INT, VALUE_DEFAULT=>1 ],
					[ VALUE_NAME=>"ReturnResult", VALUE_TYPE=>DATATYPE_MIXED, VALUE_DEFAULT=>false ],
				],
				PARAMS_OUT => null
			]
		],
		"GetCurrentInfo" => [
			FUNCTION_NAME => "GetCurrentInfo",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"InstanceID", VALUE_TYPE=>DATATYPE_INT, VALUE_DEFAULT=>0 ],
				],
				PARAMS_OUT => null
			]
		],
		"error" => [
			FUNCTION_NAME => "error",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"Message", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"ErrorCode", VALUE_TYPE=>DATATYPE_MIXED, VALUE_DEFAULT=>null ],
					[ VALUE_NAME=>"Params", VALUE_TYPE=>DATATYPE_MIXED, VALUE_DEFAULT=>null ],
				],
				PARAMS_OUT => null
			]
		],
		"debug" => [
			FUNCTION_NAME => "debug",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"DebugOption", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"Message", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"Params", VALUE_TYPE=>DATATYPE_MIXED, VALUE_DEFAULT=>null ],
				],
				PARAMS_OUT => null
			]
		],
		"getConnection" => [
			FUNCTION_NAME => "getConnection",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"ConnectionType", VALUE_TYPE=>DATATYPE_INT, VALUE_DEFAULT=>1 ],
				],
				PARAMS_OUT => null
			]
		],
		"_callSource" => [
			FUNCTION_NAME => "_callSource",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"Service", VALUE_TYPE=>DATATYPE_OBJECT ],
					[ VALUE_NAME=>"Arguments", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"Filter", VALUE_TYPE=>DATATYPE_MIXED ],
				],
				PARAMS_OUT => null
			]
		],
		"_callConnection" => [
			FUNCTION_NAME => "_callConnection",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"Service", VALUE_TYPE=>DATATYPE_OBJECT ],
					[ VALUE_NAME=>"Arguments", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"Filter", VALUE_TYPE=>DATATYPE_MIXED, VALUE_DEFAULT=>null ],
				],
				PARAMS_OUT => null
			]
		],
		"_cloneService" => [
			FUNCTION_NAME => "_cloneService",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"Service", VALUE_TYPE=>DATATYPE_OBJECT ],
					[ VALUE_NAME=>"FunctionName", VALUE_TYPE=>DATATYPE_STRING, VALUE_DEFAULT=>'' ],
				],
				PARAMS_OUT => null
			]
		],
		"_findFunctionService" => [
			FUNCTION_NAME => "_findFunctionService",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"functionname", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"ServiceName", VALUE_TYPE=>DATATYPE_MIXED, VALUE_DEFAULT=>null ],
				],
				PARAMS_OUT => null
			]
		],
		"_deviceUrl" => [
			FUNCTION_NAME => "_deviceUrl",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"Port", VALUE_TYPE=>DATATYPE_MIXED, VALUE_DEFAULT=>null ],
				],
				PARAMS_OUT => null
			]
		],
		"_hasOption" => [
			FUNCTION_NAME => "_hasOption",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"Option", VALUE_TYPE=>DATATYPE_MIXED ],
				],
				PARAMS_OUT => null
			]
		],
		"_validateValue" => [
			FUNCTION_NAME => "_validateValue",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"Param", VALUE_TYPE=>DATATYPE_OBJECT ],
					[ VALUE_NAME=>"Value", VALUE_TYPE=>DATATYPE_MIXED ],
				],
				PARAMS_OUT => null
			]
		],
		"_createFunctionValues" => [
			FUNCTION_NAME => "_createFunctionValues",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"ParamsIn", VALUE_TYPE=>DATATYPE_ARRAY ],
					[ VALUE_NAME=>"Arguments", VALUE_TYPE=>DATATYPE_ARRAY, VALUE_DEFAULT=>[] ],
				],
				PARAMS_OUT => null
			]
		],
		"_clear" => [
			FUNCTION_NAME => "_clear",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"full", VALUE_TYPE=>DATATYPE_MIXED, VALUE_DEFAULT=>true ],
				],
				PARAMS_OUT => null
			]
		],
		"_load" => [
			FUNCTION_NAME => "_load",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"JsonConfigFileName", VALUE_TYPE=>DATATYPE_MIXED ],
				],
				PARAMS_OUT => null
			]
		],
		"_save" => [
			FUNCTION_NAME => "_save",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"JsonConfigFileName", VALUE_TYPE=>DATATYPE_STRING, VALUE_DEFAULT=>'' ],
				],
				PARAMS_OUT => null
			]
		],
		"_import" => [
			FUNCTION_NAME => "_import",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"UrlToDescXml", VALUE_TYPE=>DATATYPE_MIXED ],
				],
				PARAMS_OUT => null
			]
		],
		"_sendEvents" => [
			FUNCTION_NAME => "_sendEvents",
			FUNCTION_DESC => "",
			FUNCTION_PARAMS => [
				PARAMS_IN => [
					[ VALUE_NAME=>"EventArray", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"CallbackUrl", VALUE_TYPE=>DATATYPE_MIXED ],
					[ VALUE_NAME=>"RunTimeSec", VALUE_TYPE=>DATATYPE_INT, VALUE_DEFAULT=>0 ],
				],
				PARAMS_OUT => null
			]
		]

	]		
];
 ?>