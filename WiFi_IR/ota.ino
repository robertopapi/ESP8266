
//prepare OTA
void setup_ota()
{
  //set host name and passwort
  ArduinoOTA.setHostname("irwifi");
  ArduinoOTA.setPassword("irwifi");
  //register callback routines
  ArduinoOTA.onStart(ota_onStart);
  ArduinoOTA.onEnd(ota_onEnd);
  ArduinoOTA.onProgress(ota_onProgress);
  ArduinoOTA.onError(ota_onError);
  //start OTA handling
  ArduinoOTA.begin();
}

//on start decide between program or file upload
void ota_onStart()
{
 String type;
 if (ArduinoOTA.getCommand() == U_FLASH)
  type = "sketch";
 else // U_SPIFFS
  type = "filesystem";

  // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
}

//on end no extra processing
void ota_onEnd()
{
}

void ota_onProgress(unsigned int progress, unsigned int total)
{
// uint32_t prc = progress / (total / 100);
}

void ota_onError(ota_error_t error)
{
// String msg;
// 
// msg = "OTA Error[" + String(error) + "]: ";
// if (error == OTA_AUTH_ERROR)
//  msg = msg + "Auth Failed";
// else if (error == OTA_BEGIN_ERROR)
//  msg = msg + "Begin Failed";
// else if (error == OTA_CONNECT_ERROR)
//  msg = msg + "Connect Failed";
// else if (error == OTA_RECEIVE_ERROR)
//  msg = msg + "Receive Failed";
// else if (error == OTA_END_ERROR)
//  msg = msg + "End Failed";
// else
//  msg = msg + "Failed";
}
