#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include "esp_wifi.h"
#include "esp_mac.h"
#include "esp_log.h"
#include "memory"

class WiFiService
{
private:
    wifi_init_config_t _config;
    static constexpr const char *_TAG = "wifi service";

    void _eventHandlerDispatcher(void* arg, esp_event_base_t base, int32_t id, void* data);
    // void staHandler
public:
    WiFiService(/* args */);
    ~WiFiService();

    void begin();
    
};



#endif