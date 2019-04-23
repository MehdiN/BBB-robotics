#ifndef __MPU9250_H__
#define __MPU9250_H__


#include <iostream>
#include <string>
#include <math.h>
#include "I2CDevice.h"
#include "MPU9250_MAP.h"
#include "AK8963.h"



class MPU9250: protected I2CDevice
{

struct Parameters
{
    uint8_t accel_scale;
    uint8_t gyro_scale;
    uint8_t accel_lpf;
    uint32_t gyro_lpf;
};


public: 
    MPU9250(uint8_t bus,uint8_t address);

    friend class AK8963;

    void init();
    void setup();
    
    uint16_t getGyroScale();
    uint16_t getAccelScale();


private:

    // Slave magnetometer AK8963
    // I2CDevice magnetometer;
    
    // int16_t combineRegisters(uint8_t msb, uint8_t lsb);
    void readRawAccel(int16_t *data);
    void readRawGyro(int16_t *data);
    void readRawTemp(int16_t &data);
    Parameters params;

    bool i2c_bypass;
    //
    float resolution;
    

};







#endif