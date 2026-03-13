#include "system_manager.h"

SystemManager::SystemManager():
        _ledDriver(GPIO_NUM_2),
        _buttonDriver(GPIO_NUM_4),
        _wifiService(_nvsConfig),
        _timeService(_nvsConfig),
        _ledService(_ledDriver,_buttonDriver,_timeService,_clientService),
        _apController(_wifiService,_nvsConfig),
        _apRouter(_apController),
        _httpService(_nvsConfig,_apRouter)
    {
        
    }

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
    auto authmode = _nvsConfig.read("authmode");

    _wifiService.begin();
    
    if(ssid.get() == nullptr){
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

        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
        ESP_ERROR_CHECK(esp_wifi_start());
        _httpService.start(8080);
        _apRouter.registerRouter(_httpService.getServer());
    }else{
        ESP_LOGI("system manager","ssid: %s, pass: %s", ssid.get(), pass.get());
        ESP_ERROR_CHECK(_wifiService.staInit());
        wifi_config_t wifi_config = {};

        if(ssid.get() != nullptr){
            strncpy((char*)wifi_config.sta.ssid,ssid.get(),sizeof(wifi_config.sta.ssid) -1);
        }

        if(pass.get() != nullptr){
            strncpy((char*)wifi_config.sta.password,pass.get(),sizeof(wifi_config.sta.password) -1);
        }

        wifi_config.sta.sae_pwe_h2e = WPA3_SAE_PWE_HUNT_AND_PECK;
        wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
        
        // if(authmode.get() == nullptr){
        //     wifi_config.sta.threshold.authmode = WIFI_AUTH_OPEN;
        // }else{
        //     uint8_t mode = atoi(authmode.get());
        //     wifi_config.sta.threshold.authmode = static_cast<wifi_auth_mode_t>(mode);
        // }
        
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
        ESP_ERROR_CHECK(esp_wifi_start() );
        _wifiService.waitConnected();

        _timeService.fetchTime();

        esp_timer_create_args_t nvs_update_timer_args = {};
        nvs_update_timer_args.arg = (void*)&_timeService;
        nvs_update_timer_args.callback = [](void *args){
            auto* service = static_cast<TimeService*>(args);
            for(;;) {
                service->fetchTime();
                vTaskDelay(pdMS_TO_TICKS(TIME_PERIOD)); // Update setiap periode tertentu
            }
        };

        // esp_timer_handle_t nvs_update_timer;
        // ESP_ERROR_CHECK(esp_timer_create(&nvs_update_timer_args, &_nvs_update_timer));
        // ESP_ERROR_CHECK(esp_timer_start_periodic(_nvs_update_timer, TIME_PERIOD));

        
        _clientService.begin("http://"PATH":3000/");
        const char *data = "{\"field1\":\"value1\"}";
        // _clientService.post(data);
        _clientService.get();

        xTaskCreate(&_ledTask,"led task",4096,this,3,nullptr);        
    }
}

void SystemManager::_ledTask(void* args){
    auto* self = static_cast<SystemManager*>(args);
    self->_ledService.init();
    vTaskDelete(nullptr);
}