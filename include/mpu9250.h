#ifndef __MPU9250_H__
#define __MPU9250_H__



#include <iostream>
#include <string>
#include <math.h>
#include "BBB_I2C_Device.h"
#include "mpu9250_map.h"


#define CST_G 		9.80665f
#define PI 			M_PI
#define DEG_TO_RAD 	0.0174532925199f
#define RAD_TO_DEG	57.295779513f

/* DRIVER FOR MPU-9250 */


// ACCEL + GYRO CONFIG 

#define SAMPLE_RATE SAMPLE_RATE_DIVIDER_4
#define GYRO_FULL_SCALE   GYRO_FS_1000DPS
#define GYRO_BANDWITH GYRO_DLPF_CFG_41
#define ACCEL_FULL_SCALE ACCEL_FS_2G
#define ACCEL_BANDWITH ACCEL_DLPF_CFG_44


class IMU_MPU9250
{

public:

// Constructor
IMU_MPU9250(BBB_I2C_Device mpu9250,BBB_I2C_Device ak8963):
imu(mpu9250),
mag(ak8963),
mag_enable(false),
mpu_init(false),
mag_init(false)
{
}


void initMPU9250(void);
void initAK8963(void);

void setGyroFsr(uint8_t fsr);
void setGyroDlpf(uint8_t bandwith);
void configGyro(void);
uint16_t getGyroScale(void);
void readRawGyro(int16_t *destination);
int16_t readRawTemp();

void setAccelFsr(uint8_t fsr);
void setAccelDlpf(uint8_t bandwith);
void configAccel(void);
uint16_t getAccelScale(void);
void readRawAccel(int16_t *destinaton);

void readRawMag(int16_t *destination);
void setMagFsr(uint16_t fsr){_mag_fsr = fsr;};
float getMagRes();

void selftestMPU9250(float *destination);
// void calibrateMPU9250();

private:

	BBB_I2C_Device imu;
	BBB_I2C_Device mag;

  uint8_t _gyro_fsr;
  uint8_t _gyro_bandwith;
  uint8_t _gyro_resolution;
  uint8_t _accel_fsr;
  uint8_t _accel_bandwith;
  uint8_t _accel_resolution;
  uint8_t _mag_fsr;

  int _bus;
  bool mag_enable;
  bool mpu_init;
  bool mag_init;

};


#endif
