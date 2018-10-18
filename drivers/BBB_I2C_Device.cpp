#include "BBB_I2C_Device.h"


/* BEAGLEBONE Blue/Black I2C Device driver */

// Contructor of I2C bus intance

BBB_I2C_Device::BBB_I2C_Device(uint8_t devAddr,int bus):
_devAddr(devAddr),
_bus(bus),
_inialized(0),
_in_use(0),
_old_in_use(0)
{}


// i2c init
int BBB_I2C_Device::bbb_i2c_init(int bus,uint8_t devAddr){

	_bus = bus;

	if(_bus !=1 || _bus !=2){
		std::cout << "BUS SELECTED IS NOT VALID" << std::endl;
		return -1;
	}

	_old_in_use = _in_use;
	_in_use = 1;

	const char *filename = "/dev/i2c-2";

	// switch(_bus){
	// 	case 1:
	// 		filename = I2C1_FILE;
	// 		break;
	// 	case 2:
	// 		filename = I2C2_FILE;
	// 		break;
	// 	default:
	// 		std::cout << "I2C BUS MUST BE 1 OR 2" << std::endl;
	// 		return -1;
	// }

	_file = open(filename,O_RDWR);

	#ifdef DEBUG
		std::cout << "calling ioctl slave adress change" << std::endl;
	#endif

	if(ioctl(_file, I2C_SLAVE,_devAddr) < 0){
		std::cout << "ioctl slave change failed" << std::endl;
		return -1;
	}

	_devAddr = devAddr;
	_in_use = _old_in_use;
	
	std::cout << "succefully initialized i2c bus" << bus << std::endl;
	_inialized = 1;

	return 0;
}


//	i2c set address
int BBB_I2C_Device::bbb_i2c_set_device_address(uint8_t devAddr){

	if(_bus !=1 && _bus !=2){
		throw std::string("BUS SELECTED IS NOT VALID");
	}

	// quit and return 0 if the address is correct
	if(_devAddr == devAddr){
		return 0;
	}

		// if not, change it with ioctl
	#ifdef DEBUG
		std::cout << "calling ioctl slave adress change" << std::endl;
	#endif

	if(ioctl(_file, I2C_SLAVE,devAddr) < 0){
		std::cout << "ioctl slave change failed" << std::endl;
		return -1;
	}

	_devAddr = devAddr;

	return 0;
}

// close i2c bus
int BBB_I2C_Device::bbb_i2c_close(void){

	if(_bus!=1 && _bus!=2){
		throw std::string("BUS SELECTED IS NOT VALID");
	}
	_devAddr = 0;
	if(close(_file)<0) return -1;
	_inialized = 0;
	return 0;
}


// 	claim a i2c bus
int BBB_I2C_Device::bbb_i2c_claim_bus(void){
	if(_bus!=1 && _bus!=2){
		throw std::string("BUS SELECTED IS NOT VALID");
	}
	_in_use=1;
	return 0;
}



// release i2c bus
int BBB_I2C_Device::bbb_i2c_release_bus(void){
	if(_bus!=1 && _bus!=2){
		throw std::string("BUS SELECTED IS NOT VALID");
	}
	_in_use = 0;
	return 0;
}


//get i2c bus state
int BBB_I2C_Device::bbb_i2c_get_in_use_state(void){
	if(_bus!=1 && _bus!=2){
		throw std::string("BUS SELECTED IS NOT VALID");
	}
	return _in_use;
}

// read 8 bit data of length

int BBB_I2C_Device::bbb_i2c_readBytes(uint8_t regAddr,uint8_t length, uint8_t *data){

	int ret;

	if(_bus!=1 && _bus!=2){
		throw std::string("BUS SELECTED IS NOT VALID");
	}

	if(length > MAX_LENGTH_I2C){
		std::cout << "data length is enforced as MAX_LENGTH_I2C" << std::endl;
	}
	_old_in_use = _in_use;
	_in_use = 1;

	#ifdef DEBUG
		std::cout << "i2c device Adress 0x" << std::hex << devAddr << std::endl;
		std::cout << "reading " << length << "bytes from 0x" << std::hex << regAddr << std:endl;
	#endif

	ret = write(_file,&regAddr,1);
	if(ret != 1){
		throw std::string("FAILED TO WRITE TO I2C BUS");
	}

	ret = read(_file,data,length);
	_in_use = _old_in_use;
	return ret;
}


// Read 8 bit data

int BBB_I2C_Device::bbb_i2c_readByte(uint8_t regAddr, uint8_t *data){
	return bbb_i2c_readBytes(regAddr,1, data);
}

int BBB_I2C_Device::bbb_i2c_readWords(uint8_t regAddr, uint8_t length,uint16_t *data) {

	int ret,i;
	char buffer[MAX_LENGTH_I2C];

	if(_bus!=1 && _bus!=2){
		std::cout << "BUS SELECTED IS NOT VALID"<< std::endl;
		return -1;
	}
	if(length>(MAX_LENGTH_I2C/2)){
		std::cout << "Length must be less than MAX_LENGTH_I2C/2" << std::endl;
		return -1;
	}
	// claim the bus during this operation
	_old_in_use = _in_use;
	_in_use = 1;

	#ifdef DEBUG
	std::cout << "i2c devAddr:0x" << std::hex << _devAddr << std::endl;
	std::cout << "reading " << length << "words from 0x" << std::hex << regAddr << std:endl;
	#endif

	// write first
	ret = write(_file, &regAddr, 1);
	if(ret!=1){
		throw std::string("FAILED TO WRITE TO I2C BUS");
	}

	// then read the response
	ret = read(_file, buffer, length*2);
	if(ret!=(length*2)){
		std::cout << "i2c device returned " << ret << " bytes" << std::endl;
		std::cout << "Expected " << length << " bytes" << std::endl;
		throw std::string("ERROR");
	}

	// form words from bytes and put into user's data array
	for(i=0;i<length;i++){
		data[i] = (((uint16_t)buffer[0])<<8 | buffer[1]);
	}

	// return the in_use state to previous state.
	_in_use = _old_in_use;

	return 0;
}


