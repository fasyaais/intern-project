#include "gpio_manager.h"

GPIOManager::GPIOManager(std::vector<gpio_num_t> leds,std::vector<gpio_num_t> buttons)
{
    for (auto pin : leds)
    {
        _leds[pin] = std::make_unique<LedDriver>(pin);
    }

    for (auto pin : buttons)
    {
        _buttons[pin] = std::make_unique<ButtonDriver>(pin);
    }
    
}

GPIOManager::~GPIOManager()
{
}

bool GPIOManager::hasLed(gpio_num_t pin){
    return (_leds.count(pin) > 0);
}

bool GPIOManager::hasButton(gpio_num_t pin){
    return (_buttons.count(pin) > 0);
}

ButtonDriver *GPIOManager::getButton(gpio_num_t pin){
    auto it = _buttons.find(pin);
    if(it == _buttons.end()){
        ESP_LOGE(_TAG,"pin %d not found",pin);
        return nullptr;
    }

    return it->second.get();
}

LedDriver *GPIOManager::getLed(gpio_num_t pin){
    if(!hasLed(pin)){
        ESP_LOGE(_TAG,"Pin not detected");
        return nullptr;
    }
    auto it = _leds.find(pin);
    if(it == _leds.end()){
        ESP_LOGE(_TAG,"pin %d not found",pin);
        return nullptr;
    }

    return it->second.get();
}
