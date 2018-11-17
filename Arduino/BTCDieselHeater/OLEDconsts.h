
// 'Thermometer', 8x50px
const unsigned char thermometerBitmap [] PROGMEM = {
	0x00, 0x18, 0x24, 0x24, 0x24, 0x24, 0x24, 0x26, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 
	0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x26, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 
	0x24, 0x24, 0x24, 0x24, 0x24, 0x26, 0x24, 0x24, 0x24, 0x24, 0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 
	0x7e, 0x3c
};

// 'ThermoPtr', 3x5px
const unsigned char thermoPtr [] PROGMEM = {
	0x80, 0xc0, 0xe0, 0xc0, 0x80
};

// 'Bluetooth icon', 6x11px
const unsigned char BTicon [] PROGMEM = {
	0x20, 0x30, 0x28, 0xa4, 0x68, 0x30, 0x68, 0xa4, 0x28, 0x30, 0x20,
};

// 'BatteryIcon', 15x10px
const unsigned char BatteryIcon [] PROGMEM = {
	0x30, 0x18, 0xff, 0xfe, 0x80, 0x02, 0xb6, 0xda, 0xb6, 0xda, 0xb6, 0xda, 0xb6, 0xda, 0xb6, 0xda, 
	0x80, 0x02, 0xff, 0xfe
};

// 'GlowPlugIcon', 16x10px
const unsigned char GlowPlugIcon [] PROGMEM = {
	0x00, 0x00, 0xe3, 0x8e, 0x1c, 0x70, 0x28, 0x28, 0x24, 0x48, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 
	0x14, 0x50, 0x1c, 0x70
};

// 'Fan3_1a', 16x16px
const unsigned char FanIcon1 [] PROGMEM = {
	0x03, 0xc0, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x03, 0xc0, 0x07, 0xe0, 0x06, 0x60, 
	0x7e, 0x7e, 0x87, 0xe1, 0x87, 0xe1, 0x84, 0x21, 0x84, 0x21, 0x78, 0x1e, 0x00, 0x00, 0x00, 0x00
};
// 'Fan3_2a', 16x16px
const unsigned char FanIcon2 [] PROGMEM = {
	0x00, 0x78, 0x00, 0x84, 0x00, 0x84, 0x00, 0x84, 0x00, 0x84, 0x7b, 0xf8, 0x87, 0xe0, 0x86, 0x60, 
	0x86, 0x60, 0x87, 0xe0, 0x7b, 0xf8, 0x00, 0x84, 0x00, 0x84, 0x00, 0x84, 0x00, 0x84, 0x00, 0x78
};
// 'Fan3_3a', 16x16px
const unsigned char FanIcon3 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x78, 0x1e, 0x84, 0x21, 0x84, 0x21, 0x87, 0xe1, 0x87, 0xe1, 0x7e, 0x7e, 
	0x06, 0x60, 0x07, 0xe0, 0x03, 0xc0, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x03, 0xc0
};
// 'Fan3_4a', 16x16px
const unsigned char FanIcon4 [] PROGMEM = {
	0x1e, 0x00, 0x21, 0x00, 0x21, 0x00, 0x21, 0x00, 0x21, 0x00, 0x1f, 0xde, 0x07, 0xe1, 0x06, 0x61, 
	0x06, 0x61, 0x07, 0xe1, 0x1f, 0xde, 0x21, 0x00, 0x21, 0x00, 0x21, 0x00, 0x21, 0x00, 0x1e, 0x00
};


// 'FuelIcon', 7x12px
const unsigned char FuelIcon [] PROGMEM = {
	0x10, 0x10, 0x38, 0x38, 0x7c, 0x7c, 0xfe, 0xfe, 0xfe, 0xfe, 0x7c, 0x38
};

// 'Target', 13x13px
const unsigned char TargetIcon [] PROGMEM = {
  0x0f, 0x80, 0x10, 0x40, 0x20, 0x20, 0x47, 0x10, 0x88, 0x88, 0x92, 0x48, 0x97, 0x48, 0x92, 0x48,
  0x88, 0x88, 0x47, 0x10, 0x20, 0x20, 0x10, 0x40, 0x0f, 0x80
};

// '0', 3x5px
const unsigned char Mini0 [] PROGMEM = {
  0xe0, 0xb0, 0xb0, 0xb0, 0xe0
};

// '1', 3x5px
const unsigned char Mini1 [] PROGMEM = {
  0x40, 0xc0, 0x40, 0x40, 0x40
};

// '2', 3x5px
const unsigned char Mini2 [] PROGMEM = {
  0xe0, 0x20, 0xe0, 0x80, 0xe0
};

// '3', 3x5px
const unsigned char Mini3 [] PROGMEM = {
  0xe0, 0x20, 0x60, 0x20, 0xe0
};

// '4', 3x5px
const unsigned char Mini4 [] PROGMEM = {
  0x80, 0xb0, 0xe0, 0x20, 0x20
};

// '5', 3x5px
const unsigned char Mini5 [] PROGMEM = {
  0xe0, 0x80, 0xe0, 0x20, 0xe0
};

// '6', 3x5px
const unsigned char Mini6 [] PROGMEM = {
  0xe0, 0x80, 0xe0, 0xb0, 0xe0
};

// '7', 3x5px
const unsigned char Mini7 [] PROGMEM = {
  0xe0, 0x20, 0x20, 0x20, 0x20
};

// '8', 3x5px
const unsigned char Mini8 [] PROGMEM = {
  0xe0, 0xb0, 0xe0, 0xb0, 0xe0
};

// '9', 3x5px
const unsigned char Mini9 [] PROGMEM = {
  0xe0, 0xb0, 0xe0, 0x20, 0xe0
};

// 'DP', 3x5px
const unsigned char MiniDP [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x40
};

// 'Deg', 3x5px
const unsigned char MiniDeg [] PROGMEM = {
  0xc0, 0xc0, 0x00, 0x00, 0x00
};

// 'C', 3x5px
const unsigned char MiniC [] PROGMEM = {
  0xe0, 0x80, 0x80, 0x80, 0xe0
};

// 'H', 3x5px
const unsigned char MiniH [] PROGMEM = {
  0xb0, 0xb0, 0xe0, 0xb0, 0xb0
};

// 'z', 3x5px
const unsigned char Miniz [] PROGMEM = {
  0x00, 0x00, 0xe0, 0x40, 0xe0
};

// ' ', 3x5px
const unsigned char MiniSpc [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00
};

const char* Runstates [] PROGMEM = {
  " Stopped/Ready ",
  "Starting...",
  "Igniting...",
  " Ignition retry ",
  "Ignited",
  "Running",
  "Stopping",
  "Shutdown",
  "Cooling"
};

const char* Errstates [] PROGMEM = {
  "Low voltage",     // E-01
  "High voltage",    // E-02
  "Glow plug fault", // E-03
  "Pump fault",      // E-04
  "Overheat",        // E-05
  "Motor fault",     // E-06
  "Comms fault",     // E-07
  "Flame out",       // E-08
  "Temp sense",      // E-09
  "Ignition fail",   // E-10
  "Unknown error?"   // mystery code!
};
