#include "button.h"
#include "pins.h"
#include "app_config.h"

#include "driver/gpio.h"
#include "esp_timer.h"

static bool s_last_raw_state = false;
static bool s_stable_state = false;
static int64_t s_last_change_time_ms = 0;

static bool button_read_raw(void)
{
    int level = gpio_get_level(BUTTON_GPIO);

#if BUTTON_ACTIVE_LOW
    return (level == 0);
#else
    return (level == 1);
#endif
}

void button_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BUTTON_GPIO),
        .mode = GPIO_MODE_INPUT,
#if BUTTON_ACTIVE_LOW
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
#else
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
#endif
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config(&io_conf);

    s_last_raw_state = button_read_raw();
    s_stable_state = s_last_raw_state;
    s_last_change_time_ms = esp_timer_get_time() / 1000;
}

bool button_is_pressed(void)
{
    return s_stable_state;
}

bool button_was_pressed_event(void)
{
    bool raw = button_read_raw();
    int64_t now_ms = esp_timer_get_time() / 1000;

    if (raw != s_last_raw_state) {
        s_last_raw_state = raw;
        s_last_change_time_ms = now_ms;
    }

    if ((now_ms - s_last_change_time_ms) >= BUTTON_DEBOUNCE_MS) {
        if (s_stable_state != raw) {
            s_stable_state = raw;

            if (s_stable_state) {
                return true;
            }
        }
    }

    return false;
}