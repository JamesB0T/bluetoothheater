#include "ScreenManager.h"
#include <SPI.h>
#include <Wire.h>
#include "128x64OLED.h"
#include "pins.h"
#include "BluetoothAbstract.h" 
#include "Screen1.h"
#include "Screen2.h"
#include "Screen3.h"
#include "Screen4.h"
#include "Screen5.h"
#include "Screen6.h"

//
// **** NOTE: There are two very lame libaries conspiring to make life difficult ****
// A/ The ESP32 SPI.cpp library instatiates an instance of SPI, using the VSPI port (and pins)
// B/ The Adfruit_SH1106 library has hard coded "SPI" as the SPI port instance
//
// As an ESP32 has a pin multiplexer, this is very bad form.
// The design uses the defacto HSPI pins (and port), 
// You **MUST comment out the SPIClass SPI(VSPI);**  at the end of the ESP32 SPI library
// then we declare "SPI" here, which will use HSPI!!!!

// 128 x 64 OLED support
SPIClass SPI;    // default constructor opens HSPI on standard pins : MOSI=13,CLK=14,MISO=12(unused)

////////////////////////////////////////////////////////////////////////////////////////////////
// splash creen created using image2cpp http://javl.github.io/image2cpp/
//   Settings: 
//      Black background
//      Invert [X]
//      Arduino code, single bitmap
//      Identifier: DieselSplash
//      Draw Mode: Horizontal
//
const unsigned char DieselSplash [] PROGMEM = {
	// 'Splash2, 128x64px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x3e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x06, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 
	0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 
	0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 
	0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 
	0x00, 0x08, 0x00, 0x00, 0x00, 0x0f, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x08, 0x00, 0x00, 
	0x00, 0x10, 0x00, 0x00, 0x21, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x88, 0x00, 0x00, 
	0x00, 0x18, 0x00, 0x00, 0x20, 0x80, 0x00, 0x40, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x84, 0x00, 0x00, 
	0x00, 0x14, 0x00, 0x00, 0x20, 0x40, 0x00, 0x20, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x84, 0x00, 0x00, 
	0x00, 0x52, 0x00, 0x00, 0x40, 0x40, 0x00, 0x10, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x84, 0x00, 0x00, 
	0x00, 0x34, 0x00, 0x00, 0x40, 0x20, 0x00, 0x08, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x82, 0x00, 0x00, 
	0x00, 0x18, 0x00, 0x00, 0x40, 0x10, 0x00, 0x04, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 
	0x00, 0x34, 0x00, 0x00, 0x80, 0x10, 0x00, 0x02, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 
	0x00, 0x52, 0x00, 0x00, 0x80, 0x08, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 
	0x00, 0x14, 0x00, 0x00, 0x80, 0x08, 0x01, 0xf1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 
	0x00, 0x18, 0x00, 0x01, 0x00, 0x04, 0x3e, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 
	0x00, 0x10, 0x00, 0x01, 0x00, 0x07, 0xc0, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x80, 0x1c, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x03, 0xc4, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x18, 0x40, 0x64, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x3c, 0x78, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x3c, 0x40, 0x84, 0x00, 0x00, 0x01, 0x80, 0x00, 0x01, 0xc0, 0x40, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x26, 0x23, 0x04, 0x00, 0x00, 0x00, 0x60, 0x00, 0x1e, 0x00, 0x40, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x43, 0x1c, 0x02, 0x00, 0x00, 0x00, 0x1c, 0x01, 0xe0, 0x00, 0x40, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x41, 0xf8, 0x02, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x40, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x41, 0x88, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x40, 0x88, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x40, 0x88, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x40, 0x88, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x60, 0x88, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x30, 0x88, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x10, 0x88, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x19, 0x88, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x0f, 0x88, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x07, 0xf8, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x07, 0xc1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xc2, 0x00, 0x00, 0xf8, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x1f, 0x20, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x03, 0xe8, 0x20, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x7c, 0x08, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x08, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x04, 0x00, 0x00, 0x00, 0x00, 0x21, 0x10, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x0a, 0x08, 0x00, 0x00, 0x00, 0x21, 0x10, 0x01, 0x00, 0x00, 0x84, 0x00, 0x40, 0x01, 0x40, 0x00, 
	0x11, 0x08, 0x00, 0x00, 0x00, 0x21, 0x10, 0x01, 0x00, 0x00, 0x80, 0x00, 0x40, 0x01, 0x40, 0x00, 
	0x11, 0x3a, 0x4e, 0x73, 0x18, 0xe1, 0x13, 0x1d, 0xcc, 0xb0, 0x80, 0xc7, 0x75, 0x99, 0x4c, 0xb0, 
	0x1f, 0x4a, 0x52, 0x94, 0xa5, 0x21, 0xf4, 0xa5, 0x12, 0xc0, 0x81, 0x29, 0x46, 0x25, 0x52, 0xc0, 
	0x11, 0x4a, 0x52, 0x94, 0x3d, 0x21, 0x17, 0xa5, 0x1e, 0x80, 0x81, 0x29, 0x44, 0x25, 0x5e, 0x80, 
	0x11, 0x49, 0x92, 0x94, 0xa1, 0x21, 0x14, 0x25, 0x10, 0x80, 0x85, 0x29, 0x44, 0x25, 0x50, 0x80, 
	0x11, 0x39, 0x8e, 0x93, 0x18, 0xe1, 0x13, 0x1c, 0xcc, 0x80, 0x78, 0xc9, 0x34, 0x19, 0x4c, 0x80
};


