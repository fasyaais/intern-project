#include "led_driver.h"

LedDriver::LedDriver(gpio_num_t pin):_pin(pin){
    _io_conf.pin_bit_mask = 1ULL << _pin;
    _io_conf.mode = GPIO_MODE_OUTPUT;
    gpio_config(&_io_conf);
}

void LedDriver::turnON(){
    gpio_set_level(_pin,true);
    _lastState = true;
}

void LedDriver::turnOFF(){
    gpio_set_level(_pin,false);
    _lastState = false;
}

bool LedDriver::isON(){
    return _lastState;
}