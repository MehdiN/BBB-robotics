#include "I2CDevice.h"


// Constructor

I2CDevice::I2CDevice(uint8_t bus,uint8_t device)
{
    this->file=-1;
    this->bus = bus;
    this->device = device;
    this->open();
}

int I2CDevice::open()
{ 
    const char *filename;

    if(this->bus == 0)
    {
        filename = i2c_file_0;
    }
    else if(this->bus == 1)
    {
        filename = i2c_file_1;
    } else 
    {
        perror("Invalid Bus");
        return 1;
    }
    
    if(this->file = ::open(filename,O_RDWR)<0)
    {
        perror("I2C: Failed to open i2C port");
        return 1;
    }

    if(ioctl(this->file, I2C_SLAVE,this->device) < 0){
		perror("I2C: Failed to connect to device\n");
        return 1;
	}
    return 0;
}


int I2CDevice::writeRegister(uint8_t rAdress,uint8_t data)
{
    uint8_t buffer[2];

    buffer[0] = rAdress;
    buffer[1] = data;

    if(write(this->file,buffer,2) != 2)
    {
        perror("I2C: Failed to write to the device");
        return 1;
    }
    return 0;

}


int I2CDevice::readRegister(uint8_t rAddress,uint8_t *data){
    this->readRegisters(rAddress,1,data);
}


int I2CDevice::readRegisters(uint8_t fromAdress,uint8_t length,uint8_t *data)
{
    // write a single valaue to the I2C device: used to 
    if(write(this->file,&fromAdress,1) != 1)
    {
        perror("I2C: Failed to write into the I2C Bus");
        return 0;
    }

    if(read(this->file,data,length) != length)
    {
        perror("I2C: Failed to read from device");
        return 1;
    }
    return 0;
}

int16_t I2CDevice::combineRegisters(uint8_t msb, uint8_t lsb)
{
   //shift the MSB left by 8 bits and OR with LSB
   return ((int16_t)msb<<8)|(int16_t)lsb;
}

void I2CDevice::close(){
	::close(this->file);
	this->file = -1;
}

I2CDevice::~I2CDevice() {
	if(file!=-1)
    {
        this->close();
    }
}