#include "AK8963.h"

AK8963::AK8963(uint8_t i2c_bus,uint8_t i2c_address):
    I2CDevice(i2c_bus,i2c_address)
    {
        _init = false;
        mag_scale = MAG_FS_16;
        mag_odr = MAG_ODR_100;
    }

void AK8963::init(const MPU9250 &imu)
{
    if(!imu.i2c_bypass)
    {
        return;
    }

    uint8_t ret;
    if(this->readRegister(WHO_AM_I_AK8963,&ret)<0)
    {
        perror("Error while trying to read AK8963 address");
        return;
    }

    if(ret != 0x48)
    {
        perror("This is not the AK8963");
        return;
    }

    // power down the magnetometer
    this->writeRegister(MAG_CNTL,0x00);
    usleep(100);
    // enter in Fuse ROM mode
    this->writeRegister(MAG_CNTL,0x0F);
    usleep(100);

    this->writeRegister(MAG_CNTL,MAG_FS_16 << 4 | MAG_ODR_100);
    usleep(100);
}

float AK8963::getResolution()
{
    	switch(mag_scale)
        {
            case MAG_FS_14:
                return 58.5; // (4800/2**(14-1))*100 in 100*µT/LSB ~ gauss/LSB
            case MAG_FS_16:
                return 14.6; // (4800/2**(14-1))*100 in 100*µT/LSB
            default:
                return 0;
        }
}


void AK8963::readRawMag(int16_t *data)
{
    uint8_t raw[7];
    uint8_t is_mag_ready;
    this->readRegister(MAG_ST1,&is_mag_ready);
    bool new_data = (is_mag_ready & 0x01);
    if(new_data)
    {
        this->readRegisters(MAG_XOUT_H,7,&raw[0]);
        uint8_t c = raw[0];
        if(!(c & 0x08))
        {
            data[0] = combineRegisters(raw[0],raw[1]);
            data[1] = combineRegisters(raw[2],raw[3]);
            data[2] = combineRegisters(raw[4],raw[5]);
        }
    }
}