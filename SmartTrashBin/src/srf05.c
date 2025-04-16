#include "srf05.h"
#include "systick_delay.h"

void srf05_init(void) {
    // Enable clock for GPIOA
    rcc_periph_clock_enable(RCC_GPIOA);

    // Configure TRIG as output, ECHO as input
    gpio_mode_setup(SRF05_TRIG_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SRF05_TRIG_PIN);
    gpio_mode_setup(SRF05_ECHO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, SRF05_ECHO_PIN);
}

uint32_t srf05_get_distance(void) {
    // Send 10us trigger pulse
    gpio_set(SRF05_TRIG_PORT, SRF05_TRIG_PIN);
    delay_ms(1); // Simplified delay for ~10us
    gpio_clear(SRF05_TRIG_PORT, SRF05_TRIG_PIN);

    // Wait for ECHO to go high
    while (!gpio_get(SRF05_ECHO_PORT, SRF05_ECHO_PIN));

    // Measure pulse width
    uint32_t start_time = get_systick_tick(); // Use getter function
    while (gpio_get(SRF05_ECHO_PORT, SRF05_ECHO_PIN));
    uint32_t pulse_duration = get_systick_tick() - start_time;

    // Convert to distance (cm): speed of sound = 343 m/s, divide by 2 for round trip
    // pulse_duration is in ms, so distance = (pulse_duration * 34300) / (2 * 1000)
    return (pulse_duration * 343) / 20;
}