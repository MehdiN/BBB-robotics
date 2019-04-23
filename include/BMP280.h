#ifndef __BMP280_H__
#define __BMP280_H__




#include "I2CDevice.h"
#include "BMP280_MAP.h"


struct Calibration_Params {

    public:

    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;

    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;

};



class BMP280: protected I2CDevice
{


public:
    // Constructor
    BMP280(uint8_t bus,uint8_t address);


    void initBMP280();
    void get_calib_param();
    void read_raw_data(int32_t *destination);
    void update_status();
    
    uint32_t get_comp_press(int32_t uncomp_press);
    uint32_t get_comp_temp(int32_t uncomp_temp);

private:
    uint8_t i2cBus, deviceAddress;
    Calibration_Params calib_params;
    bool _updating;
    bool _measuring;
    int32_t _temp_comp_factor;

};


#endif