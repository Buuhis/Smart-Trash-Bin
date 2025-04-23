#ifndef VL53L0X_H
#define VL53L0X_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define VL53L0X_I2C_ADDR 0x29

// Register addresses
#define VL53L0X_REG_IDENTIFICATION_MODEL_ID 0xC0
#define VL53L0X_REG_SYSRANGE_START 0x00
#define VL53L0X_REG_RESULT_RANGE_STATUS 0x14
#define VL53L0X_REG_RESULT_RANGE_MM 0x1E

bool vl53l0x_write_reg(uint8_t reg, uint8_t value);
bool vl53l0x_read_reg(uint8_t reg, uint8_t *value);
bool vl53l0x_read_reg16(uint8_t reg, uint16_t *value);
bool vl53l0x_init(void);
uint16_t vl53l0x_read_range(void);
void vl53l0x_decode_status(uint8_t status, char *buf, uint8_t len);

#endif