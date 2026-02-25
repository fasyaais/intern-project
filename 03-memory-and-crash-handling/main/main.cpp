#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_system.h"

#define MAP_DRAM_TO_IRAM(addr) (addr)

void load_fault();
void stack_overflow(void *pvParameters);
void instr_fetch();
void store_proh();
void heapTask(void *args);

extern "C" {
    void app_main();
}

void app_main(void)
{
    xTaskCreate(
        stack_overflow,
        "LED blink",
        1024,
        NULL,
        3,
        NULL
    );
    xTaskCreate(heapTask,"Memory Monitor",2048,nullptr,3,nullptr);

    // load_fault();
    // illegal_inst();
    // store_proh();
    // instr_fetch();
    // ilegal_instr();

    void *ptr = malloc(5120);
}

void heapTask(void *args)
{
    static const char* TAG = "MEM_MONITOR";
    while (true)
    {
        uint32_t free_heap = esp_get_free_heap_size();
        uint32_t minimum_free_heap = esp_get_minimum_free_heap_size(); 
        ESP_LOGI(TAG,"-----------------");
        ESP_LOGI(TAG,"Free heap     : %lu",free_heap);
        ESP_LOGI(TAG,"Min Free Heap : %lu",minimum_free_heap);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
    
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