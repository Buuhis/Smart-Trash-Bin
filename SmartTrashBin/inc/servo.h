#ifndef SERVO_H
#define SERVO_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#define SERVO_PORT GPIOA
#define SERVO_PIN GPIO8
#define SERVO_TIMER TIM1
#define SERVO_TIMER_RCC RCC_TIM1

void servo_init(void);
void servo_set_angle(uint8_t angle);

#endif /* SERVO_H */