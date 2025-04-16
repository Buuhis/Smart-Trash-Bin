#include "button.h"
#include "systick_delay.h"

void button_init(void) {
    // Enable clock for GPIOB
    rcc_periph_clock_enable(RCC_GPIOB);

    // Configure PB13 as input with pull-up
    gpio_mode_setup(BUTTON_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, BUTTON_PIN);
}

bool button_pressed(void) {
    static bool last_state = true; // Pull-up: high when not pressed
    bool current_state = gpio_get(BUTTON_PORT, BUTTON_PIN);
    bool pressed = false;

    // Detect falling edge (button pressed, pull-up makes it go low)
    if (last_state && !current_state) {
        delay_ms(50); // Simple debounce
        if (!gpio_get(BUTTON_PORT, BUTTON_PIN)) {
            pressed = true;
        }
    }

    last_state = current_state;
    return pressed;
}
