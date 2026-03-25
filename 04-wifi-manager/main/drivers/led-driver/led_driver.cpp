#include "led_driver.h"

LedDriver::LedDriver(gpio_num_t pin):_pin(pin){
    _io_conf.pin_bit_mask = 1ULL << _pin;
    _io_conf.mode = GPIO_MODE_OUTPUT;
    gpio_config(&_io_conf);
    _lastState = false;
}

void LedDriver::turnON(){
    _lastState = true;
    gpio_set_level(_pin,true);
}

void LedDriver::turnOFF(){
    _lastState = false;
    gpio_set_level(_pin,false);
}

bool LedDriver::isON(){
    return _lastState;
}

gpio_num_t LedDriver::getPin(){
    return _pin;
}