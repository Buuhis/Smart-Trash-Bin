#include "uart.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

void uart_init(uint32_t baudrate) {
    // Enable clocks
    rcc_periph_clock_enable(RCC_USART2);
    rcc_periph_clock_enable(RCC_GPIOA);

    // Configure PA2 (TX) and PA3 (RX) as alternate function
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3);
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2 | GPIO3);

    // Configure USART2
    usart_set_baudrate(USART2, baudrate);
    usart_set_databits(USART2, 8);
    usart_set_stopbits(USART2, USART_STOPBITS_1);
    usart_set_mode(USART2, USART_MODE_TX);
    usart_set_parity(USART2, USART_PARITY_NONE);
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
    usart_enable(USART2);
}

void uart_write(const char *str) {
    while (*str) {
        if (*str == '\n') {
            usart_send_blocking(USART2, '\r'); // Add carriage return before newline
        }
        usart_send_blocking(USART2, *str++);
    }
}