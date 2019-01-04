#include "bmp280.h"




void BMP280::initBMP280(){
    barometer.bbb_i2c_init();

    uint8_t ret;
    if(barometer.bbb_i2c_readByte(BMP280_ID,&ret)<0){
        printf("INIT ERROR: UNABLE TO READ REGISTER 0x%x \n",BMP280_ID);
        return;
    }

    if(ret != 0x58){
        printf("THIS IS NOT THE BMP280");
        return;
    }


    //reset and put the bmp in sleep mode
    barometer.bbb_i2c_writeByte(RESET,0xB6);
    barometer.bbb_i2c_writeByte(CNTL_MEAS,SLEEP_MODE);

    // calibrate and set the wanted seeting then go


    barometer.bbb_i2c_writeByte(CNTL_MEAS,NORML_MODE);

}


// void BBB_I2C_Device::calib_BMP280(){}