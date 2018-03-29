<?php

/** 
 * @author Xaver Bauer
 * 
 */
class IPSRpcLogger extends RpcLogger {
	private $ipsModule;
	/**
	 *
	 * @param int $LogOptions
	 * @param string $LogFileName
	 * @param RpcMessage $MessageObject
	 *
	 */
	public function __construct(IPSBaseModule $IpsModule, $LogOptions, $LogFileName = '') {
		parent::__construct ( $LogOptions, $LogFileName, new IPSRpcMessage($IpsModule));
		$this->SetParent($IpsModule);
	}
	function __wakeup(){
// 		echo __CLASS__ . " => WAKEUP\n";
		parent::__wakeup();
		$this->SetParent(null);
	}
	public function SetParent(IPSBaseModule $IpsModule=null){
		$this->ipsModule=$IpsModule;
		if($this->oMessage)
			$this->oMessage->SetParent($IpsModule);
		elseif($ipsModule)
			$this->SetMessage(new IPSRpcMessage($IpsModule));
	}
	/**
	 * {@inheritDoc}
	 * @see RpcLogger::doOutput()
	 */
	protected function doOutput($Message, $Class, $AsError) {
		if($AsError){
			IPS_LogMessage($Class,$Message);
		}elseif($this->ipsModule){
			$this->ipsModule->RequestAction('DEBUG_API',json_encode([$Class,$Message]));
		}else parent::doOutput($Message, $Class, $AsError);
	}
}
class IPSRpcMessage extends RpcMessage {
	private $ipsModule=null;
	function __construct(IPSBaseModule $IpsModule, $lang='en'){
		parent::__construct($lang);
		$this->ipsModule=$IpsModule;
	}
	function __wakeup(){
// 		echo __CLASS__ . " => WAKEUP\n";
		$this->ipsModule=null; 	
	}
	public function SetParent(IPSBaseModule $IpsModule=null){
		$this->ipsModule=$IpsModule;
	}
	protected function getMessage($MessageNumber){
		if($msg=parent::getMessage($MessageNumber) && $this->ipsModule){
			$msg=$this->ipsModule->Translate($msg);
		}
		return $msg;
	}
}
?>