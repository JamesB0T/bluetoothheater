/*
 * This file is part of the "bluetoothheater" distribution 
 * (https://gitlab.com/mrjones.id.au/bluetoothheater) 
 *
 * Copyright (C) 2018  Ray Jones <ray@mrjones.id.au>
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

#ifndef __BTC_NV_STORAGE_H__
#define __BTC_NV_STORAGE_H__

#include "GPIO.h"
#include "NVCore.h"

#include "../RTC/Timers.h"   // for sTimer


struct sHeater : public CESP32_NVStorage {
  uint8_t   Pmin;
  uint8_t   Pmax;
  uint16_t  Fmin;
  uint16_t  Fmax;
  uint8_t   ThermostatMode;
  uint8_t   setTemperature;
  uint8_t   sysVoltage;
  uint8_t   fanSensor;
  uint8_t   glowDrive;

  bool valid() {
    bool retval = true;
    retval &= Pmin < 100;
    retval &= Pmax < 150;
    retval &= Fmin < 5000;
    retval &= Fmax < 6000;
    retval &= ThermostatMode < 2;
    retval &= setTemperature < 40;
    retval &= sysVoltage == 120 || sysVoltage == 240;
    retval &= fanSensor == 1 || fanSensor == 2;
    retval &= glowDrive >= 1 && glowDrive <= 6;
    return retval;
  };
  void init() {
    Pmin = 14;
    Pmax = 45;
    Fmin = 1500;
    Fmax = 4500;
    ThermostatMode = 1;
    setTemperature = 23;
    sysVoltage = 120;
    fanSensor = 1;
    glowDrive = 5;
  };
  void load();
  void save();
};

struct sHomeMenuActions {
  uint8_t onTimeout;
  uint8_t onStart;
  uint8_t onStop;
  bool valid() {
    bool retval = true;
    retval &= onTimeout < 4;
    retval &= onStart < 4;
    retval &= onStop < 4;
    return retval;  
  }
  void init() {
    onTimeout = 0;
    onStart = 0;
    onStop = 0;
  }
};

struct sCyclicThermostat {
  int8_t Stop;
  int8_t Start;
  bool valid() {
    bool retval = true;
    retval &= Start >= -10 && Start <= 0;
    retval &= Stop >= 0 && Stop <= 10;
    return retval;  
  }
  void init() {
    Start = -1;
    Stop = 0;
  }
  bool isEnabled() const {
    return Stop != 0;
  }
  sCyclicThermostat& operator=(const sCyclicThermostat& rhs) {
    Stop = rhs.Stop;
    Start = rhs.Start;
    return *this;
  }
};

struct sCredentials : public CESP32_NVStorage {
  char SSID[32];
  char APpassword[32];
  char webUpdateUsername[32];
  char webUpdatePassword[32];
  void init() {
    strcpy(SSID, "Afterburner");
    strcpy(APpassword, "thereisnospoon");
    strcpy(webUpdateUsername, "Afterburner");
    strcpy(webUpdatePassword, "BurnBabyBurn");
  };
  void load();
  void save();
  bool valid();
};

struct sMQTTparams : public CESP32_NVStorage {
  uint8_t enabled;
  uint16_t  port;
  char host[128];
  char username[32];
  char password[32];
  void init() {
    enabled = false;
    port = 1234;
    memset(host, 0, 128);
    memset(username, 0, 32);
    memset(password, 0, 32);
  }
  sMQTTparams& operator=(const sMQTTparams& rhs) {
    enabled = rhs.enabled;
    port = rhs.port;
    memcpy(host, rhs.host, 128);
    memcpy(username, rhs.username, 32);
    memcpy(password, rhs.password, 32);
    host[127] = 0;
    username[31] = 0;
    password[31] = 0;
  }
  void load();
  void save();
  bool valid();
};

struct sBTCoptions : public CESP32_NVStorage {
  long dimTime;
  long menuTimeout;
  uint8_t degF;
  uint8_t ThermostatMethod;  // 0: standard heater, 1: Narrow Hysterisis, 2:Managed Hz mode
  uint8_t enableWifi;
  uint8_t enableOTA;
  uint8_t GPIOinMode;
  uint8_t GPIOoutMode;
  uint8_t GPIOalgMode;
  uint16_t FrameRate;
  sCyclicThermostat cyclic;
  sHomeMenuActions HomeMenu;

  bool valid() {
    bool retval = true;
    retval &= (dimTime >= -600000) && (dimTime < 600000);  // +/- 10 mins
    retval &= (menuTimeout >= 0) && (menuTimeout < 300000);  // 5 mins
    retval &= (degF == 0) || (degF == 1);
    retval &= (ThermostatMethod & 0x03) < 3;  // only modes 0, 1 or 2
    retval &= (enableWifi == 0) || (enableWifi == 1);
    retval &= (enableOTA == 0) || (enableOTA == 1);
    retval &= GPIOinMode < 4;
    retval &= GPIOoutMode < 3;
    retval &= (FrameRate >= 300) && (FrameRate <= 1500);
    retval &= cyclic.valid();
    retval &= HomeMenu.valid();
    return retval;  
  }
  void init() {
    dimTime = 60000;
    menuTimeout = 60000;
    degF = 0;
    ThermostatMethod = 0;
    enableWifi = 1;
    enableOTA = 1;
    GPIOinMode = 0;
    GPIOoutMode = 0;
    GPIOalgMode = 0;
    FrameRate = 1000;
    cyclic.init();
    HomeMenu.init();
  };
  void load();
  void save();
};


// the actual data stored to NV memory
struct sNVStore {
  sHeater Heater;
  sBTCoptions Options;
  sTimer timer[14];
  sMQTTparams MQTT;
  sCredentials Credentials;
  bool valid();
  void init();
};


class CHeaterStorage /*: public CESP32_NVStorage*/ {
protected:
  sNVStore _calValues;
public:
    CHeaterStorage();
    virtual ~CHeaterStorage() {};

