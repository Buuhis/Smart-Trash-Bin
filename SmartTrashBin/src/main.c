#include "srf05.h"
#include "servo.h"
#include "systick_delay.h"

#define DISTANCE_THRESHOLD 30 // cm

int main(void) {
    // Initialize modules
    systick_delay_init();
    srf05_init();
    servo_init();

    servo_set_angle(0); // Start with lid closed

    while (1) {
        // Measure distance
        uint32_t distance = srf05_get_distance();

        // If obstacle detected within threshold
        if (distance < DISTANCE_THRESHOLD && distance > 0) {
            servo_set_angle(135); // Open lid
            delay_ms(5000); // Wait 5 seconds
            servo_set_angle(0); // Close lid
        }

        delay_ms(100); // Small delay to avoid rapid polling
    }
}