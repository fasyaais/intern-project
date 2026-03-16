#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include "esp_wifi.h"
#include "esp_mac.h"
#include "esp_log.h"
#include "memory"
#include "app_context.h"
#include "vector"
#include "algorithm"
#include "nvs_config.h"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

class WiFiService
{
private:
    wifi_init_config_t _config;
    static constexpr const char *_TAG = "wifi service";
    // app_context_t _context;
    httpd_handle_t _server_handler;
    static uint8_t _attempt;
    NVSConfig& _nvsConfig;
    static EventGroupHandle_t _wifiEventGroup;
    
    static void _eventHandlerAP(void* arg, esp_event_base_t base, int32_t id, void* data);
    static void _eventHandlerSTA(void* arg, esp_event_base_t base, int32_t id, void* data);
    // void staHandler
public:
    WiFiService(NVSConfig& nvsConfig);
    ~WiFiService();
    
    esp_err_t apInit();
    esp_err_t staInit();
    std::vector<wifi_ap_record_t> scanAP();
    esp_err_t begin();
    void waitConnected();
};



#endif