CScreenManager::CScreenManager() 
{
  _pDisplay = NULL;
  _pActiveScreen = NULL;
  for(int i = 0; i < _maxScreens; i++)
    _pScreen[i] = NULL;
  _currentScreen = 1;
  _bReqUpdate = false;
}

CScreenManager::~CScreenManager()
{
  _pActiveScreen = NULL;      
  for(int i=0; i<_maxScreens; i++) {
    if(_pScreen[i]) {
      delete _pScreen[i]; _pScreen[i] = NULL;
    }
  }
  if(_pDisplay) {
    delete _pDisplay; _pDisplay = NULL;
  }
}

void 
CScreenManager::init()
{

  // 128 x 64 OLED support (I2C)
  // SH1106_SWITCHCAPVCC = generate display voltage from 3.3V internally
  _pDisplay = new C128x64_OLED(OLED_SDA_pin, OLED_SCL_pin);
  _pDisplay->begin(SH1106_SWITCHCAPVCC);
  Wire.begin(OLED_SDA_pin, OLED_SCL_pin, 800000);   // speed up I2C from the default crappy 100kHz set via the adafruit begin!

  // replace adafruit splash screen
  _pDisplay->clearDisplay();
  _pDisplay->drawBitmap(0, 0, DieselSplash, 128, 64, WHITE);

  // Show initial display buffer contents on the screen --
  _pDisplay->display();

  _pScreen[0] = new CScreen1(*_pDisplay, *this);
  _pScreen[1] = new CScreen2(*_pDisplay, *this);
  _pScreen[2] = new CScreen3(*_pDisplay, *this);
  _pScreen[3] = new CScreen4(*_pDisplay, *this);
  _pScreen[4] = new CScreen5(*_pDisplay, *this);
	_pScreen[5] = new CScreen6(*_pDisplay, *this);

  _switchScreen();
}

bool 
CScreenManager::checkUpdate()
{
  if(_bReqUpdate) {
    if(_pActiveScreen) {
      _pActiveScreen->show();
      _bReqUpdate = false;
			return true;
    }
  }
  return false;
}
  
void 
CScreenManager::reqUpdate()
{
  _bReqUpdate = true;
}

bool 
CScreenManager::animate()
{
  if(_pActiveScreen) 
	  return _pActiveScreen->animate();
	return false;
}

void
CScreenManager::refresh()
{
	if(_pDisplay)
		_pDisplay->display();
}

void 
CScreenManager::_switchScreen()
{
  if(_currentScreen < _maxScreens)
    _pActiveScreen = _pScreen[_currentScreen]; 
  
  //        reqDisplayUpdate();
  reqUpdate();
}

void 
CScreenManager::nextScreen()
{
  _currentScreen++;
  if(_currentScreen >= _maxScreens) {
    _currentScreen = 0;
  }
  _switchScreen();
}

void 
CScreenManager::prevScreen()
{
  _currentScreen--;
  if(_currentScreen < 0) {
    _currentScreen = _maxScreens-1;
  }
  _switchScreen();
}

void 
CScreenManager::keyHandler(uint8_t event)
{
  if(_pActiveScreen) _pActiveScreen->keyHandler(event);
}


