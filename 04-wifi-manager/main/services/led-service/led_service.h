#ifndef LED_SERVICE_H
#define LED_SERVICE_H

#include "led_driver.h"
#include "button_driver.h"
#include "time_service.h"
#include "http_client_service.h"
#include "cJSON.h"
#include "memory"

class LedService
{
private:
    static constexpr const char* _TAG = "led service";

    LedDriver& _ledDriver;
    ButtonDriver& _buttonDriver;
    TimeService& _timeService;
    HTTPClientService& _httpClientService;
    // Response _response;
public:
    LedService(LedDriver& ledDriver,ButtonDriver& buttonDriver,TimeService& timeService,HTTPClientService& httpClientService);
    // ~LedService();

    void init();
    void blink();
};

#endif