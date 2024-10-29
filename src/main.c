#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>

#include "ssd1306.h"

#define I2C0_NODE DT_NODELABEL(ssd1306)
K_MUTEX_DEFINE(screen_write);

/* Define stack size and scheduling priority used by each thread */
#define STACKSIZE 1024
#define THREAD0_PRIORITY 2
#define THREAD1_PRIORITY 5

ssd1306_device dev;
uint32_t frame = 0;

void thread0(void)
{ 
  while (1)
  {
    for (int i = 0; i < 64; i++)
    {
      for (int r = -i / 4; r <= i / 4; r++)
      {
        ssd1306_drawPixelToBuff(dev.buffer, 10 + i, 16 + r, true);
      }
    }
    k_busy_wait(1500*1000);
    printk("Slept 1\n");

    k_msleep(10);
    printk("Slept 2\n");

    k_busy_wait(1500*1000);
    printk("Slept 3\n");

    k_msleep(10);
    printk("Slept 4\n");

    k_busy_wait(1500*1000);
    printk("Slept 5\n");

    for (int i = 63; i >= 0; i--)
    {
      for (int r = -i / 4; r <= i / 4; r++)
      {
        ssd1306_drawPixelToBuff(dev.buffer, 10 + i, 16 + r, false);
      }
    }

    ssd1306_writeStringToBuff(dev.buffer, "12345678", 10, 0, CharData);
    ssd1306_writeStringToBuff(dev.buffer, "ASDFGHJK", 10, 1, CharData);
    ssd1306_writeStringToBuff(dev.buffer, "ZXCVBNMH", 10, 2, CharData);
    ssd1306_writeStringToBuff(dev.buffer, "!@#$^&*(", 10, 3, CharData);
    k_msleep(60);
    printk("Slept 6\n");
    k_busy_wait(500*1000);

    ssd1306_writeStringToBuff(dev.buffer, "        ", 10, 0, CharData);
    ssd1306_writeStringToBuff(dev.buffer, "        ", 10, 1, CharData);
    ssd1306_writeStringToBuff(dev.buffer, "        ", 10, 2, CharData);
    ssd1306_writeStringToBuff(dev.buffer, "        ", 10, 3, CharData);
  }
}

void thread1(void)
{
  /* Retrieve the API-specific device structure and make sure that the device is ready to use  */
  static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C0_NODE);
  if (!device_is_ready(dev_i2c.bus))
  {
    printk("Welp, not ready...");
    return 1;
  }
  ssd1306_device_init(&dev, &dev_i2c);
  ssd1306_init(&dev);

  uint32_t last = k_uptime_get();

  while (1)
  {
    uint32_t start = k_uptime_get();
    ssd1306_updateScreen(&dev);
    uint32_t delta2 = k_uptime_get() - start;
    uint32_t delta = k_uptime_get() - last;
    frame++;
    if (delta >= 1000) {
      
      printk("F: %d, L:%d, T:%d\n", frame, delta, delta2);
      frame = 0;
      k_msleep(30);
      last = k_uptime_get();
    }
  }
}

/* Main function is not mandatory. */
int main(void)
{
  return 0;
}

K_THREAD_DEFINE(thread0_id, STACKSIZE, thread0, NULL, NULL, NULL, THREAD0_PRIORITY, 0, 100);
K_THREAD_DEFINE(thread1_id, STACKSIZE, thread1, NULL, NULL, NULL, THREAD1_PRIORITY, 0, 0);
