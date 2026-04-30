#include <stdio.h>

#include "system_manager.h" 
#include "esp_mac.h"

extern "C" {
    void app_main();
}


void app_main(){
    
    std::vector<gpio_num_t>gp_input{GPIO_NUM_4,GPIO_NUM_39,GPIO_NUM_36,GPIO_NUM_34,GPIO_NUM_14,GPIO_NUM_35,GPIO_NUM_15};
    
    std::vector<gpio_num_t> gp_output{GPIO_NUM_26,GPIO_NUM_33,GPIO_NUM_25,GPIO_NUM_13,GPIO_NUM_27,GPIO_NUM_12,GPIO_NUM_2};

    std::unique_ptr<SystemManager> sm = std::make_unique<SystemManager>(gp_output,gp_input);
    sm->start();
    ESP_LOGI("MEM", "Size of SystemManager: %d bytes", sizeof(SystemManager));
    uint8_t mac[6];
    esp_base_mac_addr_get(mac);
    ESP_LOGI("ESP MODULE","mac "MACSTR,MAC2STR(mac));
    // sm.release();
    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
}