/*
 * This file is part of the "bluetoothheater" distribution 
 * (https://gitlab.com/mrjones.id.au/bluetoothheater) 
 *
 * Copyright (C) 2018  James Clark
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 */

#include <Arduino.h>
#include "BTCWifi.h"
#include "BTCota.h"
#include "../cfg/BTCConfig.h"
#include "../../lib/esp32FOTA/src/esp32fota.h" // local copy used due to a couple of issues
#include "../Utility/helpers.h"
#include <SPIFFS.h>
#include <Update.h>
#include <ArduinoOTA.h>
#include "../Utility/MODBUS-CRC16.h"
#include "esp_ota_ops.h"

//#define TESTFOTA


bool CheckFirmwareCRC(int filesize);
bool CheckFirmwareCRC0(int filesize);
void onSuccess();
void onWebProgress(size_t progress, size_t total);

esp32FOTA FOTA("afterburner-fota-http", int(getVersion()*1000));
unsigned long FOTAtime = millis() + 60000;  // initial check in a minutes time 
int FOTAauth = 0;

#include <esp_int_wdt.h>
#include <esp_task_wdt.h>

void hard_restart() {
  esp_task_wdt_init(1,true);
  esp_task_wdt_add(NULL);
  while(true);
}

void initOTA(){
	ArduinoOTA.setHostname("AfterburnerOTA");

	ArduinoOTA
		.onStart([]() {
		String type;
		if (ArduinoOTA.getCommand() == U_FLASH)
			type = "sketch";
		else // U_SPIFFS
			type = "filesystem";

		// NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
	    SPIFFS.end();
		DebugPort.println("Start updating " + type);
    DebugPort.handle();    // keep telnet spy alive
    ShowOTAScreen();
#if USE_SW_WATCHDOG == 1
    feedWatchdog();   // we get stuck here for a while, don't let the watchdog bite!
#endif
	})
		.onEnd([]() {
		DebugPort.println("\nEnd");
    DebugPort.handle();    // keep telnet spy alive
    forceBootInit();
    delay(100);
//    DebugPort.end();       // force graceful close of telnetspy - ensures a client will reconnect cleanly
	})
		.onProgress([](unsigned int progress, unsigned int total) {
    feedWatchdog();
    int percent = (progress / (total / 100));
    static int prevPC = 0;
    if(percent != prevPC) {
      prevPC = percent;
		  DebugPort.printf("Progress: %u%%\r", percent);
      DebugPort.handle();    // keep telnet spy alive
      ShowOTAScreen(percent);
    }
	})
		.onError([](ota_error_t error) {
		DebugPort.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) DebugPort.println("Auth Failed");
		else if (error == OTA_BEGIN_ERROR) DebugPort.println("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) DebugPort.println("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) DebugPort.println("Receive Failed");
		else if (error == OTA_END_ERROR) DebugPort.println("End Failed");
    DebugPort.handle();    // keep telnet spy alive
	});

	ArduinoOTA.begin();
}

void DoOTA()
{
  ArduinoOTA.handle();

  // manage Firmware OTA
  // this is where the controller contacts a web server to discover if new firmware is available
  // if so, it can download and implant using OTA and become effective next reboot!
  long tDelta = millis() - FOTAtime;
  if(tDelta > 0) {  
//    FOTAtime = millis() + 6000;  // 6 seconds
//    FOTAtime = millis() + 60000;  // 60 seconds
//    FOTAtime = millis() + 600000;  // 10 minutes
    FOTAtime = millis() + 3600000;  // 1 hour
    if ((WiFi.status() == WL_CONNECTED)) {   // bug workaround in FOTA where execHTTPcheck does not return false in this condition
      FOTA.onProgress(onWebProgress);        // important - keeps watchdog fed
      FOTA.onComplete(CheckFirmwareCRC0);     // upload complete, but not yet verified
      FOTA.onSuccess(onSuccess);
#ifdef TESTFOTA
      FOTA.checkURL = "http://www.mrjones.id.au/afterburner/fota/fotatest.json";
#else
      FOTA.checkURL = "http://www.mrjones.id.au/afterburner/fota/fota.json";
#endif
      DebugPort.println("Checking for new firmware...");
      if(FOTA.execHTTPcheck()) {
        DebugPort.println("New firmware available on web server!");
        if(FOTAauth == 2) {  // user has authorised update (was == 1 before auth.)
          FOTA.execOTA();    // go ahead and do the update, reading new file from web server
          FOTAauth = 0;      // and we're done.
        }
        else 
          FOTAauth = 1;   // flag that new firmware is available
      }
      else {
        FOTAauth = 0;      // cancel
      }
    }
  }
};

