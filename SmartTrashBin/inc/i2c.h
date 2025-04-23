#ifndef I2C_H
#define I2C_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

void i2c_init(void);
bool i2c_write(uint32_t i2c, uint8_t addr, uint8_t *data, uint8_t len);
bool i2c_write_read(uint32_t i2c, uint8_t addr, uint8_t *tx, uint8_t tx_len, uint8_t *rx, uint8_t rx_len);

#endif