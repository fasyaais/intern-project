#include "mpu_driver.h"

MPUDriver::MPUDriver(i2c_master_bus_handle_t bus_handle, uint16_t addr): _bus_handle(bus_handle),_addr(addr)
{
}


esp_err_t MPUDriver::init(){
    esp_err_t err;

    _dev_cfg = {};
    _dev_cfg.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    _dev_cfg.device_address = _addr;
    _dev_cfg.scl_speed_hz = 100000;
    ESP_LOGI("MPU-DRIVER","Init MPU driver");

    err = i2c_master_bus_add_device(_bus_handle, &_dev_cfg, &_dev_handle);
    
    return err;
}

void MPUDriver::read(){
    uint8_t reg_addr = 0x3b;
    uint8_t buffer[6];
    ESP_ERROR_CHECK(i2c_master_transmit_receive(_dev_handle, &reg_addr, 1, buffer, sizeof(buffer), -1));
    float x_accel = ((buffer[0] << 8) | buffer[1]) / 16384.0;
    ESP_LOGI("taG","response: %f",x_accel);
    // ESP_LOGI("taG","response: %02X %02X %02X %02X %02X %02X",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);

}

std::array<uint8_t,6> MPUDriver::readRawAccel(){
    std::array<uint8_t,6> buffer;
    ESP_ERROR_CHECK(i2c_master_transmit_receive(_dev_handle, &_accel_reg, 1, buffer.data(), buffer.size(), -1));
    return buffer;
}

std::array<uint8_t,6> MPUDriver::readRawGyro(){
    std::array<uint8_t,6> buffer;
    ESP_ERROR_CHECK(i2c_master_transmit_receive(_dev_handle, &_gyro_reg, 1, buffer.data(), buffer.size(), -1));
    return buffer;
}

std::array<uint8_t,2> MPUDriver::readRawTemp(){
    std::array<uint8_t,2> buffer;
    ESP_ERROR_CHECK(i2c_master_transmit_receive(_dev_handle, &_temp_reg, 1, buffer.data(), buffer.size(), -1));
    return buffer;
}

// accel_t MPUDriver::getAccel(){
//     accel_t data  = {};
//     auto raw_data = _readRawAccel();
//     data.x = ((raw_data[0] << 8) | raw_data[1]);
//     data.y = ((raw_data[2] << 8)| raw_data[3]);
//     data.z = ((raw_data[4] << 8)| raw_data[5]);

//     return data;
// }

// gyro_t MPUDriver::getGyro(){
//     gyro_t data  = {};
//     auto raw_data = _readRawGyro();
//     data.x = ((raw_data[0] << 8) | raw_data[1]);
//     data.y = ((raw_data[2] << 8)| raw_data[3]);
//     data.z = ((raw_data[4] << 8)| raw_data[5]);

//     return data;
// }

// float MPUDriver::getTemp(){
//     auto raw_data = _readRawTemp();
//     int16_t raw_temp = (raw_data[0] << 8 ) | raw_data[1];
//     float data = (raw_temp / 340.0) +  36.53;

//     return data;
// }
