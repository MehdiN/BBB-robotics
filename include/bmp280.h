#include "BBB_I2C_Device.h"
#include "bmp280_regmap.h"


struct CalibrationBMP {

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
void readCalibBMP280()

private:
    BBB_I2C_Device barometer;


}