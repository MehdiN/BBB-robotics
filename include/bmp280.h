#ifndef __BMP280_H__
#define __BMP280_H__




#include "BBB_I2C_Device.h"
#include "bmp280_regmap.h"


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



class BMP280 {

public:
    // Constructor
    BMP280(BBB_I2C_Device bmp):
    barometer(bmp)
    {
    }

void initBMP280();
void get_calib_param(void);
void read_raw_data(int32_t *destination);
uint32_t get_comp_press(int32_t uncomp_press);
uint32_t get_comp_temp(int32_t uncomp_temp);

private:
    BBB_I2C_Device barometer;
    Calibration_Params calib_params;


};


#endif