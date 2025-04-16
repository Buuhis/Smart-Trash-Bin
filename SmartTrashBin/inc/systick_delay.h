#ifndef SYSTICK_DELAY_H
#define SYSTICK_DELAY_H

#include <libopencm3/cm3/systick.h>
#include <stdint.h>

void systick_delay_init(void);
void delay_ms(uint32_t ms);
uint32_t get_systick_tick(void);
void sys_tick_handler(void); // Add prototype for ISR

#endif /* SYSTICK_DELAY_H */