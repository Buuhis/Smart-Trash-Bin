#include "led.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void led_init(void) {
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);

    // Configure PB1 (Green), PB0 (Yellow), PA7 (Red)
    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0 | GPIO1);
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO7);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO0 | GPIO1);
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO7);

    // Turn off all LEDs initially
    led_off(LED_GREEN);
    led_off(LED_YELLOW);
    led_off(LED_RED);
}

void led_on(led_t led) {
    switch (led) {
        case LED_GREEN:  gpio_set(GPIOB, GPIO1); break;
        case LED_YELLOW: gpio_set(GPIOB, GPIO0); break;
        case LED_RED:    gpio_set(GPIOA, GPIO7); break;
    }
}

void led_off(led_t led) {
    switch (led) {
        case LED_GREEN:  gpio_clear(GPIOB, GPIO1); break;
        case LED_YELLOW: gpio_clear(GPIOB, GPIO0); break;
        case LED_RED:    gpio_clear(GPIOA, GPIO7); break;
    }
}