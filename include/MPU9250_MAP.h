/*****************************************
******************************************
		 MPU 9250 REGISTERIES
******************************************
******************************************/

#define MPU9250_ADDRESS		0x68
#define AK8963_ADDRESS		0x0C

// GYROSCOPE AND ACCELEROMETER REGISTER MAP

//Self tests and offsets REGISTERS

#define SELF_TEST_X_GYRO 	0x00
#define SELF_TEST_Y_GYRO 	0x01
#define SELF_TEST_Z_GYRO 	0x02

#define SELF_TEST_X_ACCEL 	0x0D
#define SELF_TEST_Y_ACCEL 	0x0E
#define SELF_TEST_Z_ACCEL 	0x0F

// GYRO OFFSET

#define XG_OFFSET_H			0x13
#define XG_OFFSET_L			0x14
#define YG_OFFSET_H			0x15
#define YG_OFFSET_L			0x16
#define ZG_OFFSET_H			0x17
#define ZG_OFFSET_L			0x18

#define SMPLRT_DIV			0x19
#define CONFIG 				0x1A
#define GYRO_CONFIG			0x1B
#define ACCEL_CONFIG 		0x1C
#define ACCEL_CONFIG2		0x1D
#define LP_ACCEL_ODR		0x1E
#define WOM_THR				0x1F

#define FIFO_EN				0x23
#define I2C_MST_CTRL		0x24

#define I2C_SLV0_ADDR		0x25
#define	I2C_SLV0_REG		0x26
#define I2C_SLV0_CTRL		0x27
#define I2C_SLV1_ADDR		0x28
#define	I2C_SLV1_REG		0x29
#define I2C_SLV1_CTRL		0x2A
#define I2C_SLV2_ADDR		0x2B
#define	I2C_SLV2_REG		0x2C
#define I2C_SLV2_CTRL		0x2D
#define I2C_SLV3_ADDR		0x2E
#define	I2C_SLV3_REG		0x2F
#define I2C_SLV3_CTRL		0x30
#define I2C_SLV4_ADDR		0x31
#define	I2C_SLV4_REG		0x32
#define I2C_SLV4_DO			0x33
#define I2C_SLV4_CTRL		0x34
#define	I2C_SLV4_DI			0x35
#define I2C_MST_STATUS		0x36

#define INT_PIN_CFG			0x37
#define INT_ENABLE			0x38
#define	INT_STATUS			0x3A

// ACCEL | GYRO | TEMP OUTPUTS

#define ACCEL_XOUT_H 		0x3B
#define ACCEL_XOUT_L 		0x3C
#define ACCEL_YOUT_H 		0x3D
#define ACCEL_YOUT_L 		0x3E
#define ACCEL_ZOUT_H 		0x3F
#define ACCEL_ZOUT_L		0x40

#define TEMP_OUT_H			0x41
#define TEMP_OUT_L			0x42

#define GYRO_XOUT_H			0x43
#define GYRO_XOUT_L			0x44
#define GYRO_YOUT_H			0x45
#define GYRO_YOUT_L			0x46
#define GYRO_ZOUT_H			0x47
#define GYRO_ZOUT_L			0x48

// EXTERNAL SENSOR DATA

#define EXT_SENS_DATA_00	0x49
#define EXT_SENS_DATA_01	0x4A
#define EXT_SENS_DATA_02	0x4B
#define EXT_SENS_DATA_03	0x4C
#define EXT_SENS_DATA_04	0x4D
#define EXT_SENS_DATA_05	0x4E
#define EXT_SENS_DATA_06	0x4F
#define EXT_SENS_DATA_07	0x50
#define EXT_SENS_DATA_08	0x51
#define EXT_SENS_DATA_09	0x52
#define EXT_SENS_DATA_10	0x53
#define EXT_SENS_DATA_11	0x54
#define EXT_SENS_DATA_12	0x55
#define EXT_SENS_DATA_13	0x56
#define EXT_SENS_DATA_14	0x57
#define EXT_SENS_DATA_15	0x58
#define EXT_SENS_DATA_16	0x59
#define EXT_SENS_DATA_17	0x5A
#define EXT_SENS_DATA_18	0x5B
#define EXT_SENS_DATA_19	0x5C
#define EXT_SENS_DATA_20	0x5D
#define EXT_SENS_DATA_21	0x5E
#define EXT_SENS_DATA_22	0x5F
#define EXT_SENS_DATA_23	0x60

#define MOT_DETECT_STATUS	0x61

// I2C Slave Register

#define I2C_SLV0_DO      	0x63
#define I2C_SLV1_DO      	0x64
#define I2C_SLV2_DO      	0x65
#define I2C_SLV3_DO      	0x66

