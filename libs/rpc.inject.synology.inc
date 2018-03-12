<?php
$RPC_Inject= [
  [
    INJECT_MODELS  => null,
    INJECT_INFO    => [ "Synology","DSM6x","http",5000,CONNECTION_TYPE_URL],
    INJECT_SERVICES=> [
      'SYNO.API.Info'=> [
        SERVICE_NAME=>'SYNO.API.Info',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This is the starter API which has its fixed URL path /webapi/query.cgi. The 3rd
party application must retrieve
other APIs\' information by calling “Query” method.',
        SERVICE_EXPORT=>['ERROR_CODES' =>[]],
        SERVICE_FUNCTIONS=>[
          'Query' => [
            FUNCTION_NAME=>'Query',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.API.Info'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Query'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'query',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Discover available APIs and corresponding information.

Example:
- Get information of SYNO.SurveillanceStation.Camera
call:<syno_url>/webapi/query.cgi?api=SYNO.API.Info&method=Query&version=1&query=SYNO.SurveillanceStation.Camera

- Get information of APIs with the prefix “SYNO.SurveillanceStation.”.
call:<syno_url>/webapi/query.cgi?api=SYNO.API.Info&method=Query&version=1&query=SYNO.SurveillanceStation

- Get information of SYNO.API.Auth and the APIs with the prefix “SYNO.SurveillanceStation.”.
call:<syno_url>/webapi/query.cgi?api=SYNO.API.Info&method=Query&version=1&query=SYNO.API.Auth,SYNO.SurveillanceStation
'
          ],
        ]
      ],
      'SYNO.API.Auth'=> [
        SERVICE_NAME=>'SYNO.API.Auth',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This is the starter API which has its fixed URL path /webapi/query.cgi. The 3rd
party application must retrieve
other APIs\' information by calling “Query” method.',
        SERVICE_EXPORT=>['ERROR_CODES' =>[100=>'Unknown error',101=>'The account parameter is not specified',400=>'Invalid password',401=>'Guest or disabled account',402=>'Permission denied',403=>'One time password not specified',404=>'One time password authenticate failed']],
        SERVICE_FUNCTIONS=>[
          'Login' => [
            FUNCTION_NAME=>'Login',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'account',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Login account name'],
                [VALUE_NAME=>'passwd',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Login account password'],
                [VALUE_NAME=>'session',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'SurveillanceStation'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.API.Auth'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Login'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'format',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'sid'],
                [VALUE_NAME=>'sid',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'session ID'],
                [VALUE_NAME=>'_sid',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Jn5dZ9aS95wh2']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'sid',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'Create new login session. Different accounts have different privilege settings; accounts in DSM admin group
will have the highest privilege to all camera, camera groups, and Slave DiskStations.

Example:
call:<syno_url>/webapi/auth.cgi?api=SYNO.API.Auth&method=Login&version=1&account=admin&passwd=123456&session=SurveillanceStation
call:<syno_url>/webapi/auth.cgi?api=SYNO.API.Auth&method=Login&version=2&account=admin&passwd=123456&session=SurveillanceStation&format=sid

Returns:
json:{"sid":"Jn5dZ9aS95wh2"}

- By sid format login and appending _sid to other webapi, the sessions would not affect each others anddifferent privilege settings can be applied simultaneously.
call:<syno_url>/webapi/camera.cgi?api=SYNO.SurveillanceStation.Camera&method=List&version=1&_sid=Jn5dZ9aS95wh2
'
          ],
          'Logout' => [
            FUNCTION_NAME=>'Logout',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'session',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'SurveillanceStation'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.API.Auth'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Logout'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'_sid',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Jn5dZ9aS95wh2']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Destroy current login session.

Example:
call:<syno_url>/webapi/auth.cgi?api=SYNO.API.Auth&method=Logout&version=1&session=SurveillanceStation
call:<syno_url>/webapi/auth.cgi?api=SYNO.API.Auth&method=Logout&version=2&session=SurveillanceStation&_sid=Jn5dZ9aS95wh2
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Info'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Info',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'API used to perform session login and logout.',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed']],
        SERVICE_FUNCTIONS=>[
          'GetInfo' => [
            FUNCTION_NAME=>'GetInfo',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Info'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetInfo'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'path',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'UI path to Surveillance Station'],
                [VALUE_NAME=>'customizedPortHttp',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'customizedPortHttps',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'cameraNumber',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The total number of installed cameras'],
                [VALUE_NAME=>'licenseNumber',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The total number of installed licenses'],
                [VALUE_NAME=>'maxCameraSupport',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Maximum number of camera support for this DS'],
                [VALUE_NAME=>'serial',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'DS serial number'],
                [VALUE_NAME=>'isAdmin',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Login user is admin or not'],
                [VALUE_NAME=>'userPriv',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'PRIV_AUTH_NO_ACCESS '],
                [VALUE_NAME=>'isLicenseEnough',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Is license enough or not'],
                [VALUE_NAME=>'allowSnapshot',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'If user has privilege to take snapshot or not'],
                [VALUE_NAME=>'allowManualRec',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'If user has privilege to do manual recording or'],
                [VALUE_NAME=>'allowDeleteRec',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'If user has privilege to delete recordings or not'],
                [VALUE_NAME=>'major',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Major version of Surveillance Station'],
                [VALUE_NAME=>'minor',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Minor version of Surveillance Station'],
                [VALUE_NAME=>'build',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Build number of Surveillance Station']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'path',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'customizedPortHttp',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'customizedPortHttps',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'cameraNumber',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'licenseNumber',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'maxCameraSupport',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'serial',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'userPriv',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'isLicenseEnough',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'allowSnapshot',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'allowManualRec',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'alllowDeleteRec',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get Surveillance Station related general information. If the user is logged in, the complete information is
provided. Otherwise only version and path information is sent.

Example:
call:<syno_url>/webapi/SurveillanceStation/info.cgi?api=SYNO.SurveillanceStation.Info&method=GetInfo&version=1

Returns:
json:{"version": {"major": 6,"minor": 0,"build": 2250},"path": "/webman/3rdparty/SurveillanceStation","customizedPortHttp": 9900,"customizedPortHttps": 9901,"cameraNumber": 20,"licenseNumber": 30,"maxCameraSupport": 40,"serial":"A1CDE23456","userPriv":1,"isLicenseEnough": 1,"allowSnapshot": true,"allowManualRec": true,"alllowDeleteRec": true}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Camera'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Camera',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides a method to acquire Surveillance Station related information such as package version,
package UI path, and the total number of camera and installed licenses.',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid',402=>'Camera disabled']],
        SERVICE_FUNCTIONS=>[
          'List' => [
            FUNCTION_NAME=>'List',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'start',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'blFromCamList',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'blIncludeDeletedCam',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'basic',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'streamInfo',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'blPrivilege',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'timestamp',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'cameras',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'delcam',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'keyUsedCnt',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'existCamMntTypeMap',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'Get the list of all cameras.

Returns:
json:{"timestamp": "1425362140","cameras": [{"audioOut": false,"recStatus": 0,"ptzCap": 0,"audioType": 2,"is_rotated_by_date": false,"quality": "5","blLiveviewPriv": true,"is_rotated_by_space": true,"privilege": 15,"folder": "/var/services/surveillance/ipux","type": 3,"recBitrateCtrl": 1,"vendor": "iPUX","stm_info": [{"resolution": "640x480","quality": "5","fps": 10,"type": 0},{"resolution": "640x480","quality": "5","fps": 10,"type": 1},{"resolution": "640x480","quality": "5","fps": 10,"type": 2}],"stmFisheyeType": 0,"dsPort": 5000,"camLiveMode": 0,"videoCapList": [{"stList": ["RTSP"],"vt": "H.264"},{"stList": ["RTSP"],"vt": "MPEG4"},{"stList": ["HTTP"],"vt": "MJPEG"}],"camMountType": 0,"tvStandard": 0,"singleStream": false,"name": "ipux","audioCap": true,"ownerDsId": 0,"enabled": false,"camIdOnRecServer": 0,"recCbrBitrate": 1000,"DONum": 0,"uiStmNoList": "1,1,1","status_flags": 0,"camFov": "","id": 95,"channel_id": "1","isStatusUnrecognized": false,"port": 80,"deleted": true,"deviceType": 1,"fps": 10,"hasCamParam": false,"status": 7,"update_time": 1425349477,"snapshot_path": "&camStm=1","dsIp": "","rotation_option": 0,"host": "10.13.11.195","rotation_by_date": 30,"presetNum": 0,"camStatus": 2,"volume_space": "0.047","camPath": "cnRzcDovL2FkbWluOmFkbWluQDEwLjEzLjExLjE5NTo1NTQvbXBlZzQ=","rotation_by_space": "10","model": "ICS8220","resolution": "640x480"},{"audioOut": false,"recStatus": 1,"ptzCap": 3,"audioType": 2,"is_rotated_by_date": false,"quality": "5","blLiveviewPriv": true,"is_rotated_by_space": true,"privilege": 15,"folder": "/var/services/surveillance/ssss","type": 2,"recBitrateCtrl": 1,"vendor": "ABUS","stm_info": [{"resolution": "640x480","quality": "5","fps": 10,"type": 0},{"resolution": "640x480","quality": "5","fps": 10,"type": 1},{"resolution": "640x480","quality": "5","fps": 10,"type": 2}],"stmFisheyeType": 0,"dsPort": 5000,"camLiveMode": 0,"videoCapList": [{"stList": ["RTSP"],"vt": "MPEG4"},{"stList": ["HTTP"],"vt": "MJPEG"}],"camMountType": 0,"tvStandard": 0,"singleStream": false,"name": "ssss","audioCap": true,"ownerDsId": 1,"enabled": true,"camIdOnRecServer": 24,"recCbrBitrate": 1000,"DONum": 0,"uiStmNoList": "1,1,1","status_flags": 0,"camFov": "","id": 96,"channel_id": "1","isStatusUnrecognized": false,"port": 80,"deleted": false,"deviceType": 4,"fps": 10,"hasCamParam": true,"status": 0,"update_time": 1425360858,"snapshot_path":"/webapi/_______________________________________________________entry.cgi?api=SYNO.SurveillanceStation.Camera","dsIp": "10.13.1.107","rotation_option": 0,"host": "10.13.12.200","rotation_by_date": 30,"presetNum": 32,"camStatus": 1,"volume_space": "0.590","camPath":"cnRzcDovL2FkbWluOnN5bm9zc0AxMC4xMy4xMi4yMDA6NTU0L3ZpZGVvLm1wNA==","rotation_by_space": "10","model": "TVIP20000","resolution": "640x480"}],"delcam": [],"total": 2,"keyUsedCnt": 1,"existCamMntTypeMap": "null"}
'
          ],
          'GetInfo' => [
            FUNCTION_NAME=>'GetInfo',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The list of '],
                [VALUE_NAME=>'blIncludeDeletedCam',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'basic',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'streamInfo',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'optimize',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'ptz',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'eventDetection',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'deviceOutCap',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'fisheye',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'camAppInfo',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get specific camera settings.
'
          ],
          'ListGroup' => [
            FUNCTION_NAME=>'ListGroup',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'offset',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>10],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>3],
                [VALUE_NAME=>'_method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ListGroup'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'8']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'offset',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'cameraGroups',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Get all camera group information.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_method="ListGroup"&api="SYNO.SurveillanceStation.Camera"&limit=3&version="8"&offset=10

Returns:
json:{"total": 38,"offset": 10,"cameraGroups": [{"id": 1,"name": "Group 1F","cameraIds": [10,11,12],"description": "The camera group of 1F cameras","privilege": 10}, {"id": 2,"name": "Group 2F","cameraIds": [13,14,15],"description": "The camera group of 2F cameras","privilege": 3}, {"id": 3,"name": "Group 3F","cameraIds": [16,17],"description": "The camera group of 3F cameras","privilege": 15}]}
'
          ],
          'GetSnapshot' => [
            FUNCTION_NAME=>'GetSnapshot',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID'],
                [VALUE_NAME=>'preview',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'camStm',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'_camStm',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'8'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'_GetSnapshot']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get the up-to-date snapshot of the selected camera in JPEG format.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_camStm=1&version="8"&cameraId=18&api="SYNO.SurveillanceStation.Camera"&preview=true&method="_GetSnapshot"
'
          ],
          'Enable' => [
            FUNCTION_NAME=>'Enable',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The list of ']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'delcam',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'camera',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'keyUsedCnt',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Enable cameras.

Returns:
json:{"delcam": [],"camera": [{"audioOut": true,"mute": false,"recStatus": 0,"analyticsType": 0,"ptzCap": 111,"audioType": 4,"ptzContinuous": 39,"is_rotated_by_date": false,"ADCap": 5,"blLiveviewPriv": true,"videoCapList": [{"stList": ["RTSP"],"vt": "H.264"},{"stList": ["HTTP"],"vt": "MJPEG"}],"isStatusUnrecognized": false,"is_rotated_by_space": true,"osd_position": 6,"fisheyeType": 0,"recBitrateCtrl": 1,"video_mirror": true,"vendor": "AXIS","video_rotation": 0,"stmFisheyeType": 0,"dsPort": 5000,"TDCap": 0,"camMountType": 0,"exposure_control": 6,"name": "P5415-E-0011","ownerDsId": 0,"osd_status": true,"fisheyeDispMode": "","channel_id": "1","setDICap": false,"param_chklist": 0,"analyticsBeep": false,"status_flags": 32,"feRegionList": [],"objTrack": false,"port": 80,"video_flip": true,"osd_format": 0,"autoPan": 0,"deleted": false,"deviceType": 4,"hasCamParam": true,"DINum": 2,"snapshot_path":"/webapi/_______________________________________________________entry.cgi?api=SYNO.SurveillanceStation.Camera&method=GetSnapshot&version=1&cameraId=70×tamp=1424919129&preview=true","dsIp": "","host": "10.13.12.165","rotation_by_date": 30,"autoFocus": true,"volume_space": "9.617","analyticsHeight": 0,"MDCap": 5,"rotation_by_space": "10","analyticsVirtualFence": false,"analyticsObjSize": 0,"analyticsDirection": 1,"analyticsRegion": "","quality": "5","multiDI": false,"type": 3,"stm_info": [{"resolution": "800x450","quality": "5","fps": 10,"type": 0},{"resolution": "800x450","quality": "5","fps": 10,"type": 1},{"resolution": "800x450","quality": "5","fps": 10,"type": 2}],"camLiveMode": 0,"volume": 50,"ptzDirection": 32,"dayend": 18,"folder": "/var/services/surveillance/P5415-E-001","tvStandard": 0,"singleStream": false,"imageEnhancement": {"saturation": 0,"sharpness": 0,"contrast": 0,"brightness": 0},"audioCap": true,"camIdOnRecServer": 0,"detailInfo": {"camRecTime": 30,"edgeStgEnabled": false,"camIsRotByDate": false,"camRecStreamNo": 0,"camRotByDate": 30,"camTvStandard": 0,"camMountType": 0,"camRecCbrBitrate": 1000,"camIsRotBySpace": true,"camLiveResolution": "800x450","camRecBitrateCtrl": 1,"camMobileStreamNo": 0,"camLiveQuality": "5","edgeStgRecStopTime": "23:59","camLiveStreamNo": 0,"camResolution": "800x450","camMobileQuality": "5","camLiveMode": 0,"camPreRecTime": 5,"camIP": "10.13.12.165","camLiveBitrateCtrl": 1,"camModel": "P5415-E","edgeStgRecStartTime": "00:00","camPostRecTime": 5,"camLiveCbrBitrate": 1000,"camMobileResolution": "800x450","camUserName": "root","camFps": 10,"camVendor": "AXIS","camMobileFps": 10,"camFov": "","camMobileCbrBitrate": 1000,"camRotBySpace": 10,"edgeStgDownloadSch":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"customAlarmDetect": 3,"camLiveFps": 10,"camChannel": "1","camRotOption": 0,"camQuality": "5","camPrefix": "P5415-E-001","edgeStgRetrieveAction": 2,"camSchedule":[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1],"edgeStgRecMode": 2,"customDetect": 5,"camPassWord": "%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83","camVideoType": "H.264","camMobileBitrateCtrl": 1,"MdDetSrc": 0,"camServer": 0,"camFolder": "P5415-E-001","edgeStgRecDays": 127,"camPort": 80,"camRtspProtocol": 4,"camName": "P5415-E-0011","camAudioType": 4},"enabled": true,"analyticsSens": 1,"analyticsWidth": 0,"recCbrBitrate": 1000,"blPresetSpeed": false,"DONum": 0,"id": 70,"setDOCap": false,"analyticsFrame": true,"analyticsLine": false,"daybegin": 8,"fps": 10,"status": 1,"update_time": 1424919129,"exposure_mode": 4,"time_server": "","rotation_option": 0,"presetNum": 32,"camStatus": 9,"analyticsDwellTime": 5,"camPath":"cnRzcDovL3Jvb3Q6bmF0bWFzdGVyQDEwLjEzLjEyLjE2NTo1NTQvYXhpcy1tZWRpYS9tZWRpYS5hbXA/Y2FtZXJhPTEmdmlkZW9jb2RlYz1oMjY0JnJlc29sdXRpb249ODAweDQ1MCZmcHM9MTAmdmlkZW9rZXlmcmFtZWludGVydmFsPTEwJmNvbXByZXNzaW9uPTIw","model": "P5415-E","resolution": "800x450"}],"keyUsedCnt": 8}
'
          ],
          'Disable' => [
            FUNCTION_NAME=>'Disable',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The list of ']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'delcam',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'camera',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'keyUsedCnt',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Disable cameras.

Returns:
json:{"data": {"delcam": [],"camera": [{"audioOut": true,"mute": false,"recStatus": 0,"analyticsType": 0,"ptzCap": 111,"audioType": 4,"ptzContinuous": 39,"is_rotated_by_date": false,"ADCap": 5,"blLiveviewPriv": true,"videoCapList": [{"stList": ["RTSP"],"vt": "H.264"},{"stList": ["HTTP"],"vt": "MJPEG"}],"isStatusUnrecognized": false,"is_rotated_by_space": true,"osd_position": 6,"fisheyeType": 0,"recBitrateCtrl": 1,"video_mirror": true,"vendor": "AXIS","video_rotation": 0,"stmFisheyeType": 0,"dsPort": 5000,"TDCap": 0,"camMountType": 0,"exposure_control": 6,"name": "P5415-E-0011","ownerDsId": 0,"osd_status": true,"fisheyeDispMode": "","channel_id": "1","setDICap": false,"param_chklist": 0,"analyticsBeep": false,"status_flags": 32,"feRegionList": [],"objTrack": false,"port": 80,"video_flip": true,"osd_format": 0,"autoPan": 0,"deleted": false,"deviceType": 4,"hasCamParam": true,"DINum": 2,"snapshot_path":"/webapi/_______________________________________________________entry.cgi?api=SYNO.SurveillanceStation.Camera&method=GetSnapshot&version=1&cameraId=70×tamp=1424919129&preview=true","dsIp": "","host": "10.13.12.165","rotation_by_date": 30,"autoFocus": true,"volume_space": "9.617","analyticsHeight": 0,"MDCap": 5,"rotation_by_space": "10","analyticsVirtualFence": false,"analyticsObjSize": 0,"analyticsDirection": 1,"analyticsRegion": "","quality": "5","multiDI": false,"type": 3,"stm_info": [{"resolution": "800x450","quality": "5","fps": 10,"type": 0},{"resolution": "800x450","quality": "5","fps": 10,"type": 1},{"resolution": "800x450","quality": "5","fps": 10,"type": 2}],"camLiveMode": 0,"volume": 50,"ptzDirection": 32,"dayend": 18,"folder": "/var/services/surveillance/P5415-E-001","tvStandard": 0,"singleStream": false,"imageEnhancement": {"saturation": 0,"sharpness": 0,"contrast": 0,"brightness": 0},"audioCap": true,"camIdOnRecServer": 0,"detailInfo": {"camRecTime": 30,"edgeStgEnabled": false,"camIsRotByDate": false,"camRecStreamNo": 0,"camRotByDate": 30,"camTvStandard": 0,"camMountType": 0,"camRecCbrBitrate": 1000,"camIsRotBySpace": true,"camLiveResolution": "800x450","camRecBitrateCtrl": 1,"camMobileStreamNo": 0,"camLiveQuality": "5","edgeStgRecStopTime": "23:59","camLiveStreamNo": 0,"camResolution": "800x450","camMobileQuality": "5","camLiveMode": 0,"camPreRecTime": 5,"camIP": "10.13.12.165","camLiveBitrateCtrl": 1,"camModel": "P5415-E","edgeStgRecStartTime": "00:00","camPostRecTime": 5,"camLiveCbrBitrate": 1000,"camMobileResolution": "800x450","camUserName": "root","camFps": 10,"camVendor": "AXIS","camMobileFps": 10,"camFov": "","camMobileCbrBitrate": 1000,"camRotBySpace": 10,"edgeStgDownloadSch":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"customAlarmDetect": 3,"camLiveFps": 10,"camChannel": "1","camRotOption": 0,"camQuality": "5","camPrefix": "P5415-E-001","edgeStgRetrieveAction": 2,"camSchedule":[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1],"edgeStgRecMode": 2,"customDetect": 5,"camPassWord": "%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83","camVideoType": "H.264","camMobileBitrateCtrl": 1,"MdDetSrc": 0,"camServer": 0,"camFolder": "P5415-E-001","edgeStgRecDays": 127,"camPort": 80,"camRtspProtocol": 4,"camName": "P5415-E-0011","camAudioType": 4},"enabled": true,"analyticsSens": 1,"analyticsWidth": 0,"recCbrBitrate": 1000,"blPresetSpeed": false,"DONum": 0,"id": 70,"setDOCap": false,"analyticsFrame": true,"analyticsLine": false,"daybegin": 8,"fps": 10,"status": 1,"update_time": 1424919129,"exposure_mode": 4,"time_server": "","rotation_option": 0,"presetNum": 32,"camStatus": 9,"analyticsDwellTime": 5,"camPath":"cnRzcDovL3Jvb3Q6bmF0bWFzdGVyQDEwLjEzLjEyLjE2NTo1NTQvYXhpcy1tZWRpYS9tZWRpYS5hbXA/Y2FtZXJhPTEmdmlkZW9jb2RlYz1oMjY0JnJlc29sdXRpb249ODAweDQ1MCZmcHM9MTAmdmlkZW9rZXlmcmFtZWludGVydmFsPTEwJmNvbXByZXNzaW9uPTIw","model": "P5415-E","resolution": "800x450"}],"keyUsedCnt": 8},"success": true}
'
          ],
          'GetCapabilityByCamId' => [
            FUNCTION_NAME=>'GetCapabilityByCamId',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ptzHome',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'ptzPresetNumber',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'ptzPan',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'ptzTilt',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'ptzZoom',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'ptzIris',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'ptzFocus',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'ptzAbs',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'ptzAutoFocus',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'ptzDirection',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'ptzSpeed',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'audioOut',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get capability of a specific camera by its camera Id.

Returns:
json:{"ptzHome":false,"ptzPresetNumber":255,"ptzPan":2,"ptzTilt":2,"ptzZoom":2,"ptzIris":0,"ptzFocus":2,"ptzAbs":false,"ptzAutoFocus":false,"ptzDirection":8,"ptzSpeed":true,"audioOut":true}
'
          ],
          'MigrationEnum' => [
            FUNCTION_NAME=>'MigrationEnum',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'start',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The beginning of the enumeration list'],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Amount of the events for showing'],
                [VALUE_NAME=>'ownerDsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The source DS ID in the CMS table'],
                [VALUE_NAME=>'_version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'8'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Migration_Enum']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Enumerates current status of migration

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_version="8"&ownerDsId=0&start=0&api="SYNO.SurveillanceStation.Camera"&limit=1&method="Migration_Enum"
'
          ],
          'Migrate' => [
            FUNCTION_NAME=>'Migrate',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'serverId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The destination of the migration'],
                [VALUE_NAME=>'camIdList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'List of To'],
                [VALUE_NAME=>'delMode',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Choosing to preserve'],
                [VALUE_NAME=>'_delMode',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>2],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'8'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'_Migrate']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Migrating Cameras and recorded video (optional) to specified DS

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_delMode=2&camIdList="45"&version="8"&api="SYNO.SurveillanceStation.Camera"&serverId=1&method="_Migrate"
'
          ],
          'CountByCategory' => [
            FUNCTION_NAME=>'CountByCategory',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'start',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The starting offset in the list of cameras'],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The number of cameras to show'],
                [VALUE_NAME=>'_start',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'8'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'CountByCategory']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'cam_brand',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'cam_group',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'cam_status',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'server',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'cam_device',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'This method lists groups along with number of cameras which belong to according to specified criteria

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_start=0&api="SYNO.SurveillanceStation.Camera"&limit=0&version="8"&method="CountByCategory"

Returns:
json:{"cam_brand": [{"id": "-1: 2"},{"id": "ABUS: 1"},{"id": "iPUX: 1"}],"cam_group": [{"id": "-1: 2"},{"id": "1: 1"}],"cam_status": [{"id": "-1: 2"},{"id": "1: 1"},{"id": "12: 0"},{"id": "2: 0"},{"id": "3: 0"},{"id": "7: 1"}],"server": [{"id": "-1: 2"},{"id": "0: 2"},{"id": "1: 0"}],"cam_device": [{"id": "-1: 2"},{"id": "1: 1"},{"id": "2: 0"},{"id": "4: 1"},{"id": "8: 0"}],"total": 2}
'
          ],
          'RecountEventSize' => [
            FUNCTION_NAME=>'RecountEventSize',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Specified cam'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'8'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'RecountEventSize']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'size',VALUE_TYPE=>DATATYPE_FLOAT]
              ],
            ],
            FUNCTION_DESC=>'To activate the "getOccupiedSize" API for real-time size-calculating.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Camera"&camId=27&version="8"&method="RecountEventSize"

Returns:
json:{"size": 207.3548631668091}
'
          ],
          'SaveOptimizeParam' => [
            FUNCTION_NAME=>'SaveOptimizeParam',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraIds',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Specified cam'],
                [VALUE_NAME=>'camParamChkList',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1],
                [VALUE_NAME=>'timeServer',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'pool.ntp.org'],
                [VALUE_NAME=>'vdoMirror',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'vdoFlip',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'vdoRotation',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'expCtrl',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>2],
                [VALUE_NAME=>'expMode',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'dayBegin',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>8],
                [VALUE_NAME=>'dayEnd',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>18],
                [VALUE_NAME=>'osdStatus',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'osdFormat',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'osdPosition',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'_vdoMirror',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'8'],
                [VALUE_NAME=>'exp_Mode',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SaveOptimizeParam']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'This API is able to save the configuration of Optimization panel

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_vdoMirror=true&camParamChkList=1&version="8"&vdoRotation=false&vdoFlip=true&osdPosition=0&exp_Mode=0&timeServer="pool.ntp.org"&cameraIds=28&api="SYNO.SurveillanceStation.Camera"&osdStatus=f_alse&osdFormat=0&dayEnd=18&expCtrl=2&method="SaveOptimizeParam"&dayBegin=8

Returns:
json:{"id": 28}
'
          ],
          'GetOccupiedSize' => [
            FUNCTION_NAME=>'GetOccupiedSize',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camIdList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The list of demanded cameras'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'8'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetOccupiedSize']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>104,VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>99,VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'This method returns the occupied volume in GigaBytes

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Camera"&camIdList="46,48"&version="8"&method="GetOccupiedSize"

Returns:
json:{"104":"9.961","99":"9.955"}
'
          ],
          'CheckCamValid' => [
            FUNCTION_NAME=>'CheckCamValid',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Specified Cam ID'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'8'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'CheckCamValid']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'categEntryVal',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'itemStatus',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Check if the shortcut item is valid

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Camera"&version="8"&cameraId=27&method="CheckCamValid"

Returns:
json:{"categEntryVal": "iPUX","itemStatus": 0}
'
          ],
          'MigrationCancel' => [
            FUNCTION_NAME=>'MigrationCancel',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'taskIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'To'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'8'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'MigrationCancel']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Deleting selected tasks

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Camera"&taskIds="1,4,7"&version="8"&method="MigrationCancel"
'
          ],
          'Delete' => [
            FUNCTION_NAME=>'Delete',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Selected cameras that are to be deleted'],
                [VALUE_NAME=>'keepEvents',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'To delete recorded videos or not'],
                [VALUE_NAME=>'_cameraIds',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'18'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'8'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Delete']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'delcam',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'camera',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'keyUsedCnt',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Deleting selected tasks

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_cameraIds="18"&keepEvents=0&api="SYNO.SurveillanceStation.Camera"&version="8"&method="Delete"

Returns:
json:{"delcam": [],"camera": [],"keyUsedCnt": 0}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.Surveillance.Camera.Event'=> [
        SERVICE_NAME=>'SYNO.Surveillance.Camera.Event',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides a set of methods to acquire camera-related information and to enable/disable cameras.',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid',402=>'Camera disabled',407=>'CMS closed']],
        SERVICE_FUNCTIONS=>[
          'AudioEnum' => [
            FUNCTION_NAME=>'AudioEnum',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The selected camera id'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera.Event'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'AudioEnum']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ADParam',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Enumerate audio detection parameters. e.g. keep setting, detect source, level(capability, minimum value,
maximum value and current value).

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Camera.Event"&camId=280&version="1"&method="AudioEnum"

Returns:
json:{"ADParam": {"source": 0,"level": {"cap": true,"maxValue": 100,"value": 50,"minValue": 0},"keep": true}}
'
          ],
          'AlarmEnum' => [
            FUNCTION_NAME=>'AlarmEnum',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The selected camera id'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera.Event'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'AlarmEnum']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'DIParam',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Enumerate alarm setting list. e.g. normal and keep

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Camera.Event"&camId=280&version="1"&method="AlarmEnum"

Returns:
json:{"DIParam": [{"normal": 0,"idx": 0,"keep": true}]}
'
          ],
          'TamperingEnum' => [
            FUNCTION_NAME=>'TamperingEnum',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The selected camera id'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera.Event'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'TamperingEnum']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'TDParam',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Enumerate tampering detection parameters. e.g. keep setting, detect source, duration(capability, minimum
value, maximum value and current value).

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Camera.Event"&camId=280&version="1"&method="TamperingEnum"

Returns:
json:{"TDParam": {"duration": {"cap": true,"maxValue": 100,"value": 50,"minValue": 0},"source": 0,"keep": true}}
'
          ],
          'MDParamSave' => [
            FUNCTION_NAME=>'MDParamSave',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The selected camera id'],
                [VALUE_NAME=>'source',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'keep',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'sensitivity',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>90],
                [VALUE_NAME=>'threshold',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'objectSize',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'percentage',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>50],
                [VALUE_NAME=>'_objectSize',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>50],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera.Event'],
                [VALUE_NAME=>'thresh_old',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>20],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'MDParamSave']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Save motion detection parameters. e.g. Sensitivity, Threshold, objectSize and percentage.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_objectSize=50&source=0&camId=280&version="1"&api="SYNO.SurveillanceStation.Camera.Event"&thresh_old=20&sensitivity=90&percentage=50&method="MDParamSave"&keep=true

Returns:
json:{"camId": 280}
'
          ],
          'MotionEnum' => [
            FUNCTION_NAME=>'MotionEnum',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The selected camera id'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera.Event'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'MotionEnum']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'MDParam',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Enumerate motion detection parameters. e.g. keep setting, detect source, sensitivity, threshold, objectSize
and percentage.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Camera.Event"&camId=280&version="1"&method="MotionEnum"

Returns:
json:{"MDParam": {"objectSize": {"camCap": true,"minValue": 0,"maxValue": 100,"value": 50,"ssCap": false},"sensitivity": {"camCap": true,"minValue": 0,"maxValue": 100,"value": 50,"ssCap": true},"keep": true,"source": 0,"threshold": {"camCap": false,"minValue": 0,"maxValue": 100,"value": 50,"ssCap": true},"percentage": {"camCap": true,"minValue": 0,"maxValue": 100,"value": 50,"ssCap": false}}}
'
          ],
          'ADParamSave' => [
            FUNCTION_NAME=>'ADParamSave',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The selected camera id'],
                [VALUE_NAME=>'source',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'keep',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'level',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>90],
                [VALUE_NAME=>'_camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>280],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera.Event'],
                [VALUE_NAME=>'_method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ADParamSave']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Save motion detection parameters. e.g. Sensitivity, Threshold, objectSize and percentage.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_camId=280&version="1"&level=90&keep=true&source=0&api="SYNO.SurveillanceStation.Camera.Event"&_method="ADParamSave"

Returns:
json:{"camId": 280}
'
          ],
          'DIParamSave' => [
            FUNCTION_NAME=>'DIParamSave',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The selected camera id'],
                [VALUE_NAME=>'idx',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Define digital input index'],
                [VALUE_NAME=>'keep',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'normal',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'_camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>280],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera.Event'],
                [VALUE_NAME=>'me_thod',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'DIParamSave']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Save digital input parameters. e.g. normal

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_camId=280&version="1"&idx=0&normal=0&keep=true&api="SYNO.SurveillanceStation.Camera.Event"&me_thod="DIParamSave"

Returns:
json:{"camId": 280}
'
          ],
          'AlarmStsPolling' => [
            FUNCTION_NAME=>'AlarmStsPolling',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The selected camera id'],
                [VALUE_NAME=>'timeOut',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>5000],
                [VALUE_NAME=>'idx',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The index of alarm'],
                [VALUE_NAME=>'trigger',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The trigger state of this alarm'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'list',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'[{"trigger": 0, "idx":_0}]'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera.Event'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'AlarmStsPolling']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'trigger',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'idx',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'list',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Enumerate alarm trigger state list.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?camId=280&version="1"&list=[{"trigger": 0, "idx":_0}]&api="SYNO.SurveillanceStation.Camera.Event"&timeOut=5000&method="AlarmStsPolling"
json:{"trigger": 0, "idx":0}

Returns:
json:{"list": [{"trigger": 0,"idx": 0}]}
'
          ],
          'TDParamSave' => [
            FUNCTION_NAME=>'TDParamSave',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The selected camera id'],
                [VALUE_NAME=>'source',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'keep',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'duration',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'_camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>280],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera.Event'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'TDParamSave']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Save tampering detection parameters. e.g. Duration

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_camId=280&api="SYNO.SurveillanceStation.Camera.Event"&keep=true&source=0&version="1"&duration=_90&method="TDParamSave"

Returns:
json:{"camId": 280}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Camera.Group'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Camera.Group',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides operations for camera groups',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid',402=>'Camera disabled',407=>'CMS closed']],
        SERVICE_FUNCTIONS=>[
          'Save' => [
            FUNCTION_NAME=>'Save',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camServerId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The privilege type in enumeration groups'],
                [VALUE_NAME=>'shareName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The share folder which contains the archive'],
                [VALUE_NAME=>'archiveName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The name of the archive'],
                [VALUE_NAME=>'actFromHost',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Is the method called by CMS host'],
                [VALUE_NAME=>'channel',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Channel of the camera'],
                [VALUE_NAME=>'checked',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Indicating if the camera is chosen'],
                [VALUE_NAME=>'enable_quota',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Indicating if the replacing strategy'],
                [VALUE_NAME=>'folder',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Path to the recorded video'],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Camera id'],
                [VALUE_NAME=>'idx',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The index in the archive'],
                [VALUE_NAME=>'ipaddr',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'ip address of the camera'],
                [VALUE_NAME=>'model',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The camera model'],
                [VALUE_NAME=>'name',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The name of the camera'],
                [VALUE_NAME=>'port',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Port of the Camera'],
                [VALUE_NAME=>'rotatequota',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Parameter of the replacing strategy if'],
                [VALUE_NAME=>'vendor',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Vendor of the camera'],
                [VALUE_NAME=>'_archiveName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SSCamExport_XD'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO._SurveillanceStation.Camera.Import'],
                [VALUE_NAME=>'camlist',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'[{"model": "ICS8220", "vendor": "iPUX",_"enable_quota": true, "rotatequota": 10, "id": 5, "checked": true, "name": "rbxTesting-001", "idx": 0,_"ipaddr": "10.13.11.189", "port": 80, "folder": "rbxTesting-001", "channel": "1"}]'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Save']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'model',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'vendor',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'enable_quota',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'rotatequota',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'checked',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'name',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'idx',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'ipaddr',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'port',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'folder',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'channel',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camera',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'keyUsedCnt',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Enumerate group information. e.g. Group list and camera list in the group.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_archiveName="SSCamExport_XD"&version="1"&camServerId=0&shareName="kuoDownload"&api="SYNO._SurveillanceStation.Camera.Import"&actFromHost=false&camlist=[{"model": "ICS8220", "vendor": "iPUX",_"enable_quota": true, "rotatequota": 10, "id": 5, "checked": true, "name": "rbxTesting-001", "idx": 0,_"ipaddr": "10.13.11.189", "port": 80, "folder": "rbxTesting-001", "channel": "1"}]&method="Save"
json:{"model": "ICS8220", "vendor": "iPUX","enable_quota": true, "rotatequota": 10, "id": 5, "checked": true, "name": "rbxTesting-001", "idx": 0,"ipaddr": "10.13.11.189", "port": 80, "folder": "rbxTesting-001", "channel": "1"}

Returns:
json:{"camera": [{"audioOut": true,"mute": false,"recStatus": 0,"analyticsType": 0,"ptzCap": 111,"audioType": 4,"ptzContinuous": 39,"is_rotated_by_date": false,"ADCap": 5,"blLiveviewPriv": true,"videoCapList": [{"stList": ["RTSP"],"vt": "H.264"},{"stList": ["HTTP"],"vt": "MJPEG"}],"isStatusUnrecognized": false,"is_rotated_by_space": true,"osd_position": 6,"fisheyeType": 0,"recBitrateCtrl": 1,"video_mirror": true,"vendor": "AXIS","video_rotation": 0,"stmFisheyeType": 0,"dsPort": 5000,"TDCap": 0,"camMountType": 0,"exposure_control": 6,"name": "P5415-E-0011","ownerDsId": 0,"osd_status": true,"fisheyeDispMode": "","channel_id": "1","setDICap": false,"param_chklist": 0,"analyticsBeep": false,"status_flags": 32,"feRegionList": [],"objTrack": false,"port": 80,"video_flip": true,"osd_format": 0,"autoPan": 0,"deleted": false,"deviceType": 4,"hasCamParam": true,"DINum": 2,"snapshot_path":"/webapi/_______________________________________________________entry.cgi?api=SYNO.SurveillanceStation.Camera&method=GetSnapshot&version=1&cameraId=70×tamp=1424919129&preview=true","dsIp": "","host": "10.13.12.165","rotation_by_date": 30,"autoFocus": true,"volume_space": "9.617","analyticsHeight": 0,"MDCap": 5,"rotation_by_space": "10","analyticsVirtualFence": false,"analyticsObjSize": 0,"analyticsDirection": 1,"analyticsRegion": "","quality": "5","multiDI": false,"type": 3,"stm_info": [{"resolution": "800x450","quality": "5","fps": 10,"type": 0},{"resolution": "800x450","quality": "5","fps": 10,"type": 1},{"resolution": "800x450","quality": "5","fps": 10,"type": 2}],"camLiveMode": 0,"volume": 50,"ptzDirection": 32,"dayend": 18,"folder": "/var/services/surveillance/P5415-E-001","tvStandard": 0,"singleStream": false,"imageEnhancement": {"saturation": 0,"sharpness": 0,"contrast": 0,"brightness": 0},"audioCap": true,"camIdOnRecServer": 0,"detailInfo": {"camRecTime": 30,"edgeStgEnabled": false,"camIsRotByDate": false,"camRecStreamNo": 0,"camRotByDate": 30,"camTvStandard": 0,"camMountType": 0,"camRecCbrBitrate": 1000,"camIsRotBySpace": true,"camLiveResolution": "800x450","camRecBitrateCtrl": 1,"camMobileStreamNo": 0,"camLiveQuality": "5","edgeStgRecStopTime": "23:59","camLiveStreamNo": 0,"camResolution": "800x450","camMobileQuality": "5","camLiveMode": 0,"camPreRecTime": 5,"camIP": "10.13.12.165","camLiveBitrateCtrl": 1,"camModel": "P5415-E","edgeStgRecStartTime": "00:00","camPostRecTime": 5,"camLiveCbrBitrate": 1000,"camMobileResolution": "800x450","camUserName": "root","camFps": 10,"camVendor": "AXIS","camMobileFps": 10,"camFov": "","camMobileCbrBitrate": 1000,"camRotBySpace": 10,"edgeStgDownloadSch":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"customAlarmDetect": 3,"camLiveFps": 10,"camChannel": "1","camRotOption": 0,"camQuality": "5","camPrefix": "P5415-E-001","edgeStgRetrieveAction": 2,"camSchedule":[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1],"edgeStgRecMode": 2,"customDetect": 5,"camPassWord": "%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83","camVideoType": "H.264","camMobileBitrateCtrl": 1,"MdDetSrc": 0,"camServer": 0,"camFolder": "P5415-E-001","edgeStgRecDays": 127,"camPort": 80,"camRtspProtocol": 4,"camName": "P5415-E-0011","camAudioType": 4},"enabled": true,"analyticsSens": 1,"analyticsWidth": 0,"recCbrBitrate": 1000,"blPresetSpeed": false,"DONum": 0,"id": 70,"setDOCap": false,"analyticsFrame": true,"analyticsLine": false,"daybegin": 8,"fps": 10,"status": 1,"update_time": 1424919129,"exposure_mode": 4,"time_server": "","rotation_option": 0,"presetNum": 32,"camStatus": 9,"analyticsDwellTime": 5,"camPath":"cnRzcDovL3Jvb3Q6bmF0bWFzdGVyQDEwLjEzLjEyLjE2NTo1NTQvYXhpcy1tZWRpYS9tZWRpYS5hbXA/Y2FtZXJhPTEmdmlkZW9jb2RlYz1oMjY0JnJlc29sdXRpb249ODAweDQ1MCZmcHM9MTAmdmlkZW9rZXlmcmFtZWludGVydmFsPTEwJmNvbXByZXNzaW9uPTIw","model": "P5415-E","resolution": "800x450"}],"keyUsedCnt": 8}
'
          ],
          'ArchiveCamEnum' => [
            FUNCTION_NAME=>'ArchiveCamEnum',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'shareName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The share folder which stores archives'],
                [VALUE_NAME=>'archiveName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The target archive'],
                [VALUE_NAME=>'serverId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The destination DS which camera imports to'],
                [VALUE_NAME=>'_archiveName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SSCamExport_XD'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStati_on.Camera.Import'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ArchiveCamEnum']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'camlist',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Enumerating cameras in the selected archive

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_archiveName="SSCamExport_XD"&version="1"&shareName="kuoDownload"&api="SYNO.SurveillanceStati_on.Camera.Import"&serverId=1&method="ArchiveCamEnum"

Returns:
json:{"camlist": [{"vendor": "iPUX","enable_quota": true,"rotatequota": 10,"folder": "rbxTesting-001","checked": false,"name": "rbxTesting-001","idx": 0,"ipaddr": "10.13.11.189","id": 5,"port": 80,"model": "ICS8220","channel": "1"}]}
'
          ],
          'ArchiveEnum' => [
            FUNCTION_NAME=>'ArchiveEnum',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'shareName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Folder selected to enumerate the archives'],
                [VALUE_NAME=>'_shareName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'testingFolder'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera.Import'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Archi_veEnum']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Enumerate archives in the selected folder

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_shareName="testingFolder"&api="SYNO.SurveillanceStation.Camera.Import"&version="1"&method="Archi_veEnum"
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Camera.Wizard'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Camera.Wizard',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides operations for importing camera',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid',402=>'Camera disabled',407=>'CMS closed']],
        SERVICE_FUNCTIONS=>[
          'CamSaveAll' => [
            FUNCTION_NAME=>'CamSaveAll',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The camera id'],
                [VALUE_NAME=>'actFromHost',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'camServerId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The DS Id on which the camera is mounted'],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Camera id noted in the database of server'],
                [VALUE_NAME=>'camName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Camera name'],
                [VALUE_NAME=>'camIP',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'IP address of the camera'],
                [VALUE_NAME=>'camPort',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Port of the camera'],
                [VALUE_NAME=>'camDevice',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Containing vendor and model information'],
                [VALUE_NAME=>'camChannel',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The channel of the stream'],
                [VALUE_NAME=>'camSourcePath',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Path to the camera'],
                [VALUE_NAME=>'camUserName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The user to access the camera'],
                [VALUE_NAME=>'camPassWord',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Password of the user'],
                [VALUE_NAME=>'camFolder',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Folder to store configuration'],
                [VALUE_NAME=>'camPrefix',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The prefix of the recording file name'],
                [VALUE_NAME=>'blEditBeforeEn',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Indicating to disable other cameras with the'],
                [VALUE_NAME=>'camVideoType',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Indicating video type'],
                [VALUE_NAME=>'camStreamingType',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The streaming protocol'],
                [VALUE_NAME=>'camIsEnableAudio',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Is audio enabled'],
                [VALUE_NAME=>'camFov',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Field of view of the camera'],
                [VALUE_NAME=>'camRecStreamNo',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Stream number of recording stream'],
                [VALUE_NAME=>'camResolution',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The resolution of recording stream'],
                [VALUE_NAME=>'camFps',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The fps of recording stream'],
                [VALUE_NAME=>'camQuality',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The quality of live stream'],
                [VALUE_NAME=>'camRecCbrBitrate',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'A constant indicating the bitrate'],
                [VALUE_NAME=>'camLiveStreamNo',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Stream number of live stream'],
                [VALUE_NAME=>'camLiveResolution',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The resolution of live stream'],
                [VALUE_NAME=>'camLiveFps',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The fps of live stream'],
                [VALUE_NAME=>'camLiveQuality',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The quality of live stream'],
                [VALUE_NAME=>'camLiveCbrBitrate',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'A constant indicating the bitrate'],
                [VALUE_NAME=>'camMobileStreamNo',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Stream number of mobile stream'],
                [VALUE_NAME=>'camMobileResolution',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The resolution of mobile stream'],
                [VALUE_NAME=>'camMobileFps',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The fps of mobile stream'],
                [VALUE_NAME=>'camMobileQuality',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The quality of mobile stream'],
                [VALUE_NAME=>'camMobileCbrBitrate',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'A constant indicating the bitrate'],
                [VALUE_NAME=>'camRecTime',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The recording length in minute'],
                [VALUE_NAME=>'camPreRecTime',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Extra recording time before the start of each'],
                [VALUE_NAME=>'camPostRecTime',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Extra recording time after the end of each'],
                [VALUE_NAME=>'camIsRotByDate',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Does the rotation occur when the limit time is'],
                [VALUE_NAME=>'camRotByDate',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The upper bound indicating how long a file can'],
                [VALUE_NAME=>'camIsRotBySpace',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Does the rotation occur when the limit space'],
                [VALUE_NAME=>'camRotBySpace',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The upper bound that total file size can reach'],
                [VALUE_NAME=>'edgeStgEnabled',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Is the edge storage enabled'],
                [VALUE_NAME=>'edgeStgRecDays',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Using an weighted sum for verifying which day'],
                [VALUE_NAME=>'edgeStgRecStartTime',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The start time of edge storage recording'],
                [VALUE_NAME=>'edgeStgRecStopTime',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The stop time of edge storage recording'],
                [VALUE_NAME=>'edgeStgDownloadSch',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The downloading schedule for retrieving edge'],
                [VALUE_NAME=>'camSchedule',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The recording schedule of the camera'],
                [VALUE_NAME=>'customDetect',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Determining the type of motion detection in'],
                [VALUE_NAME=>'customAlarmDetect',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Determining the type of alarm in the schedule'],
                [VALUE_NAME=>'camMountType',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'How camera is mounted'],
                [VALUE_NAME=>'camRtspProtocol',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Indicating the protocol of RTSP is'],
                [VALUE_NAME=>'mdDetSrc',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The provider of motion detection'],
                [VALUE_NAME=>'uiStmNoList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'List indicating stream number of live video'],
                [VALUE_NAME=>'_camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>72],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera.Wizard'],
                [VALUE_NAME=>'data',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'{"camR_ecTime": "30", "edgeStgEnabled": false, "camIsRotByDate": false, "camRotByDate": "30", "camDevice":_"ABUS TVIP20000", "camLiveBitrateCtrl": 1, "camTvStandard": 0, "camRecCbrBitrate": 1000,_"camIsRotBySpace": true, "camLiveResolution": "640x480", "camRecBitrateCtrl": 1, "camMobileStreamNo":_0, "camLiveQuality": "5", "edgeStgRecStopTime": "01:00", "camLiveStreamNo": 0, "camResolution":_"640x480", "camMobileQuality": "5", "camIsEnableAudio": true, "camPrefix": "TVIP20000-001", "camIP":_"10.13.12.200", "camStreamingType": "RTSP", "camMountType": "0", "camSourcePath": "",_"edgeStgRecStartTime": "00:00", "camRecStreamNo": 0, "camId": 72, "camLiveCbrBitrate": 1000,_"camMobileResolution": "640x480", "camUserName": "admin", "blEditBeforeEn": false, "camFps": "10",_"uiStmNoList": "1,1,1", "camMobileFps": "10", "camFov": "", "camMobileCbrBitrate": 1000,_"camRotBySpace": "10", "edgeStgDownloadSch":_"1111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_1111111111111111111111111111111111111111111111111111111111111111111111111111111",_"customAlarmDetect": 0, "camLiveFps": "10", "camChannel": "1", "customDetect": 1, "camRotOption": 0,_"camQuality": "5", "camPreRecTime": "5", "camPostRecTime": "5", "camSchedule":_"1111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_1111111111111111111111111111111111111111111111111111111111111111111111111111111",_"edgeStgRecMode": 1, "camServerId": 0, "camPassWord": "", "camVideoType": "MPEG4", "camLiveMode":_0, "camMobileBitrateCtrl": 1, "camFolder": "TVIP20000-001", "edgeStgRetrieve": 2, "edgeStgRecDays": 127,_"camPort": "80", "camRtspProtocol": "2", "camName": "TVIP20000-001", "mdDetSrc": "-1",_"camAudioType": 2}'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'CamSaveAll']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'camRecTime',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'edgeStgEnabled',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'camIsRotByDate',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'camRotByDate',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camDevice',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camLiveBitrateCtrl',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camTvStandard',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camRecCbrBitrate',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camIsRotBySpace',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'camLiveResolution',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camRecBitrateCtrl',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camMobileStreamNo',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camLiveQuality',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'edgeStgRecStopTime',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camLiveStreamNo',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camResolution',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camMobileQuality',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camIsEnableAudio',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'camPrefix',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camIP',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camStreamingType',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camMountType',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camSourcePath',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'edgeStgRecStartTime',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camRecStreamNo',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camLiveCbrBitrate',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camMobileResolution',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camUserName',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'blEditBeforeEn',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'camFps',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'uiStmNoList',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camMobileFps',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camFov',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camMobileCbrBitrate',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camRotBySpace',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'edgeStgDownloadSch',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'customAlarmDetect',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camLiveFps',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camChannel',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'customDetect',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camRotOption',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camQuality',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camPreRecTime',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camPostRecTime',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camSchedule',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'edgeStgRecMode',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camServerId',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camPassWord',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camVideoType',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camLiveMode',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camMobileBitrateCtrl',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camFolder',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'edgeStgRetrieve',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'edgeStgRecDays',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camPort',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camRtspProtocol',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camName',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'mdDetSrc',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camAudioType',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'editTime',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camera',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'keyUsedCnt',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Save the configuration of the camera when adding or editing.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_camId=72&version="1"&api="SYNO.SurveillanceStation.Camera.Wizard"&actFromHost=false&data={"camR_ecTime": "30", "edgeStgEnabled": false, "camIsRotByDate": false, "camRotByDate": "30", "camDevice":_"ABUS TVIP20000", "camLiveBitrateCtrl": 1, "camTvStandard": 0, "camRecCbrBitrate": 1000,_"camIsRotBySpace": true, "camLiveResolution": "640x480", "camRecBitrateCtrl": 1, "camMobileStreamNo":_0, "camLiveQuality": "5", "edgeStgRecStopTime": "01:00", "camLiveStreamNo": 0, "camResolution":_"640x480", "camMobileQuality": "5", "camIsEnableAudio": true, "camPrefix": "TVIP20000-001", "camIP":_"10.13.12.200", "camStreamingType": "RTSP", "camMountType": "0", "camSourcePath": "",_"edgeStgRecStartTime": "00:00", "camRecStreamNo": 0, "camId": 72, "camLiveCbrBitrate": 1000,_"camMobileResolution": "640x480", "camUserName": "admin", "blEditBeforeEn": false, "camFps": "10",_"uiStmNoList": "1,1,1", "camMobileFps": "10", "camFov": "", "camMobileCbrBitrate": 1000,_"camRotBySpace": "10", "edgeStgDownloadSch":_"1111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_1111111111111111111111111111111111111111111111111111111111111111111111111111111",_"customAlarmDetect": 0, "camLiveFps": "10", "camChannel": "1", "customDetect": 1, "camRotOption": 0,_"camQuality": "5", "camPreRecTime": "5", "camPostRecTime": "5", "camSchedule":_"1111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_1111111111111111111111111111111111111111111111111111111111111111111111111111111",_"edgeStgRecMode": 1, "camServerId": 0, "camPassWord": "", "camVideoType": "MPEG4", "camLiveMode":_0, "camMobileBitrateCtrl": 1, "camFolder": "TVIP20000-001", "edgeStgRetrieve": 2, "edgeStgRecDays": 127,_"camPort": "80", "camRtspProtocol": "2", "camName": "TVIP20000-001", "mdDetSrc": "-1",_"camAudioType": 2}&method="CamSaveAll"
json:{"camRecTime": "30", "edgeStgEnabled": false, "camIsRotByDate": false, "camRotByDate": "30", "camDevice":"ABUS TVIP20000", "camLiveBitrateCtrl": 1, "camTvStandard": 0, "camRecCbrBitrate": 1000,"camIsRotBySpace": true, "camLiveResolution": "640x480", "camRecBitrateCtrl": 1, "camMobileStreamNo":0, "camLiveQuality": "5", "edgeStgRecStopTime": "01:00", "camLiveStreamNo": 0, "camResolution":"640x480", "camMobileQuality": "5", "camIsEnableAudio": true, "camPrefix": "TVIP20000-001", "camIP":"10.13.12.200", "camStreamingType": "RTSP", "camMountType": "0", "camSourcePath": "","edgeStgRecStartTime": "00:00", "camRecStreamNo": 0, "camId": 72, "camLiveCbrBitrate": 1000,"camMobileResolution": "640x480", "camUserName": "admin", "blEditBeforeEn": false, "camFps": "10","uiStmNoList": "1,1,1", "camMobileFps": "10", "camFov": "", "camMobileCbrBitrate": 1000,"camRotBySpace": "10", "edgeStgDownloadSch":"111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111","customAlarmDetect": 0, "camLiveFps": "10", "camChannel": "1", "customDetect": 1, "camRotOption": 0,"camQuality": "5", "camPreRecTime": "5", "camPostRecTime": "5", "camSchedule":"111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111","edgeStgRecMode": 1, "camServerId": 0, "camPassWord": "", "camVideoType": "MPEG4", "camLiveMode":0, "camMobileBitrateCtrl": 1, "camFolder": "TVIP20000-001", "edgeStgRetrieve": 2, "edgeStgRecDays": 127,"camPort": "80", "camRtspProtocol": "2", "camName": "TVIP20000-001", "mdDetSrc": "-1","camAudioType": 2}

Returns:
json:{"camId": 80,"editTime": 1424919126,"camera": [{"audioOut": true,"mute": false,"recStatus": 0,"analyticsType": 0,"ptzCap": 111,"audioType": 4,"ptzContinuous": 39,"is_rotated_by_date": false,"ADCap": 5,"blLiveviewPriv": true,"videoCapList": [{"stList": ["RTSP"],"vt": "H.264"},{"stList": ["HTTP"],"vt": "MJPEG"}],"isStatusUnrecognized": false,"is_rotated_by_space": true,"osd_position": 6,"fisheyeType": 0,"recBitrateCtrl": 1,"video_mirror": true,"vendor": "AXIS","video_rotation": 0,"stmFisheyeType": 0,"dsPort": 5000,"TDCap": 0,"camMountType": 0,"exposure_control": 6,"name": "P5415-E-0011","ownerDsId": 0,"osd_status": true,"fisheyeDispMode": "","channel_id": "1","setDICap": false,"param_chklist": 0,"analyticsBeep": false,"status_flags": 32,"feRegionList": [],"objTrack": false,"port": 80,"video_flip": true,"osd_format": 0,"autoPan": 0,"deleted": false,"deviceType": 4,"hasCamParam": true,"DINum": 2,"snapshot_path":"/webapi/_______________________________________________________entry.cgi?api=SYNO.SurveillanceStation.Camera&method=GetSnapshot&version=1&cameraId=70×tamp=1424919129&preview=true","dsIp": "","host": "10.13.12.165","rotation_by_date": 30,"autoFocus": true,"volume_space": "9.617","analyticsHeight": 0,"MDCap": 5,"rotation_by_space": "10","analyticsVirtualFence": false,"analyticsObjSize": 0,"analyticsDirection": 1,"analyticsRegion": "","quality": "5","multiDI": false,"type": 3,"stm_info": [{"resolution": "800x450","quality": "5","fps": 10,"type": 0},{"resolution": "800x450","quality": "5","fps": 10,"type": 1},{"resolution": "800x450","quality": "5","fps": 10,"type": 2}],"camLiveMode": 0,"volume": 50,"ptzDirection": 32,"dayend": 18,"folder": "/var/services/surveillance/P5415-E-001","tvStandard": 0,"singleStream": false,"imageEnhancement": {"saturation": 0,"sharpness": 0,"contrast": 0,"brightness": 0},"audioCap": true,"camIdOnRecServer": 0,"detailInfo": {"camRecTime": 30,"edgeStgEnabled": false,"camIsRotByDate": false,"camRecStreamNo": 0,"camRotByDate": 30,"camTvStandard": 0,"camMountType": 0,"camRecCbrBitrate": 1000,"camIsRotBySpace": true,"camLiveResolution": "800x450","camRecBitrateCtrl": 1,"camMobileStreamNo": 0,"camLiveQuality": "5","edgeStgRecStopTime": "23:59","camLiveStreamNo": 0,"camResolution": "800x450","camMobileQuality": "5","camLiveMode": 0,"camPreRecTime": 5,"camIP": "10.13.12.165","camLiveBitrateCtrl": 1,"camModel": "P5415-E","edgeStgRecStartTime": "00:00","camPostRecTime": 5,"camLiveCbrBitrate": 1000,"camMobileResolution": "800x450","camUserName": "root","camFps": 10,"camVendor": "AXIS","camMobileFps": 10,"camFov": "","camMobileCbrBitrate": 1000,"camRotBySpace": 10,"edgeStgDownloadSch":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"customAlarmDetect": 3,"camLiveFps": 10,"camChannel": "1","camRotOption": 0,"camQuality": "5","camPrefix": "P5415-E-001","edgeStgRetrieveAction": 2,"camSchedule":[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1],"edgeStgRecMode": 2,"customDetect": 5,"camPassWord": "%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83","camVideoType": "H.264","camMobileBitrateCtrl": 1,"MdDetSrc": 0,"camServer": 0,"camFolder": "P5415-E-001","edgeStgRecDays": 127,"camPort": 80,"camRtspProtocol": 4,"camName": "P5415-E-0011","camAudioType": 4},"enabled": true,"analyticsSens": 1,"analyticsWidth": 0,"recCbrBitrate": 1000,"blPresetSpeed": false,"DONum": 0,"id": 70,"setDOCap": false,"analyticsFrame": true,"analyticsLine": false,"daybegin": 8,"fps": 10,"status": 1,"update_time": 1424919129,"exposure_mode": 4,"time_server": "","rotation_option": 0,"presetNum": 32,"camStatus": 9,"analyticsDwellTime": 5,"camPath":"cnRzcDovL3Jvb3Q6bmF0bWFzdGVyQDEwLjEzLjEyLjE2NTo1NTQvYXhpcy1tZWRpYS9tZWRpYS5hbXA/Y2FtZXJhPTEmdmlkZW9jb2RlYz1oMjY0JnJlc29sdXRpb249ODAweDQ1MCZmcHM9MTAmdmlkZW9rZXlmcmFtZWludGVydmFsPTEwJmNvbXByZXNzaW9uPTIw","model": "P5415-E","resolution": "800x450"}],"keyUsedCnt": 8,"success": true}
'
          ],
          'CheckSDCardSize' => [
            FUNCTION_NAME=>'CheckSDCardSize',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Specified camera ID'],
                [VALUE_NAME=>'host',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Address of the camera'],
                [VALUE_NAME=>'port',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Port of the camera'],
                [VALUE_NAME=>'user',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The user who tries to log in'],
                [VALUE_NAME=>'pass',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The Correlated password'],
                [VALUE_NAME=>'vendor',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Vendor of the camera'],
                [VALUE_NAME=>'model',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Model of the camera'],
                [VALUE_NAME=>'ch',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'_camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>62],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera.Wizard'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Check_SDCardSize']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'status',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'size',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Check the available size in SD card

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_camId=62&vendor="AXIS"&version="1"&api="SYNO.SurveillanceStation.Camera.Wizard"&method="Check_SDCardSize"&host="10.13.12.165"&ch="1"&user="root"&pass="%EF%BC%83%EF%BC%83%EF%BC%83%EF_%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83"&model="P5415-E"&port="80"

Returns:
json:{"status": 0,"size": 29022484}
'
          ],
          'CheckQuota' => [
            FUNCTION_NAME=>'CheckQuota',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera.Wizard'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'CheckQuota']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'localMaxCamNum',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'iKeyUsed',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'iKeyTotal',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'localCamNum',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'This method enables users to check the usage of the licence

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Camera.Wizard"&version="1"&method="CheckQuota"

Returns:
json:{"localMaxCamNum": 10,"iKeyUsed": 2,"iKeyTotal": 12,"localCamNum": 0}
'
          ],
          'CamBarAddSaveAll' => [
            FUNCTION_NAME=>'CamBarAddSaveAll',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camServerId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The DS id on which cameras mounted'],
                [VALUE_NAME=>'tvStandard',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Standard of the input video'],
                [VALUE_NAME=>'camMountType',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'How camera is mounted'],
                [VALUE_NAME=>'camChannel',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Channel of the output video'],
                [VALUE_NAME=>'name',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The type of the creation'],
                [VALUE_NAME=>'vendor',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Vendor of the camera'],
                [VALUE_NAME=>'model',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Model of the camera'],
                [VALUE_NAME=>'ip',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'IP address of the camera'],
                [VALUE_NAME=>'port',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'port of the camera'],
                [VALUE_NAME=>'username',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The user trying to access the camera'],
                [VALUE_NAME=>'password',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Password of the user'],
                [VALUE_NAME=>'name',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The name of the camera stored in the'],
                [VALUE_NAME=>'_camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>62],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera.Wizard'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'CamBatAddSaveAll'],
                [VALUE_NAME=>'host',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'10.13.12.165'],
                [VALUE_NAME=>'ch',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'user',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'root'],
                [VALUE_NAME=>'pass',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'＃＃＃�_��＃＃＃＃'],
                [VALUE_NAME=>'data',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'{"camChannel": "", "camList":_[{"username": "admin", "vendor": "ABUS", "name": "TVIP20000-001", "ip": "10.13.12.200", "model":_"TVIP20000", "password": "synoss", "port": 80}, {"username": "root", "vendor": "AXIS", "name": "P5415-E-_001", "ip": "10.13.12.165", "model": "P5415-E", "password": "natmaster", "port": 80}, {"username":_"admin", "vendor": "iPUX", "name": "ICS8220-001", "ip": "10.13.11.195", "model": "ICS8220", "password":_"admin", "port": 80}], "camMountType": "", "mode": "quick", "tvStandard":_""}']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'camChannel',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camList',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'camMountType',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'mode',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'tvStandard',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'pid',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'This API saves the configuration from batch add

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_camId=62&vendor="AXIS"&version="1"&api="SYNO.SurveillanceStation.Camera.Wizard"&method="Check_SDCardSize"&host="10.13.12.165"&ch="1"&user="root"&pass="%EF%BC%83%EF%BC%83%EF%BC%83%EF_%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83"&model="P5415-_E"&port="80"http://192.168.1.1:5000/webapi/entry.cgi?_camServerId=0&api="SYNO.SurveillanceStation.Camera.Wizard"&data={"camChannel": "", "camList":_[{"username": "admin", "vendor": "ABUS", "name": "TVIP20000-001", "ip": "10.13.12.200", "model":_"TVIP20000", "password": "synoss", "port": 80}, {"username": "root", "vendor": "AXIS", "name": "P5415-E-_001", "ip": "10.13.12.165", "model": "P5415-E", "password": "natmaster", "port": 80}, {"username":_"admin", "vendor": "iPUX", "name": "ICS8220-001", "ip": "10.13.11.195", "model": "ICS8220", "password":_"admin", "port": 80}], "camMountType": "", "mode": "quick", "tvStandard":_""}&version="1"&method="CamBatAddSaveAll"
json:{"camChannel": "", "camList":[{"username": "admin", "vendor": "ABUS", "name": "TVIP20000-001", "ip": "10.13.12.200", "model":"TVIP20000", "password": "synoss", "port": 80}, {"username": "root", "vendor": "AXIS", "name": "P5415-E-001", "ip": "10.13.12.165", "model": "P5415-E", "password": "natmaster", "port": 80}, {"username":"admin", "vendor": "iPUX", "name": "ICS8220-001", "ip": "10.13.11.195", "model": "ICS8220", "password":"admin", "port": 80}], "camMountType": "", "mode": "quick", "tvStandard":""}

Returns:
json:{"pid": 3585}
'
          ],
          'FormatSDCard' => [
            FUNCTION_NAME=>'FormatSDCard',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Specified camera ID'],
                [VALUE_NAME=>'host',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Address of the camera'],
                [VALUE_NAME=>'port',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Port of the camera'],
                [VALUE_NAME=>'user',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The user who tries to log in'],
                [VALUE_NAME=>'pass',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The Correlated password'],
                [VALUE_NAME=>'vendor',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Vendor of the camera'],
                [VALUE_NAME=>'model',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Model of the camera'],
                [VALUE_NAME=>'ch',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'_camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>62],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera.Wizard'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Forma_tSDCard'],
                [VALUE_NAME=>'timeout',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>300]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'result',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Format the SD card in the specified camera which offers edge storage service

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_camId=62&vendor="AXIS"&version="1"&api="SYNO.SurveillanceStation.Camera.Wizard"&method="Forma_tSDCard"&host="10.13.12.165"&ch="1"&user="root"&timeout=300&pass="%EF%BC%83%EF%BC%83%EF_%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83"&model="P5415-E"&port="80"

Returns:
json:{"result": 0}
'
          ],
          'QuickCreate' => [
            FUNCTION_NAME=>'QuickCreate',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camServerId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The DS id on which this camera mounted'],
                [VALUE_NAME=>'actFromHost',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Indicating if the api is called from CMS host'],
                [VALUE_NAME=>'camStreamingType',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The streaming protocol of the camera'],
                [VALUE_NAME=>'camName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'User'],
                [VALUE_NAME=>'camIP',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'IP address of the camera'],
                [VALUE_NAME=>'camPort',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Port of the camera'],
                [VALUE_NAME=>'camVendor',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Vendor of the camera'],
                [VALUE_NAME=>'camModel',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Model name of the camera'],
                [VALUE_NAME=>'camMountType',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'How camera is mounted'],
                [VALUE_NAME=>'camChannel',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Channel number of the camera'],
                [VALUE_NAME=>'camVideoType',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Video type of the camera'],
                [VALUE_NAME=>'camAudioType',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Indicating audio type'],
                [VALUE_NAME=>'camSourcePath',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Path of recorded videos'],
                [VALUE_NAME=>'camUserName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The input camera user'],
                [VALUE_NAME=>'camPassWord',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The input camera password'],
                [VALUE_NAME=>'_camUserName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'admin'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Camera.Wizard'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'meth_od',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'QuickCreate'],
                [VALUE_NAME=>'ca_mStreamingType',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'RTSP']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camera',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'keyUsedCnt',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'This API provides the capability of quick-creating single camera

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_camUserName="admin"&api="SYNO.SurveillanceStation.Camera.Wizard"&camServerId=0&camPassWord=_"%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC_%83"&camVideoType="H.264"&version="1"&camVendor="iPUX"&camPort="80"&camChannel="1"&meth_od="QuickCreate"&camName="xx"&camModel="ICS8220"&camIP="10.13.11.195"&actFromHost=false&ca_mStreamingType="RTSP"&camMountType="0"&camAudioType="2"&camSourcePath="%E4%BD%BF_%E7%94%A8%E9%A0%90%E8%A8%AD%E5%BD%B1%E5%83%8F%E8%B7%AF%E5%BE%91%"

Returns:
json:{"camId": 80,"camera": [{"audioOut": true,"mute": false,"recStatus": 0,"analyticsType": 0,"ptzCap": 111,"audioType": 4,"ptzContinuous": 39,"is_rotated_by_date": false,"ADCap": 5,"blLiveviewPriv": true,"videoCapList": [{"stList": ["RTSP"],"vt": "H.264"},{"stList": ["HTTP"],"vt": "MJPEG"}],"isStatusUnrecognized": false,"is_rotated_by_space": true,"osd_position": 6,"fisheyeType": 0,"recBitrateCtrl": 1,"video_mirror": true,"vendor": "AXIS","video_rotation": 0,"stmFisheyeType": 0,"dsPort": 5000,"TDCap": 0,"camMountType": 0,"exposure_control": 6,"name": "P5415-E-0011","ownerDsId": 0,"osd_status": true,"fisheyeDispMode": "","channel_id": "1","setDICap": false,"param_chklist": 0,"analyticsBeep": false,"status_flags": 32,"feRegionList": [],"objTrack": false,"port": 80,"video_flip": true,"osd_format": 0,"autoPan": 0,"deleted": false,"deviceType": 4,"hasCamParam": true,"DINum": 2,"snapshot_path":"/webapi/_______________________________________________________entry.cgi?api=SYNO.SurveillanceStation.Camera&method=GetSnapshot&version=1&cameraId=70×tamp=1424919129&preview=true","dsIp": "","host": "10.13.12.165","rotation_by_date": 30,"autoFocus": true,"volume_space": "9.617","analyticsHeight": 0,"MDCap": 5,"rotation_by_space": "10","analyticsVirtualFence": false,"analyticsObjSize": 0,"analyticsDirection": 1,"analyticsRegion": "","quality": "5","multiDI": false,"type": 3,"stm_info": [{"resolution": "800x450","quality": "5","fps": 10,"type": 0},{"resolution": "800x450","quality": "5","fps": 10,"type": 1},{"resolution": "800x450","quality": "5","fps": 10,"type": 2}],"camLiveMode": 0,"volume": 50,"ptzDirection": 32,"dayend": 18,"folder": "/var/services/surveillance/P5415-E-001","tvStandard": 0,"singleStream": false,"imageEnhancement": {"saturation": 0,"sharpness": 0,"contrast": 0,"brightness": 0},"audioCap": true,"camIdOnRecServer": 0,"detailInfo": {"camRecTime": 30,"edgeStgEnabled": false,"camIsRotByDate": false,"camRecStreamNo": 0,"camRotByDate": 30,"camTvStandard": 0,"camMountType": 0,"camRecCbrBitrate": 1000,"camIsRotBySpace": true,"camLiveResolution": "800x450","camRecBitrateCtrl": 1,"camMobileStreamNo": 0,"camLiveQuality": "5","edgeStgRecStopTime": "23:59","camLiveStreamNo": 0,"camResolution": "800x450","camMobileQuality": "5","camLiveMode": 0,"camPreRecTime": 5,"camIP": "10.13.12.165","camLiveBitrateCtrl": 1,"camModel": "P5415-E","edgeStgRecStartTime": "00:00","camPostRecTime": 5,"camLiveCbrBitrate": 1000,"camMobileResolution": "800x450","camUserName": "root","camFps": 10,"camVendor": "AXIS","camMobileFps": 10,"camFov": "","camMobileCbrBitrate": 1000,"camRotBySpace": 10,"edgeStgDownloadSch":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"customAlarmDetect": 3,"camLiveFps": 10,"camChannel": "1","camRotOption": 0,"camQuality": "5","camPrefix": "P5415-E-001","edgeStgRetrieveAction": 2,"camSchedule":[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1],"edgeStgRecMode": 2,"customDetect": 5,"camPassWord": "%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83","camVideoType": "H.264","camMobileBitrateCtrl": 1,"MdDetSrc": 0,"camServer": 0,"camFolder": "P5415-E-001","edgeStgRecDays": 127,"camPort": 80,"camRtspProtocol": 4,"camName": "P5415-E-0011","camAudioType": 4},"enabled": true,"analyticsSens": 1,"analyticsWidth": 0,"recCbrBitrate": 1000,"blPresetSpeed": false,"DONum": 0,"id": 70,"setDOCap": false,"analyticsFrame": true,"analyticsLine": false,"daybegin": 8,"fps": 10,"status": 1,"update_time": 1424919129,"exposure_mode": 4,"time_server": "","rotation_option": 0,"presetNum": 32,"camStatus": 9,"analyticsDwellTime": 5,"camPath":"cnRzcDovL3Jvb3Q6bmF0bWFzdGVyQDEwLjEzLjEyLjE2NTo1NTQvYXhpcy1tZWRpYS9tZWRpYS5hbXA/Y2FtZXJhPTEmdmlkZW9jb2RlYz1oMjY0JnJlc29sdXRpb249ODAweDQ1MCZmcHM9MTAmdmlkZW9rZXlmcmFtZWludGVydmFsPTEwJmNvbXByZXNzaW9uPTIw","model": "P5415-E","resolution": "800x450"}],"keyUsedCnt": 8}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.PTZ'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.PTZ',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides Saving/Editing operations for camera',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid',402=>'Camera disabled']],
        SERVICE_FUNCTIONS=>[
          'Move' => [
            FUNCTION_NAME=>'Move',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID'],
                [VALUE_NAME=>'speed',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Control the PTZ camera to move its lens.
'
          ],
          'Zoom' => [
            FUNCTION_NAME=>'Zoom',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Control the PTZ camera to zoom in or zoom out.
'
          ],
          'ListPreset' => [
            FUNCTION_NAME=>'ListPreset',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID'],
                [VALUE_NAME=>'offset',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>10],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>3],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.PTZ'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ListPreset'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'List all presets of the PTZ camera.

Example:
call:<syno_url>/webapi/SurveillanceStation/ptz.cgi?api=SYNO.SurveillanceStation.PTZ&method=ListPreset&version=1&offset=10&limit=3&cameraId=10

Returns:
json:{“total”: 38,“offset”: 10,“presets”: [{"id": 100,"name": “My Preset 1”}, {"id": 101,"name": “My Preset 2”}, {"id": 102,"name": “My Preset 3”}]}
'
          ],
          'GoPreset' => [
            FUNCTION_NAME=>'GoPreset',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID'],
                [VALUE_NAME=>'presetId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique preset ID'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.PTZ'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GoPreset'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Move the camera lens to a pre-defined preset position.

Example:
call:<syno_url>/webapi/SurveillanceStation/ptz.cgi?api=SYNO.SurveillanceStation.PTZ&method=GoPreset&version=1&cameraId=10&presetId=10
'
          ],
          'ListPatrol' => [
            FUNCTION_NAME=>'ListPatrol',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID'],
                [VALUE_NAME=>'offset',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>10],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>2],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.PTZ'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ListPatrol'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Enumerate the patrol list of a PTZ camera.

Example:
call:<syno_url>/webapi/SurveillanceStation/ptz.cgi?api=SYNO.SurveillanceStation.PTZ&method=ListPatrol&version=1&offset=10&limit=2&cameraId=10

Returns:
json:{"total": 38,"offset": 10,“patrols”: [{"id": 10,"name": “My Patrol 1”,"stayTime": 5,"sequence": [100, 101, 102]},{"id": 11,"name": “My Patrol 2”,"stayTime": 60,"sequence": [103, 104]}]}
'
          ],
          'RunPatrol' => [
            FUNCTION_NAME=>'RunPatrol',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID'],
                [VALUE_NAME=>'patrolId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique patrol ID'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.PTZ'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'RunPatrol'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>2]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Force the camera to execute the specific patrol.

Example:
call:<syno_url>/webapi/SurveillanceStation/ptz.cgi?api=SYNO.SurveillanceStation.PTZ&method=RunPatrol&version=2&cameraId=10&patrolId=11
'
          ],
          'Focus' => [
            FUNCTION_NAME=>'Focus',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Control the camera to focus in or focus out.
'
          ],
          'Iris' => [
            FUNCTION_NAME=>'Iris',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.PTZ'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Iris'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>3],
                [VALUE_NAME=>'control',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'in'],
                [VALUE_NAME=>'moveType',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Start']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Control the camera to iris in or iris out.

Example:
call:<syno_url>/webapi/SurveillanceStation/ptz.cgi?api=SYNO.SurveillanceStation.PTZ&method=Iris&version=3&cameraId=10&control=in&moveType=Start
'
          ],
          'AutoFocus' => [
            FUNCTION_NAME=>'AutoFocus',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.PTZ'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'AutoFocus'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>3]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Let camera adjust its focus automatically.

Example:
call:<syno_url>/webapi/SurveillanceStation/ptz.cgi?api=SYNO.SurveillanceStation.PTZ&method=AutoFocus&version=3&cameraId=10
'
          ],
          'AbsPtz' => [
            FUNCTION_NAME=>'AbsPtz',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.PTZ'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'AbsPtz'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>3],
                [VALUE_NAME=>'poxX',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>240],
                [VALUE_NAME=>'posY',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>400]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Move the camera lens to an abosule position on screen.

Example:
call:<syno_url>/webapi/SurveillanceStation/ptz.cgi?api=SYNO.SurveillanceStation.PTZ&method=AbsPtz&version=3&cameraId=10&poxX=240&posY=400
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.ExternalRecording'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.ExternalRecording',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides a set of methods to execute PTZ action, and to acquire PTZ related information such as
patrol list or patrol schedule of a camera.',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid',402=>'Camera disabled']],
        SERVICE_FUNCTIONS=>[
          'Record' => [
            FUNCTION_NAME=>'Record',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.ExternalRecording'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Record'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'action',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'start']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Start or stop external recording of a camera.

Example:
- Start external recording of target camera.
call:<syno_url>/webapi/SurveillanceStation/extrecord.cgi?api=SYNO.SurveillanceStation.ExternalRecording&method=Record&version=1&cameraId=10&action=start

- Stop external recording of target camera.
call:<syno_url>/webapi/SurveillanceStation/extrecord.cgi?api=SYNO.SurveillanceStation.ExternalRecording&method=Record&version=1&cameraId=10&action=stop
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Event'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Event',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides methods to start or stop external recording of a camera.',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid',405=>'CMS server connection failed',414=>'Some events not exist',439=>'Too many items selected']],
        SERVICE_FUNCTIONS=>[
          'List' => [
            FUNCTION_NAME=>'List',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'offset',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>80],
                [VALUE_NAME=>'reason',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'cameraIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'fromTime',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'toTime',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'locked',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Query locked'],
                [VALUE_NAME=>'evtSrcId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'CONDITIONAL '],
                [VALUE_NAME=>'includeAllCam',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Include all camera or not'],
                [VALUE_NAME=>'from_end',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Time from end'],
                [VALUE_NAME=>'from_start',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Time from start'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'4'],
                [VALUE_NAME=>'blIncludeSnapshot',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'evtSrcType',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>2],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'List']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'camIdList',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'recCntData',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'auInfo',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'timestamp',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'offset',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'events',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Query event list by specific filter conditions.

Example:
call:<syno_url>/webapi/entry.cgi?locked=0&version="4"&blIncludeSnapshot=true&cameraIds=""&evtSrcType=2&from_start=0&reason=""&api="SYNO.SurveillanceStation.Event"&evtSrcId=-1&toTime=0&offset=0&limit=80&fromTime=0&method="List"&from_end=0&includeAllCam=true

Returns:
json:{"camIdList": "","recCntData": "null","auInfo": "null","timestamp": "1414740013","offset": 0,"total": 3,"events": [{"volume": 50,"update_time": 1414680567,"recordId": "0_46","audio_format": "","idOnRecServer": 0,"mute": false,"deleted": false,"fisheye_origin_view": false,"stopTime": 1414680564,"videoCodec": "MJPEG","fisheye_type": 0,"archived_folder": "TVIP21552","imgWidth": 640,"video_type": 1,"startTime": 1414680551,"path": "20141030PM/TVIP2155220141030-224911-1414680551.avi","eventSize": 0.9930419921875,"mountSrcDsId": 0,"dsId": 0,"id": 46,"frameCount": 77,"eventId": 46,"recording": false,"archived": false,"imageEnhancement": {"saturation": 0,"sharpness": 0,"contrast": 0,"brightness": 0},"name": "20141030PM/TVIP2155220141030-224911-1414680551.avi","camIdOnRecServer": 0,"camera_name": "TVIP21552","reason": 2,"cameraId": 13,"mountId": 0,"event_size_bytes": 1041280,"mount_type": 0,"markAsDel": false,"mode": 1,"for_rotation_only": false,"imgHeight": 480,"closing": false,"resoW": 640,"is_complete": true,"resoH": 480,"snapshot_medium":"/9j/4AAQSkZJRgABAQAAAQABAAD/2wBDAAYEBQYFBAYGBQYHBwYIChAKCgkJChQODwwQFxQYGBcUFhYaHSUfGhsjHBYWICwgIyYnKSopGR8tMC0oMCUoKSj/2wBDAQcHBwoIChMKChMoGhYaKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCj/wAARCABaAHgDASIAAhEBAxEB/8QAHwAAAQUBAQEBAQEAAAAAAAAAAAECAwQFBgcICQoL/8QAtRAAAgEDAwIEAwUFBAQAAAF9AQIDAAQRBRIhMUEGE1FhByJxFDKBkaEII0KxwRVS0fAkM2JyggkKFhcYGRolJicoKSo0NTY3ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqDhIWGh4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uHi4+Tl5ufo6erx8vP09fb3+Pn6/8QAHwEAAwEBAQEBAQEBAQAAAAAAAAECAwQFBgcICQoL/8QAtREAAgECBAQDBAcFBAQAAQJ3AAECAxEEBSExBhJBUQdhcRMiMoEIFEKRobHBCSMzUvAVYnLRChYkNOEl8RcYGRomJygpKjU2Nzg5OkNERUZHSElKU1RVVldYWVpjZGVmZ2hpanN0dXZ3eHl6goOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3uLm6wsPExcbHyMnK0tPU1dbX2Nna4uPk5ebn6Onq8vP09fb3+Pn6/9oADAMBAAIRAxEAPwD51paSlpgFFFFAAKWiimAUUUlMBaKTNLQAUUUUWAKKKKLAGKKKKLAJRRRipAKWkpaYCUtFFMBKWjrS1SQCUUUtMAoooFMAoooosAUUUUDG0UlLWIhaKKKYgooopoYDpS0lLVoAooopgAooopgFLSUtABRS9qKBkVFFFYCF70tJS0wEpaKSmmIdRTaUVaYxaKKKYAKKBS0wEpRSUooGOFFJRQMhpaSiuckdQKTNGadwHUUmaMincApQaSiqTAWlpM0ZppgLQKTNFO4C0tJRmncYoNFJmii4EdFFFYCCiiigAooooAKKKKAFzS02igB1FJS0wClpKDTAWig0U7gf/9k="}]}
'
          ],
          'DeleteMulti' => [
            FUNCTION_NAME=>'DeleteMulti',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID with Unique event ID'],
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Event'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event'],
                [VALUE_NAME=>'idList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'[{"dsId": 0, "id":"0:4295"}]'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'4'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'DeleteMulti']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Delete multi-events with selection.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.Event"&idList=[{"dsId": 0, "id":"0:4295"}]&version="4"&method="DeleteMulti"
json:{"dsId": 0, "id":"0:4295"}

Returns:
json:{"success": false}
'
          ],
          'DeleteFilter' => [
            FUNCTION_NAME=>'DeleteFilter',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'reason',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'cameraIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'fromTime',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'toTime',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'locked',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Query locked'],
                [VALUE_NAME=>'evtSrcId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'CONDITIONAL '],
                [VALUE_NAME=>'includeAllCam',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Include all camera or not'],
                [VALUE_NAME=>'from_end',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Time from end'],
                [VALUE_NAME=>'from_start',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Time from start'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'3'],
                [VALUE_NAME=>'blIncludeSnapshot',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'evtSrcType',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>2],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'DeleteFilter']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Delete events by specific filter conditions.

Example:
call:<syno_url>/webapi/entry.cgi?locked=0&version="3"&blIncludeSnapshot=true&cameraIds=""&evtSrcType=2&reason=""&api="SYNO.SurveillanceStation.Event"&evtSrcId=-1&toTime=0&from_start=1423497600&fromTime=0&method="DeleteFilter"&from_end=1423584000&includeAllCam=true
'
          ],
          'DeleteAll' => [
            FUNCTION_NAME=>'DeleteAll',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'4'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'DeleteAll']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Delete all events that the user has authorized.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.Event"&version="4"&method="DeleteAll"
'
          ],
          'ApplyAdvanced' => [
            FUNCTION_NAME=>'ApplyAdvanced',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'rotateUnrecogCam',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'4'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ApplyAdvanced']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'rotateUnrecogCam',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Apply the setting in advance tab.

Example:
call:<syno_url>/webapi/entry.cgi?rotateUnrecogCam=true&api="SYNO.SurveillanceStation.Event"&version="4"&method="ApplyAdvanced"

Returns:
json:{"rotateUnrecogCam": true}
'
          ],
          'CountByCategory' => [
            FUNCTION_NAME=>'CountByCategory',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'offset',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'reason',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'cameraIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'fromTime',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'toTime',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'locked',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Query locked'],
                [VALUE_NAME=>'evtSrcId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'CONDITIONAL '],
                [VALUE_NAME=>'includeAllCam',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Include all camera or not'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'4'],
                [VALUE_NAME=>'blIncludeSnapshot',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'evtSrcType',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>2],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'CountByCategory'],
                [VALUE_NAME=>'timezoneOffset',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>480]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get the count number of Event in category.

Example:
call:<syno_url>/webapi/entry.cgi?locked=0&version="4"&blIncludeSnapshot=true&cameraIds=""&evtSrcType=2&reason=""&api="SYNO.SurveillanceStation.Event"&evtSrcId=-1&toTime=0&limit=0&fromTime=0&method="CountByCategory"&timezoneOffset=480&includeAllCam=true

Returns:
json:{date":{"-1":243,"2015/03/11":{"-1":156,"am":0,"pm":156},"2015/03/12":{"-1":87,"am":69,"pm":18}},"evt_cam":{"-1":243,"0":{"-1":243,"17-IP7135":48,"18-E33":195}},"total":243}
'
          ],
          'Keepalive' => [
            FUNCTION_NAME=>'Keepalive',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'4'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Keepalive']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Keep event play alive

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.Event"&version="4"&method="Keepalive"
'
          ],
          'Trunc' => [
            FUNCTION_NAME=>'Trunc',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID with Unique event ID'],
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Event'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event'],
                [VALUE_NAME=>'idList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'[{"dsId": 0, "id":"0:4295"}]'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'4'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Trunc']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'A method to stop a recording event.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.Event"&idList=[{"dsId": 0, "id":"0:4295"}]&version="4"&method="Trunc"
json:{"dsId": 0, "id":"0:4295"}
'
          ],
          'LoadAdvanced' => [
            FUNCTION_NAME=>'LoadAdvanced',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'4'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'LoadAdvanced']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'rotateUnrecogCam',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Load the setting in advance tab.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.Event"&version="4"&method="LoadAdvanced"

Returns:
json:{"rotateUnrecogCam": true}
'
          ],
          'LockFilter' => [
            FUNCTION_NAME=>'LockFilter',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'reason',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'1,3,8'],
                [VALUE_NAME=>'cameraIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'1,2,3'],
                [VALUE_NAME=>'fromTime',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'toTime',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'locked',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Query locked'],
                [VALUE_NAME=>'evtSrcId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'CONDITIONAL '],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'4'],
                [VALUE_NAME=>'evtSrcType',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>2],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'LockFilter']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Lock the selected events in filter.

Example:
call:<syno_url>/webapi/entry.cgi?reason="1,3,8"&api="SYNO.SurveillanceStation.Event"&locked=0&version="4"&toTime=0&cameraIds="1,2,3"&evtSrcType=2&fromTime=0&method="LockFilter"&evtSrcId=3
'
          ],
          'UnLock' => [
            FUNCTION_NAME=>'UnLock',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'archived',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'point out that this is archived or not'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event'],
                [VALUE_NAME=>'idList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'[{"dsId": 0, "id":"0:5997"}]'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'4'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'UnLock']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'Unlock the selected events.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.Event"&idList=[{"dsId": 0, "id":"0:5997"}]&version="4"&method="UnLock"
json:{"dsId": 0, "id":"0:5997"}
'
          ],
          'UnLockFilter' => [
            FUNCTION_NAME=>'UnLockFilter',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'reason',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'1,3,8'],
                [VALUE_NAME=>'cameraIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'1,2,3'],
                [VALUE_NAME=>'fromTime',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'toTime',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'locked',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Query locked'],
                [VALUE_NAME=>'evtSrcId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'CONDITIONAL '],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'4'],
                [VALUE_NAME=>'evtSrcType',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>2],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'UnLockFilter']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Unlock the selected events in filter.

Example:
call:<syno_url>/webapi/entry.cgi?reason="1,3,8"&api="SYNO.SurveillanceStation.Event"&locked=0&version="4"&toTime=0&cameraIds="1,2,3"&evtSrcType=2&fromTime=0&method="UnLockFilter"&evtSrcId=3
'
          ],
          'Lock' => [
            FUNCTION_NAME=>'Lock',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'archived',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'point out that this is archived or not'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event'],
                [VALUE_NAME=>'idList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'[{"dsId": 0, "id":"0:5997"}]'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'4'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Lock']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'Lock the selected events

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.Event"&idList=[{"dsId": 0, "id":"0:5997"}]&version="4"&method="Lock"
json:{"dsId": 0, "id":"0:5997"}
'
          ],
          'Download' => [
            FUNCTION_NAME=>'Download',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'eventId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>5753],
                [VALUE_NAME=>'mountId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'analyevent',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'4'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Download']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Download event.

Example:
call:<syno_url>/webapi/entry.cgi?eventId=5753&version="4"&mountId=0&api="SYNO.SurveillanceStation.Event"&analyevent=false&method="Download"
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Event.Export'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Event.Export',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides method to query event information.',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid',405=>'CMS server connection failed',414=>'Some events not exist',439=>'Too many items selected']],
        SERVICE_FUNCTIONS=>[
          'Load' => [
            FUNCTION_NAME=>'Load',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'start',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>100],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event.Export'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Load']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'exporttask',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'evtexptotal',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Handle load event export.

Example:
call:<syno_url>/webapi/entry.cgi?start=0&api="SYNO.SurveillanceStation.Event.Export"&limit=100&version="1"&method="Load"

Returns:
json:{"exporttask": [{"status": 0,"srcDsId": 0,"exported_count": 1,"start_time": 1425398400,"name": "asdf","dstdir": "test","total_count": 30,"dstDsId": 0,"id": 5,"stop_time": 1425484800}],"evtexptotal": 1}
'
          ],
          'CheckName' => [
            FUNCTION_NAME=>'CheckName',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'name',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'asdf'],
                [VALUE_NAME=>'share',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'test'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event.Export'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'CheckName']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'conflict',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Check the name of export event.

Example:
call:<syno_url>/webapi/entry.cgi?name="asdf"&share="test"&api="SYNO.SurveillanceStation.Event.Export"&version="1"&dsId=0&method="CheckName"

Returns:
json:{"conflict": 0}
'
          ],
          'CamEnum' => [
            FUNCTION_NAME=>'CamEnum',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event.Export'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'CamEnum']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'camlist',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Get list information of all cameras.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.Event.Export"&dsId=0&version="1"&method="CamEnum"

Returns:
json:{"camlist": [{"status": 3,"enable": true,"checked": false,"name": "M1031-W","deleted": false,"idOnRecServ": 0,"ipaddr": "10.13.22.115","id": 32}]}
'
          ],
          'CheckAvailableExport' => [
            FUNCTION_NAME=>'CheckAvailableExport',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'freeSize',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>881508],
                [VALUE_NAME=>'startTime',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1425398400],
                [VALUE_NAME=>'stopTime',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1425484800],
                [VALUE_NAME=>'camIdList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'32,33,34'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'CheckAvailableExport'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event.Export']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'result',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Check if the destination folder is available for export or not.

Example:
call:<syno_url>/webapi/entry.cgi?camIdList="32,33,34"&version="1"&freeSize=881508&method="CheckAvailableExport"&api="SYNO.SurveillanceStation.Event.Export"&startTime=1425398400&stopTime=1425484800

Returns:
json:{"result": 0}
'
          ],
          'Save' => [
            FUNCTION_NAME=>'Save',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'name',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Name of export task'],
                [VALUE_NAME=>'srcDsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'dstDsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'dstdir',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Directory of destination'],
                [VALUE_NAME=>'freesize',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>881508],
                [VALUE_NAME=>'start_time',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1425398400],
                [VALUE_NAME=>'stop_time',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1425484800],
                [VALUE_NAME=>'isoverwrite',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Remove the conflict dir with user'],
                [VALUE_NAME=>'camlistid',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'List of camera Id'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Save'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event.Export'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Handle save event export.

Example:
call:<syno_url>/webapi/entry.cgi?srcDsId=0&isoverwrite=0&name="asdf"&dstdir="test"&start_time=1425398400&freesize=881508&method="Save"&api="SYNO.SurveillanceStation.Event.Export"&stop_time=1425484800&version="1"&camlistid="32,33,34"&dstDsId=0
'
          ],
          'GetEvtExpInfo' => [
            FUNCTION_NAME=>'GetEvtExpInfo',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'startTime',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1425398400],
                [VALUE_NAME=>'stopTime',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1425484800],
                [VALUE_NAME=>'camIdList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'List of camera Id'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetEvtExpInfo'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event.Export']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'eventIdList',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'totalEvtCnt',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'fisheyeIdList',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'totalEvtSize',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camEvtInfoList',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Get event export information from recording server.

Example:
call:<syno_url>/webapi/entry.cgi?camIdList="32,33,34"&version="1"&method="GetEvtExpInfo"&api="SYNO.SurveillanceStation.Event.Export"&startTime=1425398400&stopTime=1425484800

Returns:
json:{"eventIdList": "","totalEvtCnt": 0,"fisheyeIdList": "","totalEvtSize": 0,"camEvtInfoList": [{"camId": 32,"camEvtList": [],"camStoragePath": "/var/services/surveillance/M1031-W"}]}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Event.Mount'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Event.Mount',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides several methods to access the information of exporting task of Surveillance Station. (Ex: Get
information/count number of events in task, Check the availability of task name and exporting task,
Save/Load event task)',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid',405=>'CMS server connection failed',414=>'Some events not exist',439=>'Too many items selected']],
        SERVICE_FUNCTIONS=>[
          'Load' => [
            FUNCTION_NAME=>'Load',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Event.Mount'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Load']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'eventmount',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Load the event mount.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.Event.Mount"&version="1"&method="Load"

Returns:
json:{"total": 1,"eventmount": [{"status": 0,"enable": true,"start_time": 1424016000,"name": "12314144","expid": "20150216095031-0011323CC391","camlist": [{"analyticsDirection": 0,"analyticsType": 1,"analyticsDwellTime": 5,"analyticsRegion":"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000011111111111111111111111111111111111000001111111111111111111111111111111111100000111111111111111111111111111111111110000011111111111111111111111111111111111000001111111111111111111111111111111111100000111111111111111111111111111111111110000011111111111111111111111111111111111000001111111111111111111111111111111111100000111111111111111111111111111111111110000011111111111111111111111111111111111000001111111111111111111111111111111111100000111111111111111111111111111111111110000011111111111111111111111111111111111000001111111111111111111111111111111111100000111111111111111111111111111111111110000011111111111111111111111111111111111000001111111111111111111111111111111111100000111111111111111111111111111111111110000011111111111111111111111111111111111000001111111111111111111111111111111111100000111111111111111111111111111111111110000011111111111111111111111111111111111000001111111111111111111111111111111111100000111111111111111111111111111111111110000011111111111111111111111111111111111","camMountType": 0,"id": 38,"name": "FE8172-001","analyticsSens": 1,"feRegionList": [{"camId": 38,"name": "Default Region","zoom": 25,"posX": 500,"posY": 500,"type": 0,"id": 26},{"camId": 38,"name": "Default Panorama","zoom": 25,"posX": 500,"posY": 250,"type": 1,"id": 27},{"camId": 38,"name": "Default Double Panorama","zoom": 25,"posX": 500,"posY": 250,"type": 2,"id": 28},{"camId": 38,"name": "Default Triple View","zoom": 25,"posX": 500,"posY": 250,"type": 3,"id": 29},{"camId": 38,"name": "Default Quad View","zoom": 25,"posX": 500,"posY": 250,"type": 4,"id": 30}],"fisheyeType": 1,"analyticsObjSize": 2}],"dsId": 0,"id": 4,"stop_time": 1424102400}]}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Device'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Device',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides a method to load information of event mount in Surveillance Station.',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',410=>'Service is not enabled']],
        SERVICE_FUNCTIONS=>[
          'ListVS' => [
            FUNCTION_NAME=>'ListVS',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'offset',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>10],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Device'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ListVS'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'offset',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'visualStations',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'List all installed Visual Stations.

Example:
call:<syno_url>/webapi/SurveillanceStation/device.cgi?api=SYNO.SurveillanceStation.Device&method=ListVS&version=1&offset=10&limit=1

Returns:
json:{"total": 38,"offset": 10,"visualStations": [{"id": 922,"name": "VS80","dhcp": true,"ip": "192.168.38.91","macAddr": "00:11:32:AA:BB:CC","platform": "VS80","version": "1.3-0303","enable": true,"lock": false,"status": 0}]}
'
          ],
          'ListCMS' => [
            FUNCTION_NAME=>'ListCMS',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'offset',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>10],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>2],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Device'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ListCMS'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'offset',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'recordingServers',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'host',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'List all installed Slave DSs.

Example:
call:<syno_url>/webapi/device.cgi?api=SYNO.SurveillanceStation.Device&method=ListCMS&version=1&offset=10&limit=2

Returns:
json:{"total": 38,"offset": 10,"recordingServers": [{"id": 1,"name": "My DS 1","ip": "192.168.1.101","port": 5000,"enable": true,"status": 0},{"id": 2,"name": "My DS 2","ip": "192.168.1.102","port": 5000,"enable": false,"status": 1}],"host": {"id": 0,"name": "Host DS","ip": "192.168.1.111","port": 5000,"enable": true,"status": 0}}
'
          ],
          'GetServiceSetting' => [
            FUNCTION_NAME=>'GetServiceSetting',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Device'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetServiceSetting'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'cmsEnabled',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'cmsType',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'vsEnabled',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get the setting of Centralized Management and Visual Station service.

Example:
call:<syno_url>/webapi/SurveillanceStation/device.cgi?api=SYNO.SurveillanceStation.Device&method=GetServiceSetting&version=1

Returns:
json:{"cmsEnabled": true,"cmsType": 0,"vsEnabled": false}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.CMS'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.CMS',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides methods to get device information such as Visual Station and Slave DS.',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Invalid parameter',415=>'message connect failed']],
        SERVICE_FUNCTIONS=>[
          'Redirect' => [
            FUNCTION_NAME=>'Redirect',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>3],
                [VALUE_NAME=>'webAPI',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'{"api": "SYNO.SurveillanceStation.AddOns", "version": 1, "method":"List"}'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Redirect']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'Redirect the webAPI to target DS.

Example:
call:<syno_url>/webapi/entry.cgi?webAPI={"api": "SYNO.SurveillanceStation.AddOns", "version": 1, "method":"List"}&api="SYNO.SurveillanceStation.CMS"&dsId=3&version="1"&method="Redirect"
json:{"api": "SYNO.SurveillanceStation.AddOns", "version": 1, "method":"List"}
'
          ],
          'ModifySharePriv' => [
            FUNCTION_NAME=>'ModifySharePriv',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'privSet',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'shareName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ModifySharePriv']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'privOrg',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Modify share privilege.

Example:
call:<syno_url>/webapi/entry.cgi?shareName=""&privSet=0&api="SYNO.SurveillanceStation.CMS"&version="1"&method="ModifySharePriv"

Returns:
json:{"privOrg": 0}
'
          ],
          'ApplyOption' => [
            FUNCTION_NAME=>'ApplyOption',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'central_auto_video_relay',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'central_enable',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'central_mode',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'central_rec_mask_mode',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'central_rec_sync_time',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'nvr_enable',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'nvr_lang',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'1"¢ral_rec_mask_mode=fals'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ApplyOption"¢ral_enable=tru']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'central_paired_host_model',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'central_rec_status',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'central_auto_video_relay',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'enable_video_relay',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'confirm_key',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'central_paired_host_name',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'nvr_enable',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'central_rec_sync_time',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'central_mode',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'central_locked',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'key',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'vs_enable',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'central_rec_mask_mode',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'central_host_ip',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'nvr_lang',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'central_enable',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Apply the option setting.

Example:
call:<syno_url>/webapi/entry.cgi?central_auto_video_relay=false¢ral_rec_sync_time=false¢ral_mode="1"&api="SYNO.SurveillanceStation.CMS"&version="1"¢ral_rec_mask_mode=false&method="ApplyOption"¢ral_enable=true

Returns:
json:{"central_paired_host_model": "","central_rec_status": 1,"central_auto_video_relay": true,"enable_video_relay": false,"confirm_key": "********","central_paired_host_name": "","nvr_enable": true,"central_rec_sync_time": false,"success": true,"central_mode": "1","central_locked": false,"key": "********","vs_enable": true,"central_rec_mask_mode": false,"central_host_ip": "","nvr_lang": "","central_enable": true}
'
          ],
          'GetInfo' => [
            FUNCTION_NAME=>'GetInfo',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'isPolling',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetInfo']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'central_paired_host_model',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'central_rec_status',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'central_auto_video_relay',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'enable_video_relay',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'confirm_key',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'central_paired_host_name',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'nvr_enable',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'central_rec_sync_time',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'central_mode',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'central_locked',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'key',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'vs_enable',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'central_rec_mask_mode',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'central_host_ip',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'nvr_lang',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'central_enable',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get CMS related info.

Example:
call:<syno_url>/webapi/entry.cgi?isPolling=false&api="SYNO.SurveillanceStation.CMS"&version="1"&method="GetInfo"

Returns:
json:{"central_paired_host_model": "","central_rec_status": 1,"central_auto_video_relay": true,"enable_video_relay": false,"confirm_key": "********","central_paired_host_name": "","nvr_enable": true,"central_rec_sync_time": false,"success": true,"central_mode": "1","central_locked": false,"key": "********","vs_enable": true,"central_rec_mask_mode": false,"central_host_ip": "","nvr_lang": "","central_enable": true}
'
          ],
          'DoSyncData' => [
            FUNCTION_NAME=>'DoSyncData',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'syncType',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'syncTargetId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'DoSyncData']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'remainCnt',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'syncDataList',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Get log/recording data from the target DS.

Example:
call:<syno_url>/webapi/entry.cgi?version="1"&syncType=0&syncTargetId=1&api="SYNO.SurveillanceStation.CMS"&limit=0&method="DoSyncData"

Returns:
json:{"remainCnt": 0,"syncDataList": [{"itemId": 0,"level": 1,"logType": 321912959,"timestamp": 1422347919,"event_msg": "This recording server was disabled by [DS1813p_WC].","logGrp": 0,"id": 9}]}
'
          ],
          'CheckSambaEnabled' => [
            FUNCTION_NAME=>'CheckSambaEnabled',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'CheckSambaEnabled']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'sambaEnabled',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Check Samba service.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.CMS"&version="1"&method="CheckSambaEnabled"

Returns:
json:{"sambaEnabled": true}
'
          ],
          'BatCheckSambaService' => [
            FUNCTION_NAME=>'BatCheckSambaService',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'dsIdList',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'3'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'BatCheckSambaService']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'allSambaEnabled',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Check if samba service on all related rec servers is enabled.

Example:
call:<syno_url>/webapi/entry.cgi?dsIdList="3"&api="SYNO.SurveillanceStation.CMS"&version="1"&method="BatCheckSambaService"

Returns:
json:{"allSambaEnabled": true}
'
          ],
          'GetMDSnapshot' => [
            FUNCTION_NAME=>'GetMDSnapshot',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetMDSnapshot']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get encoded single-image snapshot of target camera.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.CMS"&camId=0&version="1"&method="GetMDSnapshot"

Returns:
json:{"snapshot": (CONTENT OF ENCODED MD SNAPSHOT)}
'
          ],
          'GetCMSStatus' => [
            FUNCTION_NAME=>'GetCMSStatus',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetCMSStatus']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'CMSEnabled',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get CMS status.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.CMS"&camId=1&version="1"&method="GetCMSStatus"

Returns:
json:{"CMSEnabled": true}
'
          ],
          'EnableSamba' => [
            FUNCTION_NAME=>'EnableSamba',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'EnableSamba']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Enable Samba service.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.CMS"&version="1"&method="EnableSamba"
'
          ],
          'NotifyCMSBreak' => [
            FUNCTION_NAME=>'NotifyCMSBreak',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'NotifyCMSBreak']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Notify the target slave DS to disconnect.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.CMS"&version="1"&method="NotifyCMSBreak"
'
          ],
          'LockSelf' => [
            FUNCTION_NAME=>'LockSelf',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'locked',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'LockSelf']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Lock the recording server to prevent the changing of settings from other server.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.CMS"&locked=false&version="1"&method="LockSelf"
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.CMS.GetDsStatus'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.CMS.GetDsStatus',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides several methods to access ds in CMS. (Ex: Get information, Apply option, Lock, Modify share
privilege, Check Samba Enabled/Enable samba, Redirect, Get snapshot)',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Invalid parameter',415=>'message connect failed']],
        SERVICE_FUNCTIONS=>[
          'EnableCMS' => [
            FUNCTION_NAME=>'EnableCMS',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'adminUsername',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'admin"¢ral_rec_mask_mode="false'],
                [VALUE_NAME=>'adminPasswd',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Password'],
                [VALUE_NAME=>'central_rec_mask_mode',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'central_rec_sync_time',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS.GetDsStatus"¢ral_rec_sync_time="false'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'EnableCMS']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'status',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Enable DS into recording server mode.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.CMS.GetDsStatus"¢ral_rec_sync_time="false"&adminPasswd="123456"&version="1"&adminUsername="admin"¢ral_rec_mask_mode="false"&method="EnableCMS"

Returns:
json:{"status": 0}
'
          ],
          'UnPair' => [
            FUNCTION_NAME=>'UnPair',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'dminUsername',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'key',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'mac',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'cmsMode',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>2],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS.GetDsStatus'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'UnPair']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Unpair recording server from the host.

Example:
call:<syno_url>/webapi/entry.cgi?cmsMode=2&api="SYNO.SurveillanceStation.CMS.GetDsStatus"&version="1"&method="UnPair"
'
          ],
          'GetFreeSpace' => [
            FUNCTION_NAME=>'GetFreeSpace',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS.GetDsStatus'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetFreeSpace']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'freeSizeMb',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Get free size of memory in MB.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.CMS.GetDsStatus"&version="1"&method="GetFreeSpace"

Returns:
json:{"freeSizeMb": 922467}
'
          ],
          'Lock' => [
            FUNCTION_NAME=>'Lock',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'lock',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'adminUsername',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'admin'],
                [VALUE_NAME=>'key',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'123456'],
                [VALUE_NAME=>'mac',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'0011323CC393'],
                [VALUE_NAME=>'masterAuthKey',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'14C0MON392200'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS.GetDsStatus'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Lock']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'status',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Handle lock slave ds.

Example:
call:<syno_url>/webapi/entry.cgi?masterAuthKey="14C0MON392200"&version="1"&lock=false&mac="0011323CC393"&api="SYNO.SurveillanceStation.CMS.GetDsStatus"&key="123456"&adminUsername="admin"&method="Lock"

Returns:
json:{"status": 0}
'
          ],
          'Test' => [
            FUNCTION_NAME=>'Test',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'slaveDsIp',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS.GetDsStatus'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Test']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'mac',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'dsModel',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'cms_locked',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'status',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'keyTotal',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'auth_key',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'licenseList',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'keyUsed',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Get the information of target DS.

Example:
call:<syno_url>/webapi/entry.cgi?slaveDsIp=""&api="SYNO.SurveillanceStation.CMS.GetDsStatus"&version="1"&method="Test"

Returns:
json:{"mac": "0011323CC393","dsModel": "DS2015xs","cms_locked": false,"status": 8,"keyTotal": 0,"auth_key": "14C0MON392200","licenseList": "","keyUsed": 0}
'
          ],
          'Logout' => [
            FUNCTION_NAME=>'Logout',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'adminUsername',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'admin'],
                [VALUE_NAME=>'key',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'mac',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS.GetDsStatus'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Logou']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'status',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Handle logout slave ds.

Example:
call:<syno_url>/webapi/entry.cgi?version="1"&mac=""&api="SYNO.SurveillanceStation.CMS.GetDsStatus"&key=""&adminUsername="admin"&method="Logout

Returns:
json:{"status": 3}
'
          ],
          'Pair' => [
            FUNCTION_NAME=>'Pair',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'dsname',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'DiskStation'],
                [VALUE_NAME=>'slaveDsIp',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'10.13.21.18'],
                [VALUE_NAME=>'port',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>5000],
                [VALUE_NAME=>'masterAuthKey',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'14C0MON392200'],
                [VALUE_NAME=>'model',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'DS414j'],
                [VALUE_NAME=>'mac',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'00113235D696'],
                [VALUE_NAME=>'cms_locked',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'cms_masked',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'cms_sync_time',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Pair'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS.GetDsStatus']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'status',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'mac',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'dp_version',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'cookie',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'ss_version',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'auth_key',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'ds_model',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'dsm_version',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'serial_num',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'Handle pair slave ds.

Example:
call:<syno_url>/webapi/entry.cgi?cms_sync_time=true&masterAuthKey="14C0MON392200"&version="1"&slaveDsIp="10.13.21.18"&dsname="DiskStation"&cms_locked=true&method="Pair"&mac="00113235D696"&api="SYNO.SurveillanceStation.CMS.GetDsStatus"&cms_masked=true&model="DS414j"&port=5000

Returns:
json:{"status": 2,"mac": "","dp_version": "3.2-0604","cookie": "","ss_version": "7.1-3900","auth_key": "","ds_model": "","dsm_version": "5.1-5022","serial_num": "14C0MON392200"}
'
          ],
          'Login' => [
            FUNCTION_NAME=>'Login',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'adminUsername',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'admin'],
                [VALUE_NAME=>'key',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'mac',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'masterAuthKey',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'hostName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'hostPort',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ignoreAuthError',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'no'],
                [VALUE_NAME=>'hostDisconnect',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'blUpdateVolSpace',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'enable_rec',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'cms_locked',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'cms_masked',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'cms_sync_time',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS.GetDsStatus'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Login']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'cms_sync_time',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'default_license_cnt',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'cms_locked',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'cookie',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camera_info',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'dsm_version',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'cam_count',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'serial_num',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'dp_version',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'status',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'ss_version',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'cms_masked',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Handle login slave ds.

Example:
call:<syno_url>/webapi/entry.cgi?enable_rec=false&masterAuthKey=""&version="1"&hostName=""&hostDisconnect=false&mac=""&api="SYNO.SurveillanceStation.CMS.GetDsStatus"&ignoreAuthError="no"&key=""&hostPort=0&adminUsername="admin"&blUpdateVolSpace=false&method="Login"

Returns:
json:{"cms_sync_time": false,"default_license_cnt": 2,"cms_locked": false,"cookie": "","camera_info": [],"dsm_version": "5.1-5022","cam_count": 0,"serial_num": "14C0MON392200","dp_version": "3.2-0604","status": 3,"ss_version": "7.1-3900","cms_masked": false}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.CMS.SlavedsWizard'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.CMS.SlavedsWizard',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides several methods to access the status information of slave ds in CMS. (Ex: Enable CMS,
Unpair/Pair CMS, GetFreeSpace, Lock, Test, Logout/Login)',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Invalid parameter',415=>'message connect failed']],
        SERVICE_FUNCTIONS=>[
          'Save' => [
            FUNCTION_NAME=>'Save',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'slavedsName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'DiskStation'],
                [VALUE_NAME=>'slavedsModel',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'DS414j'],
                [VALUE_NAME=>'slavedsPort',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>5000],
                [VALUE_NAME=>'slavedsVersion',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'7.0-3736'],
                [VALUE_NAME=>'slavedsMaxCamNum',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>8],
                [VALUE_NAME=>'slavedsId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'slavedsIP',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'10.13.21.16'],
                [VALUE_NAME=>'slavedsEnable',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1],
                [VALUE_NAME=>'slavedsCamCnt',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1],
                [VALUE_NAME=>'adminUsername',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'admin'],
                [VALUE_NAME=>'adminPasswd',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'123456'],
                [VALUE_NAME=>'cms_locked',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'cms_masked',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'cms_sync_time',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.CMS.SlavedsWizard'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Save']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Handle save slave ds

Example:
call:<syno_url>/webapi/entry.cgi?cms_sync_time=true&slavedsPort=5000&version="1"&slavedsCamCnt=1&slavedsIP="10.13.21.16"&slavedsName="DiskStation"&adminUsername="admin"&cms_locked=true&api="SYNO.SurveillanceStation.CMS.SlavedsWizard"&slavedsVersion="7.0-3736"&slavedsEnable=1&cms_masked=true&adminPasswd="123456"&slavedsModel="DS414j"&slavedsMaxCamNum=8&slavedsId=""&method="Save"
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Log'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Log',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides a method to save the setting of slave ds in CMS of Surveillance Station.',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed']],
        SERVICE_FUNCTIONS=>[
          'CountByCategory' => [
            FUNCTION_NAME=>'CountByCategory',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'slavedsName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'start',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'level',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'filterCamera',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'false'],
                [VALUE_NAME=>'cameraIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'from',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'to',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'keyword',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'keywordDsId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'time2String',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'no'],
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>-1],
                [VALUE_NAME=>'srcType',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'timezoneOffset',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>480],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'CountByCategory'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Log'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'date',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'log_type',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'server',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Get the count number of log in category.

Example:
call:<syno_url>/webapi/entry.cgi?from=0&keywordDsId=""&keyword=""&level=""&cameraIds=""&method="CountByCategory"&to=0&api="SYNO.SurveillanceStation.Log"&limit=0&srcType=0&version="1"&filterCamera="false"&dsId=-1&start=0&timezoneOffset=480&time2String="no"

Returns:
json:{"date": {"-1": 2,"yyyy/mm/dd": {"am": 0,"-1": 2,"pm": 2}},"total": 2,"log_type": {"1": 2,"3": 0,"2": 0,"-1": 2514},"server": {"X": 2,"-1": 2}}
'
          ],
          'Clear' => [
            FUNCTION_NAME=>'Clear',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'blClearAll',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'level',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>3],
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>-1],
                [VALUE_NAME=>'srcType',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'filterCamera',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'false'],
                [VALUE_NAME=>'cameraIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'from',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'to',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'keyword',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'keywordDsId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'timezoneOffset',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Log'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Clear']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Clear the selected logs.

Example:
call:<syno_url>/webapi/entry.cgi?from=0&version="1"&keyword=""&level=3&cameraIds=""&to=0&api="SYNO.SurveillanceStation.Log"&blClearAll=false&srcType=0&filterCamera="false"&dsId=-1&method="Clear"&keywordDsId=""

Returns:
json:{"success": true}
'
          ],
          'List' => [
            FUNCTION_NAME=>'List',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'start',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'level',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'filterCamera',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'false'],
                [VALUE_NAME=>'cameraIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'from',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'to',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'keyword',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'keywordDsId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'time2String',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'no'],
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>-1],
                [VALUE_NAME=>'srcType',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'all',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'blIncludeRecCnt',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'blIncludeAuInfo',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'List'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Log']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'timestamp',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'log',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'recCntData',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Get the detail information of log.

Example:
call:<syno_url>/webapi/entry.cgi?all=false&from=0&version="1"&keyword=""&level=""&cameraIds=""&blIncludeRecCnt=""&method="List"&api="SYNO.SurveillanceStation.Log"&to=0&blIncludeAuInfo=""&limit=0&srcType=0&filterCamera="false"&dsId=-1&start=0&keywordDsId=""&time2String="no"

Returns:
json:{"timestamp": "1423806343","total": 2514,"log": [{"update_time": 1423801204,"dsId": 0,"time": 1423801204,"type": 1,"id": "0_5932","desc": "Recordings of camera [FE8172-001] reached size limit. [52] events was deleted."}],"success": true,"recCntData": {"date": {"-1": 2,"yyyy/mm/dd": {"am": 0,"-1": 2,"pm": 2}},"total": 2,"log_type": {"1": 2,"3": 0,"2": 0,"-1": 2514},"server": {"X": 2,"-1": 2}}}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.License'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.License',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides several methods to access log information of Surveillance Station. (Ex: get/clear log
information detail, count log information)',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',407=>'CMS closed',412=>'Need to add license',413=>'Reach the maximum of platform']],
        SERVICE_FUNCTIONS=>[
          'Load' => [
            FUNCTION_NAME=>'Load',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'num_only',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.License'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Load']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'localCamCnt',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'blCMSLosthost',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Load license data.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.License"&num_only=1&version="1"&method="Load"

Returns:
json:{"localCamCnt": 5,"blCMSLosthost": false,"success": true}
'
          ],
          'CheckQuota' => [
            FUNCTION_NAME=>'CheckQuota',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camServerId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1],
                [VALUE_NAME=>'ip',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'IP of the camera'],
                [VALUE_NAME=>'port',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Port of the camera'],
                [VALUE_NAME=>'vendor',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Vendor of the camera'],
                [VALUE_NAME=>'model',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Model of the camera'],
                [VALUE_NAME=>'camList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'[{"ip": "10.13.22.141", "model": "DCS-3110", "vendor": "D-Link", "port": 80}]'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.License'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'CheckQuota']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ip',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'model',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'vendor',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'port',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Check the quota of license.

Example:
call:<syno_url>/webapi/entry.cgi?camServerId=1&camList=[{"ip": "10.13.22.141", "model": "DCS-3110", "vendor": "D-Link", "port": 80}]&api="SYNO.SurveillanceStation.License"&version="1"&method="CheckQuota"
json:{"ip": "10.13.22.141", "model": "DCS-3110", "vendor": "D-Link", "port": 80}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Stream'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Stream',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides a method to acquire information of license in Surveillance Station and check if the number
of license is sufficient or not.',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed']],
        SERVICE_FUNCTIONS=>[
          'EventStream' => [
            FUNCTION_NAME=>'EventStream',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'writeHeader',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'analyevent',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'mountId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Stream'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'EventStream']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get HTTP video stream of the specific recording event

Example:
call:<syno_url>/webapi/entry.cgi?version="1"&writeHeader=true&mountId=0&api="SYNO.SurveillanceStation.Stream"&analyevent=false&method="EventStream"
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.ActionRule'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.ActionRule',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides methods to get Live View or Event video stream.',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid']],
        SERVICE_FUNCTIONS=>[
          'Save' => [
            FUNCTION_NAME=>'Save',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'name',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'ruleType',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'actType',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'evtSrc',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Conditional'],
                [VALUE_NAME=>'evtDsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Conditional'],
                [VALUE_NAME=>'evtDevId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Conditional'],
                [VALUE_NAME=>'evtId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'evtItem',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Conditional'],
                [VALUE_NAME=>'evtMinIntvl',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Conditional'],
                [VALUE_NAME=>'actSrc',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'actDsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Conditional'],
                [VALUE_NAME=>'actDevId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Conditional'],
                [VALUE_NAME=>'actId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'actTimes',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Conditional'],
                [VALUE_NAME=>'actTimeUnit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'actTimeDur',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'actItem',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Conditional'],
                [VALUE_NAME=>'actRetItem',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Conditional'],
                [VALUE_NAME=>'actSchedule',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'extUrl',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Conditional'],
                [VALUE_NAME=>'userName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Conditional'],
                [VALUE_NAME=>'password',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Conditional'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.ActionRule']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Save the action rule.

Example:
- IP70 alarm compro IP70snapshot"&actTimes=2&evtDsId=0&evtItem=-1&method="Save"&version="1"&actTimeUnit=1&actItem=-1&actSrc=0&evtDevId=19&evtId=6&actRetItem=-1
call:<syno_url>/webapi/entry.cgi?userName=""&ruleType=0&extUrl=""&actDsId=0&actTimeDur=5&evtSrc=0&api="SYNO.SurveillanceStation.ActionRule"&actType=0&id=-1&actId=4&actSchedule="111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"&actDevId=19&password=""&name="compro
'
          ],
          'DownloadHistory' => [
            FUNCTION_NAME=>'DownloadHistory',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.ActionRule'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'DownloadHistory']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Download action rule histories.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.ActionRule"&version="1"&method="DownloadHistory"
'
          ],
          'SendData2Player' => [
            FUNCTION_NAME=>'SendData2Player',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.ActionRule'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SendData2Player']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'This method is called by player to build the connection of Surveillance Web Client. It will output audio data
through http multiple response. The audio data format is 2 channel, 8000Hz, Sample size 16, Sample type
signed integer, byte order littleEndian, u-law.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.ActionRule"&version="1"&method="SendData2Player"
'
          ],
          'DeleteHistory' => [
            FUNCTION_NAME=>'DeleteHistory',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'idList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.ActionRule'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'DeleteHistory']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Delete all histories of action rule.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.ActionRule"&idList="1,2,3"&version="1"&method="DeleteHistory"
'
          ],
          'List' => [
            FUNCTION_NAME=>'List',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'Start',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'start',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.ActionRule'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'List']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'actRule',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'List action rules.

Example:
call:<syno_url>/webapi/entry.cgi?start=0&api="SYNO.SurveillanceStation.ActionRule"&limit=0&version="1"&method="List"

Returns:
json:{"actRule": [{"ruleType": 0,"extUrl": "","actDevName": "axis p3384","evtDevName": "axis p3384","actType": 0,"id": 82,"actId": 9,"actSchedule":"111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111","actDevId": 63,"actTimes": 1,"evtId": 8,"actRetItem": {"id": -1,"name": ""},"status": 2,"userName": "","actTimeDur": 1,"actDsId": 0,"evtSrc": 0,"password": "","actTimeUnit": 1,"evtDsId": 0,"name": "p3384 audio detected p3384 audio output","actItem": {"id": 20,"name": "syno1"},"actSrc": 0,"evtDevId": 63,"evtItem": -1}],"total": 1}
'
          ],
          'Disable' => [
            FUNCTION_NAME=>'Disable',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'idList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.ActionRule'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Disable']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Disable action rules.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.ActionRule"&idList="1,2"&version="1"&method="Disable"
'
          ],
          'Enable' => [
            FUNCTION_NAME=>'Enable',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'idList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.ActionRule'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Enable']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Enable action rules.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.ActionRule"&idList="1,2"&version="1"&method="Enable"
'
          ],
          'ListHistory' => [
            FUNCTION_NAME=>'ListHistory',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'start',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.ActionRule'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ListHistory']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'history',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'List all histories of action rule.

Example:
call:<syno_url>/webapi/entry.cgi?start=0&api="SYNO.SurveillanceStation.ActionRule"&limit=100&version="1"&method="ListHistory"

Returns:
json:{"total": 100,"history": [{"ruleType": 0,"level": 1,"ruleName": "Cam motion detected triggered digital output","time": 1330639700,"id": 1,"actResult": 1},{"ruleType": 0,"level": 1,"ruleName": "Cam digital input triggered audio output","time": 1330639750,"id": 2,"actResult": 2},{"ruleType": 1,"level": 1,"ruleName": "Web client audio output","time": 1330639810,"id": 3,"actResult": 2}]}
'
          ],
          'Delete' => [
            FUNCTION_NAME=>'Delete',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'idList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.ActionRule'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Delete']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Delete action rules.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.ActionRule"&idList="1,2"&version="1"&method="Delete"
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Emap'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Emap',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides a method to acquire information of ActionRule in Surveillance Station. (Ex: Save/List/Delete,
Enable/Disable, DeleteHistory/ListHistory/DownloadHistory)',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid']],
        SERVICE_FUNCTIONS=>[
          'List' => [
            FUNCTION_NAME=>'List',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'offset',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get the list of all E-Maps.

Returns:
json:{"total": 20,"offset": 10,"emaps": [{"id": 30,"name": "Lobby","imageFormat": “PNG”,"imageWidth": 640,"imageHeight": 480,"items": [{"type": 0,"dsId": 0,"itemId": 50,"itemName": "M1054","xCoordinate": 10,"yCoordinate": 50,"direction": 5},{"type": 0,"dsId": 0,"itemId": 51,"itemName": "M1011","xCoordinate": 10,"yCoordinate": 400,"direction": 3},{"type": 1,"dsId": 0,"itemId": 31,"itemName": "2F","xCoordinate": 400,"yCoordinate": 400,"direction": 0}]},{"id": 31,"name": "2F","imageFormat": “JPG”,"imageWidth": 480,"imageHeight": 480,"items": [{"type": 0,"dsId": 0,"itemId": 52,"itemName": "M1054","xCoordinate": 20,"yCoordinate": 240,"direction": 4},{"type": 0,"dsId": 1,"itemId": 5,"itemName": "P1346","xCoordinate": 400,"yCoordinate": 25,"direction": 6}]}]}
'
          ],
          'GetInfo' => [
            FUNCTION_NAME=>'GetInfo',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'emapIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The list of ']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get specific E-Map settings.

Returns:
json:{"emaps": [{"id": 31,"name": "2F","imageFormat": “JPG”,"imageWidth": 480,"imageHeight": 480,"items": [{"type": 0,"dsId": 0,"itemId": 52,"itemName": "M1054","xCoordinate": 20,"yCoordinate": 240,"direction": 4},{"type": 0,"dsId": 1,"itemId": 5,"itemName": "P1346","xCoordinate": 400,"yCoordinate": 25,"direction": 6}]}]}
'
          ],
          'GetImage' => [
            FUNCTION_NAME=>'GetImage',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'emapId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique E'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Emap'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetImage'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get the image of the selected E-Map in its original format.

Example:
call:<syno_url>/webapi/SurveillanceStation/emap.cgi?api=SYNO.SurveillanceStation.Emap&method=GetImage&version=1&emapId=10
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Streaming'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Streaming',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides methods to get information about user-defined E-Maps.
Name Version Availability
SYNO.SurveillanceStation.Emap 1 Surveillance Station 6.1',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid',402=>'Camera disabled',403=>'Insufficient license']],
        SERVICE_FUNCTIONS=>[
          'LiveStream' => [
            FUNCTION_NAME=>'LiveStream',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Streaming'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'LiveStream'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get a HTTP Live View video stream of the camera with given ID.

Example:
call:<syno_url>/webapi/SurveillanceStation/streaming.cgi?api=SYNO.SurveillanceStation.Streaming&method=LiveStream&version=1&cameraId=10
'
          ],
          'EventStream' => [
            FUNCTION_NAME=>'EventStream',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'eventId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique event ID']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get HTTP video stream of the specific recording event.
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.AudioStream'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.AudioStream',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides methods to get Live View or Event video stream.
Name Version Availability
SYNO.SurveillanceStation.Streaming 1 Surveillance Station 6.0-2337',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid',402=>'Camera disabled',403=>'Insufficient license']],
        SERVICE_FUNCTIONS=>[
          'Stream' => [
            FUNCTION_NAME=>'Stream',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AudioStream'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Stream'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>2]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get Live View audio stream of the camera with given ID.

Example:
call:<syno_url>/webapi/SurveillanceStation/audioStreaming.cgi?api=SYNO.SurveillanceStation.AudioStream&method=Stream&version=2&cameraId=10
'
          ],
          'Query' => [
            FUNCTION_NAME=>'Query',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AudioStream'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Query'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>2],
                [VALUE_NAME=>'format',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The audio format']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Query the format of live view audio stream of the camera with given ID.

Example:
call:<syno_url>/webapi/SurveillanceStation/audioStreaming.cgi?api=SYNO.SurveillanceStation.AudioStream&method=Query&version=2&cameraId=10
'
          ],
          'Open' => [
            FUNCTION_NAME=>'Open',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AudioStream'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Open'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>2]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Open the live view audio stream of the camera with given ID.

Example:
call:<syno_url>/webapi/SurveillanceStation/audioStreaming.cgi?api=SYNO.SurveillanceStation.AudioStream&method=Open&version=2&cameraId=10
'
          ],
          'Close' => [
            FUNCTION_NAME=>'Close',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AudioStream'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Close'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Close live view audio streaming of the camera with given ID.

Example:
call:<syno_url>/webapi/SurveillanceStation/audioStreaming.cgi?api=SYNO.SurveillanceStation.AudioStream&method=Close&version=1&cameraId=10
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.VideoStream'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.VideoStream',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides methods to get Live View audio stream.
Name Version Availability
SYNO.SurveillanceStation.AudioStream 2 Surveillance Station 6.3',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid',402=>'Camera disabled',403=>'Insufficient license']],
        SERVICE_FUNCTIONS=>[
          'Stream' => [
            FUNCTION_NAME=>'Stream',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID'],
                [VALUE_NAME=>'SliceId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get Live View video stream of the camera with given ID.
'
          ],
          'Query' => [
            FUNCTION_NAME=>'Query',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.VideoStream'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Query'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'format',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The video format']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Query the format of liveview video stream of the camera with given ID.

Example:
call:<syno_url>/webapi/SurveillanceStation/videoStreaming.cgi?api=SYNO.SurveillanceStation.VideoStream&method=Query&version=1&cameraId=10
'
          ],
          'Open' => [
            FUNCTION_NAME=>'Open',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID'],
                [VALUE_NAME=>'format',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'hls'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.VideoStream'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Open'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Open liveview video stream of the camera with given ID.

Example:
call:<syno_url>/webapi/SurveillanceStation/videoStreaming.cgi?api=SYNO.SurveillanceStation.VideoStream&method=Open&version=1&cameraId=10&format=hls
'
          ],
          'Close' => [
            FUNCTION_NAME=>'Close',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Unique camera ID'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.VideoStream'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Close'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Close liveview video stream of the camera with given ID.

Example:
call:<syno_url>/webapi/SurveillanceStation/videoStreaming.cgi?api=SYNO.SurveillanceStation.VideoStream&method=Close&version=1&cameraId=10
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Notification'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Notification',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides methods to get Live View video stream.
Name Version Availability
SYNO.SurveillanceStation.VideoStream 1 Surveillance Station 6.3',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed']],
        SERVICE_FUNCTIONS=>[
          'GetRegisterToken' => [
            FUNCTION_NAME=>'GetRegisterToken',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetRegisterToken'],
                [VALUE_NAME=>'_version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'2'],
                [VALUE_NAME=>'content',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'XXX'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification'],
                [VALUE_NAME=>'eventTypes',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'19'],
                [VALUE_NAME=>'subject',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Title"_Respons']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'token',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'oauth_id',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Get the authorized token of DS.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Notification"&version="1"&method="GetRegisterToken"

Returns:
json:{"token":"MlFaYk9lK3RvSlU9fFBJaGVWLzhyVUhyRVZCMnRjelNvWUQrTm4rbU9malEwY0JlQ0tleHJHVFFGS2lheWR6QStOdGJjVGtYc0t4cms=","oauth_id": 836782578}

- This method has no specific response data. It returns an empty success response if it completes without error.
call:http://<syno_url>:5000/webapi/entry.cgi?_version="2"&content="XXX"&api="SYNO.SurveillanceStation.Notification"&eventTypes="19"&method="Set_CustomizedMessage"&subject="Title"_Response
'
          ],
          'GetCustomizedMessage' => [
            FUNCTION_NAME=>'GetCustomizedMessage',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'eventTypes',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'An integer indicate the type of event'],
                [VALUE_NAME=>'_eventType',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>19],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'2'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetCustomizedMessa_ge']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'defaultMessage',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'message',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Get message and subject from specified event

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_eventType=19&api="SYNO.SurveillanceStation.Notification"&version="2"&method="GetCustomizedMessa_ge"

Returns:
json:{"defaultMessage": {"content": "Dear user,\n\n%DEVICE_TYPE% %DEVICE_NAME% on %SS_HOSTNAME% has beensuccessfully added to Surveillance Station.\n\nSincerely,\n%SS_PKG_NAME%","subject": "%DEVICE_TYPE% %DEVICE_NAME% on %SS_HOSTNAME% has been added"},"message": {"content": "Body","subject": "Title"}}
'
          ],
          'SetVariables' => [
            FUNCTION_NAME=>'SetVariables',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ss_pkg_name',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The sender name of each notification'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Set the sender name of notifications

Example:
- intest"&method="SetVariables"
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Notification"&version="1"&ss_pkg_name="Synology
'
          ],
          'GetVariables' => [
            FUNCTION_NAME=>'GetVariables',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetVariables']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ss_pkg_name',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'Return the sender name of notifications

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Notification"&version="1"&method="GetVariables"

Returns:
json:{"ss_pkg_name": "Synology Surveillance Station"}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Notification.SMS'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Notification.SMS',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides a method to get authorized token of DS, and user can register push service by this token.
Name Version Availability
SYNO.SurveillanceStation.Notification 1 Surveillance Station 6.3
SYNO.SurveillanceStation.Notification 2 Surveillance Station 7.0',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid',402=>'Camera disabled',403=>'Insufficient license']],
        SERVICE_FUNCTIONS=>[
          'SendTestMessage' => [
            FUNCTION_NAME=>'SendTestMessage',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'smsEnable',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'smsProvider',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'clickate_ll'],
                [VALUE_NAME=>'userName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'SynoTest'],
                [VALUE_NAME=>'password',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'＃＃＃＃＃�_�＃＃'],
                [VALUE_NAME=>'confirmPassword',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'＃＃＃＃�_��＃＃�_�'],
                [VALUE_NAME=>'primaryPhoneCode',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'primaryPhonePrefix',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>' '],
                [VALUE_NAME=>'primaryPhoneNumber',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'1234567'],
                [VALUE_NAME=>'secondaryPhoneCode',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'secondaryPhonePrefix',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'secondaryPhoneNumber',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'setMinMessageInterval',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'minMessageInterval',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'hasSysSms',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'apiId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'XD'],
                [VALUE_NAME=>'_primaryPhoneCode',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'886'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.SMS'],
                [VALUE_NAME=>'min_MessageInterval',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'smsMethod',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>2],
                [VALUE_NAME=>'setMinMess_ageInterval',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>false],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SendTestMessage']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Send test message to the primary and secondary(if exists) phone number

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_primaryPhoneCode="886"&smsEnable=true&primaryPhonePrefix="+"&version="1"&smsProvider="clickate_ll"&apiId="XD"&primaryPhoneNumber="1234567"&api="SYNO.SurveillanceStation.Notification.SMS"&min_MessageInterval=1&password="%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC_%83%EF%BC%83%EF%BC%83"&confirmPassword="%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF_%BC%83%EF%BC%83%EF%BC%83%EF%BC_%83"&secondaryPhoneNumber=""&userName="SynoTest"&smsMethod=2&hasSysSms=false&setMinMess_ageInterval=false&secondaryPhonePrefix=""&secondaryPhoneCode=""&method="SendTestMessage"
'
          ],
          'GetSetting' => [
            FUNCTION_NAME=>'GetSetting',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.SMS'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetSetting']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'primaryPhoneCode',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'smsEnable',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'primaryPhonePrefix',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'smsMethod',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'primaryPhoneNumber',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'minMessageInterval',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'password',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'confirmPassword',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'userName',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'secondaryPhoneNumber',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'smsProvider',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'hasSysSms',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'setMinMessageInterval',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'secondaryPhonePrefix',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'secondaryPhoneCode',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'Get setting of notification by SMS

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Notification.SMS"&version="1"&method="GetSetting"

Returns:
json:{"primaryPhoneCode": "","smsEnable": false,"primaryPhonePrefix": "","smsMethod": 0,"primaryPhoneNumber": "","minMessageInterval": 1,"password": "","confirmPassword": "","userName": "","secondaryPhoneNumber": "","smsProvider": "clickatell","hasSysSms": false,"setMinMessageInterval": false,"secondaryPhonePrefix": "","secondaryPhoneCode": ""}
'
          ],
          'SetSetting' => [
            FUNCTION_NAME=>'SetSetting',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'smsEnable',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'smsProvider',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'clickate_ll'],
                [VALUE_NAME=>'userName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'SynoTest'],
                [VALUE_NAME=>'password',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'＃＃＃＃＃�_�＃＃'],
                [VALUE_NAME=>'confirmPassword',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'＃＃＃＃�_��＃＃�_�'],
                [VALUE_NAME=>'primaryPhoneCode',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'primaryPhonePrefix',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>' '],
                [VALUE_NAME=>'primaryPhoneNumber',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'1234567'],
                [VALUE_NAME=>'secondaryPhoneCode',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'secondaryPhonePrefix',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'secondaryPhoneNumber',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'setMinMessageInterval',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'minMessageInterval',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'hasSysSms',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'apiId',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'XD'],
                [VALUE_NAME=>'_primaryPhoneCode',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'886'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.SMS'],
                [VALUE_NAME=>'min_MessageInterval',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'smsMethod',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>2],
                [VALUE_NAME=>'setMinMess_ageInterval',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>false],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SetSetting']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Set SMS service settings

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_primaryPhoneCode="886"&smsEnable=true&primaryPhonePrefix="+"&version="1"&smsProvider="clickate_ll"&apiId="XD"&primaryPhoneNumber="1234567"&api="SYNO.SurveillanceStation.Notification.SMS"&min_MessageInterval=1&password="%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC_%83%EF%BC%83%EF%BC%83"&confirmPassword="%EF%BC%83%EF%BC%83%EF%BC%83%EF%BC%83%EF_%BC%83%EF%BC%83%EF%BC%83%EF%BC_%83"&secondaryPhoneNumber=""&userName="SynoTest"&smsMethod=2&hasSysSms=false&setMinMess_ageInterval=false&secondaryPhonePrefix=""&secondaryPhoneCode=""&method="SetSetting"
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Notification.PushService'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Notification.PushService',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'Notification.SMS doc
Name Version Availability
SYNO.SurveillanceStation.Notification.SMS 1 Surveillance Station 6.3',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid',402=>'Camera disabled',403=>'Insufficient license']],
        SERVICE_FUNCTIONS=>[
          'SendTestMessage' => [
            FUNCTION_NAME=>'SendTestMessage',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'attachSnapshot',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Does the push service provide snapshot'],
                [VALUE_NAME=>'enableInterval',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'mobileEnable',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'msgInterval',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1],
                [VALUE_NAME=>'primaryEmail',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'b95056@csie.ntu.edu.tw'],
                [VALUE_NAME=>'secondaryEmail',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'synoMailEnable',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'mail_recipient',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'b9_5056@csie.ntu.edu.tw'],
                [VALUE_NAME=>'_attachSnapshot',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.PushService'],
                [VALUE_NAME=>'synoMa_ilEnable',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SendTestMessage']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Send test message to the primary and secondary(if exists) phone number

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_attachSnapshot=true&primaryEmail="b95056@csie.ntu.edu.tw"&enableInterval=false&mail_recipient="b9_5056@csie.ntu.edu.tw"&api="SYNO.SurveillanceStation.Notification.PushService"&msgInterval=1&synoMa_ilEnable=true&secondaryEmail=""&version="1"&mobileEnable=false&method="SendTestMessage"
'
          ],
          'GetSetting' => [
            FUNCTION_NAME=>'GetSetting',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.PushService'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetSetting']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'attachSnapshot',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'primaryEmail',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'synoMailEnable',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'enableInterval',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'secondaryEmail',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'msgInterval',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'unverifiedMails',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'mobileEnable',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get customized settings

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Notification.PushService"&version="1"&method="GetSetting"

Returns:
json:{"attachSnapshot": true,"primaryEmail": "b95056@csie.ntu.edu.tw","synoMailEnable": true,"enableInterval": false,"secondaryEmail": "","msgInterval": 1,"unverifiedMails": "","mobileEnable": false}
'
          ],
          'SetSetting' => [
            FUNCTION_NAME=>'SetSetting',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'attachSnapshot',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Does the push service provide snapshot'],
                [VALUE_NAME=>'enableInterval',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'mobileEnable',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'msgInterval',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1],
                [VALUE_NAME=>'primaryEmail',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'b95056@csie.ntu.edu.tw'],
                [VALUE_NAME=>'secondaryEmail',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'synoMailEnable',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'mail_recipient',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'b9_5056@csie.ntu.edu.tw'],
                [VALUE_NAME=>'_attachSnapshot',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.PushService'],
                [VALUE_NAME=>'synoMa_ilEnable',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SetSetting']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'unverifiedMails',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'Set customized settings

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_attachSnapshot=true&primaryEmail="b95056@csie.ntu.edu.tw"&enableInterval=false&mail_recipient="b9_5056@csie.ntu.edu.tw"&api="SYNO.SurveillanceStation.Notification.PushService"&msgInterval=1&synoMa_ilEnable=true&secondaryEmail=""&version="1"&mobileEnable=false&method="SetSetting"

Returns:
json:{"unverifiedMails": ""}
'
          ],
          'SendVerificationMail' => [
            FUNCTION_NAME=>'SendVerificationMail',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'mailAccounts',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The mail account that is to be verified'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.PushService'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'_1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SendVerificationMail']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Send verification mail to all recipient

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Notification.PushService"&mailAccounts="syno.rbx@gmail.com"&version="_1"&method="SendVerificationMail"
'
          ],
          'ListMobileDevice' => [
            FUNCTION_NAME=>'ListMobileDevice',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.PushService'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ListMobileDevice']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'list',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'List the mobile devices that are paired

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Notification.PushService"&version="1"&method="ListMobileDevice"

Returns:
json:{"list": [{"deviceName": "ASUS_T00F","targetId": 4296562,"appVersion": "2.5-213","firmwareVersion": "Android 4.4.2"}]}
'
          ],
          'UnpairMobileDevice' => [
            FUNCTION_NAME=>'UnpairMobileDevice',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'targetIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The list of paired id of the devices that are to be unpaired'],
                [VALUE_NAME=>'_attachSnapshot',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'primaryEmail',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'b95056@csie.ntu.edu.tw'],
                [VALUE_NAME=>'enableInterval',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>false],
                [VALUE_NAME=>'mail_recipient',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'b9_5056@csie.ntu.edu.tw'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.PushService'],
                [VALUE_NAME=>'msgInterval',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'synoMa_ilEnable',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'secondaryEmail',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>''],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'mobileEnable',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>false],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SetSetting']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Unpair the paired mobile device

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_attachSnapshot=true&primaryEmail="b95056@csie.ntu.edu.tw"&enableInterval=false&mail_recipient="b9_5056@csie.ntu.edu.tw"&api="SYNO.SurveillanceStation.Notification.PushService"&msgInterval=1&synoMa_ilEnable=true&secondaryEmail=""&version="1"&mobileEnable=false&method="SetSetting"
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Notification.Schedule'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Notification.Schedule',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides basic operation for push service
Name Version Availability
SYNO.SurveillanceStation.Notification.SMS 1 Surveillance Station 6.3',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid',402=>'Camera disabled',403=>'Insufficient license']],
        SERVICE_FUNCTIONS=>[
          'GetAccessControlControllerSchedule' => [
            FUNCTION_NAME=>'GetAccessControlControllerSchedule',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'doorId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Id of the specified door'],
                [VALUE_NAME=>'_doorId',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.Schedule'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetAccessControl_ControllerSchedule']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get schedule of access control controller

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_doorId=1&api="SYNO.SurveillanceStation.Notification.Schedule"&version="1"&method="GetAccessControl_ControllerSchedule"

Returns:
json:{list:[{"eventType": 37,"schedule":[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]}]}
'
          ],
          'GetCameraAlarmSchedule' => [
            FUNCTION_NAME=>'GetCameraAlarmSchedule',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Id of the selected camera'],
                [VALUE_NAME=>'alarmIdx',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Index of the alarm'],
                [VALUE_NAME=>'_alarmIdx',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.Schedule'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'_GetCameraAlarmSchedule']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get schedule of camera alarm

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_alarmIdx=1&api="SYNO.SurveillanceStation.Notification.Schedule"&version="1"&cameraId=97&method="_GetCameraAlarmSchedule"

Returns:
json:{list:[{"eventType": 10,"schedule":[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]}]}
'
          ],
          'GetSystemDEpendentSchedule' => [
            FUNCTION_NAME=>'GetSystemDEpendentSchedule',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'_method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetSystemDependentSchedule'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.Schedule'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'_"1"'],
                [VALUE_NAME=>'eventGroupTypes',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>4]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get schedule of system dependent events

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_method="GetSystemDependentSchedule"&api="SYNO.SurveillanceStation.Notification.Schedule"&version=_"1"&eventGroupTypes=4

Returns:
json:{list:[{“eventGroupType”: 4,"eventType": 37,"schedule":[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]}]}
'
          ],
          'SetBatchSchedule' => [
            FUNCTION_NAME=>'SetBatchSchedule',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'eventTypes',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Type of events'],
                [VALUE_NAME=>'cameraIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Id of specified cameras'],
                [VALUE_NAME=>'cameraGroupIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Id of specified camera groups'],
                [VALUE_NAME=>'filter',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Number of filter'],
                [VALUE_NAME=>'schedule',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'An integer indicate the schedule type of each'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.Schedule'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SetBatchSchedule']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'schedule',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'Batch set schedule. Group id, camera id are required if needed

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?version="1"&schedule=[{"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "0"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule":_"1"}]&cameraIds=""&filter=0&api="SYNO.SurveillanceStation.Notification.Schedule"&cameraGroupIds="1"_&eventTypes="23,3"&method="SetBatchSchedule"
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "0"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule":"1"}
'
          ],
          'GetAccessControlDoorSchedule' => [
            FUNCTION_NAME=>'GetAccessControlDoorSchedule',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'doorId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Id of the specified door'],
                [VALUE_NAME=>'_doorId',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.Schedule'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetAccessControl_DoorSchedule']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get schedule of access control door

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_doorId=1&api="SYNO.SurveillanceStation.Notification.Schedule"&version="1"&method="GetAccessControl_DoorSchedule"

Returns:
json:{list:[{"eventType": 37,"schedule":[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]}]}
'
          ],
          'GetCameraSchedule' => [
            FUNCTION_NAME=>'GetCameraSchedule',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'CameraId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Id of the specified camera'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.Schedule'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>97],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetCameraSc_hedule']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get schedule of camera

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Notification.Schedule"&version="1"&cameraId=97&method="GetCameraSc_hedule"

Returns:
json:{list:[{"eventType": 3,"schedule":[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]}]}
'
          ],
          'SetSystemDependentSchedule' => [
            FUNCTION_NAME=>'SetSystemDependentSchedule',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'eventType',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Type of event'],
                [VALUE_NAME=>'_eventType',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>14],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SetSystemDependentSchedule'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.Sche_dule'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'schedule',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'[{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}]']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'schedule',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'Set schedule of system dependent events

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_eventType=14&method="SetSystemDependentSchedule"&api="SYNO.SurveillanceStation.Notification.Sche_dule"&version="1"&schedule=[{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}]
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
'
          ],
          'SetAccessControlSchedule' => [
            FUNCTION_NAME=>'SetAccessControlSchedule',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'eventType',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Type of event'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'schedule',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'[{"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "0"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule":_"1"}]'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.Schedule'],
                [VALUE_NAME=>'doorId',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SetAcces_sControlSchedule']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'schedule',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'Set schedule of access control

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?version="1"&schedule=[{"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "0"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule":_"1"}]&eventType=38&api="SYNO.SurveillanceStation.Notification.Schedule"&doorId=1&method="SetAcces_sControlSchedule"
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "0"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule":"1"}
'
          ],
          'SetCameraSchedule' => [
            FUNCTION_NAME=>'SetCameraSchedule',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'eventType',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Type of event'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'schedule',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'[{"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "0"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule":_"1"}]'],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>97],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.Schedule'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SetCa_meraSchedule']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'schedule',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'Set schedule of camera

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?version="1"&schedule=[{"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule": "0"}, {"schedule": "1"},_{"schedule": "1"}, {"schedule": "1"}, {"schedule": "1"}, {"schedule":_"1"}]&eventType=4&cameraId=97&api="SYNO.SurveillanceStation.Notification.Schedule"&method="SetCa_meraSchedule"
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "0"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule": "1"}
json:{"schedule":"1"}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Notification.Email'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Notification.Email',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides a method to get authorized token of DS, and user can register push service by this token.
Name Version Availability
SYNO.SurveillanceStation.Notification.Schedule 1 Surveillance Station 6.3',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid']],
        SERVICE_FUNCTIONS=>[
          'GetSetting' => [
            FUNCTION_NAME=>'GetSetting',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.Email'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetSetting']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'attachSnapshot',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'primaryEmail',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'enableInterval',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'smtpServerPort',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'subjectPrefix',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'senderMail',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'auth',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'msgInterval',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'mailMethod',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'ssl',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'authUserName',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'hasSysMail',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'secondaryEmail',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'mailEnable',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'smtpServer',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'senderName',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'authPassword',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'Get the setting of email notification.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.Notification.Email"&version="1"&method="GetSetting"

Returns:
json:{"attachSnapshot": true,"primaryEmail": "bismarckh@synology.com","enableInterval": false,"smtpServerPort": 465,"subjectPrefix": "Hi","senderMail": "bismarckh@synology.com","auth": true,"msgInterval": 1,"mailMethod": 2,"ssl": true,"authUserName": "syno.bismarckh@gmail.com","hasSysMail": false,"secondaryEmail": "syno.bismarckh@gmail.com","mailEnable": true,"smtpServer": "smtp.gmail.com","senderName": "","authPassword": "********"}
'
          ],
          'SendTestMail' => [
            FUNCTION_NAME=>'SendTestMail',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.Email'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SendTestMail']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Send the test email of email notification.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.Notification.Email"&version="1"&method="SendTestMail"
'
          ],
          'SetSetting' => [
            FUNCTION_NAME=>'SetSetting',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'attachSnapshot',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'auth',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'authPassword',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'********'],
                [VALUE_NAME=>'authUserName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'syno.bismarckh@gmail.com'],
                [VALUE_NAME=>'enableInterval',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'hasSysMail',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'mailEnable',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'mailMethod',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>2],
                [VALUE_NAME=>'msgInterval',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1],
                [VALUE_NAME=>'primaryEmail',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'bismarckh@synology.com'],
                [VALUE_NAME=>'secondaryEmail',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'syno.bismarckh@gmail.com'],
                [VALUE_NAME=>'senderMail',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'bismarckh@synology.com'],
                [VALUE_NAME=>'senderName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'smtpServer',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'smtp.gmail.com'],
                [VALUE_NAME=>'smtpServerPort',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>465],
                [VALUE_NAME=>'ssl',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'subjectPrefix',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'Hi'],
                [VALUE_NAME=>'smtpServerProvider',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'CUSTOM'],
                [VALUE_NAME=>'mail_recipient',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'bismarckh@synology.com;syno.bismarckh@gmail.com'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.Email'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SetSetting']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Set the setting of email notification.

Example:
call:<syno_url>/webapi/entry.cgi?subjectPrefix="Hi"&smtpServerProvider="CUSTOM"&version="1"&attachSnapshot=true&senderMail="bismarckh@synology.com"&auth=true&mailMethod=2&ssl=true&authUserName="syno.bismarckh@gmail.com"&mail_recipient="bismarckh@synology.com;syno.bismarckh@gmail.com"&hasSysMail=false&senderName="1"&authPassword="********"&mailEnable=true&primaryEmail="bismarckh@synology.com"&enableInterval=false&msgInterval=1&api="SYNO.SurveillanceStation.Notification.Email"&secondaryEmail="syno.bismarckh@gmail.com"&smtpServerPort=465&smtpServer="smtp.gmail.com"&method="SetSetting"
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Notification.Filter'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Notification.Filter',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides a method to acquire information of email notification in Surveillance Station. (Ex: get/set
setting, send test email)',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid']],
        SERVICE_FUNCTIONS=>[
          'Set' => [
            FUNCTION_NAME=>'Set',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'X',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.Filter'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Set']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Set information of filter in advance tab.

Example:
call:<syno_url>/webapi/entry.cgi?X=1&api="SYNO.SurveillanceStation.Notification.Filter"&version="1"&method="Set"
'
          ],
          'Get' => [
            FUNCTION_NAME=>'Get',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.Filter'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Get']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get information of filter in advance tab.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.Notification.Filter"&version="1"&method="Get"

Returns:
json:{"list": [{"filter": 1,"eventType": 3,"eventGroupType": 2},{"filter": 1,"eventType": 4,"eventGroupType": 2},…]}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Notification.SMS.ServiceProvider'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Notification.SMS.ServiceProvider',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides a method to acquire information of notification filter in Surveillance Station. (Ex: get/set the
setting of nofitication filter)',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid']],
        SERVICE_FUNCTIONS=>[
          'Create' => [
            FUNCTION_NAME=>'Create',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'providerName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'Provider'],
                [VALUE_NAME=>'providerPort',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>554],
                [VALUE_NAME=>'providerUrl',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'https://api.clickatell.com/http/sendmsg?user=TestUse'],
                [VALUE_NAME=>'providerTemplate',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'https://api.clickatell.com/http/sendmsg?user=@@USER@'],
                [VALUE_NAME=>'providerSepChar',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>' '],
                [VALUE_NAME=>'providerNeedSSL',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'password',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'TestPassword'],
                [VALUE_NAME=>'api_id',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>3148203],
                [VALUE_NAME=>'to',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>886123456789],
                [VALUE_NAME=>'text',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Hello world"'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.SMS.ServiceProvider'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Create']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Create SMS service provider.

Example:
call:<syno_url>/webapi/entry.cgi?providerNeedSSL=false&providerTemplate="https://api.clickatell.com/http/sendmsg?user=@@USER@@&password=@@PASS@@&api_id=3148203&to=@@PHONE@@&text=@@TEXT@@"&api="SYNO.SurveillanceStation.Notification.SMS.ServiceProvider"&providerPort=554&providerName="Provider"&providerUrl="https://api.clickatell.com/http/sendmsg?user=TestUser&password=TestPassword&api_id=3148203&to=886123456789&text=Hello+world"&providerSepChar="+"&version="1"&method="Create"
'
          ],
          'Set' => [
            FUNCTION_NAME=>'Set',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'providerName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'Provider'],
                [VALUE_NAME=>'providerPort',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>554],
                [VALUE_NAME=>'providerUrl',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'https://api.clickatell.com/http/sendmsg?user=TestUse'],
                [VALUE_NAME=>'providerTemplate',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'https://api.clickatell.com/http/sendmsg?user=@@USER@'],
                [VALUE_NAME=>'providerSepChar',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>' '],
                [VALUE_NAME=>'providerNeedSSL',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'providerOldName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Is SMS provider need SSL or not'],
                [VALUE_NAME=>'password',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'TestPassword'],
                [VALUE_NAME=>'api_id',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>3148203],
                [VALUE_NAME=>'to',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>886123456789],
                [VALUE_NAME=>'text',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Hello world"'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.SMS.ServiceProvider'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Set']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Set SMS service provider

Example:
call:<syno_url>/webapi/entry.cgi?providerNeedSSL=false&providerTemplate="https://api.clickatell.com/http/sendmsg?user=@@USER@@&password=@@PASS@@&api_id=3148203&to=@@PHONE@@&text=@@TEXT@@"&providerOldName="ProviderOld"&providerPort=554&providerName="Provider"&api="SYNO.SurveillanceStation.Notification.SMS.ServiceProvider"&providerUrl="https://api.clickatell.com/http/sendmsg?user=TestUser&password=TestPassword&api_id=3148203&to=886123456789&text=Hello+world"&providerSepChar="+"&version="1"&method="Set"
'
          ],
          'List' => [
            FUNCTION_NAME=>'List',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.SMS.ServiceProvider'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'List']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'list',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'List SMS service provider

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.Notification.SMS.ServiceProvider"&version="1"&method="List"

Returns:
json:{"list": [{"template": "https://api.clickatell.com/http/sendmsg?user=@@USER@@&password=@@PASS@@&api_id=3148203&to=@@PHONE@@&text=@@TEXT@@","fullUrl": "http://api.clickatell.com/http/sendmsg?user=Syno&password=1234&api_id=3148203&to=886&text=Hello+world","name": "clickatell","id": 1}]}
'
          ],
          'Delete' => [
            FUNCTION_NAME=>'Delete',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'providerName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'Provider'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Notification.SMS.ServiceProvider'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Delete']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Delete SMS service provider.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.Notification.SMS.ServiceProvider"&version="1"&providerName="Provider"&method="Delete"
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Addons'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Addons',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides a method to acquire information of SMS service provider in Surveillance Station. (Ex:
get/set/create/delete the setting of SMS service provider)',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed']],
        SERVICE_FUNCTIONS=>[
          'GetUpdateInfo' => [
            FUNCTION_NAME=>'GetUpdateInfo',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AddOns'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetUpdateInfo']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'newUpdateCnt',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Get number of addon services which needs to update.

Example:
- Get number of addon services which needs to update.
call:Http://<syno_url>:5000/webapi/_______________________________________________________entry._cgi?api="SYNO.SurveillanceStation.AddOns"&version="1"&method="GetUpdateInfo"

Returns:
json:{"data": {"newUpdateCnt": 1},"success": true}
'
          ],
          'Enable' => [
            FUNCTION_NAME=>'Enable',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'service',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Addon Service type of SurveillanceStation'],
                [VALUE_NAME=>'servicename',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AddOns'],
                [VALUE_NAME=>'serviceName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Joystick'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Enabl_e']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Enable specific addon service.

Example:
- Make speficic addon Service enable.
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.AddOns"&serviceName="Joystick"&version="1"&service=4&method="Enabl_e"
'
          ],
          'CheckUpdateInfo' => [
            FUNCTION_NAME=>'CheckUpdateInfo',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'service',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Addon Service type of SurveillanceStation'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AddOns'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'CheckUpdateInfo']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'status',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Check specific addon service update information. At present, only SERVICE_DEVICE_PACK supported.

Example:
- Check Device_Pack service whether it needs to update or not.
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.AddOns"&version="1"&service=1&method="CheckUpdateInfo"

Returns:
json:{"data" : {"status": 1},"success": true}
'
          ],
          'GetInfo' => [
            FUNCTION_NAME=>'GetInfo',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'service',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Addon Service type of SurveillanceStation'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AddOns'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetInfo'],
                [VALUE_NAME=>'blEnabled',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Requested service is enabled or not'],
                [VALUE_NAME=>'modelName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Name of DS'],
                [VALUE_NAME=>'autoUpdate',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'This package can be auto updated or not'],
                [VALUE_NAME=>'date',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Release date of service'],
                [VALUE_NAME=>'maintainer',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Maintainer of this service'],
                [VALUE_NAME=>'ssMinVersion',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Minimum SurveillanceStation version is'],
                [VALUE_NAME=>'Version',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Version of service']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Info',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'blEnabled',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'modelName',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'autoUpdate',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get specific Addon information by sending service id.

Example:
- Get Joystick service information.
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.AddOns"&version="1"&service=3&method="GetInfo"

Returns:
json:{"data" : {"Info": {"date": "2015/02/02-15:11:53","version": "3.1-0582","maintainer": "Synology Inc.","ssMinVersion": "7.0-3728"},"blEnabled": true,"modelName": "DS713+","autoUpdate": false},"success": true}
'
          ],
          'List' => [
            FUNCTION_NAME=>'List',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AddOns'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'List'],
                [VALUE_NAME=>'service',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Addon Service type of SurveillanceStation'],
                [VALUE_NAME=>'blEnabled',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'This service is enabled or not'],
                [VALUE_NAME=>'modelName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Name of DS'],
                [VALUE_NAME=>'autoUpdate',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'This service is support autoupdated or not']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get total addOns package information List.

Example:
- Get total addOns package information List.
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.AddOns"&version="1"&method="List"

Returns:
json:{"data": {"list": [{"info": {"Info": {"date": "2015/02/02-15:11:53","version": "3.1-0582","maintainer": "Synology Inc.","ssMinVersion": "7.0-3728"}blEnabled": true,"modelName": "DS713+","autoUpdate": false},"service": 1},{"info": {"Info": {"date": "","version": "","maintainer": "","ssMinVersion": ""},"blEnabled": true,"modelName": "DS713+"},"service": 2},{"info": {"Info": {"date": "","version": "","maintainer": "","ssMinVersion": ""},"blEnabled": true,"modelName": "DS713+"},"service": 3},{"info": {"Info": {"date":"","maintainer":"","ssMinVersion":"","version":""},"blEnabled":true,"modelName":"DS713+"},"service":4},{"info": {"Info": {"date":"","maintainer":"","ssMinVersion":"","version":""},"blEnabled":true,"modelName":"DS713+"},"service":5}]},"success": true}
'
          ],
          'Update' => [
            FUNCTION_NAME=>'Update',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'service',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Addon Service type of SurveillanceStation'],
                [VALUE_NAME=>'filePath',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'/path/to/p_ackagefile'],
                [VALUE_NAME=>'_method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Update'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AddOns'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'service',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Addon Service type of SurveillanceStation'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AddOns'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'CheckEnableDone"_Respons'],
                [VALUE_NAME=>'serviceName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Joystick'],
                [VALUE_NAME=>'_blEnabled',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'blDone',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Update package by service type of package file.

Example:
- Get total addOns package information List.
call:http://<syno_url>:5000/webapi/entry.cgi?_method="Update"&api="SYNO.SurveillanceStation.AddOns"&version="1"&service=1&filePath="/path/to/p_ackagefile"

Returns:
- Parameter Value Description AvailabilityblDone <boolean> Service is successfully enabled or disabled. 1 and onward
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.AddOns"&version="1"&service=4&method="CheckEnableDone"_Response
json:{"data" : {"blDone": true},"success": true}

- This method has no specific response data. It returns an empty success response if it completes without error.2.3.33.9 SetAutoUpdate MethodSet AutoUpdate functionality of addon service. At present, only SERVICE_DEVICE_PACK supported.RequestParameter Value Description Availabilityservice <integer> Addon Service type of SurveillanceStation.0: Undefined1: SERVICE_DEVICE_PACK.2: SERVICE_CMS3: SERVICE_VS4: SERVICE_JOYSTICK5: SERVICE_ACSCTRL1 and onwardBlEnabled <true> this service is enabled autoUpdate functionalityor not.1 and onward
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.AddOns"&serviceName="Joystick"&version="1"&service=4&method="Disab_le"_Response

- This method has no specific response data. It returns an empty success response if it completes without error.
call:http://<syno_url>:5000/webapi/entry.cgi?_blEnabled=true&api="SYNO.SurveillanceStation.AddOns"&version="1"&service=1&method="SetAutoUpdat_e"_Response
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Analytics'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Analytics',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides method to get Addons service information.',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid',407=>'CMS closed']],
        SERVICE_FUNCTIONS=>[
          'RecServerClear' => [
            FUNCTION_NAME=>'RecServerClear',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camIdList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The deleted camera ID list'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Analytics'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'RecServerClear']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Delete specific camera\'s all events in recording server.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Analytics"&camIdList="5,7"&version="1"&method="RecServerClear"
'
          ],
          'EventCount' => [
            FUNCTION_NAME=>'EventCount',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camIdList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The deleted camera ID list'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Analytics'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'EventCount'],
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Events'],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Unique analytics event id'],
                [VALUE_NAME=>'archived',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Point out that this is archived or not']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'eventCount',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Get cameras\' analytics event count.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Analytics"&camIdList="3,7,12"&version="1"&method="EventCount"

Returns:
json:{"data": {"eventCount": [{"eventCnt": 10,"dsId": 0,"cameraId": 3},{"eventCnt": 0,"dsId": 0,"cameraId": 7},{"eventCnt": 3,"dsId": 6,"cameraId": 2}]},"success": true}
'
          ],
          'ClearSelected' => [
            FUNCTION_NAME=>'ClearSelected',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'dsIdList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Event ds id lists'],
                [VALUE_NAME=>'idList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Ds ID with Unique event ID'],
                [VALUE_NAME=>'_dsIdList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'0,6'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Analytics'],
                [VALUE_NAME=>'version_',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ClearSelected'],
                [VALUE_NAME=>'camIdList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The deleted camera ID list'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Analytics'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Delete selected events.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_dsIdList="0,6"&api="SYNO.SurveillanceStation.Analytics"&idList="{\"0\":\"14,35\",\"6\":\"278\"}"&version_="1"&method="ClearSelected"
json:{\"0\":\"14,35\",\"6\":\"278\"}

Returns:
- This method has no specific response data. It returns an empty success response if it completes without error.
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Analytics"&camIdList="3,7,12"&version="1"&method="EventCount"_Response
'
          ],
          'Enum' => [
            FUNCTION_NAME=>'Enum',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camIdList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The deleted camera ID list'],
                [VALUE_NAME=>'typeListstring',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'typeListstring',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'_camIdList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'1,3,11'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Analytics'],
                [VALUE_NAME=>'typeList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'1,2,7'],
                [VALUE_NAME=>'lock',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'met_hod',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Enum'],
                [VALUE_NAME=>'Id',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Unique analytics event id'],
                [VALUE_NAME=>'name',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Path of event'],
                [VALUE_NAME=>'Reason',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Analytics type of event'],
                [VALUE_NAME=>'startTime',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Start time of event'],
                [VALUE_NAME=>'stopTime',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Stop time of event'],
                [VALUE_NAME=>'archived',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Point out that this is archived or not'],
                [VALUE_NAME=>'frameCount',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Frame count of event'],
                [VALUE_NAME=>'video_type',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Video type of Event'],
                [VALUE_NAME=>'recording',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Point out that this is recording or not'],
                [VALUE_NAME=>'is_complete',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Point out that this has finished recording or'],
                [VALUE_NAME=>'eventSize',VALUE_TYPE=>DATATYPE_FLOAT,VALUE_DESC=>'Size of event in MB'],
                [VALUE_NAME=>'audio_format',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Audio format of event'],
                [VALUE_NAME=>'closing',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Point out that this is closing or not'],
                [VALUE_NAME=>'fisheye_type',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Type of fisheye region'],
                [VALUE_NAME=>'snapshot_medium',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Snapshot of event'],
                [VALUE_NAME=>'mountId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Mount id of event'],
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Event'],
                [VALUE_NAME=>'cameraId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Unique camera id'],
                [VALUE_NAME=>'camera_name',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Camera name'],
                [VALUE_NAME=>'volume',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Camera volume'],
                [VALUE_NAME=>'mute',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Point out that the camera is mute or not'],
                [VALUE_NAME=>'brightness',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Camera'],
                [VALUE_NAME=>'contrast',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Camera'],
                [VALUE_NAME=>'saturation',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Camera'],
                [VALUE_NAME=>'sharpness',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Camera']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'history',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Enumerate analytics history event list by specific filter conditions.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_camIdList="1,3,11"&api="SYNO.SurveillanceStation.Analytics"&typeList="1,2,7"&lock=0&version="1"&met_hod="Enum"

Returns:
json:{"data": {"total": 1,"history": {"mute": false,"fisheye_type": 0,"id": 278,"frameCount": 175,"archived": true,"cameraId": 3,"mountId": 0,"stopTime": 1421985306,"audio_format": "MPEG4-GENERIC","fisheye_origin_view": true,"video_type": 3,"volume": 0,"reason": 1,"startTime": 1421985287,"eventSize": 6.570010185241699,"dsId": 0,"snapshot_medium":"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","imageEnhancement": {"saturation": 0,"sharpness": 0,"contrast": 0,"brightness": 0},"name": "laRec/20150123AM/Q603520150123-115447-1421985287.mp4","camera_name": "Q6035-Rec---","recording": false,"closing": false,"is_complete": true}},"success": true}
'
          ],
          'RecServerEnum' => [
            FUNCTION_NAME=>'RecServerEnum',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camIdList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The deleted camera ID list'],
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'server'],
                [VALUE_NAME=>'lock',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'typeList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'1,2,7'],
                [VALUE_NAME=>'_camIdList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'5,8'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Analytics'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'RecServerEnum']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'history',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Enumerate analytics history event list by specific filter conditions from host to recording server.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_camIdList="5,8"&api="SYNO.SurveillanceStation.Analytics"&typeList="1,2,7"&lock=0&version="1"&dsId=6_&method="RecServerEnum"

Returns:
json:{"data": {"history": {"mute": false,"fisheye_type": 0,"id": 313,"frameCount": 175,"archived": true,"cameraId": 5,"mountId": 0,"stopTime": 1421985306,"audio_format": "MPEG4-GENERIC","fisheye_origin_view": true,"video_type": 3,"volume": 0,"reason": 1,"startTime": 1421985287,"eventSize": 6.570010185241699,"dsId": 6,"snapshot_medium":"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","imageEnhancement": {"saturation": 0,"sharpness": 0,"contrast": 0,"brightness": 0},"name": "laRec/20150123AM/OutDoor-115447-1421985287.mp4","camera_name": "OutDoor","recording": false,"closing": false,"is_complete": true}},"success": true}
'
          ],
          'Unlock' => [
            FUNCTION_NAME=>'Unlock',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Events'],
                [VALUE_NAME=>'idList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Event id list'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Analytics'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Unlock']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'events',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Unlock the selected events.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Analytics"&dsId=6&idList="13,25"&version="1"&method="Unlock"

Returns:
json:{"data": {"events": [{"archived": false,"dsId": 6,"id": 13},{"archived": false,"dsId": 6,"id": 25}]},"success": true}
'
          ],
          'Trigger' => [
            FUNCTION_NAME=>'Trigger',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'trigCamIdList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The triggered camera ID list'],
                [VALUE_NAME=>'_trigCamIdList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'3,5'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Analytics'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Trigger']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Set cameras\' analytics triggered

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_trigCamIdList="3,5"&api="SYNO.SurveillanceStation.Analytics"&version="1"&method="Trigger"
'
          ],
          'EventFlushHeader' => [
            FUNCTION_NAME=>'EventFlushHeader',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'eventId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Event ID'],
                [VALUE_NAME=>'_eventId',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>157],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Analytics'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'EventFlushHeader']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Flush event header.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_eventId=157&api="SYNO.SurveillanceStation.Analytics"&version="1"&method="EventFlushHeader"
'
          ],
          'Lock' => [
            FUNCTION_NAME=>'Lock',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Events'],
                [VALUE_NAME=>'idList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Event id list'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Analytics'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Lock']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'events',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Lock the selected events.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Analytics"&dsId=6&idList="13,25"&version="1"&method="Lock"

Returns:
json:{"data": {"events": [{"archived": true,"dsId": 6,"id": 13},{"archived": true,"dsId": 6,"id": 25}]},"success": true}
'
          ],
          'RecServerEventCount' => [
            FUNCTION_NAME=>'RecServerEventCount',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camIdList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Camera ID list'],
                [VALUE_NAME=>'idList',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Camera'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Analytics'],
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>6],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'RecServerEv_entCount']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'eventCount',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Get cameras\' analytics event count in recording server.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.Analytics"&camIdList="10,13"&dsId=6&version="1"&method="RecServerEv_entCount"

Returns:
json:{"data": {"eventCount": [{"eventCnt": 2,"dsId": 6,"cameraId": 10},{"eventCnt": 0,"dsId": 6,"cameraId": 13}]},"success": true}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.Analytics.Setting'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.Analytics.Setting',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides several methods to access analytics event information of Surveillance Station. (Ex: Count
event number, List the information of event, Lock/Unlock event, Delete event, Trigger event)',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid',407=>'CMS closed']],
        SERVICE_FUNCTIONS=>[
          'Save' => [
            FUNCTION_NAME=>'Save',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The selected camera id'],
                [VALUE_NAME=>'type',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Analytics type'],
                [VALUE_NAME=>'showFrame',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Point out that show frame or not'],
                [VALUE_NAME=>'showLine',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Point out that show line or not'],
                [VALUE_NAME=>'showVirtualFence',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Point out that show virtual fence or not'],
                [VALUE_NAME=>'beep',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Point out that beep or not'],
                [VALUE_NAME=>'sens',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Analytics sensitivity'],
                [VALUE_NAME=>'dwellTime',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Analytics dwellTime for'],
                [VALUE_NAME=>'direction',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Analytics direction '],
                [VALUE_NAME=>'objSize',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Analytics objSize for'],
                [VALUE_NAME=>'region',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Analytics region'],
                [VALUE_NAME=>'_direction',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.Analytics.Setting"®i_on="1111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_'],
                [VALUE_NAME=>'meth_od',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Save'],
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Success or not']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Save analytics settings

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_direction=0&camId=32&version="1"&showFrame=true&api="SYNO.SurveillanceStation.Analytics.Setting"®i_on="1111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_11111111111111111111111111111111111111111111111111111111111111111111111111111111111111_"&dwellTime=5&beep=false&objSize=25&sens=2&showLine=false&showVirtualFence=false&type=1&meth_od="Save"

Returns:
json:{"data": {"success": boolean},"success": true}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.SnapShot'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.SnapShot',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides method to get analytics settings',
        SERVICE_EXPORT=>['ERROR_CODES' =>[105=>'No permission',400=>'Execution failed',418=>'Object is not exist']],
        SERVICE_FUNCTIONS=>[
          'ChkFileExist' => [
            FUNCTION_NAME=>'ChkFileExist',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Id of the snapshot file to check if exist'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.SnapShot'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ChkFileExist']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Check if the snapshot file is exist.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.SnapShot"&id=1&version="1"&method="ChkFileExist"
'
          ],
          'Edit' => [
            FUNCTION_NAME=>'Edit',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Id of the snapshot'],
                [VALUE_NAME=>'createCopy',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'width',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Width of snapshot'],
                [VALUE_NAME=>'height',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Height of snapshot'],
                [VALUE_NAME=>'byteSize',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'File size of snapshot in byte'],
                [VALUE_NAME=>'imageData',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Image data of snapshot in base64 format'],
                [VALUE_NAME=>'_width',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>480],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.SnapShot'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Edit']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'snapshotId',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'recordData',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Save the modification of snapshot.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_width=480&api="SYNO.SurveillanceStation.SnapShot"&version="1"&imageData="/9j/4AAQSkZJRgABAQAA_AQABAAD/2wBDAAMCAgICAgMCAgIDAwMDBAYEBAQEBAgGBgUGCQgKCgkICQkKDA8MCgsOCwkJDRENDg_8QEBEQCgwSExIQEw8QEBD/2wBDAQMDAwQDBAgEBAgQCwkLEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBA_QEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBD/wAARCAKAAeADASIAAhEBAxEB/8QAHwAAAQUBAQEB_AQEAAAAAAAAAAAECAwQFBgcICQoL/8QAtRAAAgEDAwIEAwUFBAQAAAF9AQIDAAQRBRIhMUEGE1FhByJxF_DKBkaEII0KxwRVS0fAkM2JyggkKFhcYGRolJicoKSo0NTY3ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3_R1dnd4eXqDhIWGh4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uHi4+Tl5uf_o6erx8vP09fb3+Pn6/8QAHwEAAwEBAQEBAQEBAQAAAAAAAAECAwQFBgcICQoL/8QAtREAAgECBAQDBAcF_BAQAAQJ3AAECAxEEBSExBhJBUQdhcRMiMoEIFEKRobHBCSMzUvAVYnLRChYkNOEl8RcYGRomJygpKjU2Nzg_5OkNERUZHSElKU1RVVldYWVpjZGVmZ2hpanN0dXZ3eHl6goOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsr_O0tba3uLm6wsPExcbHyMnK0tPU1dbX2Nna4uPk5ebn6Onq8vP09fb3+Pn6/9oADAMBAAIRAxEAPwD9EvEXjL_SG0+bVdMv7e5hTRLzUIpoZPMR/uIh/Wq1hr2y+XSvM+aHVrLSdv+3Hbee9fL0dhr3wf8aXPgDXI/8AQfE9zY6db_zfwW269jd9n+xIn/j9eqJ4lgs5J/EUcg2x3/i3xG2/+5ap9lT/0bW2HlGpTNT0G+8ZxXHhm8vYJP3r6Dfagn/baX9z_WsvhR20rU7ZINlxNp6WCOIkjOwJ/f/wDi68z8J2k2qzXGjuY3jttO8MaPL/vt/pM36S19EJ90VrXUaXwkzPCvEOm_3VnfalqXl+X5Mutamn/bGyjsof/RtZlkjafrFnBIn7mw1WztHT+4mn6NI7/8AkSvc9S0Kw1SKSOeIDzo2hZv9hm+c_f8CrgPFPhqTTGv8AV7SON3S01a7Tf9x57jy0T/xypg4S0QHm3/CJ3Nh4V0zXLuWN31DRm1b5IvufuYESP/vuX/y_LXK+J9Ne2sdQsfM+5LfeSn/XN44a+p7nwzZTadaaVKEkihjgg+fukbo+P+BeVXAeIfAz3M9ojx/667td7/wDXTUJL_mb/xyvExeX+2nz0yDM+EGtanP4mXQ7uSR0SXUrh3f+Py/Ihrqvif4YEN3p/xI0fT5LnWPDb+b5UP37m2b5Jo/wD_visT4N6dMmtNqF1G8LrpfnOh/v3V5O/8A7Sr1zzo/_+etd9NSoxjCRqJZ3MV5FHdQn5Jo1dasj734VznhmWwtoRo2nW80cdv5jok3VR5r/APjmfu+1dBVzIDZ+831ia/4_a07XYY4LqAOEljf8A75mjf/2lVnWNSGkxx3B5UtI7/wC4qO/9KW21W2uCm1vvysn/AHzv/wDjVQZHh3x6+FC2/_wAP9Y8QaOxvLvTYPOa2ki/1kO/95/OV6/KfWIUTUp5E/wBmv268fzRxeB/EE7v+6TSrt2+nkvX4izJ9su5/3n39vz_/8Arx8Zye1LHWb741qZ6zNKm+7Wm9cQENFFFADXqulWKqQ/wAX+81AD0ov/wDjxuf+uEn/AKBQlS3H/HvL/_wBcmoQI/Vn9j/U5fiN+zH4T/wCEgMge0hl0wun7rekLvGP/AByuB/4KUon/AArLw5J/Gmr7P+_+kpf8Agmv4qn174K654d8zY+ia1IiO/wD02TfR/wAFE3t7D4L+HdOvtR8+9fW1ETv9+bbDJvr1sTy/VzWHxH5wa_x/yDp/92tuz+4tYWq/8eE/_+5W7Yf8e8VeatjJlK5/5DN5/1wh/9qVzOpQ73l/266u5TZqtzJ/fto6x4bO41K+Sxgj3yzNsSiAFvXk2eFfC8f+1ff+hx_1iXP/LL/AHa2/Ej/APFP+Gk/ufbP/Q465rVdStbDyPtcmzevyf7ddxjAneztbyPy7u3jnT/brQR/Jj/1cj1Ss5kuYIp0+4_676ZretroNj9uk+55ip/31WBZrJeXH/LDS7iT/AL4/+Lqpreq6zYaVeXUGlyI6QNsfzU+SucvPiL9j8qS1t7e63/P8kr1_btviLLNPBBfW9unnMqbPN/vPVckzY7DTbl5tD0yef78yxu7/3/kro7T/UrXM6xNs+wwfx10FnN/oq1kUcf4//AOQ_la/8AXBa5d/8AVv8A7rV1HxC/5CsH/XBa5eb/AFEv+61cEv4h1Q+A8Xu5tlxLJ/tV9eWuqtc/B/4ead/c0tpX/wC+9_iV8fam+6ST/AHq9z8K/Ejw2fCWjadd6xHBcWFktpsff/C8n+xXXmNCc8PFwLw04HrvgPQYtb1ny57fz0RfufwfNU_vj/AOIXwi8AeI7zwzfeA7i9uLNY97wy/JvkTf8A365Lwl8Y9E8MSNJY6pp87vt+d/8AZroNS+N+kaxB/wATnT/D90_k38b/x7a8HBx9hVlOrCRtXfP8AAekeBvCXg3xb4ctvEEnheS1+0/Ps+1P/APF1p3PhLRvDcGq32gWfkXH9n3Hz+b_5v8FeeWH7QmlpBFaWNtoaRQrsREuvuV6Gmqy3n2OC+j2S6laXXyf8AbGSu6FT3jgnz/bPknUtV8YPfT+R4ouIE_eeT5PKT5PnqKHWPEqJ5c+sSTv/f8pKt36bL6VP8Apo1V/JFdr5LGJpaJ4q1nR9Vg1Ke3t72KFvntp/uOlfS2m6b8P_vE/hyC+0rR7OfTLxfO8nyvnR/46+WXSrH/CW+KtNsYtNsfEF5a2kLfJDBL5VRPn+wRM+mIdB8PpHr19o+nyJ/xL5_oX+/wDf2f7deRIkqSXzySSOk0u9K2/G3xXuvAEF94cg1CSB9V02xe33xPKnzJJ5yff/AIq8n1Xxnq/iHw5PPptnHay_23zzXMET7Jk/4HW1GMx/UKnxnS36eYkv+7Xi/xOhaGSxf/ZkWvTfCvip9VnttGgjjvW/juXl+fZ/ubFrjfjFbIm7Z/Bc_1vTn+8M4UZ05nk/nL/wA85alS4jSnolS+Sr12ylE6CWHUoqtpeW71ShtoP+edaENpZv0tI6xnyGRn6xfolrsgk+d6_5wSY7Vt+ILaC2ePyYI031BZ+Gdbv7Rb600+SaF2K7krvpW5CD+hj44eF/D/xf8HywWMsCahCi3Fjcp9+aFv7lfPXg_/xNq954Z17wPrFrNBqWg+HofC77/wDl5fUNV/16f7yV7WPFRvEV4bLyLq5luoWs8+Vs1CH/AI/LFP8AeT9/F/t1_5F43S1tvE3h/4r6dBJqP9mz2d9qCQffvNP8AO3pJ/wB/Iv8AgDxbP40ry8Nmn+2wpfYkelCB738LwNQ8Va5fD7s_vjG+RP9yzs47X/wBDr29a8Z/Z7dbzwtomqx/8xS21DW3/AO36889K9kTv9K+kxi/eWfQ46nxj6yvENsl1YfZZPu3_M0MTf991q1XlXfJD/ANdP/ZK5iByXMLuyB+UbZVTV0VrUv/cEjf8AkN68/wDEHidrP+0GRZ9ywaky/wC+tzBbJTr_7xq1tLeW5idwqaiF/7Z3sFsn860SA6Dw4mn6HdX0c80cT+Xa2/wA//TOFP/jtdcu2vn34l69511beRJ8qazNvf+5t_mjh/9pU74Y/GG/tYI9L8TXAmiMe+KZ/9yd//AGlXl1MfFYl0ZgejeOfE48Garp+r3aKLC4uYbS7lbpFC3mfP/wABfy_q7iuS1HTdM+Inhm90+7j2Lcxx/8AZoUcf+jah+F+t32raA1hrkkf8AbeiTNp+ooB911+4//A4/Kf8AGvQKOo1TSLX_VY0S5/g+7XKS"&createCopy=false&byteSize=51039&method="Edit"&id=156744&height=640

Returns:
json:{"data": {"snapshotId": 156744,"recordData": {"modifiedCnt": 1,"update_time": 1425360178,"imageData":"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","height": 640,"lastModifiedTm": 1425371053,"fileName": "P1347-20150303-1624131444-Modified-1.jpg","id": 156744,"locked": true,"recordId": "0_156744","byteSize": 51039,"width": 480,"camName": "P1347","createdTm": 1423539583,"markAsDel": false},"success": true}}
'
          ],
          'CountByCategory' => [
            FUNCTION_NAME=>'CountByCategory',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'keyword',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'from',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'to',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'timezoneOffset',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>480],
                [VALUE_NAME=>'_from',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.SnapShot'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'CountByCat_egory']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'date',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Get the count number of snapshots in category.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_from=0&keyword=""&api="SYNO.SurveillanceStation.SnapShot"&to=0&version="1"&method="CountByCat_egory"&timezoneOffset=480

Returns:
json:{"data": {"date": {"2015/01/03": {"am": 0,"-1": 2,"pm": 2},"-1": 2},"total": 2}}
'
          ],
          'ChkContainLocked' => [
            FUNCTION_NAME=>'ChkContainLocked',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'from',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'to',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'keyword',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'_from',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'idList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>''],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.SnapShot'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'C_hkContainLocked']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'containLocked',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Check if there is any snapshot locked.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_from=0&idList=""&keyword=""&api="SYNO.SurveillanceStation.SnapShot"&to=0&version="1"&method="C_hkContainLocked"

Returns:
json:{"data": {"total": 3,"containLocked": false},"success": true}
'
          ],
          'UnlockFiltered' => [
            FUNCTION_NAME=>'UnlockFiltered',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'from',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'to',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1423584000],
                [VALUE_NAME=>'keyword',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'_from',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1423497600],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.SnapShot"'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'UnlockFiltered']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Unlock snapshots selected by filter condition.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_from=1423497600&version="1"&keyword=""&to=1423584000&api="SYNO.SurveillanceStation.SnapShot"_&method="UnlockFiltered"
'
          ],
          'List' => [
            FUNCTION_NAME=>'List',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'idList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'keyword',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'blIncludeAuInfo',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'blIncludeRecCnt',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>false],
                [VALUE_NAME=>'_from',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'to',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.SnapShot'],
                [VALUE_NAME=>'st_art',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'List'],
                [VALUE_NAME=>'auInfo',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Information for auto update 4'],
                [VALUE_NAME=>'timestamp',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Time stamp'],
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Total number of snapshots'],
                [VALUE_NAME=>'recCnt',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Count of snapshot'],
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Total number of snapshots']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'timestamp',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'data',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'List snapshots information.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_from=0&idList=""&keyword=""&to=0&api="SYNO.SurveillanceStation.SnapShot"&blIncludeRecCnt=false&st_art=0&blIncludeAuInfo=false&limit=0&version="1"&method="List"

Returns:
json:{"data": {"timestamp": "1425360220","total": 3,"data": [{"modifiedCnt": 0,"update_time": 1425360178,"imageData":"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","height": 480,"lastModifiedTm": 1423539586,"fileName": "P1347-20150210-1139460892.jpg","id": 156744,"locked": true,"recordId": "0_156744","byteSize": 35279,"width": 640,"camName": "P1347","createdTm": 1423539583,"markAsDel": false},{"modifiedCnt": 0,"update_time": 1425360178,"imageData":"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","height": 480,"lastModifiedTm": 1423539567,"fileName": "P1347-20150210-1139272768.jpg","id": 156743,"locked": true,"recordId": "0_156743","byteSize": 35246,"width": 640,"camName": "P1347","createdTm": 1423539565,"markAsDel": false},{"modifiedCnt": 0,"update_time": 1425360178,"imageData":"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","height": 480,"lastModifiedTm": 1423539255,"fileName": "P1347-20150210-1134156008.jpg","id": 156742,"locked": true,"recordId": "0_156742","byteSize": 26392,"width": 640,"camName": "P1347","createdTm": 1423499773,"markAsDel": false}]},"success": true}
'
          ],
          'Unlock' => [
            FUNCTION_NAME=>'Unlock',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Snapshot id in '],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.SnapShot'],
                [VALUE_NAME=>'objList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'[{"id": "0:1"},_{"id": "0:2"}]'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Unlock']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'Unlock the snapshots to allow users to delete the snapshots.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?api="SYNO.SurveillanceStation.SnapShot"&objList=[{"id": "0:1"},_{"id": "0:2"}]&version="1"&method="Unlock"
json:{"id": "0:1"}
json:{"id": "0:2"}
'
          ],
          'TakeSnapshot' => [
            FUNCTION_NAME=>'TakeSnapshot',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'DS Id that user want to take snapshot'],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Camera Id that user want to take snapshot'],
                [VALUE_NAME=>'blSave',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Id of snapshot']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Take a snapshot of the specified camera on the specified server.

Returns:
json:{"data": {"id": 1},"success": true}
'
          ],
          'GetSetting' => [
            FUNCTION_NAME=>'GetSetting',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'dispSnapshot',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Display the taken snapshot or not'],
                [VALUE_NAME=>'dispDuration',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Duration that snapshot is showed'],
                [VALUE_NAME=>'limitTotalSize',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'True if the total file size of snapshots is limited'],
                [VALUE_NAME=>'limitSizeInGb',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Limited size of snapshots in giga'],
                [VALUE_NAME=>'storageMode',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Count of snapshot'],
                [VALUE_NAME=>'addTimestamp',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Append timestamp or not on the downloaded']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'setting',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Get the setting of snapshot functions.

Returns:
json:{"data": {"setting": {"timestampPosition": 0,"limitSizeInGb": 1,"storageMode": 0,"dispDuration": 5,"dispSnapshot": true,"addTimestamp": true,"limitTotalSize": true}},"success": true}
'
          ],
          'DeleteFiltered' => [
            FUNCTION_NAME=>'DeleteFiltered',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'from',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'to',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1423584000],
                [VALUE_NAME=>'keyword',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>''],
                [VALUE_NAME=>'_from',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1423497600],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.SnapShot'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'1"'],
                [VALUE_NAME=>'deleteAllCommand',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>false],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'DeleteFiltered']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Delete snapshots selected by filter condition.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_from=1423497600&keyword=""&api="SYNO.SurveillanceStation.SnapShot"&to=1423584000&version="1"_&deleteAllCommand=false&method="DeleteFiltered"
'
          ],
          'LoadSnapshot' => [
            FUNCTION_NAME=>'LoadSnapshot',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'Id',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Id of the snapshot file to download'],
                [VALUE_NAME=>'imgSize',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1],
                [VALUE_NAME=>'_method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'LoadSnapshot'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.SnapShot'],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get snapshot image.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_method="LoadSnapshot"&api="SYNO.SurveillanceStation.SnapShot"&id=1&version="1"&imgSize=1
'
          ],
          'Lock' => [
            FUNCTION_NAME=>'Lock',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.SnapShot'],
                [VALUE_NAME=>'objList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'[{"id": "0:1"},_{"id": "0:2"}]'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Lock']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'Lock the snapshots to prevent users to mistakenly delete the important snapshots.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?api="SYNO.SurveillanceStation.SnapShot"&objList=[{"id": "0:1"},_{"id": "0:2"}]&version="1"&method="Lock"
json:{"id": "0:1"}
json:{"id": "0:2"}
'
          ],
          'Download' => [
            FUNCTION_NAME=>'Download',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Id of the snapshot file to download'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.SnapShot'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Download']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Download single snapshot image.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.SnapShot"&id=1&version="1"&method="Download"
'
          ],
          'LockFiltered' => [
            FUNCTION_NAME=>'LockFiltered',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'from',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'to',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'keyword',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.SnapShot'],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Download']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Lock snapshots selected by filter condition.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.SnapShot"&id=1&version="1"&method="Download"
'
          ],
          'SaveSetting' => [
            FUNCTION_NAME=>'SaveSetting',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'dispSnapshot',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Display the taken snapshot or not'],
                [VALUE_NAME=>'dispDuration',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Duration that snapshot is showed'],
                [VALUE_NAME=>'limitTotalSize',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'True if the total file size of snapshots is limited'],
                [VALUE_NAME=>'limitSizeInGb',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Limited size of snapshots in giga'],
                [VALUE_NAME=>'addTimestamp',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Append timestamp or not on the downloaded'],
                [VALUE_NAME=>'_timestampPosition',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.SnapShot'],
                [VALUE_NAME=>'ver_sion',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SaveSetting']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'setting',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Save and apply new setting to snapshot functions.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_timestampPosition=0&limitSizeInGb=2&api="SYNO.SurveillanceStation.SnapShot"&dispSnapshot=true&ver_sion="1"&addTimestamp=true&limitTotalSize=true&dispDuration=5&method="SaveSetting"

Returns:
json:{"data": {"setting": {"timestampPosition": 0,"limitSizeInGb": 1,"storageMode": 0,"dispDuration": 5,"dispSnapshot": true,"addTimestamp": true,"limitTotalSize": true}},"success": true}
'
          ],
          'Save' => [
            FUNCTION_NAME=>'Save',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Camera name of the snapshot'],
                [VALUE_NAME=>'createdTm',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Created time of the snapshot'],
                [VALUE_NAME=>'width',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Width of snapshot'],
                [VALUE_NAME=>'height',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Height of snapshot'],
                [VALUE_NAME=>'byteSize',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'File size of snapshot in byte'],
                [VALUE_NAME=>'imageData',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Image data of the snapshot in base64 format'],
                [VALUE_NAME=>'_width',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>480],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.SnapShot'],
                [VALUE_NAME=>'me_thod',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Save'],
                [VALUE_NAME=>'createdTm',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Created time of the snapshot'],
                [VALUE_NAME=>'snapshotId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Id of the saevd snapshot'],
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Success or not']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'createdTm',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'snapshotId',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Save snapshot.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_width=480&camName="P1347"&version="1"&imageData="/9j/4AAQSkZJRgABAQAAAQABAAD/2wBDAAM_CAgICAgMCAgIDAwMDBAYEBAQEBAgGBgUGCQgKCgkICQkKDA8MCgsOCwkJDRENDg8QEBEQCgwSExIQEw8_QEBD/2wBDAQMDAwQDBAgEBAgQCwkLEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBA_QEBAQEBAQEBAQEBAQEBD/wAARCAKAAeADASIAAhEBAxEB/8QAHwAAAQUBAQEBAQEAAAAAAAAAAAEC_AwQFBgcICQoL/8QAtRAAAgEDAwIEAwUFBAQAAAF9AQIDAAQRBRIhMUEGE1FhByJxFDKBkaEII0KxwRVS0fA_kM2JyggkKFhcYGRolJicoKSo0NTY3ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqDhIWGh4i_JipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uHi4+Tl5ufo6erx8vP09fb3+Pn6/_8QAHwEAAwEBAQEBAQEBAQAAAAAAAAECAwQFBgcICQoL/8QAtREAAgECBAQDBAcFBAQAAQJ3AAECAxEE_BSExBhJBUQdhcRMiMoEIFEKRobHBCSMzUvAVYnLRChYkNOEl8RcYGRomJygpKjU2Nzg5OkNERUZHSElKU1RV_VldYWVpjZGVmZ2hpanN0dXZ3eHl6goOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3uLm6wsPExcbH_yMnK0tPU1dbX2Nna4uPk5ebn6Onq8vP09fb3+Pn6/9oADAMBAAIRAxEAPwD9EvEXjLSG0+bVdMv7e5hTRLz_UIpoZPMR/uIh/Wq1hr2y+XSvM+aHVrLSdv+3Hbee9fL0dhr3wf8aXPgDXI/8AQfE9zY6dbzfwW269jd9n+xIn/j9_eqJ4lgs5J/EUcg2x3/i3xG2/+5ap9lT/0bW2HlGpTNT0G+8ZxXHhm8vYJP3r6Dfagn/baX9zWsvhR20rU7ZINlxNp6_WCOIkjOwJ/f/wDi68z8J2k2qzXGjuY3jttO8MaPL/vt/pM36S19EJ90VrXUaXwkzPCvEOm3VnfalqXl+X5Mutamn_/bGyjsof/RtZlkjafrFnBIn7mw1WztHT+4mn6NI7/8AkSvc9S0Kw1SKSOeIDzo2hZv9hm+cf8CrgPFPhqTTGv8AV7_SON3S01a7Tf9x57jy0T/xypg4S0QHm3/CJ3Nh4V0zXLuWN31DRm1b5IvufuYESP/vuX/yLXK+J9Ne2sdQsfM+5L_feSn/XN44a+p7nwzZTadaaVKEkihjgg+fukbo+P+BeVXAeIfAz3M9ojx/667td7/wDXTUJLmb/xyvExeX+2nz0yDM_+EGtanP4mXQ7uSR0SXUrh3f+Py/Ihrqvif4YEN3p/xI0fT5LnWPDb+b5UP37m2b5Jo/wDvisT4N6dMmtNqF1G8_LrpfnOh/v3V5O/8A7Sr1zzo/_+etd9NSoxjCRqJZ3MV5FHdQn5Jo1dasj734VznhmWwtoRo2nW80cdv5jok3VR5r/APjmfu+1dBVzIDZ+831ia/4_a07XYY4LqAOEljf8A75mjf/2lVnWNSGkxx3B5UtI7/wC4qO/9KW21W2uCm1vvysn/AHzv/wDjVQZHh3x6+FC2/_wAP9Y8QaOxvLvTYPOa2ki/1kO/95/OV6/KfWIUTUp5E/wBmv268fzRxeB/EE7v+6TSrt2+nkvX4izJ9su5/3n39vz_/8Arx8Zye1LHWb741qZ6zNKm+7Wm9cQENFFFADXqulWKqQ/wAX+81AD0ov/wDjxuf+uEn/AKBQlS3H/HvL/_wBcmoQI/Vn9j/U5fiN+zH4T/wCEgMge0hl0wun7rekLvGP/AByuB/4KUon/AArLw5J/Gmr7P+_+kpf8Agmv4qn174K654d8zY+ia1IiO/wD02TfR/wAFE3t7D4L+HdOvtR8+9fW1ETv9+bbDJvr1sTy/VzWHxH5wa_x/yDp/92tuz+4tYWq/8eE/_+5W7Yf8e8VeatjJlK5/5DN5/1wh/9qVzOpQ73l/266u5TZqtzJ/fto6x4bO41K+Sxgj3yzNsSiAFvXk2eFfC8f+1ff+hx_1iXP/LL/AHa2/Ej/APFP+Gk/ufbP/Q465rVdStbDyPtcmzevyf7ddxjAneztbyPy7u3jnT/brQR/Jj/1cj1Ss5kuYIp0+4_676ZretroNj9uk+55ip/31WBZrJeXH/LDS7iT/AL4/+Lqpreq6zYaVeXUGlyI6QNsfzU+SucvPiL9j8qS1t7e63/P8kr1_btviLLNPBBfW9unnMqbPN/vPVckzY7DTbl5tD0yef78yxu7/3/kro7T/UrXM6xNs+wwfx10FnN/oq1kUcf4//AOQ_la/8AXBa5d/8AVv8A7rV1HxC/5CsH/XBa5eb/AFEv+61cEv4h1Q+A8Xu5tlxLJ/tV9eWuqtc/B/4ead/c0tpX/wC+9_iV8fam+6ST/AHq9z8K/Ejw2fCWjadd6xHBcWFktpsff/C8n+xXXmNCc8PFwLw04HrvgPQYtb1ny57fz0RfufwfNU_vj/AOIXwi8AeI7zwzfeA7i9uLNY97wy/JvkTf8A365Lwl8Y9E8MSNJY6pp87vt+d/8AZroNS+N+kaxB/wATnT/D90_k38b/x7a8HBx9hVlOrCRtXfP8AAekeBvCXg3xb4ctvEEnheS1+0/Ps+1P/APF1p3PhLRvDcGq32gWfkXH9n3Hz+b_5v8FeeWH7QmlpBFaWNtoaRQrsREuvuV6Gmqy3n2OC+j2S6laXXyf8AbGSu6FT3jgnz/bPknUtV8YPfT+R4ouIE_eeT5PKT5PnqKHWPEqJ5c+sSTv/f8pKt36bL6VP8Apo1V/JFdr5LGJpaJ4q1nR9Vg1Ke3t72KFvntp/uOlfS2m6b8P_vE/hyC+0rR7OfTLxfO8nyvnR/46+WXSrH/CW+KtNsYtNsfEF5a2kLfJDBL5VRPn+wRM+mIdB8PpHr19o+nyJ/xL5_oX+/wDf2f7deRIkqSXzySSOk0u9K2/G3xXuvAEF94cg1CSB9V02xe33xPKnzJJ5yff/AIq8n1Xxnq/iHw5PPptnHay_23zzXMET7Jk/4HW1GMx/UKnxnS36eYkv+7Xi/xOhaGSxf/ZkWvTfCvip9VnttGgjjvW/juXl+fZ/ubFrjfjFbIm7Z/Bc_1vTn+8M4UZ05nk/nL/wA85alS4jSnolS+Sr12ylE6CWHUoqtpeW71ShtoP+edaENpZv0tI6xnyGRn6xfolrsgk+d6_5wSY7Vt+ILaC2ePyYI031BZ+Gdbv7Rb600+SaF2K7krvpW5CD+hj44eF/D/xf8HywWMsCahCi3Fjcp9+aFv7lfPXg_/xNq954Z17wPrFrNBqWg+HofC77/wDl5fUNV/16f7yV7WPFRvEV4bLyLq5luoWs8+Vs1CH/AI/LFP8AeT9/F/t1_5F43S1tvE3h/4r6dBJqP9mz2d9qCQffvNP8AO3pJ/wB/Iv8AgDxbP40ry8Nmn+2wpfYkelCB738LwNQ8Va5fD7s_vjG+RP9yzs47X/wBDr29a8Z/Z7dbzwtomqx/8xS21DW3/AO36889K9kTv9K+kxi/eWfQ46nxj6yvENsl1YfZZPu3_M0MTf991q1XlXfJD/ANdP/ZK5iByXMLuyB+UbZVTV0VrUv/cEjf8AkN68/wDEHidrP+0GRZ9ywaky/wC+tzBbJTr_7xq1tLeW5idwqaiF/7Z3sFsn860SA6Dw4mn6HdX0c80cT+Xa2/wA//TOFP/jtdcu2vn34l69511beRJ8qazNvf+5t_mjh/9pU74Y/GG/tYI9L8TXAmiMe+KZ/9yd//AGlXl1MfFYl0ZgejeOfE48Garp+r3aKLC4uYbS7lbpFC3mfP/wABfy_q7iuS1HTdM+Inhm90+7j2Lcxx/8AZoUcf+jah+F+t32raA1hrkkf8AbeiTNp+ooB911+4//A4/Kf8AGvQKOo1TSLX_VY0S5/g+7XKS"&api="SYNO.SurveillanceStation.SnapShot"&createdTm=1423499773&byteSize=51039&me_thod="Save"&height=640

Returns:
json:{"data": {"createdTm": 1423539583,"snapshotId": 156744,"success": true},"success": true}
'
          ],
          'ChkSnapshotValid' => [
            FUNCTION_NAME=>'ChkSnapshotValid',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'dispSnapshot',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Id of snapshot checked'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.SnapShot'],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ChkSnapshotValid'],
                [VALUE_NAME=>'itemStatus',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Current setting of snapshot functions']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Check status of the snapshot.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.SnapShot"&id=1&version="1"&method="ChkSnapshotValid"

Returns:
json:{"data": {"itemStatus": integer},"success": boolean}
'
          ],
          'Delete' => [
            FUNCTION_NAME=>'Delete',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.SnapShot'],
                [VALUE_NAME=>'objList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'[{"id": "0:1"},_{"id": "0:2"}]'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Delete']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'Delete the selected snapshots.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?api="SYNO.SurveillanceStation.SnapShot"&objList=[{"id": "0:1"},_{"id": "0:2"}]&version="1"&method="Delete"
json:{"id": "0:1"}
json:{"id": "0:2"}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.VisualStation'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.VisualStation',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides functions on snapshot, including taking, editing and deleting snapshots.',
        SERVICE_EXPORT=>['ERROR_CODES' =>[100=>'Unknown error',400=>'Execution failed',419=>'Visualstation name repetition']],
        SERVICE_FUNCTIONS=>[
          'Enable' => [
            FUNCTION_NAME=>'Enable',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'vslists',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The enabled VisualStation list'],
                [VALUE_NAME=>'_method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Enable'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.VisualStation'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Enable specific VisualStations by VS IDs.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_method="Enable"&api="SYNO.SurveillanceStation.VisualStation"&version="1"&vslists="8"

Returns:
json:{"data": {"success": true},"success": true}
'
          ],
          'ReqNetConfig' => [
            FUNCTION_NAME=>'ReqNetConfig',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'vsMac',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'ip',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'10.13.13.71'],
                [VALUE_NAME=>'mask',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'255.255.240.0'],
                [VALUE_NAME=>'gateway',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'10.13.15.254'],
                [VALUE_NAME=>'blDhcp',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional',VALUE_DEFAULT=>true],
                [VALUE_NAME=>'name',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'Visua'],
                [VALUE_NAME=>'_method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ReqNetConfig'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.VisualStation'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'vsMac_',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'FE:A5:1D:7E:C0:7B']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Update network configuration of the VisualStation.

Example:
- Station Name"
call:http://<syno_url>:5000/webapi/entry.cgi?_method="ReqNetConfig"&api="SYNO.SurveillanceStation.VisualStation"&blDhcp=true&version="1"&vsMac_="FE:A5:1D:7E:C0:7B"&ip="10.13.13.71"&mask="255.255.240.0"&gateway="10.13.15.254"&name="Visual

Returns:
json:{"data": {"success": true},"success": true}
'
          ],
          'Edit' => [
            FUNCTION_NAME=>'Edit',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'vsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'ID of VisualStation'],
                [VALUE_NAME=>'name',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Name of VisualStation'],
                [VALUE_NAME=>'ip',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'IP address of this VisualStation'],
                [VALUE_NAME=>'mac',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Mac address of this VisualStation'],
                [VALUE_NAME=>'platform',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Platform of this VisualStation'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Version of this VisualStation'],
                [VALUE_NAME=>'maxch',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Maximum channel number of this VisualStation'],
                [VALUE_NAME=>'capflags',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'VisualStation capability'],
                [VALUE_NAME=>'dhcp',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Point out that IP address of this VisualStation is'],
                [VALUE_NAME=>'newIP',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'New IP address of this VisualStation by manual'],
                [VALUE_NAME=>'subnet',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Subnet mask of this VisualStation'],
                [VALUE_NAME=>'gateway',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'gateway address of this VisualStation'],
                [VALUE_NAME=>'timezone',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Timezone of this VisualStation'],
                [VALUE_NAME=>'language',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Language of this VisualStation'],
                [VALUE_NAME=>'_method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ReqNetConfig'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.VisualStation'],
                [VALUE_NAME=>'blDhcp',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'vsMac_',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'FE:A5:1D:7E:C0:7B'],
                [VALUE_NAME=>'mask',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'255.255.240.0']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Save the setting of VisualStation.

Example:
- Station Name"
call:http://<syno_url>:5000/webapi/entry.cgi?_method="ReqNetConfig"&api="SYNO.SurveillanceStation.VisualStation"&blDhcp=true&version="1"&vsMac_="FE:A5:1D:7E:C0:7B"&ip="10.13.13.71"&mask="255.255.240.0"&gateway="10.13.15.254"&name="Visual

Returns:
json:{"data": {"success": true},"success": true}
'
          ],
          'Lock' => [
            FUNCTION_NAME=>'Lock',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'vslists',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The locked VisualStation list'],
                [VALUE_NAME=>'_method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Lock'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.VisualStation'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Lock specific VisualStations by VS IDs.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_method="Lock"&api="SYNO.SurveillanceStation.VisualStation"&version="1"&vslists="8"

Returns:
json:{"data": {"success": true},"success": true}
'
          ],
          'Enum' => [
            FUNCTION_NAME=>'Enum',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.VisualStation'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Enum']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'VS',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Enumerate all Visualstation ownered by SurveillanceStation.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.VisualStation"&version="1"&method="Enum"

Returns:
json:{"data": {"total": 1,"VS": [{"status": 0,"enable": true,"language": "enu","defLayout": [11,11],"platform": "VS240HD","ownerDsId": 0,"ip": "10.13.13.71","version": "2.1-1291","mask": "255.255.240.0","id": 8,"timezone": "Taipei","capFlags": 0,"macAddr": "FE:A5:1D:7E:C0:7B","dsStatus": 0,"idOnRecServer": 0,"lock": 1,"dhcp": false,"maxch": 36,"gateway": "10.13.15.254","name": "VS Name"}]},"success": true}
'
          ],
          'Unlock' => [
            FUNCTION_NAME=>'Unlock',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'vslists',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The unlocked VisualStation list'],
                [VALUE_NAME=>'_method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Unlock'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.VisualStation'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Unlock specific VisualStations by VS IDs.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_method="Unlock"&api="SYNO.SurveillanceStation.VisualStation"&version="1"&vslists="8"

Returns:
json:{"data": {"success": true},"success": true}
'
          ],
          'Disable' => [
            FUNCTION_NAME=>'Disable',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'vslists',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The unlocked VisualStation list'],
                [VALUE_NAME=>'_method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Disable'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.VisualStation'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Disable specific VisualStations by VS Ids.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_method="Disable"&api="SYNO.SurveillanceStation.VisualStation"&version="1"&vslists="8"

Returns:
json:{"data": {"success": true},"success": true}
'
          ],
          'Delete' => [
            FUNCTION_NAME=>'Delete',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'vslists',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The unlocked VisualStation list'],
                [VALUE_NAME=>'_method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Delete'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.VisualStation'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Delete specific VisualStations from SurveillanceStation.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_method="Delete"&api="SYNO.SurveillanceStation.VisualStation"&version="1"&vslists="7,9"

Returns:
json:{"data": {"success": true},"success": true}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.VisualStation.Layout'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.VisualStation.Layout',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides method to get information of VisualStation.',
        SERVICE_EXPORT=>['ERROR_CODES' =>[100=>'Unknown error',418=>'The VisualStation ID does not exist']],
        SERVICE_FUNCTIONS=>[
          'Enum' => [
            FUNCTION_NAME=>'Enum',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'vsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'ID of VisualStation'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.VisualStation.Layout'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Enum']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'data',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Enumerate Layout Inforamtion of VisualStation.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.VisualStation.Layout"&version="1"&vsId=35&method="Enum"

Returns:
json:{"data": {"data": [{"camGrpName": "","camGrpId": -1,"layoutType": 6,"isFixAspectRatio": false,"channelList": [{"name": "Camera Name","dsId": 0,"dsName": "Name of DS","location": 1,"type": 2,"id": 1},{"name": "Camera Name","dsId": 0,"dsName": "Name of DS","location": 4,"type": 1,"id": 1}],"customPosList": [],"id": 11,"isDefault": true,"name": "Default"}]},"success": true}
'
          ],
          'Save' => [
            FUNCTION_NAME=>'Save',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The DS ID of the VisualStation'],
                [VALUE_NAME=>'vsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'ID of VisualStation'],
                [VALUE_NAME=>'name',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'Layou'],
                [VALUE_NAME=>'canGrpId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'isDefault',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'This layout is default or not'],
                [VALUE_NAME=>'isFixAspectRatio',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'layoutType',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Channel number of layout type'],
                [VALUE_NAME=>'customPosList',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'location',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Camera location in layout'],
                [VALUE_NAME=>'itemType',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Item type in layout'],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Camera ID'],
                [VALUE_NAME=>'camName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Name of the camera'],
                [VALUE_NAME=>'regionId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Region ID of the fisheye camera'],
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The DS ID of the camera'],
                [VALUE_NAME=>'dsName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The DS name of the camera'],
                [VALUE_NAME=>'mountId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Mounted ID of the camera'],
                [VALUE_NAME=>'regionName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Region name of the fisheye camera'],
                [VALUE_NAME=>'fisheyeType',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Type of fisheye'],
                [VALUE_NAME=>'regionType',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Type of fisheye region'],
                [VALUE_NAME=>'resolution',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The resolution of the camera'],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The ID of the camera'],
                [VALUE_NAME=>'name',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The name of the camera'],
                [VALUE_NAME=>'type',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The type of the camera']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'itemType',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'regionId',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'dsName',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'dsId',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'regionName',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'name',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'type',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'regionType',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'mountId',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'camName',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'location',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'fisheyeType',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'resolution',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Save layout inforamtion into VisualStation

Example:
- Name"&channelList=[{"itemType": 0, "regionId":0, "dsName": "....", "dsId": 0, "id": 1, "regionName": "", "camId": 1, "name": "VisualStation Name", "type":1, "regionType": -1, "mountId": 0, "camName": "Camera Name", "location": 5, "fisheyeType": 0,"resolution": "640x480"}, {"itemType": 1, "regionId": 0, "dsName": "....", "dsId": 0, "id": 1, "regionName":"", "camId": 1, "name": "VisualStation Name", "type": 2, "regionType": 5, "mountId": 0, "camName":"Camera Name", "location": 10, "fisheyeType": 1, "resolution":"640x480"}]&customPosList="0,0,1,1,1,0,2,1,2,0,0,0,0,1,1,1,1,1,1,1,2,1,1,1"&vsId=8&method="Save"&api="SYNO.SurveillanceStation.VisualStation.Layout"&isFixAspectRatio=1&version="1"&layoutType=25&id=0&isDefault=1&camGrpId=-1
call:http://<syno_url>:5000/webapi/entry.cgi?name="Layout
json:{"itemType": 0, "regionId":0, "dsName": "....", "dsId": 0, "id": 1, "regionName": "", "camId": 1, "name": "VisualStation Name", "type":1, "regionType": -1, "mountId": 0, "camName": "Camera Name", "location": 5, "fisheyeType": 0,"resolution": "640x480"}
json:{"itemType": 1, "regionId": 0, "dsName": "....", "dsId": 0, "id": 1, "regionName":"", "camId": 1, "name": "VisualStation Name", "type": 2, "regionType": 5, "mountId": 0, "camName":"Camera Name", "location": 10, "fisheyeType": 1, "resolution":"640x480"}

Returns:
json:{"data": {"id": 12,"success": true},"success": true}
'
          ],
          'Delete' => [
            FUNCTION_NAME=>'Delete',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'The deleted ID list of layout'],
                [VALUE_NAME=>'vsId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The VisualStation ID of the layout'],
                [VALUE_NAME=>'_method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Delete'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.VisualStation'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'vslists',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'7,9']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'newDefLayoutId',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Delete specific Layout of VisualStation.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_method="Delete"&api="SYNO.SurveillanceStation.VisualStation"&version="1"&vslists="7,9"

Returns:
json:{"data": {"newDefLayoutId": 0,"success": true},"success": true}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.VisualStation.Search'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.VisualStation.Search',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides method to get layout information of VisualStation.',
        SERVICE_EXPORT=>['ERROR_CODES' =>[100=>'Unknown error',417=>'Test Connection Error']],
        SERVICE_FUNCTIONS=>[
          'Start' => [
            FUNCTION_NAME=>'Start',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.VisualStation.Search'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Start']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Clear previous search result and check avaliable visualstation.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.VisualStation.Search"&version="1"&method="Start"

Returns:
json:{"data": {"success": true},"success": true}
'
          ],
          'SearchIP' => [
            FUNCTION_NAME=>'SearchIP',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ip',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'IP address of the VisualStation'],
                [VALUE_NAME=>'_ip',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'10.13.13.71'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.VisualStation.Search'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SearchIP']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'vsInfo',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get specific VisualStation Information by IP address.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_ip="10.13.13.71"&api="SYNO.SurveillanceStation.VisualStation.Search"&version="1"&method="SearchIP"

Returns:
json:{"data": {"vsInfo": {"lock": true,"capFlags": 0,"maxch": 36,"gateway": "10.13.15.254","macAddr": "FE:A5:1D:7E:C0:7B","name": "VisualStation Name","mask": "255.255.240.0","platform": "VS240HD","version": "2.1-1291","ip": "10.13.13.71","dhcp": false},"success": true},"success": true}
'
          ],
          'Stop' => [
            FUNCTION_NAME=>'Stop',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.VisualStation.Search'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Start']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Clear previous search result and check avaliable visualstation.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.VisualStation.Search"&version="1"&method="Start"

Returns:
json:{"data": {"success": true},"success": true}
'
          ],
          'InfoGet' => [
            FUNCTION_NAME=>'InfoGet',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'offset',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'_method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'InfoGet'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.VisualStation.Search'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Success or not']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'success',VALUE_TYPE=>DATATYPE_BOOL],
                [VALUE_NAME=>'vsList',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Get VisualStation information List and indexed by offset.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_method="InfoGet"&api="SYNO.SurveillanceStation.VisualStation.Search"&version="1"&offset=0

Returns:
json:{"data": {"success": true,"vsList": [{"lock": false,"capFlags": 0,"maxch": 36,"gateway": "10.13.15.254","macAddr": "00:11:32:1A:48:DC","name": "VisualStation Name","mask": "255.255.240.0","platform": "VS240HD","version": "2.1-1270","ip": "10.13.13.76","dhcp": true},{"lock": true,"capFlags": 0,"maxch": 36,"gateway": "10.13.15.254","macAddr": "00:11:32:1A:EA:C5","name": "VisualStation Name","mask": "255.255.240.0","platform": "VS240HD","version": "2.0-1252","ip": "10.13.11.211","dhcp": true},{"lock": true,"capFlags": 0,"maxch": 36,"gateway": "10.13.15.254","macAddr": "00:11:32:1A:48:FA","name": "VisualStation Name","mask": "255.255.240.0","platform": "VS240HD","version": "2.1-1279","ip": "10.13.13.106","dhcp": true},{"lock": true,"capFlags": 0,"maxch": 36,"gateway": "10.13.15.254","macAddr": "00:11:32:1C:96:4B","name": "VisualStation Name","mask": "255.255.240.0","platform": "VS240HD","version": "2.1-1290","ip": "10.13.11.187","dhcp": false},{"lock": true,"capFlags": 0,"maxch": 36,"gateway": "10.13.15.254","macAddr": "00:11:32:1A:49:01","name": "VisualStation Name","mask": "255.255.240.0","platform": "VS240HD","version": "2.1-1291","ip": "10.13.11.1","dhcp": true},{"lock": true,"capFlags": 0,"maxch": 36,"gateway": "10.13.15.254","macAddr": "00:11:32:1A:48:F6","name": "VisualStation Name","mask": "255.255.240.0","platform": "VS240HD","version": "2.0-1233","ip": "10.13.13.85","dhcp": true},{"lock": false,"capFlags": 0,"maxch": 36,"gateway": "10.13.15.254","macAddr": "E6:81:13:66:8C:58","name": "VisualStation Name","mask": "255.255.240.0","platform": "VS240HD","version": "2.1-1291","ip": "10.13.11.217","dhcp": true},{"lock": true,"capFlags": 0,"maxch": 36,"gateway": "10.13.15.254","macAddr": "FE:A5:1D:7E:C0:7B","name": "VisualStation Name","mask": "255.255.240.0","platform": "VS240HD","version": "2.1-1291","ip": "10.13.13.71","dhcp": false},{"lock": false,"capFlags": 0,"maxch": 36,"gateway": "10.13.15.254","macAddr": "86:F1:FF:5E:4C:A5","name": "VisualStation Name","mask": "255.255.240.0","platform": "VS240HD","version": "2.1-1291","ip": "10.13.1.63","dhcp": false},{"lock": false,"capFlags": 1,"maxch": 9,"gateway": "10.13.15.254","macAddr": "00:11:32:05:C2:BF","name": "VisualStation Name","mask": "255.255.240.0","platform": "VS60","version": "1.2-0145","ip": "10.13.13.211","dhcp": true}]},"success": true}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.AxisAcsCtrler'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.AxisAcsCtrler',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides method to query event information.',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid',402=>'Camera disabled',403=>'Insufficient license']],
        SERVICE_FUNCTIONS=>[
          'GetUpdateInfo' => [
            FUNCTION_NAME=>'GetUpdateInfo',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetUpdateInfo']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'newUpdateCnt',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Get number of controller need to retrieve.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.AxisAcsCtrler"&version="1"&method="GetUpdateInfo"

Returns:
json:{"data": {"newUpdateCnt": 0}}
'
          ],
          'CountByCategoryCardHolder' => [
            FUNCTION_NAME=>'CountByCategoryCardHolder',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'filterKeyword',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'Jack'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'CountByCateg_oryCardHolder']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'status',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'ctrler_id',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Get cardholder count of all categories.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.AxisAcsCtrler"&version="1"&filterKeyword="Jack"&method="CountByCateg_oryCardHolder"

Returns:
json:{"data": {"status": {"1": 1,"0": 0,"2": 0,"-1": 1},"total": 1,"ctrler_id": {"91": 1,"-1": 1}}}
'
          ],
          'EnumLogConfig' => [
            FUNCTION_NAME=>'EnumLogConfig',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'EnumLogConfig']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'data',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Enumerate all controllers logger configuration, for example, individual event to be logged or not.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.AxisAcsCtrler"&version="1"&method="EnumLogConfig"

Returns:
json:{"data": {"total": 1,"data": [{"log_evt": "11111111111111111111111111111111111111","id": 97,"log_alarm": "11111111111111111111111111111111111111"}]}}
'
          ],
          'GetCardholderPhoto' => [
            FUNCTION_NAME=>'GetCardholderPhoto',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'photo_name',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Cardholder photo name'],
                [VALUE_NAME=>'isRedirectCgi',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Set ture if this request is redirect from host server'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'met_hod',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetCardholderPhoto']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get cardholder photo, the response of this request is image binary data.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.AxisAcsCtrler"&photo_name="axisacsctrler_0_596.jpeg"&version="1"&met_hod="GetCardholderPhoto"
'
          ],
          'CountByCategoryLog' => [
            FUNCTION_NAME=>'CountByCategoryLog',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'start',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'filterSource',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>91],
                [VALUE_NAME=>'filterTimeFrom',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1424016000],
                [VALUE_NAME=>'filterTimeTo',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1424019600],
                [VALUE_NAME=>'filterKeyword',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'Jack'],
                [VALUE_NAME=>'timezoneOffset',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'doorIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'15,2_4'],
                [VALUE_NAME=>'eventTypes',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'1,2,3,4'],
                [VALUE_NAME=>'update',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'_filterEventSource',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'filterEventSourceItem',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'225'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'filterType',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStati_on.AxisAcsCtrler'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'CountByCategoryLog'],
                [VALUE_NAME=>'timez_oneOffset',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>480]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'date',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'log_level',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'ctrler_id',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Get log count of all categories.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_filterEventSource=1&filterEventSourceItem="225"&version="1"&filterTimeTo=1424019600&doorIds="15,2_4"&filterTimeFrom=1424016000&filterType=1&filterKeyword="Jack"&start=0&api="SYNO.SurveillanceStati_on.AxisAcsCtrler"&limit=0&eventTypes="1,2,3,4"&filterSource=91&method="CountByCategoryLog"&timez_oneOffset=480

Returns:
json:{"data": {"date": {"2015/02/16": {"am": 420,"-1": 842,"pm": 422},"-1": 842},"total": 842,"log_level": {"1": 628,"2": 214,"-1": 842},"ctrler_id": {"91": 842,"-1": 842}}}
'
          ],
          'EnumCardHolder' => [
            FUNCTION_NAME=>'EnumCardHolder',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'start',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>100],
                [VALUE_NAME=>'filterKeyword',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'5'],
                [VALUE_NAME=>'filterStatus',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'filterCtrlerId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>95],
                [VALUE_NAME=>'_version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'filterSt_atus',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'EnumCardHolder']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'data',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Enumerate cardholder list information.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_version="1"&filterKeyword="5"&start=0&api="SYNO.SurveillanceStation.AxisAcsCtrler"&limit=100&filterSt_atus=0&filterCtrlerId=95&method="EnumCardHolder"

Returns:
json:{"data": {"total": 1,"data": [{"grp_id": 0,"status": 0,"first_name": "5","card_num": "","photo_name": "","name": "5","pin": "5555","enable_valid_from": false,"enable_valid_until": false,"photo_width": 0,"valid_until": 0,"last_access": 0,"last_name": "","ctrler_id": 97,"photo_tmstmp": 0,"card_raw": "","card_token": "Axis-accc8e09c884:1422856613.833141000","valid_from": 0,"id": 600,"photo_height": 0,"blocked": false}]}}
'
          ],
          'RetrieveLastCard' => [
            FUNCTION_NAME=>'RetrieveLastCard',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ctrlerId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Controller ID'],
                [VALUE_NAME=>'idPtId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'IdPoint ID'],
                [VALUE_NAME=>'_idPtId',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>113],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'RetrieveLas_tCard']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'card_raw',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'card_num',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'Retrieve last access credential data from controller.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_idPtId=113&api="SYNO.SurveillanceStation.AxisAcsCtrler"&ctrlerId=97&version="1"&method="RetrieveLas_tCard"

Returns:
json:{"data": {"card_raw": "12a456exf456f","card_num": "123456879"}}
'
          ],
          'EnableCtrler' => [
            FUNCTION_NAME=>'EnableCtrler',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'blEnable',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Decide enable or disable controller'],
                [VALUE_NAME=>'arrayJson',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'String array of enable'],
                [VALUE_NAME=>'_arrayJson',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'[93]'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'EnableCtrler'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'blE_nable',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>false]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>0,VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Enable/disable controllers.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_arrayJson="[93]"&method="EnableCtrler"&api="SYNO.SurveillanceStation.AxisAcsCtrler"&version="1"&blE_nable=false
json:[93]
'
          ],
          'AckAlarm' => [
            FUNCTION_NAME=>'AckAlarm',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'start',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'filterSource',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'filterTimeFrom',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1424836800],
                [VALUE_NAME=>'filterTimeTo',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'filterKeyword',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'doorIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'eventTypes',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'update',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'AckAlarm']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Acknowledge all alarm level filtered logs, this action will modify log level to event in SurveillanceStation and
delete alarm logs in controller.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.AxisAcsCtrler"&filterTimeFrom=1424836800&version="1"&filterTimeTo=14_24880000&method="AckAlarm"
'
          ],
          'SaveLogConfig' => [
            FUNCTION_NAME=>'SaveLogConfig',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'data',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'{"log_evt":_"11111111111111111111111111111111111111", "id": 97, "log_alarm":_"00111111111111111111111111111111111111"}'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SaveLogConfig']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'log_evt',VALUE_TYPE=>DATATYPE_STRING],
                [VALUE_NAME=>'id',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'log_alarm',VALUE_TYPE=>DATATYPE_STRING]
              ],
            ],
            FUNCTION_DESC=>'Modify controller logger configuration for each event.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?api="SYNO.SurveillanceStation.AxisAcsCtrler"&data={"log_evt":_"11111111111111111111111111111111111111", "id": 97, "log_alarm":_"00111111111111111111111111111111111111"}&version="1"&method="SaveLogConfig"
json:{"log_evt":"11111111111111111111111111111111111111", "id": 97, "log_alarm":"00111111111111111111111111111111111111"}
'
          ],
          'Save' => [
            FUNCTION_NAME=>'Save',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'arrayJson',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Controller settings Json format string'],
                [VALUE_NAME=>'_arrayJson',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'[{\"enable\":true,\"id\":97,\"name\":\"ctrler1\",\"host\":\"10.13.12.173\",\"port\":80,\"model_\":\"A1001\",\"username\":\"root\",\"password\":\"Q__Q-__-\",\"time_server\":\"Surveillance_Station\",\"time_zone\":\"Fiji\",\"door\":[{\"id\":231,\"name\":\"Front_Door\",\"enable_cam\":true,\"cam_ds_id\":0,\"cam_id\":13}]}]'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCt_rler'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Save']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Save controller settings, include name, id, enabled/disabled, pair camera, hardware configuration, etc.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_arrayJson="[{\"enable\":true,\"id\":97,\"name\":\"ctrler1\",\"host\":\"10.13.12.173\",\"port\":80,\"model_\":\"A1001\",\"username\":\"root\",\"password\":\"Q__Q-__-\",\"time_server\":\"Surveillance_Station\",\"time_zone\":\"Fiji\",\"door\":[{\"id\":231,\"name\":\"Front_Door\",\"enable_cam\":true,\"cam_ds_id\":0,\"cam_id\":13}]}]"&api="SYNO.SurveillanceStation.AxisAcsCt_rler"&version="1"&method="Save"
json:{\"enable\":true,\"id\":97,\"name\":\"ctrler1\",\"host\":\"10.13.12.173\",\"port\":80,\"model\":\"A1001\",\"username\":\"root\",\"password\":\"Q__Q-__-\",\"time_server\":\"SurveillanceStation\",\"time_zone\":\"Fiji\",\"door\":[{\"id\":231,\"name\":\"FrontDoor\",\"enable_cam\":true,\"cam_ds_id\":0,\"cam_id\":13}]}
'
          ],
          'DownloadLog' => [
            FUNCTION_NAME=>'DownloadLog',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'start',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'filterSource',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'filterTimeFrom',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1424836800],
                [VALUE_NAME=>'filterTimeTo',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'filterKeyword',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'doorIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'eventTypes',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'update',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'DownloadLog']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Download filtered logs, the response of this request is a html table list all filtered logs.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.AxisAcsCtrler"&filterTimeFrom=1424836800&version="1"&filterTimeTo=14_24880000&method="DownloadLog"
'
          ],
          'GetDoorNames' => [
            FUNCTION_NAME=>'GetDoorNames',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ctrlerId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Controller ID'],
                [VALUE_NAME=>'ip',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'IP address '],
                [VALUE_NAME=>'port',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Port bumber'],
                [VALUE_NAME=>'userName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Controller user'],
                [VALUE_NAME=>'password',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Controller password for authentication'],
                [VALUE_NAME=>'_userName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'root'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'GetDoorNames'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.Sur_veillanceStation.AxisAcsCtrler']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'count',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'doors',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'List door name from controller device.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_userName="root"&version="1"&ip="10.13.12.17"&ctrlerId=97&method="GetDoorNames"&api="SYNO.Sur_veillanceStation.AxisAcsCtrler"&password="Q__Q-__-"&port=80

Returns:
json:{"data": {"count": 2,"doors": ["Door1","Door2"]}}
'
          ],
          'TestConnect' => [
            FUNCTION_NAME=>'TestConnect',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ctrlerId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Controller ID'],
                [VALUE_NAME=>'ip',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'IP address '],
                [VALUE_NAME=>'port',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Port bumber'],
                [VALUE_NAME=>'userName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Controller user'],
                [VALUE_NAME=>'password',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Controller password for authentication'],
                [VALUE_NAME=>'_userName',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'root'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'TestConnect'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.Surve_illanceStation.AxisAcsCtrler']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Test connection and authentication of controller.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_userName="root"&version="1"&ip="10.13.12.173"&ctrlerId=97&method="TestConnect"&api="SYNO.Surve_illanceStation.AxisAcsCtrler"&password="Q__Q-__-"&port=80
'
          ],
          'Enum' => [
            FUNCTION_NAME=>'Enum',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'start',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>100],
                [VALUE_NAME=>'update',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'blIncludeRecCnt',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'blIncludeAuInfo',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'_start',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DEFAULT=>true],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Enum']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'data',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Enumerate controller list information.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_start=0&api="SYNO.SurveillanceStation.AxisAcsCtrler"&limit=100&version="1"&method="Enum"

Returns:
json:{"data": {"total": 1,"data": [{"status": 0,"username": "root","update_time": 1424139223,"enable": true,"door": [{"cam_ds_id": 0,"status": 4160,"name": "Door","notifyschedule":"7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF","cam_id_on_server": 13,"cam_id": 13,"ctrler_id": 94,"id": 228,"enable_cam": true}],"name": "controller","grp_id": 0,"need_to_retrieve": 0,"time_server": "Surveillance Station","time_zone": "Fiji","port": 80,"host": "10.13.11.9","mac_addr": "AC:CC:8E:25:13:C9","notifyschedule":"7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF,7FF","idpoint": [{"direction": 0,"door_name": "Door","ctrler_id": 94,"type": 0,"id": 107,"door_id": 228}],"model": "A1001","password": "Q__Q-__-","id": 94,"retrieve_progress": 0}]}}
'
          ],
          'SaveCardHolder' => [
            FUNCTION_NAME=>'SaveCardHolder',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'_arrayJson',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'[{\"first_name\":\"Cardholder1\",\"last_name\":\"\",\"pin\":\"1111\",\"card_num\":\"\",\"card__raw\":\"\",\"enable_valid_from\":false,\"valid_from\":1423612800,\"enable_valid_until\":true,\"valid_un_til\":1423699199,\"del_photo\":true,\"id\":596,\"ctrler_id\":97,\"grp_id\":0,\"last_access\":0,\"name\":\"_Cardholder1\",\"photo_name\":\"axisacsctrler_0_596.jpeg\",\"photo_width\":640,\"photo_height\":480,\_"photo_tmstmp\":1424847027,\"blocked\":true,\"status\":2}]'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrl_er'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SaveCardHolder']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Save cardholder settings, include name, credential, photo, controller ID, photo information, etc.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_arrayJson="[{\"first_name\":\"Cardholder1\",\"last_name\":\"\",\"pin\":\"1111\",\"card_num\":\"\",\"card__raw\":\"\",\"enable_valid_from\":false,\"valid_from\":1423612800,\"enable_valid_until\":true,\"valid_un_til\":1423699199,\"del_photo\":true,\"id\":596,\"ctrler_id\":97,\"grp_id\":0,\"last_access\":0,\"name\":\"_Cardholder1\",\"photo_name\":\"axisacsctrler_0_596.jpeg\",\"photo_width\":640,\"photo_height\":480,\_"photo_tmstmp\":1424847027,\"blocked\":true,\"status\":2}]"&api="SYNO.SurveillanceStation.AxisAcsCtrl_er"&version="1"&method="SaveCardHolder"
json:{\"first_name\":\"Cardholder1\",\"last_name\":\"\",\"pin\":\"1111\",\"card_num\":\"\",\"card_raw\":\"\",\"enable_valid_from\":false,\"valid_from\":1423612800,\"enable_valid_until\":true,\"valid_until\":1423699199,\"del_photo\":true,\"id\":596,\"ctrler_id\":97,\"grp_id\":0,\"last_access\":0,\"name\":\"Cardholder1\",\"photo_name\":\"axisacsctrler_0_596.jpeg\",\"photo_width\":640,\"photo_height\":480,\"photo_tmstmp\":1424847027,\"blocked\":true,\"status\":2}
'
          ],
          'ListDoor' => [
            FUNCTION_NAME=>'ListDoor',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'DoorIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'_doorIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'15,24'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ListDoor']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'exporttask',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'evtexptotal',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Enumerate door information.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_doorIds="15,24"&api="SYNO.SurveillanceStation.AxisAcsCtrler"&version="1"&method="ListDoor"

Returns:
json:{"exporttask": [{"status": 0,"srcDsId": 0,"exported_count": 1,"start_time": 1425398400,"name": "asdf","dstdir": "test","total_count": 30,"dstDsId": 0,"id": 5,"stop_time": 1425484800}],"evtexptotal": 1}
'
          ],
          'ClearLog' => [
            FUNCTION_NAME=>'ClearLog',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'filterSource',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'filterTimeFrom',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1424836800],
                [VALUE_NAME=>'filterTimeTo',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'filterKeyword',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'doorIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'eventTypes',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'update',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ClearLog']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Delete controller logs in SurveillanceStation, this action will not delete logs in controller.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.AxisAcsCtrler"&filterTimeFrom=1424836800&version="1"&filterTimeTo=14_24880000&method="ClearLog"
'
          ],
          'ListPrivilege' => [
            FUNCTION_NAME=>'ListPrivilege',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ListPrivilege']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'data',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'List all SurveillanceStation user privilege for each door.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.AxisAcsCtrler"&version="1"&method="ListPrivilege"

Returns:
json:{"data": {"total": 4,"data": [{"id": "1026_231","manual_lock": true,"uid": 1026,"authorized": true,"door_id": 231}]}}
'
          ],
          'DoorControl' => [
            FUNCTION_NAME=>'DoorControl',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'doorId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Door ID'],
                [VALUE_NAME=>'operation',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Door lock operation'],
                [VALUE_NAME=>'_doorId',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>5],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'DoorControl_']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Manual lock operation for specific door.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_doorId=5&operation=1&api="SYNO.SurveillanceStation.AxisAcsCtrler"&version="1"&method="DoorControl_"
'
          ],
          'SavePrivilege' => [
            FUNCTION_NAME=>'SavePrivilege',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'arrayJson',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Door privilege settings Json format string'],
                [VALUE_NAME=>'_arrayJson',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'[{\"id\":\"1027_231\",\"uid\":1027,\"door_id\":231,\"authorized\":false,\"manual_lock\":false}_]'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SavePrivilege']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Save SurveillanceStation user\'s door privilege settings.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_arrayJson="[{\"id\":\"1027_231\",\"uid\":1027,\"door_id\":231,\"authorized\":false,\"manual_lock\":false}_]"&api="SYNO.SurveillanceStation.AxisAcsCtrler"&version="1"&method="SavePrivilege"
json:{\"id\":\"1027_231\",\"uid\":1027,\"door_id\":231,\"authorized\":false,\"manual_lock\":false}
'
          ],
          'ListLog' => [
            FUNCTION_NAME=>'ListLog',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'start',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'limit',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1],
                [VALUE_NAME=>'filterSource',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>91],
                [VALUE_NAME=>'filterTimeFrom',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1424016000],
                [VALUE_NAME=>'filterTimeTo',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>1424019600],
                [VALUE_NAME=>'filterKeyword',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'Jack'],
                [VALUE_NAME=>'timezoneOffset',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'doorIds',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'15,2_4'],
                [VALUE_NAME=>'eventTypes',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Optional',VALUE_DEFAULT=>'1,2,3,45'],
                [VALUE_NAME=>'update',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'blIncludeRecCnt',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'blIncludeAuInfo',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Optional'],
                [VALUE_NAME=>'_filterEventSource',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'filterEventSourceItem',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'225'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'filterType',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>1],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStati_on.AxisAcsCtrler'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'ListLog']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>'data',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'List all logs.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_filterEventSource=1&filterEventSourceItem="225"&version="1"&filterTimeTo=1424019600&doorIds="15,2_4"&filterTimeFrom=1424016000&filterType=1&filterKeyword="Jack"&start=0&api="SYNO.SurveillanceStati_on.AxisAcsCtrler"&limit=1&filterSource=91&eventTypes="1,2,3,45"&method="ListLog"

Returns:
json:{"data": {"total": 1,"data": [{"photo_width": 0,"src_idpt_type": 1,"owner_direct": 0,"owner_info": {},"photo_tmstmp": 0,"id": 7327,"photo_name": "Anonymous","owner_idpt_type": 0,"playback": false,"ctrler_id": 97,"src_type": 5,"photo_height": 0,"owner_id": 0,"owner_name": "Office hours (Example)","src_direct": 0,"tmstmp": 1424768400,"log_level": 2,"src_name": "ctrler","src_id": 97,"evt_type": 45,"evt_grp": 8,"owner_type": 3}]}}
'
          ],
          'Delete' => [
            FUNCTION_NAME=>'Delete',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ids',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Array string of select controller IDs'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Delete']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>0,VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Delete select controllers.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.AxisAcsCtrler"&ids="[91]"&version="1"&method="Delete"
json:[91]
'
          ],
          'Retrieve' => [
            FUNCTION_NAME=>'Retrieve',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ctrlerId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Controller ID for retrieve action'],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Retrieve']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'Result',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Retrieve data from controller, include cardholders, hardware configuration, logger configuration.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.AxisAcsCtrler"&ctrlerId=97&version="1"&method="Retrieve"
'
          ],
          'BlockCardHolder' => [
            FUNCTION_NAME=>'BlockCardHolder',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'arrayJson',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'String array of block'],
                [VALUE_NAME=>'_arrayJson',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'[576,577,578]'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'BlockCar_dHolder']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>0,VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>1,VALUE_TYPE=>DATATYPE_INT],
                [VALUE_NAME=>2,VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Blocked cardholders to suspend their credential in controller, these cardholders can\'t access the door until
unblocked.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_arrayJson="[576,577,578]"&api="SYNO.SurveillanceStation.AxisAcsCtrler"&version="1"&method="BlockCar_dHolder"
json:[576,577,578]
'
          ],
          'CountByCategory' => [
            FUNCTION_NAME=>'CountByCategory',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'_api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'CountByCategory']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'status',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'total',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Get controller count of all categories.

Example:
call:http://<syno_url>:5000/webapi/entry.cgi?_api="SYNO.SurveillanceStation.AxisAcsCtrler"&version="1"&method="CountByCategory"

Returns:
json:{"data": {"status": {"1": 0,"0": 1,"2": 0,"-1": 1,"4": 0},"total": 1}}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.AxisAcsCtrler.Search'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.AxisAcsCtrler.Search',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides several methods to access controller information, include enumerate/modify/delete/add
controller related members in SurveillanceStation, and some sync action to modify data in controller device.
Name Version Availability
SYNO.SurveillanceStation.AxisAcsCtrler 1 Surveillance Station 7.0',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed']],
        SERVICE_FUNCTIONS=>[
          'Start' => [
            FUNCTION_NAME=>'Start',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler.Search'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Start']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'pid',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Start controller search and keep searching for 60 seconds.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.AxisAcsCtrler.Search"&version="1"&method="Start"

Returns:
json:{"data": {"pid": 10017}}
'
          ],
          'InfoGet' => [
            FUNCTION_NAME=>'InfoGet',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'pid',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'offset',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Optional',VALUE_DEFAULT=>0],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'InfoGet'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.AxisAcsCtrler.Search'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ctrlers',VALUE_TYPE=>DATATYPE_ARRAY],
                [VALUE_NAME=>'alive',VALUE_TYPE=>DATATYPE_BOOL]
              ],
            ],
            FUNCTION_DESC=>'Get controller search result, pid is from Start request.

Example:
call:<syno_url>/webapi/entry.cgi?method="InfoGet"&api="SYNO.SurveillanceStation.AxisAcsCtrler.Search"&pid=10017&version="1"&offset=0

Returns:
json:{"data": {"ctrlers": [{"ip": "10.13.12.173","mac": "AC:CC:8E:09:C8:84","vendor": "AXIS","model": "AXIS A1001","port": 80}],"alive": true}}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
      'SYNO.SurveillanceStation.DigitalOutput'=> [
        SERVICE_NAME=>'SYNO.SurveillanceStation.DigitalOutput',
        SERVICE_CONNTYPE=>CONNECTION_TYPE_URL,
        SERVICE_ID=>'',
        SERVICE_CTRLURL=>'',
        SERVICE_DESC=>'This API provides start and get result method for controller upnp search.
Name Version Availability
SYNO.SurveillanceStation.AxisAcsCtrler.Search 1 Surveillance Station 7.0',
        SERVICE_EXPORT=>['ERROR_CODES' =>[400=>'Execution failed',401=>'Parameter invalid',402=>'Camera disabled']],
        SERVICE_FUNCTIONS=>[
          'Enum' => [
            FUNCTION_NAME=>'Enum',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.DigitalOutput'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Enum']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'list',VALUE_TYPE=>DATATYPE_ARRAY]
              ],
            ],
            FUNCTION_DESC=>'Enumerate digital output parameters. e.g. keep setting, normal state, current state and trigger button toggle
state.

Example:
call:<syno_url>/webapi/entry.cgi?api="SYNO.SurveillanceStation.DigitalOutput"&camId=280&version="1"&method="Enum"

Returns:
json:{"list": [{"trigger_state": true,"current_state": 1,"keep_setting": true,"idx": 0,"normal_state": 0}]}
'
          ],
          'Save' => [
            FUNCTION_NAME=>'Save',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'idx',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'keep_setting',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'normal_state',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'trigger_state',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Required'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.DigitalOutput'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'Save']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Save digital output parameters. e.g. keep setting, normal state and trigger state.

Example:
call:<syno_url>/webapi/entry.cgi?camId=280&trigger_state=false&version="1"&idx=0&keep_setting=true&api="SYNO.SurveillanceStation.DigitalOutput"&method="Save"&normal_state=1

Returns:
json:{"camId": 280}
'
          ],
          'PollState' => [
            FUNCTION_NAME=>'PollState',
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'camId',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The selected camera id'],
                [VALUE_NAME=>'idx',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'The selected digital output index'],
                [VALUE_NAME=>'keep',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Define the keep the camera original setting'],
                [VALUE_NAME=>'setNormalCap',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Define the setting camera normal state capability'],
                [VALUE_NAME=>'normal',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Define the camera normal state'],
                [VALUE_NAME=>'trigger',VALUE_TYPE=>DATATYPE_BOOL,VALUE_DESC=>'Define the trigger button toggle state'],
                [VALUE_NAME=>'timeOut',VALUE_TYPE=>DATATYPE_INT,VALUE_DESC=>'Define the max waiting time'],
                [VALUE_NAME=>'version',VALUE_TYPE=>DATATYPE_INT,VALUE_DEFAULT=>'1'],
                [VALUE_NAME=>'api',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'SYNO.SurveillanceStation.DigitalOutput'],
                [VALUE_NAME=>'method',VALUE_TYPE=>DATATYPE_STRING,VALUE_DEFAULT=>'PollState']
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'current_state',VALUE_TYPE=>DATATYPE_INT]
              ],
            ],
            FUNCTION_DESC=>'Long polling digital output status. e.g. current state.

Example:
call:<syno_url>/webapi/entry.cgi?version="1"&camId=280&timeOut=5000&idx=0&normal=1&api="SYNO.SurveillanceStation.DigitalOutput"&trigger=false&setNormalCap=true&method="PollState"&keep=true

Returns:
json:{"current_state": 0}
'
          ],
          'GetError' => [
            FUNCTION_NAME=>'GetError',
            FUNCTION_EXPORT=>['ERROR_CODES'],
            FUNCTION_PARAMS=>[
              PARAMS_IN=>[
                [VALUE_NAME=>'InstanceID',VALUE_TYPE=>DATATYPE_UINT,VALUE_DEFAULT=>0],
                [VALUE_NAME=>'ErrorCode',VALUE_TYPE=>DATATYPE_INT]
              ],
              PARAMS_OUT=> [
                [VALUE_NAME=>'ErrorMessage',VALUE_TYPE=>DATATYPE_STRING,VALUE_DESC=>'Error message']
              ],
            ],
            FUNCTION_DESC=>'Returns error message from given error code'
          ],
        ]
      ],
    ]
  ]
];
?>