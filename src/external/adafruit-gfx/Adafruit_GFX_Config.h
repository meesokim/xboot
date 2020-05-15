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

#include <i2c/i2c.h>

class I2C {
    struct i2c_device_t * dev = 0;
public:
    I2C()
    {
        struct i2c_t * i2c = search_i2c("i2c-f1c100s.0");
        if (i2c)
        {
            dev = (i2c_device_t*)malloc(sizeof(struct i2c_device_t));
            dev->i2c = i2c;
            dev->flags = 0;
        }
    }
    void write(uint8_t addr, char *buff, uint32_t size) {
        if (dev)
        {
#if 0            
        	struct i2c_msg_t msg;
            msg.addr = addr;
            msg.flags = dev->flags & I2C_M_TEN;
            msg.len = size;
            i2c_transfer(dev->i2c, &msg, 1);
#else
            dev->addr = addr;
            i2c_master_send(dev, buff, size);
#endif
#if 0
            printf("%02x:", addr);
            for(int i = 0; i < size; i++)
                printf("%02x,", buff[i]);
            printf("\r\n");
#endif
        }
    };
};

#endif
