#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include "led_mode.h"
#include "driver/gpio.h"

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
    gpio_num_t getPin();
};

#endif