#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include "driver/i2c_master.h"
#include "array"

class I2CDriver
{
private:
    i2c_master_bus_handle_t* _bus_handle;
    i2c_master_dev_handle_t* _dev_handle;
public:
    I2CDriver(i2c_master_bus_handle_t* bus_handle,i2c_master_dev_handle_t* dev_handle);
    ~I2CDriver();

    void read();
    void write();
};



#endif