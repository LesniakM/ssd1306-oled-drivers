#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>
#include <stdbool.h>
#include "ssd1306_font_5x8.h"

#define SSD1306_HEIGHT 32
#define SSD1306_WIDTH 128

typedef struct {
    const struct i2c_dt_spec* i2c_spec;
    uint8_t* buffer;
} ssd1306_device;

void ssd1306_device_init(ssd1306_device* dev, const struct i2c_dt_spec* i2c_spec);
uint8_t ssd1306_init(ssd1306_device* dev);
uint8_t ssd1306_drawPixel(ssd1306_device* dev, uint8_t x, uint8_t y, bool color);
uint8_t ssd1306_drawPixelToBuff(uint8_t *buffer, uint8_t x, uint8_t y, bool color);
//void writeChar(unsigned char ch, uint8_t start_x, uint8_t row, const fontData font[][5]);
//void writeCharToBuff(unsigned char ch, uint8_t start_x, uint8_t row, const fontData font[][5]);
//void writeStringToBuff(string s, uint8_t start_x, uint8_t row, const fontData font[][5]);
uint8_t ssd1306_updateScreen(ssd1306_device* dev);
//void updateScreenDMA();


uint8_t ssd1306_writeCommand(const struct i2c_dt_spec *dev_i2c, uint8_t command);
uint8_t ssd1306_writeData(const struct i2c_dt_spec * dev_i2c, uint8_t* data, uint16_t data_len);
//uint8_t _writeDataDMA(uint8_t* data, uint16_t data_len);
uint8_t ssd1306_updatePage(ssd1306_device* dev, uint8_t x, uint8_t p_index);
//void _updateChar(uint8_t start_x, uint8_t row, uint8_t char_width);


#endif