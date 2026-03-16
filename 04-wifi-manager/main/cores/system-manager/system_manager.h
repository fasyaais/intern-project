#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "nvs_config.h"
#include "wifi_service.h"
#include "ap_router.h"
#include "ap_controller.h"
#include "http_client_service.h"
#include "time_service.h"
#include "led_service.h"
#include "http_client_service.h"

#define WIFI_SSID "ESP TASK 2024"
#define WIFI_PASS "password"

// #define TIME_PERIOD (86400000000ULL)
#define TIME_PERIOD (10*1000000)

class SystemManager
{
private:
    NVSConfig _nvsConfig;

    LedDriver _ledDriver;
    ButtonDriver _buttonDriver;
    WiFiService _wifiService;
    HTTPClientService _clientService;
    TimeService _timeService;
    LedService _ledService;
    
    APController _apController;
    APRouter _apRouter;
    
    HTTPService _httpService;

    esp_timer_handle_t _nvs_update_timer;
    TaskHandle_t _blinkHandler;

    static void _ledTask(void* args);
    static void _ledBlinkTask(void* args);
public:
    SystemManager();
    void start();
};

#endif