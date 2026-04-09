#include <stdio.h>
#include "circular_buffer_service.h"
#include "driver/uart.h"
#include "uart_service.h"
#include "i2c_driver.h"
#include "driver/gpio.h"

extern "C"
{
    void app_main(void);
}

// QueueHandle_t uart_queue;
// const int uart_buffer_size = (1024 * 2);
// const uart_port_t uart_num = UART_NUM_0;

// void uart_event_task(void *pvParameters){
//     size_t buffered_size;
//     if(xQueueReceive(uart_queue,&buffered_size,portMAX_DELAY)){
        
//     }
// }


void app_main(void)
{
    static UartService uartServ(UART_NUM_0);
    
    uart_config_t uart_config = {};
    uart_config.baud_rate = 115200;
    uart_config.data_bits = UART_DATA_8_BITS;
    uart_config.parity = UART_PARITY_DISABLE;
    uart_config.stop_bits = UART_STOP_BITS_1;
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_config.rx_flow_ctrl_thresh = 122;

    const int uart_buffer_size = (1024 * 2);
    ESP_ERROR_CHECK(uart_driver_install(uartServ.getUartNum(), uart_buffer_size, uart_buffer_size, 10, uartServ.getQueueHandle(), 0));
    ESP_ERROR_CHECK(uart_param_config(uartServ.getUartNum(), &uart_config));
    uart_set_pin(uartServ.getUartNum(), UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    i2c_master_bus_config_t i2c_mst_config = {};
    i2c_mst_config.clk_source = I2C_CLK_SRC_DEFAULT;
    i2c_mst_config.i2c_port = I2C_NUM_0;
    i2c_mst_config.scl_io_num = GPIO_NUM_4;
    i2c_mst_config.sda_io_num = GPIO_NUM_5;
    i2c_mst_config.glitch_ignore_cnt = 7;
    i2c_mst_config.flags.enable_internal_pullup = true;

    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

    i2c_device_config_t dev_cfg = {};
    dev_cfg.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    dev_cfg.device_address = 0x58;
    dev_cfg.scl_speed_hz = 100000;

    i2c_master_dev_handle_t dev_handle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));

    I2CDriver mpu(&bus_handle,&dev_handle);
    
    xTaskCreate(UartService::start,"uart_write_task",4096,(void*)&uartServ,3,nullptr);

    // printf("hello world\n");
    // CircularBufferService<uint8_t> circBuff(20);
    // circBuff.put(20);
    // auto val = circBuff.get();
    // printf("%d\n", *val);
}
