// Place Holder Config File - User config vars and defines to be moved here
// Jimmy C

//////////////////////////////////////////////////////////////////////////////
// Configure bluetooth options
// ** Recommended to use HC-05 for now **
// If none are enabled, we'll use an abstract class that only reports 
// to the debug port what would  have been sent
//
#define USE_HC05_BLUETOOTH     1
#define USE_BLE_BLUETOOTH      0
#define USE_CLASSIC_BLUETOOTH  0

//////////////////////////////////////////////////////////////////////////////
// Configure WiFi options
//
// *** Presently ESP32 Bluetooth and WiFi do not co-exist well (ala don't work!) ***
//     HC-05 works OK with WiFi
//
#define USE_WIFI      1
#define USE_OTA       1
#define USE_WEBSERVER 1


///////////////////////////////////////////////////////////////////////////////
// limit rate of Bluetooth delivery from enthusiastic OEM controllers
//
#define OEM_TO_BLUETOOTH_MODERATION_TIME  700
// show when we did moderate data frames to bluetooth
#define REPORT_SUPPRESSED_OEM_DATA_FRAMES 0


///////////////////////////////////////////////////////////////////////////////
// debug reporting options
//
// true: each frame of data is reported on a new lines
// false: controller, then heater response frmaes are reported on a single line (excel CSV worthy!)
//
#define TERMINATE_OEM_LINE false    /* when an OEM controller exists */
#define TERMINATE_BTC_LINE false    /* when an OEM controller does not exist */

///////////////////////////////////////////////////////////////////////////////
// LED monitoring
//
//   1: enable specific LED function
//   0: disable specific LED function
//
#define RX_LED  1   /* flash when receiving blue wire data */
#define BT_LED  0   /* flash when sending bluetooth data */


///////////////////////////////////////////////////////////////////////////////
//  DS18B20 temperature sensing
//
#define TEMPERATURE_INTERVAL 1000
  

#define SUPPORT_OEM_CONTROLLER 1

///////////////////////////////////////////////////////////////////////////////
// SH1106 128x64 OLED support
//
// 0: I2C, 
// 1: HW SPI
//
#define OLED_HW_SPI 1
