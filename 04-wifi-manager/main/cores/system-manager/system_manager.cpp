#include "system_manager.h"

SystemManager::SystemManager():
        _apController(_wifiService,_nvsConfig),
        _apRouter(_apController),
        _httpService(_nvsConfig,_apRouter)
            {}

void SystemManager::start(){
    esp_err_t err = nvs_flash_init();
    if(err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND){
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    
    ESP_ERROR_CHECK(err);
    ESP_ERROR_CHECK(_nvsConfig.open());
    auto ssid = _nvsConfig.read("ssid");
    auto pass = _nvsConfig.read("password");
    
    _nvsConfig.erase("ssid");
    
    _wifiService.begin();
    
    if(ssid == nullptr){
        _wifiService.apInit();
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
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    }
    // else if(ssid){
    //     _wifiService.staInit();
    // }
    
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_wifi_start());

    _httpService.start(8080);

    _apRouter.registerRouter(_httpService.getServer());
}