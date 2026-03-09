#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include "esp_wifi.h"
#include "esp_mac.h"
#include "esp_log.h"
#include "memory"
#include "app_context.h"

class WiFiService
{
private:
    wifi_init_config_t _config;
    static constexpr const char *_TAG = "wifi service";
    // app_context_t _context;
    httpd_handle_t _server_handler;
    static uint8_t _attempt;

    static void _eventHandler(void* arg, esp_event_base_t base, int32_t id, void* data);
    // void staHandler
public:
    WiFiService(/* args */);
    esp_err_t apInit();
    esp_err_t staInit();
    void begin();
    
};



#endif