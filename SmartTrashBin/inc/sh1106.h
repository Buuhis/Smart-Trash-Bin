#ifndef SH1106_H
#define SH1106_H

#include <libopencm3/stm32/i2c.h>
#include <stdint.h>

#define SH1106_I2C_ADDR 0x3C // Default I2C address for SH1106 (7-bit)

void sh1106_init(uint32_t i2c);
void sh1106_clear(void);
void sh1106_display(uint32_t i2c); // Updated to include i2c parameter
void sh1106_draw_string(uint8_t x, uint8_t y, const char *str);

#endif // SH1106_H