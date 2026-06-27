/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"

#if defined(CYW43_WL_GPIO_LED_PIN)
#include "pico/cyw43_arch.h"
#endif

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
#endif

static bool led_is_cyw43 = false;

#if !defined(PICO_DEFAULT_LED_PIN) && !defined(CYW43_WL_GPIO_LED_PIN)
#warning blink_simple example requires a board with a regular LED or CYW43 LED support
#endif

// Initialize the GPIO for the LED
void pico_led_init(void) {
#if defined(CYW43_WL_GPIO_LED_PIN)
    if (cyw43_arch_init()) {
        panic("cyw43_arch_init failed");
    }
    led_is_cyw43 = true;
#elif defined(PICO_DEFAULT_LED_PIN)
    // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
    // so we can use normal GPIO functionality to turn the led on and off
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
#endif
}

// Turn the LED on or off
void pico_set_led(bool led_on) {
#if defined(CYW43_WL_GPIO_LED_PIN)
    if (led_is_cyw43) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
        return;
    }
#endif

#if defined(PICO_DEFAULT_LED_PIN)
    // Just set the GPIO on or off
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#endif
}

int main() {
    pico_led_init();
    while (true) {
        pico_set_led(true);
        sleep_ms(LED_DELAY_MS);
        pico_set_led(false);
        sleep_ms(LED_DELAY_MS);
    }
}
