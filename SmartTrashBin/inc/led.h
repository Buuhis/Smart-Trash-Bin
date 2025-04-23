#ifndef LED_H
#define LED_H

typedef enum {
    LED_GREEN,
    LED_YELLOW,
    LED_RED
} led_t;

void led_init(void);
void led_on(led_t led);
void led_off(led_t led);

#endif