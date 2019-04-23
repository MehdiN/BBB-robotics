#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include "sys/ioctl.h"
#include "linux/i2c.h"
#include "linux/i2c-dev.h"

#define MAX_LENGTH_I2C 128



class I2CDevice
{

private:
    uint8_t bus;
    uint8_t device;
    int file;

public:

        // constantes i2c file from beagle bone 
        static constexpr char *i2c_file_0 = "/dev/i2c-1";
        static constexpr char *i2c_file_1 = "/dev/i2c-2";

        //constructor
        I2CDevice(uint8_t bus,uint8_t device);
        
        virtual int open(void);
        virtual int writeRegister(uint8_t regAdress,uint8_t data);
        virtual int readRegister(uint8_t regAddress,uint8_t *data);
        virtual int readRegisters(uint8_t fromrAdress,uint8_t length,uint8_t *data);
        virtual int16_t combineRegisters(uint8_t msb, uint8_t lsb);
        virtual void close();
        // Destructor
        virtual ~I2CDevice();

}


