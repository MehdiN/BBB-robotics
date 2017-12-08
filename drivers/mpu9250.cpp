#include "mpu9250.h"
#include "drivers/i2c_device.h"


IMU_MPU9250::IMU_MPU9250(int bus,BBB_I2C_Device mpu9250){

}


void IMU_MPU9250::initMPU9250(){

	this->bus = 2;
	
	mpu9250(MPU9250_ADDRESS,bus,0,0,0)
	
	// Check if the device is the mpu9250
	uint8_t ret;
	
	if(mpu9250.bbb_i2c_read_one_byte(WHO_AM_I_MPU9250,&ret)<0){
		throw string( "INIT ERROR: UNABLE TO READ REGISTER %x",WHO_AM_I_MPU9250);
	}
	if(ret != 0x71){
		throw string("THIS IS NOT THE MPU-9250");
	}

	// Power On the IMU and restore the defaults.

	mpu9250.bbb_i2c_write_one_byte(PWR_MGMT_1,0x00); 
	mpu9250.bbb_i2c_write_one_byte(PWR_MGMT_1,0x01); // auto select best available clocksource

}

void configGyro(){

	mpu9250.bbb_i2c_write_one_byte(GYRO_CONFIG,GYRO_SCALE_250DPS);

}

void configAccel(){
	mpu9250.bbb_i2c_write_byte(ACCEL_CONFIG,ACCEL_SCALE_2G);
}