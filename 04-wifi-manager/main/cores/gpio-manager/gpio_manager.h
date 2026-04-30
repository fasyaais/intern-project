#ifndef GPIO_MANAGER_H
#define GPIO_MANAGER_H

#include <map>
#include "led_driver.h"
#include "button_driver.h"
#include <memory>
#include <vector>
#include <esp_log.h>
#include "nvs_config.h"
#include "cJSON.h"

class GPIOManager
{
private:
    static constexpr const char* _TAG = "gpio manager";

    std::map<gpio_num_t,std::unique_ptr<LedDriver>>     _leds;
    std::map<gpio_num_t,std::unique_ptr<ButtonDriver>>  _buttons;
    NVSConfig& _nvs;
public:
    GPIOManager(std::vector<gpio_num_t> leds,std::vector<gpio_num_t> buttons,NVSConfig& nvs);
    ~GPIOManager();

    bool hasLed(gpio_num_t pin);
    bool hasButton(gpio_num_t pin);

    ButtonDriver *getButton(gpio_num_t pin);
    LedDriver *getLed(gpio_num_t pin);
    void lastState();
    // std::map<gpio_num_t,std::unique_ptr<LedDriver>>     getLedList();
    // std::map<gpio_num_t,std::unique_ptr<ButtonDriver>>  getButtonList();
};

#endif