#include "i2c_driver.h"

I2CDriver::I2CDriver(i2c_master_bus_handle_t* bus_handle,i2c_master_dev_handle_t* dev_handle):_bus_handle(bus_handle),_dev_handle(dev_handle)
{
}

I2CDriver::~I2CDriver()
{
    i2c_master_bus_rm_device(*_dev_handle);
}

void I2CDriver::read(){
    // i2c_master_transmit_receive();
    // uint8_t data_rd[6];
    std::array<uint8_t,6> data_rd;
    i2c_master_receive(*_dev_handle, data_rd.data(), 6, -1);
}
