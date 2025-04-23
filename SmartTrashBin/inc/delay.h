#ifndef DELAY_H
#define DELAY_H

#include <libopencm3/cm3/systick.h>
#include <stdint.h>

void delay_init(void);          // Unified initialization function
void delay_ms(uint32_t ms);     // Delay function
uint32_t get_systick_tick(void); // Function to get current tick count
void sys_tick_handler(void);    // SysTick interrupt handler

#endif /* DELAY_H */