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


// i2c device init
int BBB_I2C_Device::bbb_i2c_init(){


	if(_bus !=1 && _bus !=2){
		printf( "INVALID BUS" );
		return -1;
	}

	// set status at 1
	_in_use = 1;

	const char *filename;

	switch(_bus){
		case 1:
			filename = i2c_file_1;
			break;
		case 2:
			filename = i2c_file_2;
			break;
		default:
			printf( "I2C BUS MUST BE 1 OR 2" );
			return -1;
	}

	// open i2c file
	_file = open(filename,O_RDWR);

	if(ioctl(_file, I2C_SLAVE,_devAddr) < 0){
		printf( "ioctl slave change failed" );
		return -1;
	}

	//set old status
	_old_in_use = _in_use;
	
	printf("Succefully initialized i2c bus number %d \n",_bus);
	_inialized = 1;

	return 0;
}


//	i2c set address
int BBB_I2C_Device::bbb_i2c_set_device_address(uint8_t devAddr){

	if(_bus !=1 && _bus !=2){
		printf( "BUS SELECTED IS NOT VALID" );
	}

	// quit and return 0 if the address is correct
	if(_devAddr == devAddr){
		return 0;
	}

		// if not, change it with ioctl
	#ifdef DEBUG
		printf("calling ioctl slave adress change");
	#endif

	if(ioctl(_file, I2C_SLAVE,devAddr) < 0){
		printf("ioctl slave change failed");
		return -1;
	}

	_devAddr = devAddr;

	return 0;
}

// close i2c bus
int BBB_I2C_Device::bbb_i2c_close(void){
	if(_bus!=1 && _bus!=2){
		printf("BUS SELECTED IS NOT VALID");
		return -1;
	}
	_devAddr = 0;
	if(close(_file)<0){ return -1;}
	_inialized = 0;
	return 0;
}


// 	claim a i2c bus
int BBB_I2C_Device::bbb_i2c_claim_bus(void){
	if(_bus!=1 && _bus!=2){
		printf( "BUS SELECTED IS NOT VALID" );
		return -1;
	}
	_in_use=1;
	return 0;
}



// release i2c bus
int BBB_I2C_Device::bbb_i2c_release_bus(void){
	if(_bus!=1 && _bus!=2){
		printf("BUS SELECTED IS NOT VALID");
		return -1;
	}
	_in_use = 0;
	return 0;
}


//get i2c bus state
int BBB_I2C_Device::bbb_i2c_get_in_use_state(void){
	if(_bus!=1 && _bus!=2){
		printf("BUS SELECTED IS NOT VALID");
		return -1;
	}
	return _in_use;
}

// read 8 bit data of length

int BBB_I2C_Device::bbb_i2c_readBytes(uint8_t regAddr,uint8_t length, uint8_t *data){

	int ret;

	if(_bus!=1 && _bus!=2){
		printf("BUS: %d IS NOT VALID \n",_bus);
		return -1;
	}

	if(length > MAX_LENGTH_I2C){
		printf( "data length is enforced as MAX_LENGTH_I2C" );
		length = MAX_LENGTH_I2C;
	}
	_old_in_use = _in_use;
	_in_use = 1;


#ifdef DEBUG
	printf("Read to adress 0x%x \n",_devAddr);
	printf("Reading %d bytes from 0x%x \n",length,regAddr);
	printf("File read is %i \n",_file);
#endif

	// write to register
	ret = write(_file,&regAddr,1);
	if(ret != 1){
		printf("FAILED TO WRITE TO I2C BUS \n");
		return -1;
	}

	if(read(_file,data,length) != length){
		printf("FAILED TO READ FROM I2C BUS \n");
		return -1;
	};

#ifdef DEBUG
	printf("Data is %s \n",data);
#endif

	_in_use = _old_in_use;
	return 0;
}


// Read 8 bit data
int BBB_I2C_Device::bbb_i2c_readByte(uint8_t regAddr, uint8_t *data){
	return bbb_i2c_readBytes(regAddr,1, data);
}

int BBB_I2C_Device::bbb_i2c_readWords(uint8_t regAddr, uint8_t length,uint16_t *data) {

	int ret,i;
	char buffer[MAX_LENGTH_I2C];

	if(_bus!=1 && _bus!=2){
		printf( "BUS SELECTED IS NOT VALID");
		return -1;
	}
	if(length>(MAX_LENGTH_I2C/2)){
		printf( "Length must be less than MAX_LENGTH_I2C/2" );
		return -1;
	}

	// claim the bus during this operation
	_old_in_use = _in_use;
	_in_use = 1;

	#ifdef DEBUG
	printf( "i2c devAddr:%x", _devAddr );
	printf( "reading %i words from 0x%x",length,regAddr );
	#endif

	// write first
	ret = write(_file, &regAddr, 1);
	if(ret!=1){
		printf("FAILED TO WRITE TO I2C BUS");
	}

	// then read the response
	ret = read(_file, buffer, length*2);
	
	if(ret!=(length*2)){
		printf( "i2c device returned %d bytes",ret);
		printf("Expected %d bytes",length);
		printf("ERROR");
		return -1;
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
		printf("BUS SELECTED IS NOT VALID");
		return -1;
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
	printf("Writing to i2c devAddr:0%x \n",_devAddr);
	printf("Writing %d bytes to 0x%x \n",length,regAddr);

	printf("Writed Data: ");
	for (i=0; i<length; i++){
		printf("%x",data[i]);
	}
	printf("\n");
#endif

	// send the bytes
	ret = write(_file, writeData, length+1);
	// write should have returned the correct # bytes written
	if( ret!=(length+1)){
		printf("I2C WRITE FAILED");
		return -1;
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
	printf("Writing to i2c devAddr %x",_devAddr);
	printf("writing %d bytes to %x",length,regAddr);
	for (i=0; i<length; i++){
		printf("%d",writeData[i]);
	}
	printf("\n");
#endif


	ret = write(_file, writeData, (length*2)+1);
	if(ret!=(length*2)+1){
		printf( "I2C WRITE FAILED" );
		return -1;
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
		printf("BUS SELECTED IS NOT VALID");
		return -1;
	}

	// claim the bus during this operation
	_old_in_use= _in_use;
	_in_use = 1;

#ifdef DEBUG
	printf("Send to i2c devAddr:%x \n",_devAddr);
	printf("Sending %d bytes \n",length);
#endif

	// send the bytes
	ret = write(_file, data, length);
	// write should have returned the correct # bytes written
	if(ret!=length){
		printf("rc_i2c_send failed\n");
		return -1;
	}

#ifdef DEBUG
	printf("0x");
	for (int i = 0; i<length; i++){
		printf("%x",data[i]);
	}
	printf("\n");
#endif

	// return the in_use state to previous state.
	_in_use = _old_in_use;

	return 0;
}

// send one byte

int BBB_I2C_Device::bbb_i2c_sendByte(uint8_t data){
	return bbb_i2c_sendBytes(1,&data);
}
