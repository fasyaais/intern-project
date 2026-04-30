#ifndef MPU_H
#define MPU_H

struct accel_t
{
    float x;
    float y;
    float z;
};

struct gyro_t
{
    float x;
    float y;
    float z;
};

struct mpu_data_t
{
    float temp;
    accel_t accel;
    gyro_t gyro;
};


#endif