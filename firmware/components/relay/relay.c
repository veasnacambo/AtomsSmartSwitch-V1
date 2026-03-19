#include "relay.h"
#include "pins.h"
#include "app_config.h"

#include "driver/gpio.h"

static bool s_relay_state = false;

static void relay_apply_output(bool on)
{
#if RELAY_ACTIVE_HIGH
    gpio_set_level(RELAY_GPIO, on ? 1 : 0);
#else
    gpio_set_level(RELAY_GPIO, on ? 0 : 1);
#endif
}

void relay_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << RELAY_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config(&io_conf);

    s_relay_state = false;
    relay_apply_output(s_relay_state);
}

void relay_on(void)
{
    s_relay_state = true;
    relay_apply_output(s_relay_state);
}

void relay_off(void)
{
    s_relay_state = false;
    relay_apply_output(s_relay_state);
}

void relay_toggle(void)
{
    s_relay_state = !s_relay_state;
    relay_apply_output(s_relay_state);
}

bool relay_get_state(void)
{
    return s_relay_state;
}
