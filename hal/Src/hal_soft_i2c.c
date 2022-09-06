#include "hal_soft_i2c.h"
#include "hal_delay.h"
#include "hal_gpio.h"
#define CHIP_GD32F303

static GpioEnum SCL_GPIO[I2C_MAX_NUM] = {0};
static GpioEnum SDA_GPIO[I2C_MAX_NUM] = {0};

#if defined(CHIP_LPC1113)
    #define READ_SDA(i)     ((LPC_GPIO[SDA_GPIO[(i)].port].DATA[1UL << SDA_GPIO[(i)].pin]) != 0)
    #define READ_SCL(i)     ((LPC_GPIO[SCL_GPIO[(i)].port].DATA[1UL << SCL_GPIO[(i)].pin]) != 0)

    #define SCL_SET_H(i)    LPC_GPIO[SCL_GPIO[(i)].port].DATA[1UL << SCL_GPIO[(i)].pin] = 1UL << SCL_GPIO[(i)].pin
    #define SCL_SET_L(i)    LPC_GPIO[SCL_GPIO[(i)].port].DATA[1UL << SCL_GPIO[(i)].pin] = 0UL << SCL_GPIO[(i)].pin

    #define SDA_SET_H(i)    LPC_GPIO[SDA_GPIO[(i)].port].DATA[1UL << SDA_GPIO[(i)].pin] = 1UL << SDA_GPIO[(i)].pin
    #define SDA_SET_L(i)    LPC_GPIO[SDA_GPIO[(i)].port].DATA[1UL << SDA_GPIO[(i)].pin] = 0UL << SDA_GPIO[(i)].pin

    #define SDA_IN(i)       LPC_GPIO[SDA_GPIO[(i)].port].DIR &= ~(1UL << SDA_GPIO[(i)].pin)
    #define SDA_OUT(i)      LPC_GPIO[SDA_GPIO[(i)].port].DIR |=  (1UL << SDA_GPIO[(i)].pin)

    #define SCL_IN(i)       LPC_GPIO[SCL_GPIO[(i)].port].DIR &= ~(1UL << SCL_GPIO[(i)].pin)
    #define SCL_OUT(i)      LPC_GPIO[SCL_GPIO[(i)].port].DIR |=  (1UL << SCL_GPIO[(i)].pin)
#elif defined(CHIP_LPC1778)
    #define READ_SDA(i)     ((LPC_GPIO[SDA_GPIO[(i)].port].PIN >> SDA_GPIO[(i)].pin) & 1)
    #define READ_SCL(i)     ((LPC_GPIO[SCL_GPIO[(i)].port].PIN >> SCL_GPIO[(i)].pin) & 1)

    #define SCL_SET_H(i)    LPC_GPIO[SCL_GPIO[(i)].port].SET |= 1UL << SCL_GPIO[(i)].pin
    #define SCL_SET_L(i)    LPC_GPIO[SCL_GPIO[(i)].port].CLR |= 1UL << SCL_GPIO[(i)].pin

    #define SDA_SET_H(i)    LPC_GPIO[SDA_GPIO[(i)].port].SET |= 1UL << SDA_GPIO[(i)].pin
    #define SDA_SET_L(i)    LPC_GPIO[SDA_GPIO[(i)].port].CLR |= 1UL << SDA_GPIO[(i)].pin

    #define SDA_IN(i)       LPC_GPIO[SDA_GPIO[(i)].port].DIR &= ~(1UL << SDA_GPIO[(i)].pin)
    #define SDA_OUT(i)      LPC_GPIO[SDA_GPIO[(i)].port].DIR |=  (1UL << SDA_GPIO[(i)].pin)

    #define SCL_IN(i)       LPC_GPIO[SCL_GPIO[(i)].port].DIR &= ~(1UL << SCL_GPIO[(i)].pin)
    #define SCL_OUT(i)      LPC_GPIO[SCL_GPIO[(i)].port].DIR |=  (1UL << SCL_GPIO[(i)].pin)
