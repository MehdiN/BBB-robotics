#include "main.h"

int main(){
    // Create a I2C device to MPU adress
    uint8_t devAddr = 0x68;
    int bus = 2;

    BBB_I2C_Device mpu9k(devAddr,bus);
    mpu9k.bbb_i2c_init(devAddr,bus);
    uint8_t ret;

    mpu9k.bbb_i2c_readByte(0x75,&ret);

    std::cout << std::hex << ret << std::endl;

    return 0;

}