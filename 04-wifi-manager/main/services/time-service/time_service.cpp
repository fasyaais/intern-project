#include "time_service.h"

TimeService::TimeService(NVSConfig& nvsConfig):_nvsConfig(nvsConfig){}

void TimeService::init(){
    ESP_LOGI(_TAG,"Initializing SNTP");
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG_MULTIPLE(1,ESP_SNTP_SERVER_LIST("pool.ntp.org"));
    setenv("TZ","WIB-7",1);
    tzset();
    esp_netif_sntp_init(&config);
}

esp_err_t TimeService::_obtainTime(){
    int retry = 0;
    const int retry_count = 10;
    while (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(2000)) != ESP_OK && ++retry < retry_count) {
        ESP_LOGI(_TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
    }
    if (retry == retry_count) {
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t TimeService::fetchTime(){
    esp_err_t err;
    init();
    if (_obtainTime() != ESP_OK)
    {
        err = ESP_FAIL;
    }

    time(&_now);
    ESP_LOGI(_TAG,"timestamp: %lld",_now);
    err = _nvsConfig.writeNumber("timestamp",_now);

    if(err != ESP_OK){
        goto exit;
    }

exit:
    esp_netif_sntp_deinit();
    if (err != ESP_OK) {
        ESP_LOGE(_TAG, "Error updating time in nvs");
    } else {
        ESP_LOGI(_TAG, "Updated time in NVS");
    }
    return err;
}

uint64_t TimeService::getNow(){
    time(&_now);
    return _now;
}