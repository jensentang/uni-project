#include "LCD_I2C.h"
#include <cstdarg>
#include <cstdio>

// Bit masks for HD44780 / PCF8574 interface
#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE    0x04
#define LCD_RS        0x01


LCD_I2C::LCD_I2C(I2C *i2cBus, uint8_t deviceAddress)
    : _i2c(i2cBus), _addr(deviceAddress) {}

void LCD_I2C::sendNibble(uint8_t nibble, uint8_t mode) {
    uint8_t data = (nibble << 4) | mode | LCD_BACKLIGHT;
    
    char buf[2];
    buf[0] = data | LCD_ENABLE;  // Clock Enable HIGH
    buf[1] = data & ~LCD_ENABLE; // Clock Enable LOW
    
    _i2c->write(_addr, buf, 2);
}

void LCD_I2C::sendByte(uint8_t value, uint8_t mode) {
    sendNibble(value >> 4, mode);
    sendNibble(value & 0x0F, mode);
    ThisThread::sleep_for(2ms);
}

void LCD_I2C::init() {
    ThisThread::sleep_for(50ms);
    
    // HD44780 initialization sequence into 4-bit mode
    sendNibble(0x03, 0);
    ThisThread::sleep_for(5ms);
    sendNibble(0x03, 0);
    ThisThread::sleep_for(1ms);
    sendNibble(0x03, 0);
    sendNibble(0x02, 0);

    // Configuration commands
    sendByte(0x28, 0); // Function set: 4-bit, 2-line/4-line, 5x8 dots
    sendByte(0x0C, 0); // Display control: Display ON, Cursor OFF
    sendByte(0x06, 0); // Entry mode: Increment automatically
    cls();
}

void LCD_I2C::cls() {
    sendByte(0x01, 0); // Clear display
    ThisThread::sleep_for(2ms);
}

void LCD_I2C::locate(uint8_t col, uint8_t row) {
    static const uint8_t rowOffsets[] = {0x00, 0x40, 0x14, 0x54};
    if (row > 3) row = 3;
    sendByte(0x80 | (col + rowOffsets[row]), 0);
}

void LCD_I2C::printf(const char *format, ...) {
    char buffer[64];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    const char *ptr = buffer;
    while (*ptr) {
        sendByte((uint8_t)(*ptr++), LCD_RS);
    }
}