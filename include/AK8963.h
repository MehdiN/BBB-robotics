#ifndef __AK8963_H__
#define __AK8963_H__

#include "MPU9250.h"


class AK8963: protected I2CDevice
{

public:
    AK8963(uint8_t bus,uint8_t address);
    // friend class MPU9250;

    void init(const MPU9250 &imu);
    float getResolution();

private:
    void readRawMag(int16_t *data);

    bool _init;
    uint8_t mag_scale;
    uint8_t mag_odr;
    //


};

#endif 