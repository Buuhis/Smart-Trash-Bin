#include "servo.h"

void servo_init(void) {
    // Enable clock for GPIOA and TIM1
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(SERVO_TIMER_RCC);

    // Configure PA8 as alternate function for TIM1_CH1
    gpio_mode_setup(SERVO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, SERVO_PIN);
    gpio_set_af(SERVO_PORT, GPIO_AF1, SERVO_PIN);

    // Configure TIM1 for PWM
    timer_set_mode(SERVO_TIMER, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(SERVO_TIMER, 16 - 1); // 16MHz / 16 = 1MHz
    timer_set_period(SERVO_TIMER, 20000 - 1); // 20ms period (50Hz)
    timer_set_oc_mode(SERVO_TIMER, TIM_OC1, TIM_OCM_PWM1);
    timer_enable_oc_output(SERVO_TIMER, TIM_OC1);
    timer_enable_break_main_output(SERVO_TIMER);
    timer_enable_counter(SERVO_TIMER);
}

void servo_set_angle(uint8_t angle) {
    // SG92R: 0° = 0.5ms (500), 180° = 2.5ms (2500)
    uint32_t pulse_width = 500 + ((angle * 2000) / 180);
    timer_set_oc_value(SERVO_TIMER, TIM_OC1, pulse_width);
}