#ifndef _ADAFRUIT_GFX_CONFIG_H_
#define _ADAFRUIT_GFX_CONFIG_H_

// Uncomment this to turn off the builtin splash
//#define NO_SPLASH_ADAFRUIT

// Uncomment this to enable all functionality
//#define GFX_WANT_ABSTRACTS

// Uncomment this to enable only runtime font scaling, without all the rest of the Abstracts
//#define GFX_SIZEABLE_TEXT
#define PinName int

class SPI {
public:
    void write(uint8_t data) {};
};

class I2C {
public:
    void write(uint8_t addr, char *buff, uint32_t size) {};
};

#endif
