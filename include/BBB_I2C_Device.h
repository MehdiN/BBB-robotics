#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include <stdint.h>
#include "sys/ioctl.h"
#include "linux/i2c-dev.h"


#define I2C1_FILE "/dev/i2c-1"
#define I2C2_FILE "/dev/i2c-2"

#define MAX_LENGTH_I2C 128

// #define DEBUG 1

class BBB_I2C_Device
{

public:

	BBB_I2C_Device(uint8_t devAddr,int bus);
	
	static constexpr auto i2c_file_1 = "/dev/i2c-1";
	static constexpr auto i2c_file_2 = "/dev/i2c-2";

	int bbb_i2c_init(void);
	int bbb_i2c_set_device_address(uint8_t devAddr);
	int bbb_i2c_close(void);
	int bbb_i2c_claim_bus(void);
	int bbb_i2c_release_bus(void);
	int bbb_i2c_get_in_use_state(void);
	int bbb_i2c_is_init(void){return _inialized;};
	int bbb_i2c_readBytes(uint8_t regAddr,uint8_t length, uint8_t *data);
	int bbb_i2c_readByte(uint8_t regAddr, uint8_t *data);
	int bbb_i2c_readWords(uint8_t regAddr, uint8_t length,uint16_t *data);
	int bbb_i2c_readWord(uint8_t regAddr, uint16_t *data);
	int bbb_i2c_readBit( uint8_t regAddr, uint8_t bitNum,uint8_t *data);
	int bbb_i2c_writeBytes(uint8_t regAddr, uint8_t length,uint8_t* data);
	int bbb_i2c_writeByte( uint8_t regAddr, uint8_t data);
	int bbb_i2c_writeWords(uint8_t regAddr, uint8_t length,uint16_t* data);
	int bbb_i2c_writeWord(int bus, uint8_t regAddr, uint16_t data);
	int bbb_i2c_writeBit(uint8_t regAddr, uint8_t bitNum,uint8_t data);
	int bbb_i2c_sendBytes(uint8_t length, uint8_t* data);
	int bbb_i2c_sendByte(uint8_t data);

private:
	uint8_t _devAddr;
	int _bus;
	int _file;
	int _inialized;
	int _in_use;
	int _old_in_use;
};