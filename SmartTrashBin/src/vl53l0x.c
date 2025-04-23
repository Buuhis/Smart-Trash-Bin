#include "vl53l0x.h"
#include "i2c.h"
#include "uart.h"
#include <libopencm3/stm32/i2c.h>
#include <stdio.h>
#include <string.h>

bool vl53l0x_write_reg(uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};
    bool success = i2c_write(I2C1, VL53L0X_I2C_ADDR, data, 2);
    if (!success) {
        uart_write("I2C write failed\r\n");
    }
    return success;
}

bool vl53l0x_read_reg(uint8_t reg, uint8_t *value) {
    bool success = i2c_write_read(I2C1, VL53L0X_I2C_ADDR, &reg, 1, value, 1);
    if (!success) {
        uart_write("I2C read failed\r\n");
    }
    return success;
}

bool vl53l0x_read_reg16(uint8_t reg, uint16_t *value) {
    uint8_t data[2];
    bool success = i2c_write_read(I2C1, VL53L0X_I2C_ADDR, &reg, 1, data, 2);
    if (success) {
        *value = (data[0] << 8) | data[1];
    } else {
        uart_write("I2C read16 failed\r\n");
    }
    return success;
}

bool vl53l0x_init(void) {
    uint8_t model_id;
    if (!vl53l0x_read_reg(VL53L0X_REG_IDENTIFICATION_MODEL_ID, &model_id) || model_id != 0xEE) {
        uart_write("VL53L0X model ID check failed\r\n");
        return false;
    }

    // Enhanced initialization sequence
    vl53l0x_write_reg(0x88, 0x00); // I2C standard mode
    vl53l0x_write_reg(0x80, 0x01);
    vl53l0x_write_reg(0xFF, 0x01);
    vl53l0x_write_reg(0x00, 0x00);
    vl53l0x_write_reg(0x91, 0x3C); // Reference SPAD
    vl53l0x_write_reg(0x00, 0x01);
    vl53l0x_write_reg(0xFF, 0x00);
    vl53l0x_write_reg(0x80, 0x00);

    // Calibration steps
    vl53l0x_write_reg(0x89, 0x00); // VHV calibration
    vl53l0x_write_reg(0x8B, 0x01); // Phase calibration

    // Wait for sensor to stabilize
    for (volatile int i = 0; i < 200000; i++); // ~20ms at 16 MHz

    return true;
}

uint16_t vl53l0x_read_range(void) {
    // Start single-shot measurement
    if (!vl53l0x_write_reg(VL53L0X_REG_SYSRANGE_START, 0x01)) {
        return 0xFFFF;
    }

    // Poll for data ready (bit 0 of status register)
    uint8_t status;
    uint32_t timeout = 2000; // 2 second timeout
    char buf[64];
    while (timeout--) {
        if (!vl53l0x_read_reg(VL53L0X_REG_RESULT_RANGE_STATUS, &status)) {
            return 0xFFFF;
        }
        memset(buf, 0, sizeof(buf)); // Clear buffer
        vl53l0x_decode_status(status, buf, sizeof(buf));
        uart_write(buf);
        if (status & 0x01) {
            break;
        }
        for (volatile int i = 0; i < 20000; i++); // ~2ms delay
    }

    if (!(status & 0x01)) {
        uart_write("Data not ready\r\n");
        return 0xFFFF; // Timeout
    }

    // Read distance
    uint16_t distance;
    if (!vl53l0x_read_reg16(VL53L0X_REG_RESULT_RANGE_MM, &distance)) {
        return 0xFFFF;
    }

    // Clear interrupt
    vl53l0x_write_reg(VL53L0X_REG_SYSRANGE_START, 0x00);

    return distance;
}

void vl53l0x_decode_status(uint8_t status, char *buf, uint8_t len) {
    // memset(buf, 0, len); // Clear buffer
    // snprintf(buf, len, "Status: 0x%02X ", status);
    // if (status & 0x01) {
    //     strncat(buf, "(Data Ready)\r\n", len - strlen(buf));
    // } else {
    //     if (status & 0x02) {
    //         strncat(buf, "(Signal Fail)\r\n", len - strlen(buf));
    //     } else if (status & 0x04) {
    //         strncat(buf, "(Phase Fail)\r\n", len - strlen(buf));
    //     } else if (status & 0x08) {
    //         strncat(buf, "(Min Range Fail)\r\n", len - strlen(buf));
    //     } else if (status & 0x40) {
    //         strncat(buf, "(Sigma Fail)\r\n", len - strlen(buf));
    //     } else {
    //         strncat(buf, "(Unknown Error)\r\n", len - strlen(buf));
    //     }
    // }
}