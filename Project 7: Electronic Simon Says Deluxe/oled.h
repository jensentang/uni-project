#ifndef ADAFRUIT_SSD1306_H
#define ADAFRUIT_SSD1306_H

#include "mbed.h"

class Adafruit_SSD1306_I2c {
public:
    Adafruit_SSD1306_I2c(I2C &i2c, PinName rst, uint8_t i2cAddress, uint8_t rawHeight = 64, uint8_t rawWidth = 128);
    
    void begin();
    void clearDisplay();
    void display();
    void setTextCursor(int16_t x, int16_t y);
    void setTextSize(uint8_t size);
    void printf(const char *format, ...);
    void drawPixel(int16_t x, int16_t y, bool color);
    void drawChar(int16_t x, int16_t y, unsigned char c, bool color, uint8_t size);

private:
    I2C &_i2c;
    uint8_t _addr;
    uint8_t _width;
    uint8_t _height;
    int16_t _cursorX;
    int16_t _cursorY;
    uint8_t _textSize;
    uint8_t _buffer[1024]; // 128x64 pixels = 1024 bytes

    void sendCommand(uint8_t c);
};

#endif