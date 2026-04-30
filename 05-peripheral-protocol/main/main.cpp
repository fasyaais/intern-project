#include <stdio.h>
#include "circular_buffer_service.h"
#include "driver/uart.h"
#include "uart_service.h"
#include "rfid_driver.h"
#include "lcd_driver.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "esp_lcd_panel_ops.h"
#include "esp_lcd_io_spi.h"
#include "esp_lcd_ili9488.h"
#include "lcd.h"

#include "pn532.h" 
#include "pn532_driver_i2c.h"
#define CONFIG_MIFAREDEBUG

#include "mpu_driver.h"
#include "mpu_service.h"

extern "C"
{
    void app_main(void);
}


void app_main(void)
{
    static UartService uartServ(UART_NUM_0);
    
    SemaphoreHandle_t i2c_mutex = xSemaphoreCreateMutex();

    const int uart_buffer_size = (1024 * 2);
    uart_config_t uart_config = {};
    uart_config.baud_rate = 115200;
    uart_config.data_bits = UART_DATA_8_BITS;
    uart_config.parity = UART_PARITY_DISABLE;
    uart_config.stop_bits = UART_STOP_BITS_1;
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_config.rx_flow_ctrl_thresh = 122;

    ESP_ERROR_CHECK(uart_driver_install(uartServ.getUartNum(), uart_buffer_size, uart_buffer_size, 10, uartServ.getQueueHandle(), 0));
    ESP_ERROR_CHECK(uart_param_config(uartServ.getUartNum(), &uart_config));
    uart_set_pin(uartServ.getUartNum(), UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // gpio_reset_pin(GPIO_NUM_21);
    // gpio_reset_pin(GPIO_NUM_22);
    
    i2c_master_bus_config_t i2c_mst_config = {};
    i2c_mst_config.clk_source = I2C_CLK_SRC_DEFAULT;
    i2c_mst_config.i2c_port = I2C_NUM_0;
    i2c_mst_config.scl_io_num = GPIO_NUM_22;
    i2c_mst_config.sda_io_num = GPIO_NUM_21;
    // i2c_mst_config.glitch_ignore_cnt = 7;
    i2c_mst_config.flags.enable_internal_pullup = true;

    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

    // for (size_t i = 0; i < 256; i++)
    // {
    //     if(i2c_master_probe(bus_handle, i, -1) == ESP_OK){
    //         ESP_LOGI("i2c","found device addr %02X",i);
    //     }
    // }

    // pn532_io_t pn532_io;
    // esp_err_t err;
    // ESP_ERROR_CHECK(pn532_new_driver_i2c(GPIO_NUM_21, GPIO_NUM_22, GPIO_NUM_NC, GPIO_NUM_NC, 0, &pn532_io));
    
    // RFIDDriver rfid(pn532_io);
    // rfid.init();

    MPUDriver mpuDriver((i2c_master_bus_handle_t)bus_handle, 0x68);
    MPUService mpuService(mpuDriver);
    mpuDriver.init();
    float temp = mpuService.getTemp();
    ESP_LOGI("tag", "data temp : %f",temp);
    
    spi_device_handle_t spi;
    spi_bus_config_t buscfg = {};
    buscfg.miso_io_num = GPIO_NUM_19;
    buscfg.mosi_io_num = GPIO_NUM_23;
    buscfg.sclk_io_num = GPIO_NUM_18;
    buscfg.quadwp_io_num = GPIO_NUM_NC;
    buscfg.quadhd_io_num = GPIO_NUM_NC;
    // buscfg.max_transfer_sz = (16 * 320 * 2 + 8);

    spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);

    lcd_config_t lcd_config = {};
    lcd_config.bus = SPI2_HOST;
    lcd_config.cs_pin = GPIO_NUM_5;
    lcd_config.dc_pin = GPIO_NUM_4;
    lcd_config.rst_pin = GPIO_NUM_15;
    lcd_config.frequency = 2*1000*1000;

    // LcdDriver lcd(lcd_config);
    // lcd.init();
    // lcd.draw();
    xTaskCreate(UartService::start,"uart_write_task",4096,(void*)&uartServ,3,nullptr);
    xTaskCreate(MPUService::start,"mpu_sensor_task",4096,(void*)&mpuService,3,nullptr);
    // xTaskCreate(RFIDDriver::start,"rfid_task",4096,(void*)&rfid,3,nullptr);
    // xTaskCreate(I2CDriver::start,"rfid_task",4096,(void*)&rfid,3,nullptr);

    // printf("hello world\n");
    // CircularBufferService<uint8_t> circBuff(20);
    // circBuff.put(20);
    // auto val = circBuff.get();
    // printf("%d\n", *val);
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
