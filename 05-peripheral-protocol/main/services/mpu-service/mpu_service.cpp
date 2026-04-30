#include "mpu_service.h"

MPUService::MPUService(MPUDriver& driver): _driver(driver)
{
}


accel_t MPUService::getAccel(){
    accel_t data  = {};
    auto raw = _driver.readRawAccel();
    int16_t rawX = (int16_t)((raw[0] << 8) | raw[1]);
    int16_t rawY = (int16_t)((raw[2] << 8) | raw[3]);
    int16_t rawZ = (int16_t)((raw[4] << 8) | raw[5]);

    data.x = (float)rawX / 16384.0f;
    data.y = (float)rawY / 16384.0f;
    data.z = (float)rawZ / 16384.0f;

    return data;
}

gyro_t MPUService::getGyro(){
    gyro_t data  = {};
    auto raw = _driver.readRawGyro();
    
    int16_t rawX = (int16_t)((raw[0] << 8) | raw[1]);
    int16_t rawY = (int16_t)((raw[2] << 8) | raw[3]);
    int16_t rawZ = (int16_t)((raw[4] << 8) | raw[5]);

    data.x = (float)rawX / 131.0f;
    data.y = (float)rawY / 131.0f;
    data.z = (float)rawZ / 131.0f;

    return data;
}

float MPUService::getTemp(){
    auto raw_data = _driver.readRawTemp();
    int16_t raw_temp = (raw_data[0] << 8 ) | raw_data[1];
    float data = (raw_temp / 340.0) +  36.53;

    return data;
}

void MPUService::start(void *args){
    auto* it = static_cast<MPUService*>(args);
    
    for(;;){

        ESP_LOGI("tag","================================");
        accel_t accel_data = it->getAccel();
        ESP_LOGI("tag","accel x = %f, y = %f, z = %f", accel_data.x, accel_data.y, accel_data.z);
        gyro_t gyro_data = it->getGyro();
        ESP_LOGI("tag","gyro x = %f, y = %f, z = %f", gyro_data.x, gyro_data.y, gyro_data.z);
        float temp = it->getTemp();
        ESP_LOGI("tag","temp = %f", temp);
        ESP_LOGI("tag","================================\n");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}