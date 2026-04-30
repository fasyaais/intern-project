#include "lcd_driver.h"

LcdDriver::LcdDriver(lcd_config_t lcd_config):_lcd_config(lcd_config)
{
    _io_handle = nullptr;
    _panel_handle =nullptr;
}

LcdDriver::~LcdDriver()
{
}

esp_err_t LcdDriver::init()
{
    esp_err_t ret;
    
    _io_config = {};
    _io_config.dc_gpio_num = _lcd_config.dc_pin;
    _io_config.cs_gpio_num = _lcd_config.cs_pin;
    _io_config.pclk_hz = _lcd_config.frequency;
    _io_config.lcd_cmd_bits = 8;
    _io_config.lcd_param_bits = 8;
    _io_config.spi_mode = 0;
    _io_config.trans_queue_depth = 10;

    ret = esp_lcd_new_panel_io_spi(_lcd_config.bus,&_io_config,&_io_handle);

    if(ret != ESP_OK) return ret;

    _panel_config = {};
    _panel_config.reset_gpio_num = _lcd_config.rst_pin;
    _panel_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR;
    _panel_config.bits_per_pixel = 18;

    size_t buffer_size = LCD_W_RES * CHUNK;

    ret = esp_lcd_new_panel_ili9488(_io_handle,&_panel_config,buffer_size,&_panel_handle);
    if(ret != ESP_OK) return ret;

    ret = esp_lcd_panel_reset(_panel_handle);
    if(ret != ESP_OK) return ret;
    
    ret = esp_lcd_panel_init(_panel_handle);
    if(ret != ESP_OK) return ret;

    ret = esp_lcd_panel_invert_color(_panel_handle,false);
    if(ret != ESP_OK) return ret;

    ret = esp_lcd_panel_disp_on_off(_panel_handle,true);
    if(ret != ESP_OK) return ret;
    
    return ret;
}

void LcdDriver::draw()
{
    uint16_t *canvas = (uint16_t*)heap_caps_calloc(1, LCD_W_RES * CHUNK * sizeof(uint16_t),MALLOC_CAP_DMA);

    for (size_t i = 0; i < LCD_W_RES * CHUNK; i++)
    {
        canvas[i] = 0x001f;
    }
    
    for (size_t i = 0; i < LCD_H_RES; i += CHUNK)
    {
        int y_end = i + CHUNK;
        esp_lcd_panel_draw_bitmap(_panel_handle,0,i,LCD_W_RES,y_end,canvas);
    }
    
}
