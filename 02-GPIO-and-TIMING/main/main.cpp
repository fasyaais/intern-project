#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include <cstdint>
#include <driver/gptimer.h>
#include <driver/gpio.h>

#define LED_PIN GPIO_NUM_2

static bool stateLED = false;

extern "C" {
    void app_main();
}

static bool example_timer_on_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
    stateLED = !stateLED;
    gpio_set_level(LED_PIN, stateLED);
    return true;
}

void app_main(void)
{
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT, 
        .direction = GPTIMER_COUNT_UP,      
        .resolution_hz = 1 * 1000 * 1000,
    };

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config,&gptimer));

    gptimer_alarm_config_t alarm_config = {
        .alarm_count =  1* 1000*1000,
        .reload_count = 0, 
        .flags = {.auto_reload_on_alarm = true}     
    };

    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));

    gptimer_event_callbacks_t cbs = {
        .on_alarm = example_timer_on_alarm_cb,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer,&cbs,NULL));

    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));

    
}