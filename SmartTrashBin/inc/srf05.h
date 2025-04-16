#ifndef SRF05_H
#define SRF05_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#define SRF05_TRIG_PORT GPIOA
#define SRF05_TRIG_PIN GPIO11
#define SRF05_ECHO_PORT GPIOA
#define SRF05_ECHO_PIN GPIO12

void srf05_init(void);
uint32_t srf05_get_distance(void);

#endif /* SRF05_H */