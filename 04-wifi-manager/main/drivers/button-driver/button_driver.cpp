#include "button_driver.h"

ButtonDriver::ButtonDriver(gpio_num_t pin):_pin(pin){
    _io_config.pin_bit_mask = 1ULL <<_pin;
    _io_config.mode = GPIO_MODE_INPUT;
    _io_config.pull_up_en = GPIO_PULLUP_ENABLE;
    _io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    _io_config.intr_type = GPIO_INTR_NEGEDGE;

    gpio_config(&_io_config);
    _debounce_tick = pdMS_TO_TICKS(200);
    _last_reading = gpio_get_level(_pin);
    _stable_state = _last_reading;
}

ButtonDriver::ButtonDriver(gpio_num_t pin,uint16_t ms):_pin(pin){
    _io_config.pin_bit_mask = 1ULL <<_pin;
    _io_config.mode = GPIO_MODE_INPUT;
    _io_config.pull_up_en = GPIO_PULLUP_ENABLE;
    _io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    _io_config.intr_type = GPIO_INTR_NEGEDGE;
    ESP_ERROR_CHECK(gpio_config(&_io_config));

    _debounce_tick = pdMS_TO_TICKS(ms);
    _last_reading = gpio_get_level(_pin);
    _stable_state = _last_reading;
    
}

bool ButtonDriver::click(){
    uint8_t current_reading = gpio_get_level(_pin);
    _clicked = false;
    if (current_reading != _last_reading)
    {
        _last_debounce_time = xTaskGetTickCount();
        _last_reading = current_reading;
    }

    if ((xTaskGetTickCount() - _last_debounce_time) > _debounce_tick)
    {
        if (_stable_state != current_reading)
        {
            _stable_state = current_reading;
            if (_stable_state == 0) 
            {
                _clicked = true;
            }
        }
    }
    vTaskDelay(pdMS_TO_TICKS(10));
    return _clicked;
}