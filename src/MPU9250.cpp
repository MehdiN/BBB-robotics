#include "MPU9250.h"


MPU9250::MPU9250(uint8_t i2c_bus,uint8_t i2c_address):
    I2CDevice(i2c_bus, i2c_address)
    {
        //this->magnetometer = I2CDevice(i2c_bus,AK8963_ADDRESS);
        i2c_bypass = false;
    }



void MPU9250::init()
{
    // Check WHOIAM
    uint8_t ret;
    if(this->readRegister(WHO_AM_I_MPU9250,&ret)<0)
    {
        perror("IMU: Error while reading WHOIAM register \n");
        return;
    }
    if(ret != 0x71)
    {
        printf("This isn't the mpu9250 but the 0x%x \n",ret);
        return;
    }

    // Power on the IMU and restire the defaults.
    this->writeRegister(PWR_MGMT_1,0x00);
    // Wait 100 us for all registers to clear
    usleep(100);
    // auto select the best available clock source
    this->writeRegister(PWR_MGMT_1,0x01);
    usleep(200);

    //enable i2c control
    this->writeRegister(USER_CTRL,I2C_MST_EN);
    // set the clock at 400 Hz  
    this->writeRegister(I2C_MST_CTRL,I2C_MST_CTRL);
    // reset the MPU9250
    this->writeRegister(PWR_MGMT_1,PWR_RESET);
    usleep(100);
    // Set the gyro clock source
    this->writeRegister(PWR_MGMT_1,0x01);
    // enable gyro and accel
    this->writeRegister(PWR_MGMT_2,0x00);
    // set the sample divider @ 500 Hz
    this->writeRegister(SMPLRT_DIV,SAMPLE_RATE_DIVIDER_1);

    // FIFO Mode config
    this->writeRegister(CONFIG,0x00<<6);

    // Enable I2c Bypass for acceding the magnetometer
    this->writeRegister(INT_PIN_CFG,BYPASS_EN);
    i2c_bypass = true;

}

void MPU9250::setup()
{
    // config Gyro
    this->writeRegister(CONFIG,params.gyro_lpf);
    this-> writeRegister(GYRO_CONFIG,params.gyro_scale);

    //config Accel
    this->writeRegister(ACCEL_CONFIG,params.accel_scale);
    this->writeRegister(ACCEL_CONFIG2,params.gyro_lpf);
}


// int16_t MPU9250::combineRegisters(uint8_t msb, uint8_t lsb){
//    //shift the MSB left by 8 bits and OR with LSB
//    return ((int16_t)msb<<8)|(int16_t)lsb;
// }

void MPU9250::readRawAccel(int16_t *data)
{
    uint8_t raw[6];
    this->readRegisters(ACCEL_XOUT_H,6,raw);
    data[0] = combineRegisters(raw[0],raw[1]);
    data[1] = combineRegisters(raw[2],raw[3]);
    data[2] = combineRegisters(raw[4],raw[5]);
}

void MPU9250::readRawAccel(int16_t *data)
{
    uint8_t raw[6];
    this->readRegisters(GYRO_XOUT_H,6,raw);
    data[0] = combineRegisters(raw[0],raw[1]);
    data[1] = combineRegisters(raw[2],raw[3]);
    data[2] = combineRegisters(raw[4],raw[5]);
}

void MPU9250::readRawTemp(int16_t &data)
{
    uint8_t raw[2];
    this->readRegisters(TEMP_OUT_H,2,raw);
    data = combineRegisters(raw[0],raw[1]);
}

uint16_t MPU9250::getGyroScale()
{
	// select the scale in LSB/dps
	switch(params.gyro_scale){
		case GYRO_FS_250DPS:
			return 131;
		case GYRO_FS_500DPS:
			return 65;
		case GYRO_FS_1000DPS:
			return 32;
		case GYRO_FS_2000DPS:
			return 16;
		default:
			std::cout << "NO VALID SCALE" << std::endl;
			return 0x00;
	}
}
uint16_t MPU9250::getAccelScale(){
	// elect the resolution for the accelerometer in LSB/g
	switch(params.accel_scale){
	case ACCEL_FS_2G:
    	return 16384;
	case ACCEL_FS_4G:
   		return 8192;
	case ACCEL_FS_8G:
    	return 4096;
	case ACCEL_FS_16G:
    	return 2048;
	default:
		std::cout << "NO VALID SCALE" << std::endl;
		return 0x00;
	}
}