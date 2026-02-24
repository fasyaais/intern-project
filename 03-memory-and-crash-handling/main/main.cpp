#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include <cstdint>

void load_fault();
void stack_overflow(void *pvParameters);
void instr_fetch();
void store_proh();

extern "C" {
    void app_main();
}


void app_main(void)
{
    // xTaskCreate(
    //     stack_overflow,
    //     "LED blink",
    //     512,
    //     NULL,
    //     3,
    //     NULL
    // );

    // load_fault();
    // illegal_inst();
    // store_proh();
    instr_fetch();
}

void stack_overflow(void *pvParameters)
{
    TickType_t ledTick = xTaskGetTickCount();
    uint32_t ledPeriod = 1000;
    bool stateLED = false;
    gpio_num_t led_pin = GPIO_NUM_2;

    gpio_set_direction(led_pin,GPIO_MODE_OUTPUT);
    while (true)
    {
        if((xTaskGetTickCount() - ledTick) > (ledPeriod/2)){
            printf("debug\n");
            stateLED = !stateLED;
            gpio_set_level(led_pin, stateLED);
            ledTick = xTaskGetTickCount();
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void load_fault(){
    uint8_t load;
    while (1)
    {   
        uint8_t *buff = nullptr;
        load = buff[1];
        printf("%d\n",load);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

typedef struct {
    uint8_t buf[10];
    uint8_t data;
} store_test_t;

void store_access_fault(store_test_t * store_test)
{
    store_test->data = 0x5F;
    printf("Data: %d\n", store_test->data);
}

void store_proh(){
    store_test_t * store_test = NULL;
    store_access_fault(store_test);
}

void instr_fetch()
{
    typedef void (*fptr_t)(void);
    volatile fptr_t fptr = (fptr_t) 0x4;
    fptr();
}