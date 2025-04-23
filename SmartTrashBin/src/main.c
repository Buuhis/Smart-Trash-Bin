#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>
#include <stdio.h>
#include <string.h>
#include "vl53l0x.h"
#include "i2c.h"
#include "uart.h"
#include "led.h"
#include "sh1106.h"
#include "srf05.h"
#include "servo.h"
#include "button.h"
#include "delay.h"

#define DISTANCE_THRESHOLD 30 // cm

int main(void) {
    // Clock setup: HSI 16 MHz
    rcc_set_sysclk_source(RCC_CFGR_SW_HSI);
    while (!rcc_is_osc_ready(RCC_HSI));
    rcc_ahb_frequency = 16000000;
    rcc_apb1_frequency = 16000000;
    rcc_apb2_frequency = 16000000;

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
    bool is_open = false; // Track lid state

    // Main loop
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
            snprintf(buf, sizeof(buf), "Distance: %u cm\r\n", distance_cm);
            uart_write(buf);
        }

        // SRF05-based automatic lid control
        uint32_t distance_srf05 = srf05_get_distance();
        if (distance_srf05 < DISTANCE_THRESHOLD && distance_srf05 > 0) {
            servo_set_angle(135); // Open lid
            is_open = true;
            delay_ms(5000); // Wait 5 seconds
            servo_set_angle(0); // Close lid
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

        delay_ms(100); // Small delay to avoid rapid polling
    }
}