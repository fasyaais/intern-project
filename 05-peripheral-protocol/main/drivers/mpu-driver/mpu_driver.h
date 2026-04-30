#ifndef MPU_DRIVER_H
#define MPU_DRIVER_H

#include "driver/i2c_master.h"
#include "esp_log.h"
#include "string"
#include "array"
#include "mpu.h"

class MPUDriver
{
private:
    i2c_device_config_t _dev_cfg;
    i2c_master_dev_handle_t _dev_handle;
    i2c_master_bus_handle_t _bus_handle;
    uint16_t _addr;
    uint8_t _accel_reg = 0x3b;
    uint8_t _gyro_reg = 0x43;
    uint8_t _temp_reg = 0x41;

    std::array<uint8_t,6> _raw_accel;
    
    public:
    MPUDriver(i2c_master_bus_handle_t bus_handle,uint16_t addr);
    esp_err_t init();
    void read();
    
    std::array<uint8_t,6> readRawAccel();
    std::array<uint8_t,6> readRawGyro();
    std::array<uint8_t,2> readRawTemp();
};

#endif