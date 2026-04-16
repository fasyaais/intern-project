#ifndef LCD_CONFIG_H
#define LCD_CONFIG_H

#include "soc/gpio_num.h"

struct lcd_config_t
{
    gpio_num_t dc_pin;
    gpio_num_t cs_pin;
    gpio_num_t rst_pin;
    uint32_t frequency;
    int bus;
};


#endif