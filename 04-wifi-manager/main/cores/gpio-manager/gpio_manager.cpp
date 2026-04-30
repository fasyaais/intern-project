#include "gpio_manager.h"

GPIOManager::GPIOManager(std::vector<gpio_num_t> leds, std::vector<gpio_num_t> buttons, NVSConfig &nvs) : _nvs(nvs)
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

bool GPIOManager::hasLed(gpio_num_t pin)
{
    return (_leds.count(pin) > 0);
}

bool GPIOManager::hasButton(gpio_num_t pin)
{
    return (_buttons.count(pin) > 0);
}

ButtonDriver *GPIOManager::getButton(gpio_num_t pin)
{
    auto it = _buttons.find(pin);
    if (it == _buttons.end())
    {
        ESP_LOGE(_TAG, "pin %d not found", pin);
        return nullptr;
    }

    return it->second.get();
}

LedDriver *GPIOManager::getLed(gpio_num_t pin)
{
    if (!hasLed(pin))
    {
        ESP_LOGE(_TAG, "Pin not detected");
        return nullptr;
    }
    auto it = _leds.find(pin);
    if (it == _leds.end())
    {
        ESP_LOGE(_TAG, "pin %d not found", pin);
        return nullptr;
    }

    return it->second.get();
}

void GPIOManager::lastState()
{
    auto data_ptr = _nvs.read("data");
    if (!data_ptr)
    {
        ESP_LOGW(_TAG, "Database not found");
        return;
    }
    std::string data = data_ptr.get();

    if (data.empty())
    {
        ESP_LOGW(_TAG, "Database GPIO empty");
        return;
    }
    cJSON *json = cJSON_Parse(data.c_str());
    cJSON *element = nullptr;
    bool found = false;

    cJSON_ArrayForEach(element, json)
    {
        cJSON *pin = cJSON_GetObjectItemCaseSensitive(element, "pin");
        gpio_num_t pinNumber = (gpio_num_t)pin->valueint;
        cJSON *state = cJSON_GetObjectItemCaseSensitive(element, "state");
        LedDriver* led =getLed(pinNumber);
        if(cJSON_IsTrue(state)) led->turnON();
        else led->turnOFF();
    }
    cJSON_free(json);
    json = nullptr;
}

// std::map<gpio_num_t,std::unique_ptr<LedDriver>> GPIOManager::getLedList(){
//     return _leds;
// }
// std::map<gpio_num_t,std::unique_ptr<ButtonDriver>>  GPIOManager::getButtonList(){
//     return _buttons;
// }