<?php

/** 
 * @author Xaver Bauer
 * 
 */
class IPSRpcLogger extends RpcLogger {
	private $ipsModule;
	/**
	 *
	 * @param
	 *        	$LogOptions
	 *        	
	 * @param
	 *        	$LogFileName
	 *        	
	 * @param RpcMessage $MessageObject
	 *
	 */
	public function __construct(IPSBaseModule $IpsModule, $LogOptions, $LogFileName = '') {
		parent::__construct ( $LogOptions, $LogFileName, new IPSRpcMessage($IpsModule));
		$this->ipsModule=$IpsModule;
	}
	/**
	 * {@inheritDoc}
	 * @see RpcLogger::doOutput()
	 */
	protected function doOutput($Message, $Class, $AsError) {
		if($AsError){
			IPS_LogMessage($Class,$Message);
		}else{
			$this->ipsModule->RequestAction('DEBUG_API',json_encode([$Class,$Message]));
		}
	}
}
class IPSRpcMessage extends RpcMessage {
	private $ipsModule;
	function __construct(IPSBaseModule $IpsModule, $lang='en'){
		parent::__construct($lang);
		$this->ipsModule=$IpsModule;
	}
	protected function getMessage($MessageNumber){
		if($msg=parent::getMessage($MessageNumber)){
			$msg=$this->ipsModule->Translate($msg);
		}
		return $msg;
	}
}
?>