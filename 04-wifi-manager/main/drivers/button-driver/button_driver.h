#ifndef BUTTON_DRIVER_H
#define BUTTON_DRIVER_H

#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"

class ButtonDriver
{
private:
    gpio_config_t _io_config;
    gpio_num_t _pin;
    uint8_t _last_reading;
    uint8_t _stable_state;
    TickType_t _last_debounce_time;
    TickType_t _debounce_tick;
    bool _clicked;
public:
    ButtonDriver(gpio_num_t pin);
    ButtonDriver(gpio_num_t pin,uint16_t ms);
    
    bool click();
};

#endif
