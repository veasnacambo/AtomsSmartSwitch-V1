#include "led.h"
#include "pins.h"
#include "app_config.h"

#include "driver/gpio.h"

static bool s_led_state = false;

static void led_apply_output(bool on)
{
#if LED_ACTIVE_HIGH
    gpio_set_level(LED_GPIO, on ? 1 : 0);
#else
    gpio_set_level(LED_GPIO, on ? 0 : 1);
#endif
}

void led_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config(&io_conf);

    s_led_state = false;
    led_apply_output(s_led_state);
}

void led_set(bool on)
{
    s_led_state = on;
    led_apply_output(s_led_state);
}

void led_toggle(void)
{
    s_led_state = !s_led_state;
    led_apply_output(s_led_state);
}