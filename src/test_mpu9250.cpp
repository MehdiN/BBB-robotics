#include <iostream>
#include <string>
#include <math.h>
#include <ctime>

#include "AK8963.h"
#include "MPU9250.h"


int main()
{
    uint8_t bus;
    bus = 2;
    MPU9250 imu(bus,MPU9250_ADDRESS);
    AK8963 mag(bus,AK8963_ADDRESS);


    imu.init();
    mag.init(imu);


    int16_t gyro_data[3];
    int16_t accel_data[3];
    int16_t mag_data[3];
    int16_t temp_data;
    float accelRes;
    float gyroRes;
    float magRes;


    accelRes = 1/(float)imu.getAccelScale();
    gyroRes = 1/(float)imu.getGyroScale();
    magRes = mag.getResolution()/100.0f;



    imu.readRawAccel(accel_data);
    imu.readRawTemp(gyro_data);
    mag.readRawMag(mag_data);
    temp_data = imu.readRawTemp();

}