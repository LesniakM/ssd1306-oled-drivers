#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>

#include "ssd1306.h"

#define I2C0_NODE DT_NODELABEL(ssd1306)

int main(void)
{
  /* Retrieve the API-specific device structure and make sure that the device is ready to use  */
  static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C0_NODE);
  if (!device_is_ready(dev_i2c.bus))
  {
    printk("Welp, not ready...");
    return 1;
  }
  ssd1306_device dev;
  ssd1306_device_init(&dev, &dev_i2c);

  ssd1306_init(&dev);
  ssd1306_updateScreen(&dev);

  while (1)
  {
    for (int i = 0; i < 64; i++)
    {
      for (int r = -i / 4; r <= i / 4; r++)
      {
        if ((r + i) % 2 == 0)
        {
          ssd1306_drawPixelToBuff(dev.buffer, 20 + i, 16 + r, true);
        }
        else
        {
          ssd1306_drawPixelToBuff(dev.buffer, 20 + i, 16 + r, false);
        }
      }
      ssd1306_updateScreen(&dev);
    }
    
    for (int i = 64; i > 0; i--)
    {
      for (int r = -i / 4; r <= i / 4; r++)
      {
        ssd1306_drawPixelToBuff(dev.buffer, 20 + i, 16 + r, false);
      }
      ssd1306_updateScreen(&dev);
    }

    for (int i = 0; i < 64; i++)
    {
      for (int r = -i / 4; r <= i / 4; r++)
      {
        if ((r + i) % 2 == 0)
        {
          ssd1306_drawPixelToBuff(dev.buffer, 20 + i, 16 + r, false);
        }
        else
        {
          ssd1306_drawPixelToBuff(dev.buffer, 20 + i, 16 + r, true);
        }
      }
      ssd1306_updateScreen(&dev);
    }
    

    for (int i = 64; i > 0; i--)
    {
      for (int r = -i / 4; r <= i / 4; r++)
      {
        ssd1306_drawPixelToBuff(dev.buffer, 20 + i, 16 + r, true);
      }
      ssd1306_updateScreen(&dev);
    }
    

    for (int i = 64; i > 0; i--)
    {
      for (int r = -i / 4; r <= i / 4; r++)
      {
        ssd1306_drawPixelToBuff(dev.buffer, 20 + i, 16 + r, false);
      }
      ssd1306_updateScreen(&dev);
    }
    
  }

  return 0;
}