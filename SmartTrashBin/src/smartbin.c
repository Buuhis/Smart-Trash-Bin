#include "smartbin.h"
#include "vl53l0x.h"
#include "i2c.h"
#include "uart.h"
#include "led.h"
#include "sh1106.h"
#include "srf05.h"
#include "servo.h"
#include "button.h"
#include "delay.h"
#include <libopencm3/stm32/usart.h>
#include <stdio.h>
#include <string.h>

#define DISTANCE_THRESHOLD 30 // cm

static bool is_open = false; // Track lid state

void smartbin_init(void) {
    // Initialize peripherals
    led_init();
    uart_init(115200);
    i2c_init();
    delay_init();
    srf05_init();
    servo_init();
    button_init();

    // Initialize VL53L0X
    led_on(LED_GREEN); // Program running
    if (!vl53l0x_init()) {
        led_on(LED_RED); // Initialization failed
        uart_write("VL53L0X init failed\r\n");
        while (1);
    }
    led_on(LED_YELLOW); // Init successful

    // Initialize SH1106
    sh1106_init(I2C2);
    sh1106_clear();
    sh1106_display(I2C2);

    // Servo initial state
    servo_set_angle(0); // Start with lid closed
}

void smartbin_run(void) {
    while (1) {
        // VL53L0X-based LED and display control
        uint16_t distance_vl53 = vl53l0x_read_range();
        char buf[64];
        memset(buf, 0, sizeof(buf));

        if (distance_vl53 == 0xFFFF) {
            led_on(LED_RED);
            led_off(LED_GREEN);
            led_off(LED_YELLOW);
            uart_write("Measurement error\r\n");

            sh1106_clear();
            sh1106_draw_string(10, 16, "Error!");
            sh1106_display(I2C2);
        } else {
            uint16_t distance_cm = distance_vl53 / 10; // mm to cm

            if (distance_cm > 40) {
                led_off(LED_GREEN);
                led_off(LED_YELLOW);
                led_off(LED_RED);

                sh1106_clear();
                sh1106_display(I2C2);
            } else if (distance_cm >= 34 && distance_cm <= 40) {
                led_on(LED_GREEN);
                led_off(LED_YELLOW);
                led_off(LED_RED);

                sh1106_clear();
                sh1106_draw_string(10, 16, "Hay cho toi rac :))");
                sh1106_display(I2C2);
            } else if (distance_cm >= 16 && distance_cm < 34) {
                led_on(LED_YELLOW);
                led_off(LED_GREEN);
                led_off(LED_RED);

                sh1106_clear();
                sh1106_draw_string(10, 16, "Hay cho toi rac :))");
                sh1106_display(I2C2);
            } else if (distance_cm < 16) {
                led_on(LED_RED);
                led_off(LED_GREEN);
                led_off(LED_YELLOW);

                sh1106_clear();
                sh1106_draw_string(10, 16, "Toi no roi :((");
                sh1106_display(I2C2);
            }

            // snprintf(buf, sizeof(buf), "Distance: %u mm\r\n", distance_cm);
            //uart_write(buf);
        }

        // SRF05-based automatic lid control
        uint32_t distance_srf05 = srf05_get_distance();
        if (distance_srf05 < DISTANCE_THRESHOLD && distance_srf05 > 0) {
            servo_set_angle(135); // Open lid
            is_open = true;
            delay_ms(5000);       // Wait 5 seconds
            servo_set_angle(0);   // Close lid
            is_open = false;
        }

        // Button-based manual lid control
        if (button_pressed()) {
            if (is_open) {
                servo_set_angle(0); // Close lid
                is_open = false;
            } else {
                servo_set_angle(135); // Open lid
                is_open = true;
            }
        }

        // UART-based control
        if (usart_get_flag(USART2, USART_FLAG_RXNE)) { // Check if data is available
            char cmd = usart_recv_blocking(USART2); // Read character

            switch (cmd) {
                case 's': // Show status
                    uart_write(is_open ? "Bin is open\r\n" : "Bin is closed\r\n");
                    break;
                case 'o': // Open bin
                    servo_set_angle(135);
                    is_open = true;
                    uart_write("Bin opened\r\n");
                    break;
                case 'c': // Close bin
                    servo_set_angle(0);
                    is_open = false;
                    uart_write("Bin closed\r\n");
                    break;
                case 't': // Show control
                    uart_write("s: Show status\r\n");
                    uart_write("o: Open bin\r\n");
                    uart_write("c: Close bin\r\n");
                    uart_write("p: Print space of bin\r\n");
                    break;
                case 'p': 
                    snprintf(buf, sizeof(buf), "Space: %u mm\r\n", distance_vl53);
                    uart_write(buf);
                default:
                    break;
            }
        }

        delay_ms(100); // Small delay to avoid rapid polling
    }
}
