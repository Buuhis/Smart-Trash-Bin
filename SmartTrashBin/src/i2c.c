#include "i2c.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include "uart.h"
#include <stdio.h>

void i2c_init(void) {
    // Enable clock for GPIOB, I2C1, and I2C2
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_I2C1);
    rcc_periph_clock_enable(RCC_I2C2);

    // Configure I2C1: PB6 (SCL), PB7 (SDA)
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6 | GPIO7);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO6 | GPIO7);
    gpio_set_af(GPIOB, GPIO_AF4, GPIO6 | GPIO7);

    // Configure I2C2: PB10 (SCL), PB3 (SDA)
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10 | GPIO3);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO10 | GPIO3);
    gpio_set_af(GPIOB, GPIO_AF4, GPIO10); // AF4 for I2C2 SCL
    gpio_set_af(GPIOB, GPIO_AF9, GPIO3);  // AF9 for I2C2 SDA

    // Reset I2C peripherals
    rcc_periph_reset_pulse(RST_I2C1);
    rcc_periph_reset_pulse(RST_I2C2);

    // Configure I2C1 and I2C2: 100 kHz, Standard Mode, 16 MHz APB1 clock
    i2c_peripheral_disable(I2C1);
    i2c_set_standard_mode(I2C1);
    i2c_set_clock_frequency(I2C1, 16); // 16 MHz
    i2c_set_trise(I2C1, 17); // 1000 ns / 62.5 ns = 16 + 1
    i2c_set_ccr(I2C1, 80);   // 100 kHz: 5000 ns / 62.5 ns = 80
    i2c_peripheral_enable(I2C1);

    i2c_peripheral_disable(I2C2);
    i2c_set_standard_mode(I2C2);
    i2c_set_clock_frequency(I2C2, 16); // 16 MHz
    i2c_set_trise(I2C2, 17); // 1000 ns / 62.5 ns = 16 + 

    i2c_set_ccr(I2C2, 80);   // 100 kHz: 5000 ns / 62.5 ns = 80
    i2c_peripheral_enable(I2C2);
}

bool i2c_write(uint32_t i2c, uint8_t addr, uint8_t *data, uint8_t len) {
    i2c_transfer7(i2c, addr, data, len, NULL, 0);
    if (I2C_SR1(i2c) & I2C_SR1_AF) {
        char buf[32];
        snprintf(buf, sizeof(buf), "I2C NACK at addr 0x%02X\n", addr);
        uart_write(buf);
        I2C_CR1(i2c) |= I2C_CR1_STOP; // Clear NACK
        return false;
    }
    return true;
}

bool i2c_write_read(uint32_t i2c, uint8_t addr, uint8_t *tx, uint8_t tx_len, uint8_t *rx, uint8_t rx_len) {
    i2c_transfer7(i2c, addr, tx, tx_len, rx, rx_len);
    if (I2C_SR1(i2c) & I2C_SR1_AF) {
        char buf[32];
        snprintf(buf, sizeof(buf), "I2C NACK at addr 0x%02X\n", addr);
        uart_write(buf);
        I2C_CR1(i2c) |= I2C_CR1_STOP; // Clear NACK
        return false;
    }
    return true;
}