#elif defined(CHIP_STM32F103)
    #define READ_SDA(i)    ((STM32_GPIO(SDA_GPIO[(i)].port)->IDR & (1UL << SDA_GPIO[(i)].pin)) != 0)
    #define READ_SCL(i)    ((STM32_GPIO(SCL_GPIO[(i)].port)->IDR & (1UL << SCL_GPIO[(i)].pin)) != 0)

    #define SCL_SET_H(i)   STM32_GPIO(SCL_GPIO[(i)].port)->BSRR = (1UL << SCL_GPIO[(i)].pin)
    #define SCL_SET_L(i)   STM32_GPIO(SCL_GPIO[(i)].port)->BSRR = (1UL << SCL_GPIO[(i)].pin) << 16U

    #define SDA_SET_H(i)   STM32_GPIO(SDA_GPIO[(i)].port)->BSRR = (1UL << SDA_GPIO[(i)].pin)
    #define SDA_SET_L(i)   STM32_GPIO(SDA_GPIO[(i)].port)->BSRR = (1UL << SDA_GPIO[(i)].pin) << 16U

    #define SDA_IN(i)      SetGpioPinDir(SDA_GPIO[(i)], GPIO_INPUT)
    #define SDA_OUT(i)     SetGpioPinDir(SDA_GPIO[(i)], GPIO_OUTPUT)

    #define SCL_IN(i)      SetGpioPinDir(SCL_GPIO[(i)], GPIO_INPUT)
    #define SCL_OUT(i)     SetGpioPinDir(SCL_GPIO[(i)], GPIO_OUTPUT)


#elif defined(CHIP_GD32F303)
    #define READ_SDA(i)    GetGpioPinState(SDA_GPIO[i])
    #define READ_SCL(i)    GetGpioPinState(SCL_GPIO[i])

    #define SCL_SET_H(i)   SetGpioPinState(SCL_GPIO[i], HIGH_LEVEL)
    #define SCL_SET_L(i)   SetGpioPinState(SCL_GPIO[i], LOW_LEVEL)

    #define SDA_SET_H(i)   SetGpioPinState(SDA_GPIO[i], HIGH_LEVEL)
    #define SDA_SET_L(i)   SetGpioPinState(SDA_GPIO[i], LOW_LEVEL)

    #define SDA_IN(i)      SetGpioPinDir(SDA_GPIO[i], GPIO_INPUT)
    #define SDA_OUT(i)     SetGpioPinDir(SDA_GPIO[i], GPIO_OUTPUT)

    #define SCL_IN(i)      SetGpioPinDir(SCL_GPIO[i], GPIO_INPUT)
    #define SCL_OUT(i)     SetGpioPinDir(SCL_GPIO[i], GPIO_OUTPUT)
#endif

#define WAIT_ACK_TIMES  500

void I2cInit(HalI2cEnum idx, GpioEnum scl_gpio, GpioEnum sda_gpio)
{
    DelayInit();
    SCL_GPIO[idx] = scl_gpio;
    SDA_GPIO[idx] = sda_gpio;
    SetGpioPinDir(SCL_GPIO[idx], GPIO_OUTPUT);
    SetGpioPinDir(SDA_GPIO[idx], GPIO_OUTPUT);
    SetGpioPinState(SCL_GPIO[idx], HIGH_LEVEL);
    SetGpioPinState(SDA_GPIO[idx], HIGH_LEVEL);
}

void I2cStart(uint8_t idx)
{
    SDA_OUT(idx);     //sda线输出
    SDA_SET_H(idx);
    SCL_SET_H(idx);
    DelayUs(4);//TODO 所有毫秒换成微秒
    SDA_SET_L(idx);//START:when CLK is high,DATA change form high to low
    DelayUs(4);
    SCL_SET_L(idx);//钳住I2c总线，准备发送或接收数据
}

void I2cStop(uint8_t idx)
{
    SDA_OUT(idx);//sda线输出
    SCL_SET_L(idx);
    SDA_SET_L(idx);//STOP:when CLK is high DATA change form low to high
    DelayUs(4);
    SCL_SET_H(idx);
    SDA_SET_H(idx);//发送I2c总线结束信号
    DelayUs(4);
}

