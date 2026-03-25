#ifndef GPIO_MANAGER_H
#define GPIO_MANAGER_H

#include <map>
#include "led_driver.h"
#include "button_driver.h"
#include <memory>
#include <vector>
#include <esp_log.h>

class GPIOManager
{
private:
    static constexpr const char* _TAG = "gpio manager";

    std::map<gpio_num_t,std::unique_ptr<LedDriver>>     _leds;
    std::map<gpio_num_t,std::unique_ptr<ButtonDriver>>  _buttons;
public:
    GPIOManager(std::vector<gpio_num_t> leds,std::vector<gpio_num_t> buttons);
    ~GPIOManager();

    bool hasLed(gpio_num_t pin);
    bool hasButton(gpio_num_t pin);

    ButtonDriver *getButton(gpio_num_t pin);
    LedDriver *getLed(gpio_num_t pin);
};

#endif