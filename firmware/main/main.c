#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "relay.h"
#include "button.h"
#include "led.h"
#include "app_config.h"

void app_main(void)
{
    relay_init();
    button_init();
    led_init();

    led_set(relay_get_state());

    printf("Atoms Smart Switch V1 - Stage 1 start\n");

    while (1) {
        if (button_was_pressed_event()) {
            relay_toggle();
            led_set(relay_get_state());

            printf("Relay state: %s\n", relay_get_state() ? "ON" : "OFF");
        }

        vTaskDelay(pdMS_TO_TICKS(APP_LOOP_DELAY_MS));
    }
}