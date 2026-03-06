#include <stdio.h>

#include "nvs_config.h"
#include "wifi_service.h"

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
    esp_err_t err = nvs_flash_init();
    if(err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND){
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
    ESP_ERROR_CHECK(err);
    NVSConfig nvsConfig = NVSConfig();
    ESP_ERROR_CHECK(nvsConfig.open());
    auto ssid = nvsConfig.read("ssid");
    auto pass = nvsConfig.read("password");
    
    if(ssid == nullptr){
        
    }
}