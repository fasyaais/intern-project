#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <driver/gpio.h>
#include "led_mode.h"

class LedDriver
{
private:
    gpio_config_t _io_conf;
    gpio_num_t _pin;
    bool _lastState;
public:
    LedDriver(gpio_num_t pin);

    void turnON();
    void turnOFF();
    bool isON();
};

#endif