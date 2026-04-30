#ifndef LED_SERVICE_H
#define LED_SERVICE_H

#include "led_driver.h"
#include "button_driver.h"
#include "time_service.h"
#include "http_client_service.h"
#include "cJSON.h"
#include "memory"
#include "gpio_manager.h"
#include "gpio.h"
#include "storage_service.h"

class LedService
{
private:
    static constexpr const char* _TAG = "led service";

    // LedDriver& _ledDriver;
    // ButtonDriver& _buttonDriver;
    TimeService& _timeService;
    GPIOManager& _gpioManager;
    HTTPClientService& _httpClientService;

    ButtonDriver* _button = _gpioManager.getButton(GPIO_NUM_4);
    LedDriver* _led = _gpioManager.getLed(GPIO_NUM_26);
    StorageService& _storageService;
    // Response _response;
public:
    LedService(TimeService& timeService,GPIOManager& gpioManager,HTTPClientService& httpClientService,StorageService& storage);
    // LedService(LedDriver& ledDriver,ButtonDriver& buttonDriver,TimeService& timeService,HTTPClientService& httpClientService);
    // ~LedService();

    void init();
    void blink();
};

#endif