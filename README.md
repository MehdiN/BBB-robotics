# BeagleBone Black/Blue libraries for Robotics 

**Hardware**: 
 * BeagleBone Bone Blue or Black
 * MPU9250 IMU (embedded with the BeagleBone Blue)
 * BMP280 pressure sensor (embedded with the BeagleBone Blue)

## I2C I/O
 * I2CDevice -> Master class for handling I2C device
 * MU9250 -> mpu9520 basic driver
 * AK8963 -> ak8963 basic driver (by enabling I2C bypass of the MPU9250)
 * BMP280 -> bmp280 basic driver

## STATUS

|Sensors|Status|
|-------|------|
|MPU9250| Done |
|AK8963| Done |
|BMP280| WIP |

## TODO

 * Add self-test and calibration features
 * Add backend class
 * Generic class to handle external sensors

