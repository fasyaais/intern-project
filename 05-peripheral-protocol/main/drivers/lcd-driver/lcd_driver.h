#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include "esp_lcd_ili9488.h"
#include "esp_lcd_io_spi.h"
#include "esp_lcd_panel_ops.h"
#include "lcd.h"
#include "freertos/FreeRTOS.h"

#define LCD_H_RES 480
#define LCD_W_RES 320
#define CHUNK 20

class LcdDriver
{
private:
    lcd_config_t _lcd_config;

    esp_lcd_panel_io_handle_t _io_handle;
    esp_lcd_panel_io_spi_config_t _io_config;

    esp_lcd_panel_handle_t _panel_handle;
    esp_lcd_panel_dev_config_t _panel_config;
public:
    LcdDriver(lcd_config_t lcd_config);
    ~LcdDriver();

    esp_err_t init();
    void draw();
};


#endif