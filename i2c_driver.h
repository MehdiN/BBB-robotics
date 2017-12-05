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

private:
	uint8_t _devAddr;
	int _bus;
	int _file;
	int _inialized;
	int _in_use;
	int _old_in_use;
}