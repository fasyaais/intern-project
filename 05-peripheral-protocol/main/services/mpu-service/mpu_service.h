#ifndef MPU_SERVICE_H
#define MPU_SERVICE_H

#include "mpu_driver.h"
#include "freertos/FreeRTOS.h"

class MPUService
{
private:
    // i2c_master_bus_handle_t bus_handle,uint16_t addr
    MPUDriver& _driver;
public:
    MPUService(MPUDriver& driver);
    ~MPUService();
    
    accel_t getAccel();
    gyro_t getGyro();
    float getTemp();

    static void start(void *args);
};


#endif