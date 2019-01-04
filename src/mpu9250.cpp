#include "mpu9250.h"


void IMU_MPU9250::initMPU9250(){

	// init the I2c object
	imu.bbb_i2c_init();
	// Check if the device is the mpu9250
	uint8_t ret;
	if(imu.bbb_i2c_readByte(WHO_AM_I_MPU9250,&ret)<0){
		printf("INIT ERROR: UNABLE TO READ REGISTER 0x%x \n",WHO_AM_I_MPU9250);
		return;
	}

	if(ret != 0x71){
		printf("This isn't the mpu9250 but the 0x%x \n",ret);
		return;
	}

  // Power On the IMU and restore the defaults.
	imu.bbb_i2c_writeByte(PWR_MGMT_1,0x00);
	usleep(100);// Wait for all registers to reset;
	imu.bbb_i2c_writeByte(PWR_MGMT_1,0x01); // auto select best available clocksource
	usleep(200); //

	// enable i2c master control
	imu.bbb_i2c_writeByte(USER_CTRL,I2C_MST_EN);
	// set the i2c bus to 400Hz
	imu.bbb_i2c_writeByte(I2C_MST_CTRL,I2C_MST_CLK);
	// reset the MPU9520
	imu.bbb_i2c_writeByte(PWR_MGMT_1,PWR_RESET);
	usleep(100); // wait the MPU to come back
	// set the gyro clocksource
	imu.bbb_i2c_writeByte(PWR_MGMT_1,0x01);
	// enable gyro and accelerometer
	imu.bbb_i2c_writeByte(PWR_MGMT_2,0x00);
	// set the sample divider
	imu.bbb_i2c_writeByte(SMPLRT_DIV,SAMPLE_RATE);

	// FIFO MODE CONFIG
	imu.bbb_i2c_writeByte(CONFIG,0x00<<6);


	// Gyro Configuration
	setGyroFsr(GYRO_FULL_SCALE);
	setGyroDlpf(GYRO_BANDWITH);
	configGyro();
	
	// Accel Configuration
	setAccelFsr(ACCEL_FULL_SCALE);
	setAccelDlpf(ACCEL_BANDWITH);
	configAccel();

	imu.bbb_i2c_writeByte(INT_PIN_CFG,BYPASS_EN);
	usleep(200);
	// set the initialized at true
	mpu_init= true;
	printf("GYRO AND ACCEL READY !\n");
  }


void IMU_MPU9250::setGyroFsr(uint8_t fsr){
	// Select the full scale resolution for the Gyscope
		_gyro_fsr = fsr;
}

void IMU_MPU9250::setGyroDlpf(uint8_t bandwith){
	_gyro_bandwith = bandwith;
}

void IMU_MPU9250::configGyro(){
	imu.bbb_i2c_writeByte(CONFIG,_gyro_bandwith);
	imu.bbb_i2c_writeByte(GYRO_CONFIG,_gyro_fsr);
}

