#include <stdio.h>

#include "system_manager.h" 

extern "C" {
    void app_main();
}

// void app_main(void)
// {
//     // Inisasi NVS
//     esp_err_t err = nvs_flash_init();
//     if(err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND){
//         ESP_ERROR_CHECK(nvs_flash_erase());
//         err = nvs_flash_init();
//     }
//     ESP_ERROR_CHECK(err);

//     ESP_ERROR_CHECK(esp_netif_init());
//     ESP_ERROR_CHECK(esp_event_loop_create_default());

//     NVSConfig nvsConfig = NVSConfig();
//     ESP_ERROR_CHECK(nvsConfig.open());
//     nvsConfig.write("msg","Hello write");
//     auto hello = nvsConfig.read("msg");
//     ESP_LOGI("monitor","%s",hello.get());
//     ESP_ERROR_CHECK(nvsConfig.open());
// }

void app_main(){
    static SystemManager sm;
    sm.start();
}