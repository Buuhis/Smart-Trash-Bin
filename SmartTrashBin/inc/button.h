#ifndef BUTTON_H
#define BUTTON_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#define BUTTON_PORT GPIOB
#define BUTTON_PIN GPIO13

void button_init(void);
bool button_pressed(void);

#endif // BUTTON_H