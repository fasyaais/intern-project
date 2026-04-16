#include "i2c_driver.h"

I2CDriver::I2CDriver(i2c_master_bus_handle_t bus_handle,uint16_t addr):_bus_handle(bus_handle),_addr(addr)
{
    i2c_device_config_t dev_cfg = {};
    // dev_cfg.dev_addr_length = I2C_ADDR_BIT_LEN_;
    dev_cfg.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    dev_cfg.device_address = addr;
    dev_cfg.scl_speed_hz = 100000;

    ESP_ERROR_CHECK(i2c_master_bus_add_device(_bus_handle, &dev_cfg, &_dev_handle));
}

I2CDriver::~I2CDriver()
{
    i2c_master_bus_rm_device(_dev_handle);
}

void I2CDriver::read() {
    uint8_t reg = _addr;
    // uint8_t reg = 0x37;
    std::array<uint8_t, 256> rx_buffer = {0}; 

    // esp_err_t ret = i2c_master_transmit_receive(
    //     _dev_handle,
    //     &reg, 1,
    //     data_rd.data(), data_rd.size(),
    //     pdMS_TO_TICKS(100) // Timeout 100ms
    // );
    esp_err_t ret =i2c_master_receive(_dev_handle,rx_buffer.data(),rx_buffer.size(),100);

    if (ret == ESP_OK) {
        ESP_LOGI("RFID", "Data: %02X %02X %02X", rx_buffer[0], rx_buffer[1], rx_buffer[2]);
    } else {
        ESP_LOGE("RFID", "Gagal baca: %s", esp_err_to_name(ret));
    }
}

void I2CDriver::start(void *args){
    I2CDriver* self = static_cast<I2CDriver*>(args);
    for (;;)
    {
        self->read();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}