// Read 16 bits data

int BBB_I2C_Device::bbb_i2c_readWord(uint8_t regAddr, uint16_t *data) {
	return bbb_i2c_readWords(regAddr, 1, data);
}


// i2c read bit

int BBB_I2C_Device::bbb_i2c_readBit( uint8_t regAddr, uint8_t bitNum,uint8_t *data) {
	uint8_t b;
	uint8_t count = bbb_i2c_readByte(regAddr, &b);
	*data = b & (1 << bitNum);
	return count;
}

// I2C writes bytes

int BBB_I2C_Device::bbb_i2c_writeBytes(uint8_t regAddr, uint8_t length,uint8_t* data){

	int i,ret;
	uint8_t writeData[length+1];

	if(_bus!=1 && _bus!=2){
		throw std::string("BUS SELECTED IS NOT VALID");
	}

	// claim the bus during this operation
	_old_in_use = _in_use;
	_in_use = 1;

	// assemble array to send, starting with the register address
	writeData[0] = regAddr;
	for(i=0; i<length; i++){
		writeData[i+1] = data[i];
	}

#ifdef DEBUG
	std::cout << "i2c devAddr:0x%x  " << std::hex <<< _devAddr << std::endl;
	std::cout << "writing " << length << " bytes to 0x" << regAddr <<std::endl;
	std::cout <<"0x" ;
	for (i=0; i<length; i++){
		std::cout << "data[i]";
	}
	std::cout << "\n";
#endif

	// send the bytes
	ret = write(_file, writeData, length+1);
	// write should have returned the correct # bytes written
	if( ret!=(length+1)){
		throw std::string("I2C WRITE FAILED");

	}
	// return the in_use state to previous state.
	_in_use = _old_in_use;
	return 0;
}

// I2C Write unique byte

int BBB_I2C_Device::bbb_i2c_writeByte( uint8_t regAddr, uint8_t data) {
	return bbb_i2c_writeBytes(regAddr, 1, &data);
}

// I2C write words

int BBB_I2C_Device::bbb_i2c_writeWords(uint8_t regAddr, uint8_t length,uint16_t* data){

	int i,ret;
	uint8_t writeData[(length*2)+1];

   // claim the bus during this operation
	_old_in_use = _in_use;
	_in_use = 1;

   // assemble bytes to send
   writeData[0] = regAddr;
	for (i=0; i<length; i++){
		writeData[(i*2)+1] = (uint8_t)(data[i] >> 8);
		writeData[(i*2)+2] = (uint8_t)(data[i]);
	}

#ifdef DEBUG
	std::cout << "i2c devAddr:0x%x  " << std::hex <<< _devAddr << std::endl;
	std::cout << "writing " << length << " bytes to 0x" << regAddr <<std::endl;
	std::cout <<"0x" ;
	for (i=0; i<length; i++){
		std::cout << writedata[i];
	}
	std::cout << "\n";
#endif


	ret = write(_file, writeData, (length*2)+1);
	if(ret!=(length*2)+1){
		throw std::string("I2C WRITE FAILED");
	}

	// return the in_use state to previous state.
	_in_use = _old_in_use;

   return 0;
}


// write one word

int BBB_I2C_Device::bbb_i2c_writeWord(int bus, uint8_t regAddr, uint16_t data) {
	return bbb_i2c_writeWords(regAddr, 1, &data);
}

int BBB_I2C_Device::bbb_i2c_writeBit(uint8_t regAddr, uint8_t bitNum,uint8_t data){
	uint8_t b;
	// read back the current state of the register
	bbb_i2c_readByte(regAddr, &b);
	// modify that bit in the register
	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	// write it back
	return bbb_i2c_writeByte(regAddr, b);
}

// send bytes

int BBB_I2C_Device::bbb_i2c_sendBytes(uint8_t length, uint8_t* data){
	int ret=0;

	if(_bus!=1 && _bus!=2){
		throw std::string("BUS SELECTED IS NOT VALID");
	}

	// claim the bus during this operation
	_old_in_use= _in_use;
	_in_use = 1;

#ifdef DEBUG
	std::cout << "i2c devAddr:0x%x  " << _devAddr << std::endl;
	std::cout <<"sending " << length << " bytes" << std:endl;
#endif

	// send the bytes
	ret = write(_file, data, length);
	// write should have returned the correct # bytes written
	if(ret!=length){
		std::cout << "rc_i2c_send failed\n";
		return -1;
	}

#ifdef DEBUG
	std::cout <<"0x" ;
	for (int i = 0; i<length; i++){
		std::cout << data[i];
	}
	std::cout << "\n";
#endif

	// return the in_use state to previous state.
	_in_use = _old_in_use;

	return 0;
}

// send one byte

int BBB_I2C_Device::bbb_i2c_sendByte(uint8_t data){
	return bbb_i2c_sendBytes(1,&data);
}