  // TODO: These are only here to allow building without fully 
  // fleshing out NV storage for Due, Mega etc
  // these should be removed once complete (pure virtual)
  void init() {};
  void load() {};
  void save() {};


    float getPmin();
    float getPmax();
    unsigned short getFmin();
    unsigned short getFmax();
    unsigned char getDesiredTemperature();
    unsigned char getThermostatMode();
    unsigned char getThermostatMethodMode();
    float         getThermostatMethodWindow();
    unsigned char getSysVoltage();
    unsigned char getFanSensor();
    unsigned char getGlowDrive();
    long getDimTime();
    long getMenuTimeout();
    unsigned char getDegFMode();
    unsigned char getWifiEnabled();
    unsigned char getOTAEnabled();
    const sCyclicThermostat& getCyclicMode() const;
    const sMQTTparams& getMQTTinfo() const;
    GPIOinModes getGPIOinMode();
    GPIOoutModes getGPIOoutMode();
    GPIOalgModes getGPIOalgMode();
    uint16_t     getFrameRate();
    const sHomeMenuActions& getHomeMenu() const;
    const sCredentials& getCredentials() const;

    void setPmin(float);
    void setPmax(float);
    void setFmin(unsigned short val);
    void setFmax(unsigned short val);
    void setDesiredTemperature(unsigned char val);
    void setThermostatMode(unsigned char val);
    void setThermostatMethodMode(unsigned char val);
    void setThermostatMethodWindow(float val);
    void setSystemVoltage(float fVal);
    void setFanSensor(unsigned char val);
    void setGlowDrive(unsigned char val);
    void setDimTime(long val);
    void setMenuTimeout(long val);
    void setDegFMode(unsigned char val);
    void setWifiEnabled(unsigned char val);
    void setOTAEnabled(unsigned char val);
    void setCyclicMode(const sCyclicThermostat& val);
    void setGPIOinMode(unsigned char val);
    void setGPIOoutMode(unsigned char val);
    void setGPIOalgMode(unsigned char val);
    void setFrameRate(uint16_t val);
    void setHomeMenu(sHomeMenuActions val);

    void getTimerInfo(int idx, sTimer& timerInfo);
    void setTimerInfo(int idx, const sTimer& timerInfo);
    void setMQTTinfo(const sMQTTparams& info);
    void setCredentials(const sCredentials& info);
};



class CESP32HeaterStorage : public CHeaterStorage {
public:
  CESP32HeaterStorage();
  virtual ~CESP32HeaterStorage();
  void init();
  void load();
  void save();
};

extern CHeaterStorage& NVstore;

#endif // __BTC_NV_STORAGE_H__

