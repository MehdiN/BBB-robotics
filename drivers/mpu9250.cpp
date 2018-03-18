#include "mpu9250.h"



#define CST_G 		9.80665f
#define PI 			M_PI
#define DEG_TO_RAD 	0.0174532925199f
#define RAD_TO_DEG	57.295779513f


IMU_MPU9250::IMU_MPU9250(int bus,BBB_I2C_Device mpu9250,BBB_I2C_Device ak8963){

_bus(2)
imu(mpu9250),
mag(ak8963),
_gyro_fsr(),
_gyro_dlpf(),
_accel_fsr()
_accel_dlpf(),
sample_rate(),
mag_enable(false),
mpu_initialized(false)
}


void IMU_MPU9250::initMPU9250(){

  // init the I2c object
  imu.bbb_i2c_init(_bus,MPU9250_ADDRESS);
  mag.bbb_i2c_init(_bus,AK8963_ADDRESS);
	// Check if the device is the mpu9250
	uint8_t ret;
	if(imu.bbb_i2c_readByte(WHO_AM_I_MPU9250,&ret)<0){
		throw string( "INIT ERROR: UNABLE TO READ REGISTER %x",WHO_AM_I_MPU9250);
	}
	if(ret != 0x71){
		throw string("THIS IS NOT THE MPU-9250");
	}
  // Power On the IMU and restore the defaults.
	imu.bbb_i2c_writeByte(PWR_MGMT_1,0x00);
	usleep(100);// Wait for all register to reset;
	imu.bbb_i2c_writeByte(PWR_MGMT_1,0x01); // auto select best available clocksource
	usleep(200); //
  // enable i2c master control
  imu.bbb_i2c_writeByte(USER_CTRL,I2C_MST_EN);
  // set the i2c bus to 400Hz
  imu.bbb_i2c_writeByte(I2C_MST_CTRL,I2C_MST_CLK);
  // TODO: add compass
  // reset the MPU9520
  imu.bbb_i2c_writeByte(PWR_MGMT_1,PWR_RESET);
  usleep(100); // wait the MPU to come back
  // set the gyro clocksource
  imu.bbb_i2c_writeByte(PWR_MGMT_1,0x01);
  // enable gyro and accelerometer
  imu.bbb_i2c_writeByte(PWR_MGMT_2,0x00);
  //config the gyro
  _gyro_fsr = 2000; // 2000 dps
  _gyro_dlpf = 20; // 20 hz
  configGyro(gyro_fsr,_gyro_dlpf);
  _gyro_scale = 2000.0f/32767.5f * DEG_TO_RAD;
  // config the accelerometer
  _accel_fsr = 2;
  _accel_dlpf = 20;
  configAccel(_accel_fsr,_accel_dlpf);
  _accel_scale = CST_G * 2.0f/32767.5f;
  //set the sample_rate divider to 200Hz
  imu.bbb_i2c_writeByte(SMPLRT_DIV,0x04);
  mpu_initialized = true;
  }





void IMU_MPU9250::configGyro(uint8_t fsr,uint8_t dlpf){
	// set the fsr
	setFrsGyro(fsr);
	// set the DLPF
  set_GyroDLPF(dlpf);
}

void IMU_MPU9250::setFsrGyro(uint8_t fsr){
	// Select the full scale resolution for the Gyscope
	switch(fsr){
	   case 250:
		  imu.bbb_i2c_writeByte(GYRO_CONFIG,GYRO_FS_250DPS);
      break;
	  case 500:
      imu.bbb_i2c_writeByte(GYRO_CONFIG,GYROFS_500DPS);
      break
	  case 1000:
		  imu.bbb_i2c_writeByte(GYRO_CONFIG,GYRO_FS_1000DPS);
      break;
    case 2000:
		  imu.bbb_i2c_writeByte(GYRO_CONFIG,GYRO_FS_2000DPS);
      break;
	}
}

void IMU_MPU9250::set_GyroDLPF(uint8_t freq){
  imu.bbb_i2c_writeByte(GYRO_CONFIG,0x00)
	switch(freq){
		case 5:
			imu.bbb_i2c_writeByte(CONFIG,GYRO_DLPF_CFG_5);
      break
		case 10:
			imu.bbb_i2c_writeByte(CONFIG,GYRO_DLPF_CFG_10);
      break;
		case 20:
			imu.bbb_i2c_writeByte(CONFIG,GYRO_DLPF_CFG_20);
      break;
		case 41:
			imu.bbb_i2c_writeByte(CONFIG,GYRO_DLPF_CFG_41);
      break;
		case 92:
			imu.bbb_i2c_writeByte(CONFIG,GYRO_DLPF_CFG_92);
      break;
		case 184:
			imu.bbb_i2c_writeByte(CONFIG,GYRO_DLPF_CFG_184);
      break;
	}
}

void IMU_MPU9250::configAccel(uint8_t fsr;uint8_t dlpf){
	//set the fsr
	setFsrAccel(fsr)
	// Use or not the DLPF
  set_AccelDLPF(dlpf)
	}
}


void IMU_MPU9250::setFsrAccel(uint8_t fsr){
	// Select the full scale resolution for the accelerometer
	switch(fsr){
	case ACCEL_FS_2G:
		imu.bbb_i2c_writeByte(ACCEL_CONFIG,ACCEL_FS_2G);
    break;
	case ACCEL_FS_4G:
		imu.bbb_i2c_writeByte(ACCEL_CONFIG,ACCEL_FS_4G);
    break;
	case ACCEL_FS_8G:
		imu.bbb_i2c_writeByte(ACCEL_CONFIG,ACCEL_FS_8G);
    break;
	case ACCEL_FS_16G:
		imu.bbb_i2c_writeByte(ACCEL_CONFIG,ACCEL_FS_16G);
    break;
	}
}

void IMU_MPU9250::set_AccelDLPF(uint8_t freq){
	// Select the digital low pass filter band width
	imu.bbb_i2c_write_byter(ACCEL_CONFIG2,(0x00<<3))
	switch(freq){
		case 5:
			imu.bbb_i2c_writeByte(ACCEL_CONFIG2,ACCEL_DLPF_CFG_5);
      break;
		case 10:
			imu.bbb_i2c_writeByte(ACCEL_CONFIG2,ACCEL_DLPF_CFG_10);
      break;
		case 20:
			imu.bbb_i2c_writeByte(ACCEL_CONFIG2,ACCEL_DLPF_CFG_21);
      break;
		case 40:
			imu.bbb_i2c_writeByte(ACCEL_CONFIG2,ACCEL_DLPF_CFG_44);
      break;
		case 100:
			imu.bbb_i2c_writeByte(ACCEL_CONFIG2,ACCEL_DLP_CFG_99);
      break
		case 200:
			imu.bbb_i2c_writeByte(ACCEL_CONFIG2,ACCEL_DLP_CFG_218);
      break;
	}
}

/***
void IMU_MPU9250::initAK8963(){

	//Enable i2c bypass to allow access to magnetometer:
	imu.bbb_i2c_writeByte(USER_CTRL,I2C_MST_EN);
	imu.bbb_i2c_writeByte(I2C_MST_CTRL,I2C_MST_CLK);
	imu.bbb_i2c_writeByte(INT_PIN_CFG,BYPASS_EN)

	// Init the AK8963
	uint8_t ret;
	if(mag.bbb_i2c_read_one_byte(AK8963_ADDRESS,&ret)<0){
		throw string("ERROR TO READ AK8963");
	};
	if(ret != WHO_AM_I_AK8963){
		throw string("ERROR")
	}
}
**/
