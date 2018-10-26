#include <iostream>
#include <string>
#include <math.h>
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

    inertial_unit.initMPU9250();
    inertial_unit.initAK8963();

    int16_t gyro_data[3];
    int16_t accel_data[3];
    int16_t temp_data;
    int16_t aScale;
    int16_t gScale;

    aScale = inertial_unit.getAccelScale();
    gScale = inertial_unit.getGyroScale();


    for(int i=0;i<10;i++){
    //     std::cout<< "BEGIN Read" << std::endl;
        inertial_unit.readGyro(gyro_data);
        inertial_unit.readAccel(accel_data);
        temp_data = inertial_unit.readTemp();
        std::cout << "DISPLAY" << std::endl;
        printf("gx: %f | gy: %f | gz: %f \n",(float)gyro_data[0]/gScale,(float)gyro_data[1]/gScale,(float)gyro_data[2]/gScale);
        printf("ax: %f | ay: %f | az: %f \n",(float)accel_data[0]/aScale,(float)accel_data[1]/aScale,(float)accel_data[2]/aScale);
        printf("Temperature is %.1f°C \n",temp_data / 333.87 + 21.0);
        usleep(100);
     }


    
    return 0;

}
