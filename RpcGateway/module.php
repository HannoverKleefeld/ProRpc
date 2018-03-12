<?php
require_once __DIR__. '/../libs/loader.php';
/** 
 * @author Xaver Bauer
 * 
 */
class RpcGateway extends IPSRpcGateway{
	function Create(){
		parent::Create();
	}
	public function CreateConfig(String $Url){
		
	}
	
	function ApplyChanges(){
		parent::ApplyChanges();
	}
	function RequestAction($Ident, $Value){
		parent::RequestAction($Ident, $Value);
		
	}
	function ReceiveData($JSONString){
		parent::ReceiveData($JSONString);
		
		
	}

	function GetConfigurationForm(){
		$form=parent::GetConfigurationForm();		
		return $form; // json_encode($form);
	}
	
	
	

	
}

