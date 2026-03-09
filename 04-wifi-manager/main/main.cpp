#include <stdio.h>

#include "nvs_config.h"
#include "wifi_service.h"

#define WIFI_SSID "ESP TASK 2024"
#define WIFI_PASS "password"

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
    
    ESP_ERROR_CHECK(err);
    NVSConfig nvsConfig = NVSConfig();
    ESP_ERROR_CHECK(nvsConfig.open());
    auto ssid = nvsConfig.read("ssid");
    auto pass = nvsConfig.read("password");

    WiFiService wifiService;
    wifiService.begin();
    wifiService.apInit();
    // wifiService.staInit();

    // TESTING
    uint8_t ssid_len = strlen(WIFI_SSID);
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .ssid_len = ssid_len,
            .channel = 1,
            .authmode = WIFI_AUTH_OPEN,
            .max_connection = 4,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));

    ESP_ERROR_CHECK(esp_wifi_start());
}