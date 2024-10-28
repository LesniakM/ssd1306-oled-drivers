#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <stdbool.h>
#include <stdint.h>
#include "ssd1306.h"

bool x_reverse = false;
bool y_reverse = false;
uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8] = {};

void ssd1306_device_init(ssd1306_device* dev, const struct i2c_dt_spec* i2c_spec) {
    dev->buffer = SSD1306_Buffer;
    dev->i2c_spec = i2c_spec;
    // Additional setup could go here
}


  // Init LCD
uint8_t ssd1306_init(ssd1306_device* dev) {
  k_msleep(100);
  uint8_t errors = 0;

  errors += ssd1306_writeCommand(dev->i2c_spec, 0xA8);   // Set MUX ratio:
  errors += ssd1306_writeCommand(dev->i2c_spec, 0x1F);   // 0x1F (31) for 128x32, 0x3F (63) for 128x64.

  errors += ssd1306_writeCommand(dev->i2c_spec, 0xD3);   // Set display offset:
  errors += ssd1306_writeCommand(dev->i2c_spec, 0x00);   // No offset.

  errors += ssd1306_writeCommand(dev->i2c_spec, 0x40);   // Set Display Start Line (0x40~0x7F).

  errors += ssd1306_writeCommand(dev->i2c_spec, 0xA1);   // Set Segment Re-map (A0h/A1h). Set to A0 to flip horizontally.
  errors += ssd1306_writeCommand(dev->i2c_spec, 0xC8);   // Set COM Output Scan Direction (Scan direction: from COM0 to COM63, 0xC8 for reversed).

  errors += ssd1306_writeCommand(dev->i2c_spec, 0xDA);   // Set COM Pins Hardware Configuration to:
  errors += ssd1306_writeCommand(dev->i2c_spec, 0x02);   // 0x02 for 128x32, 0x12 for 128x64. 

  errors += ssd1306_writeCommand(dev->i2c_spec, 0x81);   // Set contrast Control for BANK0 (81h) to:
  errors += ssd1306_writeCommand(dev->i2c_spec, 0x20);   // 0x7F.

  errors += ssd1306_writeCommand(dev->i2c_spec, 0xA4);   // Get pixel info from RAM.

  errors += ssd1306_writeCommand(dev->i2c_spec, 0xA6);   // Normal display mode (0 in RAM: OFF, 1 in RAM: ON), 0xA6 for reversed.

  errors += ssd1306_writeCommand(dev->i2c_spec, 0xD5);   // Set Display Clock Divide Ratio/ Oscillator Frequency to:
  errors += ssd1306_writeCommand(dev->i2c_spec, 0xF0);   // F - max freq, 0 - min divider. 

  errors += ssd1306_writeCommand(dev->i2c_spec, 0x8D);   // Set charge pump to:
  errors += ssd1306_writeCommand(dev->i2c_spec, 0x14);   // 0x14 - enabled.

  errors += ssd1306_writeCommand(dev->i2c_spec, 0x20);   // Set addressing mode to:
  errors += ssd1306_writeCommand(dev->i2c_spec, 0x00);   // Horizontal mode. 0x10 for Page mode.

  errors += ssd1306_writeCommand(dev->i2c_spec, 0x21);   // Set min and max column
  errors += ssd1306_writeCommand(dev->i2c_spec, 0x00);   // Horizontal mode. 0x10 for Page mode.
  errors += ssd1306_writeCommand(dev->i2c_spec, 127);   // Horizontal mode. 0x10 for Page mode.

  errors += ssd1306_writeCommand(dev->i2c_spec, 0xAF);   // Enable display.

  return errors;
}


uint8_t ssd1306_drawPixelToBuff(uint8_t *buffer, uint8_t x, uint8_t y, bool color)
{   
  
  if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
  {
    return 99;
  }

  if (color)
  {
    buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
  }
  else
  {
    buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
  }
  return 0;
}

uint8_t ssd1306_drawPixel(ssd1306_device* dev, uint8_t x, uint8_t y, bool color)
{ 
  uint8_t errors = 0;
  errors += ssd1306_drawPixelToBuff(dev->buffer, x, y, color);
  errors += ssd1306_updatePage(dev, x, y / 8);
  return errors;
}

