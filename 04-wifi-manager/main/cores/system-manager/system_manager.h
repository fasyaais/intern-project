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
#include "gpio_manager.h"
#include "storage_service.h"

#define WIFI_SSID "ESP TASK 2024"
#define WIFI_PASS "password"

// #define TIME_PERIOD (86400000000ULL)
#define TIME_PERIOD (10*1000000)

class SystemManager
{
private:
    std::vector<gpio_num_t> _leds;
    std::vector<gpio_num_t> _buttons;

    NVSConfig _nvsConfig;

    // LedDriver _ledDriver;
    // ButtonDriver _buttonDriver;
    GPIOManager _gpioManager;
    WiFiService _wifiService;
    HTTPClientService _clientService;
    TimeService _timeService;
    StorageService _storageService;
    LedService _ledService;
    
    APController _apController;
    APRouter _apRouter;
    
    HTTPService _httpService;

    esp_timer_handle_t _nvs_update_timer;
    TaskHandle_t _blinkHandler;

    static void _ledTask(void* args);
    static void _ledBlinkTask(void* args);
public:
    SystemManager(std::vector<gpio_num_t> leds, std::vector<gpio_num_t> buttons);
    void start();
};

#endif