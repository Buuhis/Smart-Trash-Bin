#include "systick_delay.h"

static volatile uint32_t tick = 0;

void systick_delay_init(void) {
    systick_set_reload(16000 - 1); // 16MHz clock, 1ms tick
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_counter_enable();
    systick_interrupt_enable();
}

void delay_ms(uint32_t ms) {
    uint32_t target = tick + ms;
    while (tick < target);
}

uint32_t get_systick_tick(void) {
    return tick;
}

void sys_tick_handler(void) {
    tick++;
}