int isUpdateAvailable(bool test)
{
  if(test) {
//    return FOTAauth == 1;
    if(FOTAauth == 1) {
      return FOTA.getNewVersion();
    }
    else {
      return 0;
    }
  }
  else
  {
    FOTAauth = 2;
    FOTAtime = millis();  // force immediate update test
    return true;
  }
  
}

void checkFOTA()
{
  FOTAtime = millis();
}

const int CRCbufsize = 1024;
uint8_t CRCReadBuff[CRCbufsize];   
/*
bool CheckFirmwareCRC(int filesize)
{
  const esp_partition_t* pUsePartition = esp_ota_get_next_update_partition(NULL);
  if(NULL == pUsePartition) {
    DebugPort.println("CheckCRC: FAILED - bad partition?");
    return false;
  }
  int size = (filesize >> 2) << 2;  // mod 4
  if((filesize - size) != 2) {
    // we expect 2 extra bytes where the custom CRC is added
    // all normal applications without CRC are multiples of 4
    DebugPort.println("CheckCRC: FAILED - bad source file size");
    return false;
  }

  CModBusCRC16 CRCengine;

  int processed = 0;
  while(processed < size) {
    int toRead = size - processed;
    if(toRead > CRCbufsize)
      toRead = CRCbufsize;

    ESP.flashRead(pUsePartition->address + processed, (uint32_t*)CRCReadBuff, toRead);
    CRCengine.process(toRead, CRCReadBuff); 
    processed += toRead;
  }
  ESP.flashRead(pUsePartition->address + processed, (uint32_t*)CRCReadBuff, 4);
  uint32_t fileCRC = CRCReadBuff[0] + (CRCReadBuff[1]<<8);

  DebugPort.printf("Upload CRC TEST: calcCRC= %04X, fileCRC = %08X\r\n", CRCengine.get(), fileCRC);

  return fileCRC == CRCengine.get();
}*/

// CRC of everything, including our extra CRC bytes should return ZERO :-)
bool CheckFirmwareCRC0(int filesize)
{
  const esp_partition_t* pUsePartition = esp_ota_get_next_update_partition(NULL);
  if(NULL == pUsePartition) {
    DebugPort.println("CheckCRC: FAILED - bad partition?");
    return false;
  }
  if((filesize & 0x3) != 2) {   // mod 4 == 2?
    // we expect 2 extra bytes where the custom CRC is added
    // all normal applications without CRC are multiples of 4
    DebugPort.println("CheckCRC: FAILED - bad source file size");
    return false;
  }

  CModBusCRC16 CRCengine;

  int processed = 0;
  while(processed < filesize) {
    int toRead = filesize - processed;
    if(toRead > CRCbufsize)
      toRead = CRCbufsize;

    ESP.flashRead(pUsePartition->address + processed, (uint32_t*)CRCReadBuff, toRead);
    CRCengine.process(toRead, CRCReadBuff); 
    processed += toRead;
  }

  bool retval = CRCengine.get() == 0;
  DebugPort.printf("CheckCRC: %s\r\n", retval ? "OK" : "FAILED");
  
  return retval;
}

void onSuccess() 
{
  forceBootInit();
}

void onWebProgress(size_t progress, size_t total)
{
  feedWatchdog();
  int percent = (progress / (total / 100));
  static int prevPC = 0;
  if(percent != prevPC) {
    prevPC = percent;
		DebugPort.printf("Progress: %u%%\r", percent);
    DebugPort.handle();    // keep telnet spy alive
    ShowOTAScreen(percent, eOTAWWW);
  }
}
