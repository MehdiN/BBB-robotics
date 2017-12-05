#include "i2c_driver.h"


// typedef struct bbb_i2c_t {
// 	/*data*/
// 	uint8_t devAddr; // Device adress
// 	int bus; // number of bus 0 1 2 (don't use O)
// 	int file;
// 	int inialized;
// 	int in_use;
// } bbb_i2c_t;


// bbb_i2c_t i2c[3]; //

// Contructor of I2C bus intance
BBB_I2C_Device::BBB_I2C_Device(uint8_t devAddr,int bus,int file,int inialized,int in_use):
_devAddr(devAddr),
_bus(bus),
_file(file),
_inialized(inialized),
_in_use(in_use),
_old_in_use(0)
{}


// i2c init
int bbb_i2c_init(int bus,uint8_t devAddr){

	if(bus !=1 && bus !=2){
		throw strig("BUS SELECTED IS NOT VALID");
		return -1;
	}
	_old_in_use = _in_use;
	_in_use = 1;

	_file = 0;
	_devAddr  = devAddr;
	_bus  = bus;
	_inialized = 1;

	switch(bus){
		case 1:
			_file = open(I2C1_FILE,O_RDWR);
			break;
		case 2:
			_file = open(I2C2_FILE,O_RDWR);
			break;
		default:
			throw string("I2C BUS MUST BE 1 OR 2");
		return -1;
	}

	#ifdef DEBUG
		std::cout << "calling ioctl slave adress change" << std::endl
	#endif
	
	if(ioctl(_file, I2C_SLAVE,_devAddr) < 0){
		std::cout << "ioctl slave change failed" << std::endl
		return -1
	}


	_devAddr = devAddr
	_in_use = old_in_use;

	#ifdef DEBUG
		std::cout << "succefully initialized i2c_" << bus << std::endl;
	#endif

	return 0;
}


//	i2c set address
int bbb_i2c_set_device_address(uint8_t devAddr){

	if(_bus !=1 && _bus !=2){
		throw string("BUS SELECTED IS NOT VALID");
		return -1;
	}

	// quit and return 0 if the address is correct
	if(_devAddr == devAddr){
		return 0;
	}

		// if not, change it with ioctl
	#ifdef DEBUG
		std::cout << "calling ioctl slave adress change" << std::endl
	#endif
	
	if(ioctl(_file, I2C_SLAVE,devAddr) < 0){
		std::cout << "ioctl slave change failed" << std::endl
		return -1
	}

	_devAddr = devAddr;
	
	return 0;
}

// close i2c bus
int bbb_i2c_close(){

	if(_bus!=1 && _bus!=2){
		throw string("BUS SELECTED IS NOT VALID");
		return -1;
	}
	_devAddr = 0;
	if(close(_file)<0) return -1;
	_inialized = 0;
	return 0;
}


// 	claim a i2c bus
int bbb_i2c_claim_bus(){
	if(_bus!=1 && _bus!=2){
		throw string("BUS SELECTED IS NOT VALID");
		return -1;
	}
	i2c[bus].in_use=1;
	return 0;
}



// release i2c bus 
int bbb_i2c_release_bus(){
	if(_bus!=1 && _bus!=2){
		throw string("BUS SELECTED IS NOT VALID");
		return -1;
	}
	i2c[bus].in_use=0;
	return 0;
}


//get i2c bus state
int bbb_i2c_get_in_use_state(){
	if(_bus!=1 && _bus!=2){
		throw string("BUS SELECTED IS NOT VALID");
		return -1;
	}
	return _in_use;
}

// read bytes 

int bbb_i2c_read_bytes(uint8_t regAddr,uint8_t length, uint8_t *data){
	
	if(_bus!=1 && _bus!=2){
	throw string("BUS SELECTED IS NOT VALID");
	return -1;
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
		throw string("FAILED TO WRITE TO I2C BUS")
		return -1;
	}

	ret = read(_file,data,length);
	_in_use = _old_in_use;
	return ret;
}

int bbb_i2c_read_one_byte(uint8_t regAddr, uint8_t *data){
	bbb_i2c_read_bytes(regAddr,1, data);
}

int bbb_i2c_read_words(uint8_t regAddr, uint8_t length,uint16_t *data) {
	
	int ret,i;
	char buffer[MAX_I2C_LENGTH];

	if(_bus!=1 && _bus!=2){
		throw string("BUS SELECTED IS NOT VALID");
		return -1;
	}
	if(length>(MAX_I2C_LENGTH/2)){
		std::cout << "Length must be less than MAX_I2C_LENGTH/2" << std::endl;
		printf("rc_i2c_read_words length must be less than MAX_I2C_LENGTH/2\n"); 
		return -1;
	}
	// claim the bus during this operation
	int old_in_use = i2c[bus].in_use;
	i2c[bus].in_use = 1;
	
	#ifdef DEBUG
	printf("i2c devAddr:0x%x  ", i2c[bus].devAddr);
	printf("reading %d words from 0x%x\n", length, regAddr);
	#endif

	// write first 
	ret = write(i2c[bus].file, &regAddr, 1);
	if(ret!=1){
		printf("write to i2c bus failed\n");
		return -1;
	}

	// then read the response
	ret = read(i2c[bus].file, buf, length*2);
	if(ret!=(length*2)){
		printf("i2c device returned %d bytes\n",ret);
		printf("expected %d bytes instead\n",length);
		return -1;
	}
	
	// form words from bytes and put into user's data array
	for(i=0;i<length;i++){
		data[i] = (((uint16_t)buf[0])<<8 | buf[1]); 
	}
	
	// return the in_use state to previous state.
	i2c[bus].in_use = old_in_use;
	
	return 0;
}

/******************************************************************
* rc_i2c_read_word
******************************************************************/
int rc_i2c_read_word(int bus, uint8_t regAddr, uint16_t *data) {
	return rc_i2c_read_words(bus, regAddr, 1, data);
}

/******************************************************************
* rc_i2c_read_bit
******************************************************************/
int rc_i2c_read_bit(int bus, uint8_t regAddr, uint8_t bitNum,\
												uint8_t *data) {
	uint8_t b;
	uint8_t count = rc_i2c_read_byte(bus, regAddr, &b);
	*data = b & (1 << bitNum);
	return count;
}