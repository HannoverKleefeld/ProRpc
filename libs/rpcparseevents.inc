<?php
/** @file rpcparseevents.inc
 * @brief Helper functions tp parse events
 * 
 * @author Xaver Bauer
 * @date 15.03.2018
 * @version 2.0.0
 * - Created
 * @package rpcparseevents
 * @brief Helper functions tp parse events
 * @copydetails rpcparseevents.inc
 */
 
/** @brief parse xml elements
 * 
 * @param SimpleXMLElement $XMLEvent
 * @return bool|mixed|null
 */
function RpcParseEvent(SimpleXMLElement $XMLEvent){
	$mixed2value=function ($mixed){
		if(is_numeric($mixed))$mixed=is_float($mixed)?floatval($mixed):intval($mixed);
		elseif(strcasecmp($mixed,'true')==0) $mixed=TRUE;
		elseif(strcasecmp($mixed,'false')==0) $mixed=FALSE;
		return $mixed;
	};
	$valuesFromLastChangeXML=function ($xml)use($mixed2value){
		$getAttributes=function($item)use($mixed2value){
			$out=[];
			if(!empty($item['@attributes']))foreach ($item['@attributes'] as $name=>$value)$out[$name]=$mixed2value($value);
			return $out;
		};
		$a=json_decode(json_encode($xml),true);
		$values=[];
		foreach ($a as $iname=>$instance){
			if($iname=="@attributes")continue;
			$attr=implode(',', $getAttributes($instance));
			$instanceID=intval($attr);
			foreach($instance as $vname=>$var){
				if($vname=="@attributes")continue;
				$vname=strtoupper($vname);
				if(empty($var[0])){
					$attr=$getAttributes($var);
					if(!empty($attr['channel'])){
						$ch=$attr['channel']; $val=$attr['val'];
						if(strtoupper($ch)=='MASTER')$data[$vname]=$val; else $data[$vname][$ch]=$val;
					}else{
						$attr=implode(',', $attr);
						$data[$vname]=$attr;				
					}
				}elseif(is_array($var)){
					$balance=null;	
					foreach($var as $index=>$props){
						$attr=$getAttributes($props);
						if(!empty($attr['channel'])){
							$ch=$attr['channel'];
							$val=$attr['val'];
							if(stripos($ch,'lf')!==false||stripos($ch,'rf')!==false){
								$balance[strtoupper($ch)]=$val;
							}elseif(strtoupper($ch)=='MASTER')
								$data[$vname]=$val;
							else $data[$vname][$ch]=$val;
						}else{
							$attr=implode(',', $attr);
							$data[$vname][$index]=$attr;				
						}
					}
					if($balance){
						if(!$vl=$balance['LF'])		$balance=$balance['RF'];
						elseif(!$vr=$balance['RF'])	$balance=-$balance['LF'];
	 					elseif($vl==$vr)			$balance=0;
	 					elseif($vl>$vr)				$balance=-($vl - $vr);
	 					else 						$balance=abs($vl - $vr);
	 					$data[NAMES_PROPS[PROP_BALANCE_CONTROL]]=$balance;
					}
				}else 
					$this->SendDebug(__FUNCTION__,"Unknown var => $var",0);
			}
			$values[$instanceID]=$data;		
		}
		return $values;
	};	
	$out=null;
	if($events=$XMLEvent->xpath('/propertyset/property/LastChange')){
		if(count($events)>0)foreach($events as $event){
			$o=$valuesFromLastChangeXML($event->Event);
			$out[key($o)]=current($o);
		}
	}elseif($props=$XMLEvent->xpath('/propertyset/property')){
		foreach($props as $prop){
			foreach($prop as $key=>$value){
				if(is_array($value))
					$value=json_encode($value);
				else 
					$value=$mixed2value(trim($value->__toString()));
				$out[0][$key]=$value;
			}
		}
	}else return false;
	return $out;
}
/** @brief Output an error
 * @return bool Always false
 */
function RpcParseSendError(){
	header('HTTP/1.1 404 ERROR');
	echo "HTTP/1.1 404 ERROR\n\n";
	return false;
}
/** @brief Output an accept 
 *  @return bool Always true
 */       
function RpcParseSendAccept(){
	header('HTTP/1.1 202 Accepted');
	echo "HTTP/1.1 202 Accepted\n\n";
	return true;
}
/** @brief parse an device event from php://input
 * @param bool $DoOutput if True sends accept or error back
 * @return string|bool|mixed|null
 */
function RpcParseEventFromInput( bool $DoOutput=true){
	$request = trim(file_get_contents('php://input'));
	if(empty($request)){
		if($DoOutput)RpcParseSendError();
		return 'ERROR => Empty data received.. sending Error back to disable event on remote device';
	}
	$request=htmlspecialchars_decode($request);
	if(!$xml=simplexml_load_string('<?xml version="1.0" encoding="UTF-8" standalone="yes"?>'.str_replace(['e:'],'',$request))){
		if($DoOutput)$accept();
		return 'FAILED => Parsing XML data => '.$request;
	}
	if($DoOutput)RpcParseSendAccept();
	return RpcParseEvent($xml);
}
?>