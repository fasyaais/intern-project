#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include "driver/i2c_master.h"
#include "esp_log.h"
#include "pn532.h"

class RFIDDriver
{
private:
    const char* _TAG = "mpu_driver";

    pn532_io_t& _handle;
    i2c_master_dev_handle_t _dev_handle;
    uint16_t _addr;
public:
    RFIDDriver(pn532_io_t& handle);

    esp_err_t init();
    // void read();
    static void start(void *args);
};



#endif