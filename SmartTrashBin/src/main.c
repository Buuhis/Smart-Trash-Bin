#include "srf05.h"
#include "servo.h"
#include "systick_delay.h"
#include "button.h"

#define DISTANCE_THRESHOLD 30 // cm

int main(void) {
    // Initialize modules
    systick_delay_init();
    srf05_init();
    servo_init();
    button_init(); // Initialize button

    servo_set_angle(0); // Start with lid closed
    bool is_open = false; // Track lid state

    while (1) {
        // SRF05-based automatic lid control (unchanged)
        uint32_t distance = srf05_get_distance();
        if (distance < DISTANCE_THRESHOLD && distance > 0) {
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