/*
void ssd1306_writeChar(unsigned char ch, uint8_t start_x, uint8_t row, const fontData font[][5])
{ 
  uint8_t font_width = 5;
  for (int i = 0; i < font_width; i++) 
  {
    SSD1306_Buffer[start_x + row * SSD1306_WIDTH + i] = font[ch][i];
  }
  
  ssd1306_updateChar(start_x, row, font_width);
}


void ssd1306_writeCharToBuff(unsigned char ch, uint8_t start_x, uint8_t row, const fontData font[][5])
{ 
  uint8_t font_width = 5;
  for (int i = 0; i < font_width; i++) 
  {
    SSD1306_Buffer[start_x + row * SSD1306_WIDTH + i] = font[ch][i];
  }
}

void ssd1306_writeStringToBuff(string s, uint8_t start_x, uint8_t row, const fontData font[][5])
{ 
  uint8_t font_width_with_spaces = 6;
  uint8_t chars_printed = 0;
  for (char character : s)
  { 
    if (start_x + chars_printed*font_width_with_spaces < SSD1306_WIDTH)
    {
      ssd1306_writeCharToBuff(character, start_x + chars_printed*font_width_with_spaces, row, font);
      chars_printed++;
    }
  }
}
*/

uint8_t ssd1306_writeCommand(const struct i2c_dt_spec * dev_i2c, uint8_t command)
{   
  //HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
  //HAL_I2C_Mem_Write(_i2c, _address, 0x00, 1, &command, 1, 50);
  //i2c_reg_write_byte_dt
  return i2c_reg_write_byte_dt(dev_i2c, 0x00, command);
}

uint8_t ssd1306_writeData(const struct i2c_dt_spec * dev_i2c, uint8_t* data, uint16_t data_len)
{ 
    return i2c_burst_write_dt(dev_i2c, 0x40, data, data_len);
}

/*
uint8_t ssd1306_writeData(const struct i2c_dt_spec * dev_i2c, uint8_t* data, uint16_t data_len)
{ 
    // return HAL_I2C_Mem_Write(_i2c, _address, 0x40, 1, data, data_len, 100);
    uint8_t buf[1 + data_len];
    buf[0] = 0x40;           // Memory address (register)
    memcpy(&buf[1], data, data_len);  // Copy data after the memory address

    // Perform the I2C write operation
    return i2c_write_dt(dev_i2c, buf, sizeof(buf));
}*/

uint8_t ssd1306_writeDataDMA(uint8_t* data, uint16_t data_len)
{
    return 99;//HAL_I2C_Mem_Write_DMA(_i2c, _address, 0x40, 1, data, data_len);
}

// Page is vertical 8 bit chunk of LCD.
uint8_t ssd1306_updatePage(ssd1306_device* dev, uint8_t x, uint8_t p_index)
{
  uint8_t errors = 0;
  errors += ssd1306_writeCommand(dev->i2c_spec, 0xB0 + p_index);    // Set the page start address of the target display location by command (0xB0 to 0xB7).
  errors += ssd1306_writeCommand(dev->i2c_spec, x % 16);            // Set the lower start column address of pointer by command (0x00~0x0F).
  errors += ssd1306_writeCommand(dev->i2c_spec, 0x10 + x/16);       // Set the upper start column address of pointer by command (0x10~0x1F).

  errors += ssd1306_writeData(dev->i2c_spec, &dev->buffer[x + p_index * 128], 1);
  return errors;
}

/*
// Page is vertical 8 bit chunk of LCD.
void ssd1306_updateChar(uint8_t *buffer, uint8_t start_x, uint8_t row, uint8_t char_width)
{
  ssd1306_writeCommand(0xB0 + row);              // Set the page start address of the target display location by command (0xB0 to 0xB7).
  ssd1306_writeCommand(start_x % 16);            // Set the lower start column address of pointer by command (0x00~0x0F).
  ssd1306_writeCommand(0x10 + start_x/16);       // Set the upper start column address of pointer by command (0x10~0x1F).

  ssd1306_writeData(buffer[start_x + row * 128], char_width);
}
*/

uint8_t ssd1306_updateScreen(ssd1306_device* dev)
{
  uint8_t errors = 0;
  errors += ssd1306_writeCommand(dev->i2c_spec, 0xB0);       // Set the page start address of the target display location by command (0xB0 to 0xB7).
  errors += ssd1306_writeCommand(dev->i2c_spec, 0x00);       // Set the lower start column address of pointer by command (0x00~0x0F).
  errors += ssd1306_writeCommand(dev->i2c_spec, 0x10);       // Set the upper start column address of pointer by command (0x10~0x1F).
  errors += ssd1306_writeData(dev->i2c_spec, &dev->buffer[0], 512);
  return errors;
}

/*
void ssd1306_updateScreenDMA()
{
    ssd1306_writeCommand(0xB0);       // Set the page start address of the target display location by command (0xB0 to 0xB7).
    ssd1306_writeCommand(0x00);       // Set the lower start column address of pointer by command (0x00~0x0F).
    ssd1306_writeCommand(0x10);       // Set the upper start column address of pointer by command (0x10~0x1F).
    ssd1306_writeDataDMA(&SSD1306_Buffer[0], 512);
    //_writeDataDMA(&SSD1306_Buffer[0], SSD1306_WIDTH);
}
*/