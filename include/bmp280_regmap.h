




#define BMP280_ID 0x0D // 0x58 should be read
#define RESET     0xE0 
/* if 0xB6 is written, it resets the device 
with the complete power on reset procedure
*/


#define TEMP_XLSB  0XFC
#define TEMP_LSB   0xFB
#define TEMP_MSB   0xFA
#define PRESS_XLSB 0xF9
#define PRESS_LSB  0xF8
#define PRESS_MBS  0xF7

#define CONFIG     0xF5
#define CNTL_MEAS  0xF4
#define STATUS     0xF3

// POWER MODE
#define SLEEP_MODE 0x00
#define FORCE_MODE 0x01
#define NORML_MODE 0x03


// OVERSAMPLING -> CNTL_MEAS

// Pressure
#define SKIP_P      0x00<<2
#define OSRS_P_1    0x01<<2
#define OSRS_P_2    0x02<<2
#define OSRS_P_4    0x03<<2
#define OSRS_P_8    0x04<<2
#define OSRS_P_16   0x05<<2

// Temp
#define SKIP_T      0x00<<5
#define OSRS_T_1    0x01<<5
#define OSRS_T_2    0x02<<5
#define OSRS_T_4    0x03<<5
#define OSRS_T_8    0x04<<5
#define OSRS_T_16   0x05<<5

// CONFIG 

// ODR
#define ODR_0_5_MS   0x00
#define ODR_62_5_MS  0x01
#define ODR_125_MS   0x02
#define ODR_250_MS   0x03
#define ODR_500_MS   0x04
#define ODR_1000_MS  0x05
#define ODR_2000_MS  0x06
#define ODR_4000_MS  0x07

// IIR Filter
#define FILTER_OFF      0x00
#define FILTER_COEFF_2  0x01
#define FILTER_COEFF_4  0x02
#define FILTER_COEFF_8  0X03
#define FILTER_COEFF_16 0X04

// TRIMMING PARAMETERS

#define CALIB_T1_LSB  0x88
#define CALIB_T1_MSB  0x89
#define CALIB_T2_LSB  0x8A
#define CALIB_T2_MSB  0x8B
#define CALIB_T3_LSB  0x8C
#define CALIB_T3_MSB  0x8D

#define CALIB_P1_LSB  0x8E
#define CALIB_P1_MSB  0x8F
#define CALIB_P2_LSB  0x90
#define CALIB_P2_MSB  0x91
#define CALIB_P3_LSB  0x92
#define CALIB_P3_MSB  0x93
#define CALIB_P4_LSB  0x94
#define CALIB_P4_MSB  0x95
#define CALIB_P5_LSB  0x96
#define CALIB_P5_MSB  0x97
#define CALIB_P6_LSB  0x98
#define CALIB_P6_MSB  0x99
#define CALIB_P7_LSB  0x9A
#define CALIB_P7_MSB  0x9B
#define CALIB_P8_LSB  0x9C
#define CALIB_P8_MSB  0x9D
#define CALIB_P9_LSB  0x9E
#define CALIB_P9_MSB  0x9F
