#include "sh1106.h"
#include "font.h"
#include <libopencm3/stm32/i2c.h>

static uint8_t display_buffer[128 * 8]; // 128x64 pixels, 8 pages

// Send a command to SH1106
static void sh1106_write_command(uint32_t i2c, uint8_t cmd) {
    uint8_t data[2] = {0x00, cmd}; // Control byte 0x00 for commands
    i2c_transfer7(i2c, SH1106_I2C_ADDR, data, 2, NULL, 0);
}

// Send data to SH1106
static void sh1106_write_data(uint32_t i2c, uint8_t *data, uint16_t len) {
    uint8_t buffer[129];
    buffer[0] = 0x40; // Control byte 0x40 for data
    for (uint16_t i = 0; i < len; i++) {
        buffer[i + 1] = data[i];
    }
    i2c_transfer7(i2c, SH1106_I2C_ADDR, buffer, len + 1, NULL, 0);
}

void sh1106_init(uint32_t i2c) {
    // Initialization sequence for SH1106
    sh1106_write_command(i2c, 0xAE); // Display OFF
    sh1106_write_command(i2c, 0xD5); // Set display clock divide ratio
    sh1106_write_command(i2c, 0x80);
    sh1106_write_command(i2c, 0xA8); // Set multiplex ratio
    sh1106_write_command(i2c, 0x3F); // 64MUX
    sh1106_write_command(i2c, 0xD3); // Set display offset
    sh1106_write_command(i2c, 0x00);
    sh1106_write_command(i2c, 0x40); // Set start line
    sh1106_write_command(i2c, 0x8D); // Charge pump
    sh1106_write_command(i2c, 0x14); // Enable charge pump
    sh1106_write_command(i2c, 0x20); // Memory mode
    sh1106_write_command(i2c, 0x00); // Horizontal addressing
    sh1106_write_command(i2c, 0xA1); // Segment remap
    sh1106_write_command(i2c, 0xC8); // COM output scan direction
    sh1106_write_command(i2c, 0xDA); // COM pins
    sh1106_write_command(i2c, 0x12);
    sh1106_write_command(i2c, 0x81); // Contrast control
    sh1106_write_command(i2c, 0xCF);
    sh1106_write_command(i2c, 0xD9); // Pre-charge period
    sh1106_write_command(i2c, 0xF1);
    sh1106_write_command(i2c, 0xDB); // VCOMH deselect level
    sh1106_write_command(i2c, 0x40);
    sh1106_write_command(i2c, 0xA4); // Entire display ON
    sh1106_write_command(i2c, 0xA6); // Normal display
    sh1106_write_command(i2c, 0xAF); // Display ON
}

void sh1106_clear(void) {
    for (uint16_t i = 0; i < sizeof(display_buffer); i++) {
        display_buffer[i] = 0;
    }
}

void sh1106_display(uint32_t i2c) {
    for (uint8_t page = 0; page < 8; page++) {
        sh1106_write_command(i2c, 0xB0 + page); // Set page
        sh1106_write_command(i2c, 0x02);        // Set column low nibble
        sh1106_write_command(i2c, 0x10);        // Set column high nibble
        sh1106_write_data(i2c, &display_buffer[page * 128], 128);
    }
}

void sh1106_draw_string(uint8_t x, uint8_t y, const char *str) {
    uint8_t page = y / 8;
    uint8_t pixel_y = y % 8;
    uint16_t offset = page * 128 + x;

    while (*str) {
        if (x > 122) break; // Prevent buffer overflow (5 pixels per char + 1 pixel spacing)
        uint8_t c = *str - 32; // Font starts at ASCII 32 (space)
        if (c > 95) c = 0;     // Default to space for unsupported chars
        for (uint8_t i = 0; i < 5; i++) {
            if (x + i < 128) {
                display_buffer[offset + i] |= (font_5x7[c * 5 + i] << pixel_y);
            }
        }
        x += 6; // 5 pixels for char + 1 pixel spacing
        offset += 6;
        str++;
    }
}