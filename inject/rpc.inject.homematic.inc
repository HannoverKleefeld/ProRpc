<?php
$RPC_Inject= [
	[	
		INJECT_MODELS  => null,
		INJECT_INFO    => [
			'Homematic','CCUx','http',2001,CONNECTION_TYPE_JSON	
		],	
		INJECT_SERVICES => [
			'JsonApi'=>[
				SERVICE_NAME => 'JsonApi',
				SERVICE_PORT => 2001,
				SERVICE_CTRLURL => '/',	
				SERVICE_FUNCTIONS => [
					'init'=> [
						FUNCTION_NAME=>'init',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
									[VALUE_NAME=>'url',VALUE_TYPE=>DATATYPE_STRING],
									[VALUE_NAME=>'interface_id',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>null,
						],
						FUNCTION_DESC=>"Mit dieser Methode teilt die Logikschicht dem Schnittstellenprozess mit, dass sie gerade gestartet wurde. Der Schnittstellenprozess wird sich daraufhin selbst initialisieren und z.B. mit listDevices() die der Logikschicht bekannten Geräte abfragen.\nDer Parameter url gibt die Adresse des XmlRpc-Servers an, unter der die Logikschicht zu erreichen ist.\nDer Parameter interface_id teilt dem Schnittstellenprozess die Id, mit unter der er sich gegenüber der Logikschicht identifiziert.\n\nZum Abmelden von der Ereignisbehandlung wird interface_id leer gelassen.",
					],			
					'getLinks'=>[
						FUNCTION_NAME=>'init',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
									[VALUE_NAME=>'address',VALUE_TYPE=>DATATYPE_STRING],
									[VALUE_NAME=>'flags',VALUE_TYPE=>DATATYPE_INT]
							],
							PARAMS_OUT=>[
									[VALUE_NAME=>'Link',VALUE_TYPE=>DATATYPE_ARRAY]
							]
						],	
						FUNCTION_DESC=>"Diese Methode gibt alle einem logischen Kanal oder Gerät zugeordneten Kommunikationsbeziehungen zurück.\nDer Parameter address ist die Kanal- oder Geräteadresse des logischen Objektes, auf das sich die Abfrage bezieht. Bei address==\"\" werden alle Kommunikationsbeziehungen des gesamten Schnittstellenprozesses zurückgegeben.\nDer Parameter flags ist ein bitweises oder folgender Werte:\n· 1 = GL_FLAG_GROUP\nWenn address einen Kanal bezeichnet, der sich in einer Gruppe befindet, werden die Kommunikationsbeziehungen für alle Kanäle der Gruppe zurückgegeben.\n· 2 = GL_FLAG_SENDER_PARAMSET\nDas Feld SENDER_PARAMSET des Rückgabewertes wird gefüllt.\n· 4 = GL_FLAG_RECEIVER_PARAMSET\nDas Feld RECEIVER_PARAMSET des Rückgabewertes wird gefüllt.\nflags ist optional. Defaultwert ist 0x00.\nDer Rückgabewert ist ein Array von Strukturen. Jede dieser Strukturen enthält die folgenden Felder:\n· SENDER\nDatentyp String. Adresse des Senders der Kommunikationsbeziehung\n· RECEIVER\nDatentyp String. Adresse des Empfängers der Kommunikationsbeziehung\n· FLAGS\nDatentyp i4. FLAGS ist ein bitweises oder folgender Werte:\n 1=LINK_FLAG_SENDER_BROKEN\nDiese Verknüpfung ist auf der Senderseite nicht intakt\n 2=LINK_FLAG_RECEIVER_BROKEN\nDiese Verknüpfung ist auf der Empfängerseite nicht intakt\n· NAME\nDatentyp String. Name der Kommunikationsbeziehung\n· DESCRIPTION\nDatentyp String. Textuelle Beschreibung der Kommunikationsbeziehung\n· SENDER_PARAMSET\nDatentyp Paramset. Parametersatz dieser Kommunikationsbeziehung für die Senderseite\n· RECEIVER_PARAMSET\nDatentyp Paramset. Parametersatz dieser Kommunikationsbeziehung für die Empfängerseite",
					],
					'addLink'=>[
						FUNCTION_NAME=>'addLink',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'sender',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'receiver',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'description',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>null
						],
						FUNCTION_DESC=>"Diese Methode erstellt eine Kommunikationsbeziehung zwischen zwei logischen Geräten. Die Parameter sender und receiver bezeichnen die beiden zu verknüpfenden Partner. Die Parameter name und description sind optional und beschreiben die Verknüpfung näher.",
					],
					'removeLink'=>[
						FUNCTION_NAME=>'removeLink',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'sender',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'receiver',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>null
						],
						FUNCTION_DESC=>"Diese Methode löscht eine Kommunikationsbeziehung zwischen zwei Geräten. Die Parameter sender und receiver bezeichnen die beiden Kommunikationspartnern deren Kommunikationszuordnung gelöscht werden soll."
					],
					'setLinkInfo'=>[
						FUNCTION_NAME=>'setLinkInfo',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'sender',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'receiver',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'name',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'description',VALUE_TYPE=>DATATYPE_STRING]
							], 
							PARAMS_OUT=>null 
						],
						FUNCTION_DESC=>"Diese Methode ändert die beschreibenden Texte einer Kommunikationsbeziehung. Die Parameter sender und receiver bezeichnen die beiden zu verknüpfenden Partner. Die Parameter name und description beschreiben die Verknüpfung textuell."
					],
					'getLinkInfo'=>[
						FUNCTION_NAME=>'getLinkInfo',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'sender',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'receiver',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'Info',VALUE_TYPE=>DATATYPE_ARRAY]
									
							]
						],
						FUNCTION_DESC=>"Diese Methode gibt den Namen und die Beschreibung für eine bestehende Kommunikationsbeziehung zurück. Die Parameter sender_address und receiver_address bezeichnen die beiden verknüpften Partner.",
					],
					'activateLinkParamset'=>[
						FUNCTION_NAME=>'activateLinkParamset',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'address',VALUE_TYPE=>DATATYPE_STRING],
									[VALUE_NAME=>'peer_address',VALUE_TYPE=>DATATYPE_STRING],
									[VALUE_NAME=>'long_press',VALUE_TYPE=>DATATYPE_BOOL]
							],
							PARAMS_OUT=>null
						],
						FUNCTION_DESC=>"Mit dieser Methode wird ein Link-Parameterset aktiviert. Das logische Gerät verhält sich dann so als ob es direkt von dem entsprechenden zugeordneten Gerät angesteuert worden wäre. Hiermit kann z.B. ein Link-Parameter-Set getestet werden.\nDer Parameter address ist die Addresses des anzusprechenden logischen Gerätes.\nDer Parameter peer_address ist die Addresse des Kommunikationspartners, dessen Link- Parameter-Set aktiviert werden soll.\nDer Parameter long_press gibt an, ob das Parameterset für den langen Tastendruck aktiviert werden soll.",
					],
					'getLinkPeers'=>[
						FUNCTION_NAME=>'getLinkPeers',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'address',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'LinkPeers',VALUE_TYPE=>DATATYPE_ARRAY]
							]
						],
						FUNCTION_DESC=>"Diese Methode gibt alle einem logischen Gerät zugeordneten Kommunikationspartner zurück. Die zurückgegebenen Werte können als Parameter paramset_key für getParamset() und putParamset() verwendet werden. Der Parameter address ist die Adresse eines logischen Gerätes.",
					],
					'determineParameter'=>[
						FUNCTION_NAME=>'determineParameter',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'address',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'paramset_key',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'parameter_id',VALUE_TYPE=>DATATYPE_STRING]
							], 
							PARAMS_OUT=>null,
						],			
						FUNCTION_DESC=>"Mit dieser Methode wird ein Parameter eines Parameter-Sets automatisch bestimmt. Der Parameter kann bei erfolgreicher Ausführung anschließend sofort über getParamset gelesen werden.\nDer Parameter address ist die Addresses eines logischen Gerätes.\nDer Parameter paramset_key ist MASTER, VALUES oder die Adresse eines Kommunikationspartners für das entsprechende Link-Parameter-Set (siehe getLinkPeers).\nDer Parameter parameter_id bestimmt den automatisch zu bestimmenden Parameter.",
					],
					'getParamsetDescription'=>[
						FUNCTION_NAME=>'getParamsetDescription',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'address',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'paramset_type',VALUE_TYPE=>DATATYPE_STRING]
							], 
							PARAMS_OUT=>[
								[VALUE_NAME=>'Description',VALUE_TYPE=>DATATYPE_ARRAY]
							]
						],
						FUNCTION_DESC=>"Mit dieser Methode wird die Beschreibung eines Parameter-Sets ermittelt. Der Parameter address ist die Adresse eines logischen Gerätes (z.B. von listDevices zurückgegeben). Der Parameter paramset_type ist MASTER, VALUES oder LINK."
					],
					'getParamsetId'=>[
						FUNCTION_NAME=>'getParamsetId',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'address',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'type',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'ID',VALUE_TYPE=>DATATYPE_STRING]
							]
						],
						FUNCTION_DESC=>"Diese Methode gibt die Id eines Parametersets zurück. Diese wird verwendet, um spezialisierte Konfigurationsdialoge (Easymode) den Parametersets zuzuordnen.",
					],
					'getParamset'=>[
						FUNCTION_NAME=>'getParamset',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'address',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'paramset_key',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'Paramset',VALUE_TYPE=>DATATYPE_ARRAY]
							]
						],
						FUNCTION_DESC=>"Mit dieser Methode wird ein komplettes Parameter-Set für ein logisches Gerät gelesen. Der Parameter address ist die Addresses eines logischen Gerätes. Der Parameter paramset_key ist MASTER, VALUES oder die Adresse eines Kommunikationspartners für das entsprechende Link-Parameter-Set (siehe getLinkPeers).",
					],
					'putParamset'=>[
						FUNCTION_NAME=>'putParamset',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'address',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'paramset_key',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'Paramset',VALUE_TYPE=>DATATYPE_ARRAY]
							],
							PARAMS_OUT=>null
						],
						FUNCTION_DESC=>"Mit dieser Methode wird ein komplettes Parameter-Set für ein logisches Gerät geschrieben. Der Parameter address ist die Addresses eines logischen Gerätes. Der Parameter paramset_key ist MASTER, VALUES oder die Adresse eines Kommunikationspartners für das entsprechende Link-Parameter-Set (siehe getLinkPeers).\nDer Parameter set ist das zu schreibende Parameter-Set. In set nicht vorhandene Member werden einfach nicht geschrieben und behalten ihren alten Wert.",
					],
					'getValue'=>[
						FUNCTION_NAME=>'getValue',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'address',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'value_key',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'ValueType',VALUE_TYPE=>DATATYPE_STRING]
							]
						],
						FUNCTION_DESC=>"Mit dieser Methode wird ein einzelner Wert aus dem Parameter-Set VALUES gelesen. Der Parameter address ist die Addresse eines logischen Gerätes. Der Parameter value_key ist der Name des zu lesenden Wertes. Die möglichen Werte für value_key ergeben sich aus der ParamsetDescription des entsprechenden Parameter-Sets VALUES.",
					],
					'setValue'=>[
						FUNCTION_NAME=>'setValue',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'address',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'value_key',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ValueType',VALUE_TYPE=>DATATYPE_STRING]
							],		
							PARAMS_OUT=>null
						],
						FUNCTION_DESC=>"Mit dieser Methode wird ein einzelner Wert aus dem Parameter-Set VALUES geschrieben. Der Parameter address ist die Addresse eines logischen Gerätes. Der Parameter value_key ist der Name des zu schreibenden Wertes. Die möglichen Werte für value_key ergeben sich aus der ParamsetDescription des entsprechenden Parameter-Sets VALUES. Der Parameter value ist der zu schreibende Wert.",
					],
					'setInstallMode'=>[
						FUNCTION_NAME=>'setInstallMode',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'on',VALUE_TYPE=>DATATYPE_BOOL]
							],
							PARAMS_OUT=>null
						],
						FUNCTION_DESC=>"Diese Methode aktiviert und deaktiviert den Installations-Modus, in dem neue Geräte an der HomeMatic-CCU angemeldet werden können.\nDer Parameter on bestimmt, ob der Installations-Modus aktiviert oder deaktiviert werden soll.",
					],
					'getInstallMode'=>[
						FUNCTION_NAME=>'getInstallMode',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>null,
							PARAMS_OUT=>[
								[VALUE_NAME=>'InstallMode',VALUE_TYPE=>DATATYPE_INT]
							]
						],
						FUNCTION_DESC=>"Diese Methode gibt die verbliebene Restzeit in Sekunden im Anlernmodus zurück.\nDer Wert 0 bedeutet, der Anlernmodus ist nicht aktiv.",
					],
					'getKeyMismatchDevice'=>[
						FUNCTION_NAME=>'getKeyMismatchDevice',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'reset',VALUE_TYPE=>DATATYPE_BOOL]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'MismatchDevice',VALUE_TYPE=>DATATYPE_STRING]
							]
						],
						FUNCTION_DESC=>"Diese Methode gibt die Seriennummer des letzten Gerätes zurück, das aufgrund eines falschen AES-Schlüssels nicht angelernt werden konnte.\nMit reset=true wird diese Information im Schnittstellenprozess zurückgesetzt.",
					],
					'addDevice'=>[
						FUNCTION_NAME=>'addDevice',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'serial_number',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'DeviceDescription',VALUE_TYPE=>DATATYPE_ARRAY]
							]
						],
						FUNCTION_DESC=>"Diese Methode lernt ein Gerät anhand seiner Seriennummer an die CCU an. Diese Funktion wird nicht von jedem Gerät unterstützt. Rückgabewert ist die DeviceDescription des neu angelernten Geräts.",
					],
					'listDevices'=>[
						FUNCTION_NAME=>'listDevices',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>null,
							PARAMS_OUT=>[
								[VALUE_NAME=>'description',VALUE_TYPE=>DATATYPE_ARRAY]
							]
						],
						FUNCTION_DESC=>"Diese Methode gibt alle dem Schnittstellenprozess bekannten Geräte in Form von Gerätebeschreibungen zurück.",
					],
					'searchDevices'=>[
						FUNCTION_NAME=>'searchDevices',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>null,
							PARAMS_OUT=>[
								[VALUE_NAME=>'Count',VALUE_TYPE=>DATATYPE_INT]
							]
						],
						FUNCTION_DESC=>"Diese Methode durchsucht den Bus nach neuen Geräten und gibt die Anzahl gefundener Geräte zurück. Die neu gefundenen Geräte werden mit newDevices der Logikschicht gemeldet.",
					],
					'deleteDevice'=>[
						FUNCTION_NAME=>'deleteDevice',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'address',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'flags',VALUE_TYPE=>DATATYPE_INT]
							],		
							PARAMS_OUT=>null
						],
						FUNCTION_DESC=>"Diese Methode löscht ein Gerät aus dem Schnittstellenprozess.\nDer Parameter address ist die Addresse des zu löschenden Gerätes.\nflags ist ein bitweises oder folgender Werte:\n· 0x01=DELETE_FLAG_RESET\nDas Gerät wird vor dem Löschen in den Werkszustand zurückgesetzt\n· 0x02=DELETE_FLAG_FORCE\nDas Gerät wird auch gelöscht, wenn es nicht erreichbar ist\n· 0x04=DELETE_FLAG_DEFER\nWenn das Gerät nicht erreichbar ist, wird es bei nächster Gelegenheit gelöscht.",
					],
					'getDeviceDescription'=>[
						FUNCTION_NAME=>'getDeviceDescription',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'address',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[
								[VALUE_NAME=>'description',VALUE_TYPE=>DATATYPE_ARRAY]
							]
						],
						FUNCTION_DESC=>"Diese Methode gibt die Gerätebeschreibung des als address übergebenen Gerätes zurück.",
					],
					'changekey'=>[
						FUNCTION_NAME=>'changekey',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'passphrase',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>null
						],
						FUNCTION_DESC=>"Diese Methode ändert den vom Schnittstellenprozess verwendeten AES-Schlüssel. Der Schlüssel wird ebenfalls in allen angelernten Geräten getauscht.",
					],
					'setTempKey'=>[
						FUNCTION_NAME=>'setTempKey',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'passphrase',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>null
						],
						FUNCTION_DESC=>"Diese Methode ändert den von der CCU verwendeten temporären AES-Schlüssel. Der temporäre AES-Schlüssel wird verwendet, um ein Gerät anzulernen, in dem ein anderer Schlüssel gespeichert ist als der Schlüssel der CCU.",
					],
					'listTeams'=>[
						FUNCTION_NAME=>'listTeams',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>null,
							PARAMS_OUT=>[ [VALUE_NAME=>'DeviceDescriptions',VALUE_TYPE=>DATATYPE_ARRAY]]
						],		
						FUNCTION_DESC=>"Diese Methode gibt alle dem Schnittstellenprozess bekannten Teams in Form von Gerätebeschreibungen zurück.",
					],
					'setTeam'=>[
						FUNCTION_NAME=>'setTeam',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'address',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'team_address',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>null
						],
						FUNCTION_DESC=>"Diese Methode fügt den Kanal channel_address zum Team team_address hinzu.\nBei team_address=\"\" wird der Kanal channel_address seinem eigenen Team zugeordnet.\nDabei muss team_address entweder leer sein (\"\") oder eine Seriennummer eines existierenden Teams enthalten. Teams werden dabei je nach Bedarf erzeugt und gelöscht.",
					],
					'restoreConfigToDevice'=>[
						FUNCTION_NAME=>'restoreConfigToDevice',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[ [VALUE_NAME=>'address',VALUE_TYPE=>DATATYPE_STRING]],
							PARAMS_OUT=>null
						],
						FUNCTION_DESC=>"Diese Methode überträgt alle zu einem Gerät in der CCU gespeicherten Konfigurationsdaten erneut an das Gerät.",
					],
					'clearConfigCache'=>[
						FUNCTION_NAME=>'clearConfigCache',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[ [VALUE_NAME=>'address',VALUE_TYPE=>DATATYPE_STRING]],
							PARAMS_OUT=>null
						],
						FUNCTION_DESC=>"Diese Methode löscht alle zu einem Gerät in der CCU gespeicherten Konfigurationsdaten. Diese werden nicht sofort wieder vom Gerät abgefragt, sondern wenn sie das nächste mal benötigt werden.",
					],
					'rssiInfo'=>[
						FUNCTION_NAME=>'rssiInfo',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>null,
							PARAMS_OUT=>[ [VALUE_NAME=>'Info',VALUE_TYPE=>DATATYPE_ARRAY]]
						],
						FUNCTION_DESC=>"Gibt ein zweidimensionales assoziatives Array zurück, dessen Schlüssel die Geräteadressen sind. Die Felder des assoziativen Arrays sind Tupel, die die Empfangsfeldstärken zwischen beiden Schlüsselgeräten für beide Richtungen in dbm angeben. ein Wert von 65536 bedeutet, dass keine Informationen vorliegen.\n· Rückgabewert[Gerät 1][Gerät 2][0]\nEmpfangsfeldstärke an Gerät 1 für Sendungen von Gerät 2\n· Rückgabewert[Gerät 1][Gerät 2][1]\nEmpfangsfeldstärke an Gerät 2 für Sendungen von Gerät 1.",
					],
					'updateFirmware'=>[
						FUNCTION_NAME=>'updateFirmware',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[ [VALUE_NAME=>'devices',VALUE_TYPE=>DATATYPE_ARRAY]],
							PARAMS_OUT=>[ [VALUE_NAME=>'status',VALUE_TYPE=>DATATYPE_ARRAY]]
						],
						FUNCTION_DESC=>"Diese Methode führt ein Firmware-Update der in devices enthaltenen Geräte durch. Die Geräte werden durch Ihre jeweilige Seriennummer spezifiziert. Der Rückgabewert gibt für jedes Gerät an, ob das Firmware-Update erfolgreich war.",
					],
					'logLevel'=>[
						FUNCTION_NAME=>'logLevel',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[ [VALUE_NAME=>'level',VALUE_TYPE=>DATATYPE_INT]],
							PARAMS_OUT=>[[VALUE_NAME=>'level',VALUE_TYPE=>DATATYPE_INT]]
						],
						FUNCTION_DESC=>"Diese Methode gibt den aktuellen Log-Level zurück (1) bzw. setzt diesen (2). Folgende Werte sind für level möglich:\n· 6=LOG_FATAL_ERROR: Nur schwere Fehler loggen.\n· 5=LOG_ERROR: Zusätzlich normale Fehler loggen.\n· 4=LOG_WARNING: Zusätzlich Warnungen loggen.\n· 3=LOG_NOTICE: Zusätzlich Notizmeldungen loggen.\n· 2=LOG_INFO: Zusätzlich Infomeldungen loggen.\n· 1=LOG_DEBUG: Zusätzlich Debugmeldungen loggen.\n· 0=LOG_ALL: Alles wird geloggt.",
					],
					'reportValueUsage'=>[
						FUNCTION_NAME=>'reportValueUsage',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'address',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'value_id',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'ref_counter',VALUE_TYPE=>DATATYPE_INT]
							],
							PARAMS_OUT=>[ [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]]
						],
						FUNCTION_DESC=>"Diese Methode teilt dem Interfaceprozess in ref_counter mit, wie oft der Wert value_id des Kanals address innerhalb der Logikschicht (z.B. in Programmen) verwendet bwird. Dadurch kann der Interfaceprozess die Verbindung mit der entsprechenden Komponente herstellen bzw. löschen. Diese Funktion sollte bei jeder Änderung aufgerufen werden.\nDer Rückgabewert ist true, wenn die Aktion sofort durchgeführt wurde. Er ist false, wenn die entsprechende Komponente nicht erreicht werden konnte und vom Benutzer zunächst in den Config-Mode gebracht werden muss. Der Interfaceprozess hat dann aber die neue Einstellung übernommen und wird sie bei nächster Gelegenheit automatisch an die Komponente übertragen.\nIn diesem Fall ist dann auch der Wert CONFIG_PENDING im Kanal MAINTENANCE der Komponente gesetzt.",
					],
					'setBidcosInterface'=>[
						FUNCTION_NAME=>'setBidcosInterface',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'device_address',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'interface_address',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'rooming',VALUE_TYPE=>DATATYPE_BOOL]
							],
							PARAMS_OUT=>null
						],
						FUNCTION_DESC=>"Diese Methode setzt das für die Kommunikation mit dem durch device_address spezifizierten Gerät verwendete Bidcos-Interface. Die Seriennummer des in Zukunft für die Kommunikation mit diesem Gerät zu verwendenden Interfaces wird in interface_address übergeben. Ist der Parameter roaming gesetzt, so wird die Interfacezuordnung für das Gerät automatisch in Abhängigkeit von der Empfangsfeldstärke angepasst. Das ist für nicht ortsfeste Geräte wie Fernbedienungen sinnvoll.",
					],
					'listBidcosInterfaces'=>[
						FUNCTION_NAME=>'listBidcosInterfaces',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>null,
							PARAMS_OUT=>[ [VALUE_NAME=>'Interfaces',VALUE_TYPE=>DATATYPE_ARRAY]]
						],
						FUNCTION_DESC=>"Diese Methode gibt eine Liste aller vorhandenen BidCoS-Interfaces in Form eines Arrays von Structs zurück.\nDer Rückgabewert ist ein Array von Strukturen. Jede dieser Strukturen enthält die folgenden Felder:\n· ADDRESS\nDatentyp String. Seriennummer des BidCoS-Interfaces.\n· DESCRIPTION\nDatentyp String. Textuelle Beschreibung des Interfaces wie in der Konfigurationsdatei für den Schnittstellenprozess angegeben.\n· CONNECTED\nDatentyp Boolean. Gibt an, ob zum Zeitpunkt der Abfrage eine Kommunikationsverbindung zum Interface besteht.\n· DEFAULT\nDatentyp Boolean. Gibt an, ob es sich um das Standardinterface handelt. Das Standardinterface wird verwendet, wenn das einem Gerät zugeordnete Interface nicht mehr existiert.",
					],
					'getServiceMessages'=>[
						FUNCTION_NAME=>'getServiceMessages',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>null,
							PARAMS_OUT=>[ [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_ARRAY]]
						],		
						FUNCTION_DESC=>"Diese Methode gibt eine Liste aller vorhandenen Servicemeldungen in Form eines Arrays zurück.\nDer Rückgabewert ist ein Array mit einem Element pro Servicemeldung. Jedes Element ist wiederum ein Array mit drei Feldern:\n· Rückgabewert[index][0]\nDatentyp String. Adresse (Seriennummer) des Kanals, der die Servicemeldung generiert hat\n· Rückgabewert[index][1]\nDatentyp String. ID der Servicemeldung (CONFIG_PENDING, UNREACH, etc.)\n· Rückgabewert[index][2]\nDatentyp variabel. Wert der Servicemeldung",
					],
					'getAllMetadata'=>[
						FUNCTION_NAME=>'getAllMetadata',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[ [VALUE_NAME=>'object_id',VALUE_TYPE=>DATATYPE_STRING]],
							PARAMS_OUT=>[ [VALUE_NAME=>'Struct',VALUE_TYPE=>DATATYPE_ARRAY]]
						],
						FUNCTION_DESC=>"Diese Methode gibt alle zuvor gesetzten Metadaten zu einem Objekt zurück.\nobject_id ist die Id des Metadaten-Objekts. Üblicherweise ist dies die Seriennummer eines Gerätes oder Kanals.\nDurch Übergabe einer beliebigen Id können aber auch eigene Metadaten-Objekte angelegt werden.\nDer Rückgabewert ist ein Struct, der zu jedem zuvor gesetzten Metadatum ein Feld enthält. Der Feldname ist der zuvor an setMetadata() als Parameter data_id übergebene Wert. Der Wert des Feldes entspricht in Datentyp und Wert der zuvor an setMetadata() als Parameter value übergebenen Variablen.",
					],
					'getMetadata'=>[
						FUNCTION_NAME=>'getMetadata',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'object_id',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'data_id',VALUE_TYPE=>DATATYPE_STRING]
							],
							PARAMS_OUT=>[[VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_MIXED]]
						],
						FUNCTION_DESC=>"Diese Methode gibt ein Metadatum zu einem Objekt zurück.\nobject_id ist die Id des Metadaten-Objekts. Üblicherweise ist dies die Seriennummer eines Gerätes oder Kanals. Durch Übergabe einer beliebigen Id können aber auch eigene Metadaten-Objekte angelegt werden.\ndata_id ist die Id des abzufragenden Metadatums. Diese Id kann frei gewählt werden. Der Rückgabewert entspricht in Datentyp und Wert der zuvor an setMetadata() als Parameter value übergebenen Variablen.",
					],
					'setMetadata'=>[
						FUNCTION_NAME=>'setMetadata',
						FUNCTION_PARAMS=>[	
							PARAMS_IN=>[
								[VALUE_NAME=>'object_id',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'data_id',VALUE_TYPE=>DATATYPE_STRING],
								[VALUE_NAME=>'value',VALUE_TYPE=>DATATYPE_MIXED]
							],
							PARAMS_OUT=>null
						],
						FUNCTION_DESC=>"Diese Methode setzt ein Metadatum zu einem Objekt.\nobject_id ist die Id des Metadaten-Objekts. Üblicherweise ist dies die Seriennummer eines Gerätes oder Kanals. Durch Übergabe einer beliebigen Id können aber auch eigene Metadaten-Objekte angelegt werden.\ndata_id ist die Id des zu setzenden Metadatums. Diese Id kann frei gewählt werden.\nvalue ist eine beliebige Variable. Diese wird gespeichert und kann später mittels getMetadata() und getAllMetadata()wieder abgefragt werden.",
					]
				]		
			]
		]
	]		
];

?>