uint16_t IMU_MPU9250::getGyroScale(){
	// select the scale in LSB/dps
	switch(_gyro_fsr){
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

void IMU_MPU9250::configAccel(){
	imu.bbb_i2c_writeByte(ACCEL_CONFIG,_accel_fsr);
	imu.bbb_i2c_writeByte(ACCEL_CONFIG2,_accel_bandwith);
}

void IMU_MPU9250::setAccelFsr(uint8_t fsr){
	_accel_fsr = fsr;
}

uint16_t IMU_MPU9250::getAccelScale(){
	// elect the resolution for the accelerometer in LSB/g
	switch(_accel_fsr){
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

void IMU_MPU9250::setAccelDlpf(uint8_t bandwith){
	_accel_bandwith = bandwith;
}

void IMU_MPU9250::readRawGyro(int16_t *destination){
	uint8_t rawData[6];
	imu.bbb_i2c_readBytes(GYRO_XOUT_H,6,&rawData[0]);
	destination[0]= (int16_t)((int16_t)(rawData[0]<< 8) | rawData[1]);
	destination[1]= (int16_t)((int16_t)(rawData[2]<< 8) | rawData[3]);
	destination[2]= (int16_t)((int16_t)(rawData[4]<< 8) | rawData[5]);
}

void IMU_MPU9250::readRawAccel(int16_t *destination){
	uint8_t rawData[6];
	imu.bbb_i2c_readBytes(ACCEL_XOUT_H,6,&rawData[0]);
	destination[0]= (int16_t)((int16_t)(rawData[0]<< 8) | rawData[1]);
	destination[1]= (int16_t)((int16_t)(rawData[2]<< 8) | rawData[3]);
	destination[2]= (int16_t)((int16_t)(rawData[4]<< 8) | rawData[5]);
}

int16_t IMU_MPU9250::readRawTemp(){
  uint8_t rawData[2];  // x/y/z gyro register data stored here
  imu.bbb_i2c_readBytes(TEMP_OUT_H, 2, &rawData[0]);
  return (int16_t)(((int16_t)rawData[0]) << 8 | rawData[1]);
}


void IMU_MPU9250::initAK8963(){

	//Enable i2c bypass to allow access to magnetometer: 
	// imu.bbb_i2c_writeByte(USER_CTRL,I2C_MST_EN);
	// imu.bbb_i2c_writeByte(I2C_MST_CTRL,I2C_MST_CLK);
	// imu.bbb_i2c_writeByte(INT_PIN_CFG,BYPASS_EN);

	if(!mpu_init){
		std::cout << "I2C BYPASS DISABLED" <<std::endl;
	}

	mag.bbb_i2c_init();

	// Init the AK8963
	uint8_t ret;
	if(mag.bbb_i2c_readByte(WHO_AM_I_AK8963,&ret)<0){
		std::cout << "CANNOT READ AK8963 ADRESS" << std::endl;
		return;
		}

	if(ret != 0x48){
		printf("THIS IS NOT THE AK8963 ");
		printf("THIS IS THE 0c%x",ret);
		return;
	}

	printf("THIS IS THE AK8963 ! \n");

	mag.bbb_i2c_writeByte(MAG_CNTL,0x00); //power down the ak8963
	usleep(100);
	mag.bbb_i2c_writeByte(MAG_CNTL,0x0F); // enter in Fuse ROM mode
	usleep(100);
	mag.bbb_i2c_writeByte(MAG_CNTL,MAG_FS_16 << 4 | MAG_ODR_100);
	setMagFsr(MAG_FS_16);
	usleep(100);
	mag_init = true;
	mag_enable = true;
	usleep(100);
	printf("MAG READY \n");
}

float IMU_MPU9250::getMagRes(){
	switch(_mag_fsr){
		case MAG_FS_14:
			return 58.5; // (4800/2**(14-1))*100 in 100*µT/LSB ~ gauss/LSB
		case MAG_FS_16:
			return 14.6; // (4800/2**(14-1))*100 in 100*µT/LSB
		default:
		printf("NO VALID SCALE");
			return 0;
	}
}


void IMU_MPU9250::readRawMag(int16_t * destination){
	uint8_t rawData[7];
	uint8_t is_mag_ready;
	mag.bbb_i2c_readByte(MAG_ST1,&is_mag_ready);
	// check if new data is ready
	bool newMagData = (is_mag_ready & 0x01);
	if(newMagData == true){
		mag.bbb_i2c_readBytes(MAG_XOUT_H,7,&rawData[0]);
		uint8_t c = rawData[0];
		if(!(c & 0x08)){ // check AK8963 ST2 for Magnetic Overflow
			destination[0]= (int16_t)((int16_t)(rawData[0]<< 8) | rawData[1]);
			destination[1]= (int16_t)((int16_t)(rawData[2]<< 8) | rawData[3]);
			destination[2]= (int16_t)((int16_t)(rawData[4]<< 8) | rawData[5]);
		}
	}
}

// TODO: calibration 
// void IMU_MPU9250::calibrateMPU9250(){}


void IMU_MPU9250::selftestMPU9250(float *destination){
	uint8_t rawData[6] = {0};
	uint8_t self_test[6];
	int32_t gyroAvg[3]= {0},accelAvg[3] = {0},gyroSTAvg[3]= {0},accelSTAvg[3]= {0};
	float factoryTrim[6];
	
	// Are these step necessary ?
	imu.bbb_i2c_init();
	// Power On the IMU and restore the defaults.
	imu.bbb_i2c_writeByte(PWR_MGMT_1,0x00);
	
	
	usleep(100);// Wait for all registers to reset;
	imu.bbb_i2c_writeByte(SMPLRT_DIV,SAMPLE_RATE_DIVIDER_0);
	imu.bbb_i2c_writeByte(CONFIG,GYRO_DLPF_CFG_92);
	imu.bbb_i2c_writeByte(GYRO_CONFIG,GYRO_FS_250DPS);
	imu.bbb_i2c_writeByte(ACCEL_CONFIG2,ACCEL_DLPF_CFG_99);
	imu.bbb_i2c_writeByte(ACCEL_CONFIG,ACCEL_FS_2G);

	usleep(25);

	for (uint16_t ii = 0;ii<200;ii++){
		
		imu.bbb_i2c_readBytes(ACCEL_XOUT_H,6,&rawData[0]);
		accelAvg[0] += (int16_t)((int16_t)(rawData[0]<< 8) | rawData[1]);
		accelAvg[1] += (int16_t)((int16_t)(rawData[2]<< 8) | rawData[3]);
		accelAvg[2] += (int16_t)((int16_t)(rawData[4]<< 8) | rawData[5]);

		imu.bbb_i2c_readBytes(GYRO_XOUT_H,6,&rawData[0]);
		gyroAvg[0] += (int16_t)((int16_t)(rawData[0]<< 8) | rawData[1]);
		gyroAvg[1] += (int16_t)((int16_t)(rawData[2]<< 8) | rawData[3]);
		gyroAvg[2] += (int16_t)((int16_t)(rawData[4]<< 8) | rawData[5]);
	}

	for (uint16_t ii = 0;ii<3;ii++){
		accelAvg[ii] /= 200;
		gyroAvg[ii] /= 200;
	}

	// enable self-test
   	imu.bbb_i2c_writeByte(ACCEL_CONFIG, 0xE0); // Enable self test on all three axes and set accelerometer range to +/- 2 g
	imu.bbb_i2c_writeByte(GYRO_CONFIG, 0xE0); // Enable self test on all three axes and set gyro range to +/- 250 degrees/s

	usleep(100);

		for (uint16_t ii = 0;ii<200;ii++){
		
		imu.bbb_i2c_readBytes(ACCEL_XOUT_H,6,&rawData[0]);
		accelSTAvg[0] += (int16_t)((int16_t)(rawData[0]<< 8) | rawData[1]);
		accelSTAvg[1] += (int16_t)((int16_t)(rawData[2]<< 8) | rawData[3]);
		accelSTAvg[2] += (int16_t)((int16_t)(rawData[4]<< 8) | rawData[5]);

		imu.bbb_i2c_readBytes(GYRO_XOUT_H,6,&rawData[0]);
		gyroSTAvg[0] += (int16_t)((int16_t)(rawData[0]<< 8) | rawData[1]);
		gyroSTAvg[1] += (int16_t)((int16_t)(rawData[2]<< 8) | rawData[3]);
		gyroSTAvg[2] += (int16_t)((int16_t)(rawData[4]<< 8) | rawData[5]);
	}

	for (uint16_t ii = 0;ii<3;ii++){
		accelSTAvg[ii] /= 200;
		gyroSTAvg[ii] /= 200;
	}

	// disable self-test
   	imu.bbb_i2c_writeByte(ACCEL_CONFIG, 0x00);
	imu.bbb_i2c_writeByte(GYRO_CONFIG, 0x00);

	usleep(100);

	imu.bbb_i2c_readByte(SELF_TEST_X_ACCEL,&self_test[0]);
	imu.bbb_i2c_readByte(SELF_TEST_Y_ACCEL,&self_test[1]);
	imu.bbb_i2c_readByte(SELF_TEST_Y_ACCEL,&self_test[2]);
	imu.bbb_i2c_readByte(SELF_TEST_X_GYRO,&self_test[3]);
	imu.bbb_i2c_readByte(SELF_TEST_Y_GYRO,&self_test[4]);
	imu.bbb_i2c_readByte(SELF_TEST_Z_GYRO,&self_test[5]);

	// Factory trim

	factoryTrim[0] = (float)(2620/2)*pow(1.01,(float)self_test[0]-1);
	factoryTrim[1] = (float)(2620/2)*pow(1.01,(float)self_test[1]-1);
	factoryTrim[2] = (float)(2620/2)*pow(1.01,(float)self_test[2]-1);
	factoryTrim[3] = (float)(2620/2)*pow(1.01,(float)self_test[3]-1);
	factoryTrim[4] = (float)(2620/2)*pow(1.01,(float)self_test[4]-1);
	factoryTrim[5] = (float)(2620/2)*pow(1.01,(float)self_test[5]-1);

	for (int i = 0; i < 3; i++) {
     destination[i]   = 100.0*((float)(accelSTAvg[i] - accelAvg[i]))/factoryTrim[i] - 100.;
     destination[i+3] = 100.0*((float)(gyroSTAvg[i] - gyroAvg[i]))/factoryTrim[i+3] - 100.;
	}
}