uint8_t I2cWaitAck(uint8_t idx)
{
    SDA_IN(idx);      //SDA设置为输入
    SDA_SET_H(idx); DelayUs(1);
    SCL_SET_H(idx); DelayUs(1);

    uint16_t err = 0;
    while (READ_SDA(idx)) {
        err++;
        if (err > WAIT_ACK_TIMES) {
            I2cStop(idx);
            DelayUs(5);
            return 1;
        }
    }
    SCL_SET_L(idx);//时钟输出0
    return 0;
}

void I2cAck(uint8_t idx)
{
    SCL_SET_L(idx);
    SDA_OUT(idx);
    SDA_SET_L(idx);
    DelayUs(2);
    SCL_SET_H(idx);
    DelayUs(2);
    SCL_SET_L(idx);
}

void I2cNoAck(uint8_t idx)
{
    SCL_SET_L(idx);
    SDA_OUT(idx);
    SDA_SET_H(idx);
    DelayUs(2);
    SCL_SET_H(idx);
    DelayUs(2);
    SCL_SET_L(idx);
}

void I2cWriteByte(uint8_t idx, uint8_t txd)
{
    SDA_OUT(idx);
    SCL_SET_L(idx);//拉低时钟开始数据传输
    for(uint8_t t = 0; t < 8; t++) {
        if((txd&0x80)>>7) {
            SDA_SET_H(idx);
        }
        else {
            SDA_SET_L(idx);
        }

        txd <<= 1;
        DelayUs(2);
        SCL_SET_H(idx);
        DelayUs(2);
        SCL_SET_L(idx);
        DelayUs(2);
    }
}

uint8_t I2cReadByte(uint8_t idx, uint8_t send_ack)
{
    uint8_t receive = 0;
    SDA_IN(idx);//SDA设置为输入
    for(uint8_t i = 0; i < 8; i++) {
        SCL_SET_L(idx);
        DelayUs(2);
        SCL_SET_H(idx);
        receive <<= 1;
        if (READ_SDA(idx)) {
            receive++;
        }
        DelayUs(1);
    }
    if (send_ack) {
        I2cAck(idx); //发送ACK
    }
    else {
        I2cNoAck(idx);//发送nACK
    }
    return receive;
}

uint8_t I2cDeviceReadBytes(uint8_t idx, uint8_t dev_addr, uint16_t read_addr, uint8_t r_addr_size, uint8_t *data, uint8_t len)
{
    I2cStart(idx);

    I2cWriteByte(idx, dev_addr);        // 写
    if (I2cWaitAck(idx)) { return 1; }

    if (r_addr_size == 2) {
        I2cWriteByte(idx, read_addr >> 8);
        if (I2cWaitAck(idx)) { return 1; }
    }
    I2cWriteByte(idx, read_addr & 0xFF);
    if (I2cWaitAck(idx)) { return 1; }

    I2cStart(idx);

    I2cWriteByte(idx, dev_addr + 1);    // 读
    if (I2cWaitAck(idx)) { return 1; }
    if (len > 1) {
        for (uint8_t i = 0; i < len-1; i++) {
            data[i] = I2cReadByte(idx, 1);
        }
    }
    data[len-1] = I2cReadByte(idx, 0);

    I2cStop(idx);
    return 0;
}

uint8_t I2cDeviceWriteBytes(uint8_t idx, uint8_t dev_addr, uint16_t write_addr, uint8_t w_addr_size, uint8_t *data, uint8_t len)
{
    I2cStart(idx);

    I2cWriteByte(idx, dev_addr);        // 写
    if (I2cWaitAck(idx)) { return 1; }

    if (w_addr_size == 2) {
        I2cWriteByte(idx, write_addr >> 8);
        if (I2cWaitAck(idx)) { return 1; }
    }
    I2cWriteByte(idx, write_addr & 0xFF);
    if (I2cWaitAck(idx)) { return 1; }

    // 写数据
    for (uint8_t i = 0; i < len; i++) {
        I2cWriteByte(idx, data[i]);
        if (I2cWaitAck(idx)) { return 1; }
    }

    I2cStop(idx);
    return 0;
}

