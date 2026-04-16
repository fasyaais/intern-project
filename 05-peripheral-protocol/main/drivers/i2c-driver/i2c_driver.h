#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include "driver/i2c_master.h"
#include "array"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

class I2CDriver
{
private:
    const char* _TAG = "mpu_driver";

    i2c_master_bus_handle_t _bus_handle;
    i2c_master_dev_handle_t _dev_handle;
    uint16_t _addr;
public:
    I2CDriver(i2c_master_bus_handle_t bus_handle,uint16_t addr);
    ~I2CDriver();

    void read();
    void write();
    static void start(void *args);
};



#endif