#ifndef __HAL_SOFT_I2C_H
#define __HAL_SOFT_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_gpio.h"

#define I2C_MAX_NUM 6

typedef enum {
    HAL_I2C1 = 0,
    HAL_I2C2,
    HAL_I2C3,
    HAL_I2C4,
    HAL_I2C5,
    HAL_I2C6,
} HalI2cEnum;

void I2cInit(HalI2cEnum idx, GpioEnum scl_gpio, GpioEnum sda_gpio);
void I2cWriteByte(uint8_t idx, uint8_t txd);
uint8_t I2cReadByte(uint8_t idx, uint8_t send_ack);
uint8_t I2cDeviceReadBytes(uint8_t idx, uint8_t dev_addr, uint16_t read_addr, uint8_t r_addr_size, uint8_t *data, uint8_t len);
uint8_t I2cDeviceWriteBytes(uint8_t idx, uint8_t dev_addr, uint16_t write_addr, uint8_t w_addr_size, uint8_t *data, uint8_t len);

void I2cStart(uint8_t idx);
void I2cStop(uint8_t idx);
uint8_t I2cWaitAck(uint8_t idx);
void I2cAck(uint8_t idx);
void I2cNoAck(uint8_t idx);


#ifdef __cplusplus
}
#endif

#endif
