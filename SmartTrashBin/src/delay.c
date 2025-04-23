#include "delay.h"

static volatile uint32_t tick = 0;

void delay_init(void) {
    // Configure SysTick for 1ms interrupts
    systick_set_reload(16000 - 1); // 16 MHz clock, 1ms tick
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