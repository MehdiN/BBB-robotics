#include "BMP280.h"


BMP280::BMP280(uint8_t I2CBus,uint8_t I2CAddress):
    I2CDevice(I2CBus,I2CAddress)
    {
        _updating = false;
        _measuring = false;
    };



void BMP280::initBMP280()
{
    uint8_t ret;
    if(this->readRegister(BMP280_ID,&ret)<0){
        printf("INIT ERROR: UNABLE TO READ REGISTER 0x%x \n",BMP280_ID);
        return;
    }

    if(ret != 0x58){
        printf("THIS IS NOT THE BMP280");
        return;
    }

    //reset and put the bmp in sleep mode
    this->writeRegister(RESET,0xB6); // soft_reset
    this->writeRegister(CNTL_MEAS,SLEEP_MODE);
    usleep(200);
    //get the trimming parameters and set the seeting;
    this->get_calib_param();
    //
    usleep(200);
    this->writeRegister(CNTL_MEAS,OSRS_P_16);
    this->writeRegister(CNTL_MEAS,OSRS_T_2);
    this->writeRegister(CONFIG,FILTER_COEFF_16);
    this->writeRegister(CONFIG,ODR_0_5_MS);
    // set the bmp to normal mode
    this->writeRegister(CNTL_MEAS,NORML_MODE);
}


void BMP280::get_calib_param(){
    uint8_t trim_data[24];
    int ret;
    ret = this->readRegisters(CALIB_T1_LSB,24,&trim_data[0]);
    if (ret == 0){
        calib_params.dig_T1 = (uint16_t)((uint16_t)trim_data[0]<<8) | ((uint16_t)trim_data[1]);
        calib_params.dig_T2 = (int16_t)((int16_t)trim_data[2]<<8) | ((uint16_t)trim_data[3]);
        calib_params.dig_T3 = (int16_t)((int16_t)trim_data[4]<<8) | ((uint16_t)trim_data[5]);
        calib_params.dig_P1 = (uint16_t)((uint16_t)trim_data[6]<<8) | ((uint16_t)trim_data[7]);
        calib_params.dig_P2 = (int16_t)((int16_t)trim_data[8] <<8) | ((int16_t)trim_data[9]);
        calib_params.dig_P3 = (int16_t)((int16_t)trim_data[10]<<8) | ((int16_t)trim_data[11]);
        calib_params.dig_P4 = (int16_t)((int16_t)trim_data[12]<<8) | ((int16_t)trim_data[13]);
        calib_params.dig_P5 = (int16_t)((int16_t)trim_data[14]<<8) | ((int16_t)trim_data[15]);
        calib_params.dig_P6 = (int16_t)((int16_t)trim_data[16]<<8) | ((int16_t)trim_data[17]);
        calib_params.dig_P7 = (int16_t)((int16_t)trim_data[18]<<8) | ((int16_t)trim_data[19]);
        calib_params.dig_P8 = (int16_t)((int16_t)trim_data[20]<<8) | ((int16_t)trim_data[21]);
        calib_params.dig_P9 = (int16_t)((int16_t)trim_data[22]<<8) | ((int16_t)trim_data[23]);
    }
}


void BMP280::read_raw_data(int32_t *destination){
    uint8_t data[6];
    this->readRegisters(PRESS_MBS,6,&data[0]);
    destination[0] = (int32_t) ((((uint32_t) (data[0])) << 12) | (((uint32_t) (data[1])) << 4) | ((uint32_t) data[2] >> 4));
    destination[1] = (int32_t) ((((uint32_t) (data[3])) << 12) | (((uint32_t) (data[4])) << 4) | ((uint32_t) data[5] >> 4));
} 


// 32 bit implementation
uint32_t BMP280::get_comp_temp(int32_t uncomp_temp){   
    int32_t t1,t2;
    int32_t temp_coeff;
    int32_t temperature = 0;

    t1 = ((((uncomp_temp >> 3) - ((int32_t) calib_params.dig_T1 << 1))) * ((int32_t) calib_params.dig_T2)) >> 11;
    t2 = (((((uncomp_temp >> 4) - ((int32_t) calib_params.dig_T1)) * ((uncomp_temp >> 4) - ((int32_t) calib_params.dig_T1))) >> 12) * ((int32_t) calib_params.dig_T3)) >> 14;

    temp_coeff = t1 + t2;

    temperature = (temp_coeff * 5 + 128) >> 8;
    _temp_comp_factor = temp_coeff;

    return temperature; // in centidegree
}


// 64 bit implementation 
uint32_t BMP280::get_comp_press(int32_t uncomp_press){
    int64_t pressure =0;
    int64_t p1,p2;
    int32_t temp_coef = 0;

    temp_coef = _temp_comp_factor;

    p1 = ((int64_t)temp_coef) - 128000;
    p2 = p1*p1*((int64_t)calib_params.dig_P6);
    p2 = p2 + ((p1*(int64_t)calib_params.dig_P5)<<17);
    p2 = p2 + (((int64_t)calib_params.dig_P4)<<35);
    p1 = ((p1*(int64_t)calib_params.dig_P8)>>8) + ((p1*(int64_t)calib_params.dig_P2)<<12);
    p1 = ((int64_t(1<<47) + p1) * ((int64_t) calib_params.dig_P1)) >> 33;
    
    if (p1 != 0) {
            pressure = 1048576 - uncomp_press;
            pressure = (((pressure << 31) - p2) * 3125) / p1;
            p1 = (((int64_t) calib_params.dig_P9) * (pressure >> 13) * (pressure >> 13)) >> 25;
            p2 = (((int64_t) calib_params.dig_P8) * pressure) >> 19;
            pressure = ((pressure + p1 + p2) >> 8) + (((int64_t) calib_params.dig_P7) << 4);
        } else {
            pressure = 0;
        }

    return (uint32_t)pressure; // to divide by 256 to have the value in Pa
}


void BMP280::update_status(){
    uint8_t ret;
   this->readRegister(STATUS,&ret);
    // the device is measuring if the bit_1
    // the device is updating if bit_3 = 1 
    
    if(ret & 0x08){
        _updating = true;
    } else {
        _updating = false;
    }

    if(ret & 0x01){
        _measuring = true;
    } else {
        _measuring = false;
    }

}