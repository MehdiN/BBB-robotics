#include <iostream>
#include <string>
#include <math.h>
#include <ctime>
#include "../include/mpu9250.h"

int main(){
    // Create a I2C device to MPU adress
    int bus = 2;

    BBB_I2C_Device mpu9k(MPU9250_ADDRESS,bus);
    BBB_I2C_Device ak89(AK8963_ADDRESS,bus);
    IMU_MPU9250 inertial_unit(mpu9k,ak89);

    // uint8_t ret;
    // mpu9k.bbb_i2c_init();
    // mpu9k.bbb_i2c_readByte(0x75,&ret);
    // printf("Data read: %x \n",ret);

    // std::clock_t start = std::clock();
    // double duration(0);

    inertial_unit.initMPU9250();
    inertial_unit.initAK8963();

    int16_t gyro_data[3];
    int16_t accel_data[3];
    int16_t compass_data[3];
    int16_t temp_data;
    float accelRes;
    float gyroRes;
    float magRes;

    accelRes = 1/(float)inertial_unit.getAccelScale();
    gyroRes = 1/(float)inertial_unit.getGyroScale();
    magRes = inertial_unit.getMagRes()/100.0f;
    while(1){
        // duration = (std::clock() - start)/ (double) CLOCKS_PER_SEC;
        inertial_unit.readRawGyro(gyro_data);
        inertial_unit.readRawAccel(accel_data);
        inertial_unit.readRawMag(compass_data);
        temp_data = inertial_unit.readRawTemp();
        std::cout << "DISPLAY" << std::endl;
        printf("gx: %.5f | gy: %.5f | gz: %.5f \n",(float)gyro_data[0]*gyroRes,(float)gyro_data[1]*gyroRes,(float)gyro_data[2]*gyroRes);
        printf("ax: %.5f | ay: %.5f | az: %.5f \n",(float)accel_data[0]*accelRes,(float)accel_data[1]*accelRes,(float)accel_data[2]*accelRes);
        printf("mx: %.5f | my: %.5f | mz: %.5f \n",(float)compass_data[0]*magRes,(float)compass_data[1]*magRes,(float)compass_data[2]*magRes);
        printf("Temperature is %.1f°C \n",temp_data / 333.87 + 21.0);
        std::cout << "==================================\n" << std::endl;
        usleep(200000);
     }
    return 0;

}
