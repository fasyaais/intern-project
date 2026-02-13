#include <stdio.h>
#include "freertos/FreeRTOS.h"
// #include <cstdint>
#include <driver/gptimer.h>
#include <driver/gpio.h>

#define LED_PIN GPIO_NUM_2
#define BUTTON_PIN GPIO_NUM_4

void blinkHardwareTimer();
void debounceButton();
void blinkPolling();
void buttonISR();

static bool stateLED = false;
static uint8_t debounceDelay = 200; // millisecond
static SemaphoreHandle_t button_sem;

typedef enum {
    LED_ON,
    LED_OFF
} led_mode_t;

static led_mode_t ledMode = LED_OFF; 

extern "C" {
    void app_main();
}

static bool example_timer_on_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
    stateLED = !stateLED;
    gpio_set_level(LED_PIN, stateLED);
    return true;
}

static void IRAM_ATTR buttonISRInterrupt(void *arg){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(button_sem,&xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void app_main(void)
{
    ESP_ERROR_CHECK(gpio_set_direction(LED_PIN,GPIO_MODE_OUTPUT));
    // ESP_ERROR_CHECK(gpio_set_direction(BUTTON_PIN,GPIO_MODE_INPUT));
    // ESP_ERROR_CHECK(gpio_set_pull_mode(BUTTON_PIN,GPIO_PULLUP_ONLY));
    
    // blinkHardwareTimer();
    // debounceButton();
    // blinkPolling();
    buttonISR();
}

void blinkPolling(){
    TickType_t ledTick = xTaskGetTickCount();
    uint8_t ledPeriod = 1000;
    while (true)
    {
        if((xTaskGetTickCount() - ledTick) > (ledPeriod/2)){
            stateLED = !stateLED;
            gpio_set_level(LED_PIN, stateLED);
        }
        vTaskDelay(100);
    }
}

void blinkHardwareTimer()
{
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT, 
        .direction = GPTIMER_COUNT_UP,      
        .resolution_hz = 1 * 1000 * 1000,
    };

    gptimer_handle_t gptimer = NULL;
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config,&gptimer));

    gptimer_alarm_config_t alarm_config = {
        .alarm_count =  1 * 1000*1000,
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

void debounceButton(){
    TickType_t lastButtonTick = xTaskGetTickCount();
    uint8_t lastButtonState = 0;
    for(;;){
        uint8_t buttonState = gpio_get_level(BUTTON_PIN);
        if(((xTaskGetTickCount() - lastButtonTick) > pdMS_TO_TICKS(debounceDelay)) && (buttonState != lastButtonState)){
            lastButtonTick = xTaskGetTickCount();
            lastButtonState = buttonState;
            gpio_set_level(LED_PIN, lastButtonState);
            printf("last button state: %d\n",lastButtonState);   
        }
        vTaskDelay(50);
    }
}

void buttonISR(){
    button_sem  = xSemaphoreCreateBinary();
    
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << BUTTON_PIN,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };
    
    gpio_config(&io_conf);
    
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN,buttonISRInterrupt,nullptr);

    while (1)
    {
        if (xSemaphoreTake(button_sem, portMAX_DELAY) == pdTRUE) {
            ledMode = (ledMode == LED_ON) ? LED_OFF : LED_ON;
            gpio_set_level(LED_PIN, (ledMode == LED_ON));
            vTaskDelay(pdMS_TO_TICKS(200));
            printf("Button state : %d\n",ledMode);
        }
        vTaskDelay(50);
    }   
}