#define I2C_MST_DELAY_CTRL 	0x67
#define SIGNAL_PATH_RESET  	0x68
#define MOT_DETECT_CTRL  	0x69

//

#define USER_CTRL        	0x6A
#define PWR_MGMT_1       	0x6B // return 0x01
#define PWR_MGMT_2       	0x6C
#define PWR_RESET         	0x01 << 7

#define DMP_BANK         	0x6D
#define DMP_RW_PNT       	0x6E
#define DMP_REG          	0x6F
#define DMP_REG_1        	0x70
#define DMP_REG_2        	0x71

#define FIFO_COUNTH      	0x72
#define FIFO_COUNTL      	0x73
#define FIFO_R_W         	0x74

#define WHO_AM_I_MPU9250 	0x75 // return 0x71

#define XA_OFFSET_H      	0x77
#define XA_OFFSET_L      	0x78
#define YA_OFFSET_H      	0x7A
#define YA_OFFSET_L      	0x7B
#define ZA_OFFSET_H      	0x7D
#define ZA_OFFSET_L 	 	0x7E

// REGITER MAP FOR MAGNETOMETER

#define WHO_AM_I_AK8963		0x00
#define INFO_AK8963			0x01

#define MAG_ST1 			0x02 // data ready status

#define MAG_XOUT_H 			0x03
#define MAG_XOUT_L 			0x04
#define MAG_YOUT_H 			0x05
#define MAG_YOUT_L 			0x06
#define MAG_ZOUT_H 			0x07
#define MAG_ZOUT_L 			0x08

#define MAG_ST2   	    	0x09
#define MAG_CNTL  	    	0x0A // main mode control register
#define MAG_ASTC  	    	0x0C // Self test control
#define MAG_I2CDIS	    	0x0F // I2C disable
#define MAG_ASAX  	    	0x10 // x-axis sensitivity adjustment value
#define MAG_ASAY  	    	0x11 // y-axis sensitivity adjustment value
#define MAG_ASAZ 			0x12 // z-axis sensitivity adjustment value


/*********************************
=============CONFIG===============
*********************************/
// GYRO & TEMP DLPF

#define GYRO_DLPF_CFG_5		0X06 // delay=33ms
#define GYRO_DLPF_CFG_10	0X05 // delay=17ms
#define GYRO_DLPF_CFG_20 	0X04 // delay=10ms
#define GYRO_DLPF_CFG_41	0X03 // delay=6ms
#define GYRO_DLPF_CFG_92	0X02 // delay=4ms
#define GYRO_DLPF_CFG_184	0X01 // delay=3ms
#define GYRO_DLPF_CFG_250   0x00 // delay=1ms

//	GYROSCOPE FULL SCALE RESOLUTION

#define GYRO_FS_250DPS 		0x00 << 3
#define GYRO_FS_500DPS 		0x01 << 3
#define GYRO_FS_1000DPS 	0x02 << 3
#define GYRO_FS_2000DPS 	0x03 << 3

//	ACCELEROMETER FULL SCALE RESOLUTION

#define ACCEL_FS_2G			0x00 << 3
#define ACCEL_FS_4G			0x01 << 3
#define ACCEL_FS_8G			0x02 << 3
#define ACCEL_FS_16G		0x03 << 3

// ACCELEROMETER DIGITAL LOW PASS FILTER

#define ACCEL_DLPF_CFG_5	0X06 // delay=32ms
#define ACCEL_DLPF_CFG_10	0X05 // delay=16ms
#define ACCEL_DLPF_CFG_21   0X04 // delay=8ms
#define ACCEL_DLPF_CFG_44	0X03 // delay=4ms
#define ACCEL_DLPF_CFG_99	0X02 // delay=2ms
#define ACCEL_DLPF_CFG_218	0X01 // delay=1ms

// I2C BYPASS CONFIG

#define I2C_MST_EN		0x01 << 5
#define I2C_MST_CLK		0x0D
#define BYPASS_EN		0X01 << 1

// SAMPLE RATE = 1000/(1+SMPLRT_DIV)

#define	SAMPLE_RATE_DIVIDER_0	0x00 // Fe=1khz
#define	SAMPLE_RATE_DIVIDER_1	0x01 // Fe=500Hz
#define	SAMPLE_RATE_DIVIDER_2	0x02 // Fe=333Hz
#define	SAMPLE_RATE_DIVIDER_3	0x03 // Fe=250Hz
#define	SAMPLE_RATE_DIVIDER_4   0x04 // Fe=200Hz

// MAGNETOMETER CONFIG

#define MAG_FS_16 0x00
#define MAG_FS_14 0x01
#define MAG_ODR_100	0x06
#define MAG_ODR_8 0x02