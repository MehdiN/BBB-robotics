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

void MPU9250::startSelfTest(float *destination)
{
    uint8_t data[6] = {0};
    uint8_t self_test[6];
    int32_t gyroAvg[3]={0},accelAvg[3]={0},gyroSTAvg[3]={0},accelSTAvg[3]={0};

    float factoryTrim[3];

    this->init();
    this->writeRegister(PWR_MGMT_1,0x00);
    usleep(100);

    this->writeRegister(SMPLRT_DIV,SAMPLE_RATE_DIVIDER_0);
	this->writeRegister(CONFIG,GYRO_DLPF_CFG_92);
	this->writeRegister(GYRO_CONFIG,GYRO_FS_250DPS);
	this->writeRegister(ACCEL_CONFIG2,ACCEL_DLPF_CFG_99);
	this->writeRegister(ACCEL_CONFIG,ACCEL_FS_2G);

    usleep(200);

    for (uint16_t ii=0;ii<200;ii++)
    {
        this->readRegisters(ACCEL_XOUT_H,6,&data[0]);
        accelAvg[0] += combineRegisters(data[0],data[1]);
        accelAvg[1] += combineRegisters(data[2],data[3]);
        accelAvg[2] += combineRegisters(data[4],data[5]);

        this->readRegisters(GYRO_XOUT_H,6,&data[0]);
        gyroAvg[0] += combineRegisters(data[0],data[1]);
        gyroAvg[1] += combineRegisters(data[2],data[3]);
        gyroAvg[2] += combineRegisters(data[4],data[5]);
    }

    for (uint16_t ii = 0;ii<3;ii++)
    {
        accelAvg[ii] /= 200;
        gyroAvg[ii] /= 200;
    }

    // enable self_test;
    this->writeRegister(ACCEL_CONFIG,0xE0);
    this->writeRegister(GYRO_CONFIG,0xE0);

    usleep(200);

    for(uint16_t ii;ii<200;ii++)
    {
        this->readRegisters(ACCEL_XOUT_H,6,&data[0]);
        accelSTAvg[0] += combineRegisters(data[0],data[1]);
        accelSTAvg[1] += combineRegisters(data[2],data[3]);
        accelSTAvg[2] += combineRegisters(data[4],data[5]);

        this->readRegisters(GYRO_XOUT_H,6,&data[0]);
        gyroSTAvg[0] += combineRegisters(data[0],data[1]);
        gyroSTAvg[1] += combineRegisters(data[2],data[3]);
        gyroSTAvg[2] += combineRegisters(data[4],data[5]);
    }

    for (uint16_t ii = 0;ii<3;ii++)
    {
		accelSTAvg[ii] /= 200;
		gyroSTAvg[ii] /= 200;
	}

    // disable self-self_test
    this->writeRegister(ACCEL_CONFIG,0x00);
    this->writeRegister(GYRO_CONFIG,0x00);

    this->readRegister(SELF_TEST_X_ACCEL,&self_test[0]);
	this->readRegister(SELF_TEST_Y_ACCEL,&self_test[1]);
	this->readRegister(SELF_TEST_Y_ACCEL,&self_test[2]);
	this->readRegister(SELF_TEST_X_GYRO,&self_test[3]);
	this->readRegister(SELF_TEST_Y_GYRO,&self_test[4]);
	this->readRegister(SELF_TEST_Z_GYRO,&self_test[5]);

    	// Factory trim

	factoryTrim[0] = (float)(2620/2)*pow(1.01,(float)self_test[0]-1);
	factoryTrim[1] = (float)(2620/2)*pow(1.01,(float)self_test[1]-1);
	factoryTrim[2] = (float)(2620/2)*pow(1.01,(float)self_test[2]-1);
	factoryTrim[3] = (float)(2620/2)*pow(1.01,(float)self_test[3]-1);
	factoryTrim[4] = (float)(2620/2)*pow(1.01,(float)self_test[4]-1);
	factoryTrim[5] = (float)(2620/2)*pow(1.01,(float)self_test[5]-1);

	for (int i = 0; i < 3; i++) 
    {
     destination[i]   = 100.0*((float)(accelSTAvg[i] - accelAvg[i]))/factoryTrim[i] - 100.;
     destination[i+3] = 100.0*((float)(gyroSTAvg[i] - gyroAvg[i]))/factoryTrim[i+3] - 100.;
    }

}