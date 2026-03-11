#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "nvs_config.h"
#include "wifi_service.h"
#include "ap_router.h"
#include "ap_controller.h"
#include "http_client_service.h"

#define WIFI_SSID "ESP TASK 2024"
#define WIFI_PASS "password"

class SystemManager
{
private:
    NVSConfig _nvsConfig;
    WiFiService _wifiService;
    HTTPClientService _clientService;
    APController _apController;
    APRouter _apRouter;
    HTTPService _httpService;
public:
    SystemManager();
    void start();
};

#endif