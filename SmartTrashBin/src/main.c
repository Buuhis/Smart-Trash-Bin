#include <libopencm3/stm32/rcc.h>
#include "smartbin.h"

int main(void) {
    // Clock setup: HSI 16 MHz
    rcc_set_sysclk_source(RCC_CFGR_SW_HSI);
    while (!rcc_is_osc_ready(RCC_HSI));
    rcc_ahb_frequency  = 16000000;
    rcc_apb1_frequency = 16000000;
    rcc_apb2_frequency = 16000000;

    // Initialize and run smart bin
    smartbin_init();
    smartbin_run();

    return 0; // Never reached
}