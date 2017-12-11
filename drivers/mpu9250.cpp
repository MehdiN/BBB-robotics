#include "mpu9250.h"



#define CST_g 		9.80665f
#define PI 			M_PI
#define DEG_TO_RAD 	0.0174532925199f
#define RAD_TO_DEG	57.295779513f


IMU_MPU9250::IMU_MPU9250(int bus,BBB_I2C_Device mpu9250){
imu(mpu9250)
magnetometer
}


void IMU_MPU9250::initMPU9250(){

	//this->bus = 2;
	
	//mpu9250(MPU9250_ADDRESS,bus,0,0,0)
	
	// Check if the device is the mpu9250
	uint8_t ret;
	
	if(imu.bbb_i2c_read_one_byte(WHO_AM_I_MPU9250,&ret)<0){
		throw string( "INIT ERROR: UNABLE TO READ REGISTER %x",WHO_AM_I_MPU9250);
	}
	if(ret != 0x71){
		throw string("THIS IS NOT THE MPU-9250");
	}

	// Power On the IMU and restore the defaults.
	imu.bbb_i2c_write_one_byte(PWR_MGMT_1,0x00); // Power On the IMU
	imu.bbb_i2c_write_one_byte(PWR_MGMT_1,0x01); // auto select best available clocksource


}





void IMU_MPU9250::configGyro(){
	// set the fsr
	setFrsGyro(fsr);
	// Use or not the DLPF
	if(dlpf){
		
	} else {
		bbb_i2c_write_one_byte(GYRO_CONFIG,00)
	}

	
}

void IMU_MPU9250::setFsrGyro(uint8_t fsr){
	// Select the full scale resolution for the Gyscope
	switch(fsr){
	case GYRO_FS_250DPS:
		imu.bbb_i2c_write_one_byte(GYRO_CONFIG,GYRO_FS_250DPS);
	case GYRO_FS_500DPS:
		imu.bbb_i2c_write_one_byte(GYRO_CONFIG,GYRO_FS_500DPS);
	case GYRO_FS_1000DPS:
		imu.bbb_i2c_write_one_byte(GYRO_CONFIG,GYRO_FS_1000DPS);
	case GYRO_FS_2000DPS:
		imu.bbb_i2c_write_one_byte(GYRO_CONFIG,GYRO_FS_2000DPS);
	}
}

void IMU_MPU9250::set_GyroDLPF(uint8_t freq){
	switch(freq){
		case GYRO_DLPF_CFG_5:
			imu.bbb_i2c_write_one_byte(CONFIG,GYRO_DLPF_CFG_5);
		case GYRO_DLPF_CFG_10:
			imu.bbb_i2c_write_one_byte(CONFIG,GYRO_DLPF_CFG_10);
		case GYRO_DLPF_CFG_20:
			imu.bbb_i2c_write_one_byte(CONFIG,GYRO_DLPF_CFG_20);
		case GYRO_DLPF_CFG_41:
			imu.bbb_i2c_write_one_byte(CONFIG,GYRO_DLPF_CFG_41);
		case GYRO_DLPF_CFG_92:
			imu.bbb_i2c_write_one_byte(CONFIG,GYRO_DLPF_CFG_92);
		case GYRO_DLPF_CFG_184:
			imu.bbb_i2c_write_one_byte(CONFIG,GYRO_DLPF_CFG_184);
	}
}

void IMU_MPU9250::configAccel(uint8_t fsr;uint8_t freq;bool dlpf){
	//set the fsr
	fsrAccel(fsr)
	// Use or not the DLPF 
	if(dlpf){
		set_AccelDLPF(freq);
	} else {
		imu.bbb_i2c_write_one_byte(ACCEL_CONFIG2,(0x01 << 3));
	}
}


void IMU_MPU9250::setFsrAccel(uint8_t fsr){
	// Select the full scale resolution for the accelerometer
	switch(fsr){
	case ACCEL_FS_2G:
		imu.bbb_i2c_write_one_byte(ACCEL_CONFIG,ACCEL_FS_2G);
	case ACCEL_FS_4G:
		imu.bbb_i2c_write_one_byte(ACCEL_CONFIG,ACCEL_FS_4G);
	case ACCEL_FS_8G:
		imu.bbb_i2c_write_one_byte(ACCEL_CONFIG,ACCEL_FS_8G);
	case ACCEL_FS_16G:
		imu.bbb_i2c_write_one_byte(ACCEL_CONFIG,ACCEL_FS_16G);
	}
}

void IMU_MPU9250::set_AccelDLPF(uint8_t freq){
	// Select the digital low pass filter band width
	imu.bbb_i2c_write_byter(ACCEL_CONFIG2,(0x00<<3))
	switch(freq){
		case ACCEL_DLP_CFG_5:
			imu.bbb_i2c_write_one_byte(ACCEL_CONFIG2,ACCEL_DLPF_CFG_5);
		case ACCEL_DLP_CFG_10:
			imu.bbb_i2c_write_one_byte(ACCEL_CONFIG2,ACCEL_DLPF_CFG_10);
		case ACCEL_DLP_CFG_21:
			imu.bbb_i2c_write_one_byte(ACCEL_CONFIG2,ACCEL_DLPF_CFG_21);
		case ACCEL_DLP_CFG_44:
			imu.bbb_i2c_write_one_byte(ACCEL_CONFIG2,ACCEL_DLPF_CFG_44);
		case ACCEL_DLP_CFG_99:
			imu.bbb_i2c_write_one_byte(ACCEL_CONFIG2,ACCEL_DLP_CFG_99);
		case ACCEL_DLP_CFG_218:
			imu.bbb_i2c_write_one_byte(ACCEL_CONFIG2,ACCEL_DLP_CFG_218);
	}
}



// void IMU_MPU9250::mpu_set_bypass(){}

void IMU_MPU9250::initAK8963(){
	//Enable i2c bypass to allow access to magnetometer:
	imu.bbb_i2c_write_one_byte(USER_CTRL,I2C_MST_EN);
	imu.bbb_i2c_write_one_byte(I2C_MST_CTRL,I2C_MST_CLK);
	imu.bbb_i2c_write_one_byte(INT_PIN_CFG,BYPASS_EN)
	
	// Init the AK8963
	uint8_t ret;
	if(imu.bbb_i2c_read_one_byte(AK8963_ADDRESS,&ret)<0){
		throw string("ERROR TO READ AK8963")
	};
	if(ret != WHO_AM_I_AK8963)
}










void IMU_MPU9250::configMag(){

}

