#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include "sys/ioctl.h"
#include "linux/i2c-dev"


#define I2C1_FILE "/dev/i2c-1"
#define I2C2_FILE "/dev/i2c-2"
#define MAX_LENGTH_I2C 128

class BBB_I2C_Device
{

public:

	int bbb_i2c_init(int bus,uint8_t devAddr);
	int bbb_i2c_set_device_address(uint8_t devAddr);
	int bbb_i2c_close(void);
	int bbb_i2c_claim_bus(void);
	int bbb_i2c_release_bus(void);
	int bbb_i2c_get_in_use_state(void);
	int bbb_i2c_read_bytes(uint8_t regAddr,uint8_t length, uint8_t *data);
	int bbb_i2c_read_one_byte(uint8_t regAddr, uint8_t *data);
	int bbb_i2c_read_words(uint8_t regAddr, uint8_t length,uint16_t *data);
	int bbb_i2c_read_one_word(uint8_t regAddr, uint16_t *data);
	int bbb_i2c_read_bit( uint8_t regAddr, uint8_t bitNum,uint8_t *data);
	int bbb_i2c_write_bytes(uint8_t regAddr, uint8_t length,uint8_t* data);
	int bbb_i2c_write_one_byte( uint8_t regAddr, uint8_t data);
	int bbb_i2c_write_words(uint8_t regAddr, uint8_t length,uint16_t* data);
	int bbb_i2c_write_word(int bus, uint8_t regAddr, uint16_t data);
	int bbb_i2c_write_bit(uint8_t regAddr, uint8_t bitNum,uint8_t data);
	int bbb_i2c_send_bytes(uint8_t length, uint8_t* data);
	int bbb_i2c_send_byte(uint8_t data);

private:
	uint8_t _devAddr;
	int _bus;
	int _file;
	int _inialized;
	int _in_use;
	int _old_in_use;
}