#ifndef LCD_I2C_H
#define LCD_I2C_H

#include "mbed.h"

// Hardware address constants (shifted left for 8-bit mbed API)
constexpr uint8_t LCD_ADDR_27 = 0x27 << 1; 
constexpr uint8_t LCD_ADDR_3F = 0x3F << 1; 

class LCD_I2C {
public:
    LCD_I2C(I2C *i2cBus, uint8_t deviceAddress = LCD_ADDR_27);
    
    void init();
    void cls();
    void locate(uint8_t col, uint8_t row);
    void printf(const char *format, ...);

private:
    I2C *_i2c;
    uint8_t _addr;

    void sendByte(uint8_t value, uint8_t mode);
    void sendNibble(uint8_t nibble, uint8_t mode);
};



#endif // LCD_I2C_H