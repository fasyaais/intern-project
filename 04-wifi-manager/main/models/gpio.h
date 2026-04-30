#ifndef GPIO_H
#define GPIO_H

#include "driver/gpio.h"
#include "chrono"

typedef struct
{
    gpio_num_t pin;
    bool state;
    uint64_t last_click_at;
} gpio_t;

#endif