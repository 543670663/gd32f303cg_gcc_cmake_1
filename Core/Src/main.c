
#include "gd32f30x.h"
#include <stdio.h>
#include "hal_gpio.h"
#include "hal_delay.h"
#include "drv_log.h"
#include "gd25qxx.h"
#include "hal_spi.h"
#include "hal_dac.h"
#include "hal_adc.h"
/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/

#define BUFFER_SIZE                    256
#define TX_BUFFER_SIZE                 BUFFER_SIZE
#define RX_BUFFER_SIZE                 BUFFER_SIZE

#define  VERSION_ID                    "1.0.0"
#define  SFLASH_ID                     0xC84013
#define  FLASH_WRITE_ADDRESS           0x000000
#define  FLASH_READ_ADDRESS            FLASH_WRITE_ADDRESS

uint32_t int_device_serial[3];
uint8_t  count;
uint8_t  tx_buffer[TX_BUFFER_SIZE];
uint8_t  rx_buffer[TX_BUFFER_SIZE];
uint32_t flash_id = 0;
uint16_t i = 0;
uint8_t  is_successful = 0;

static void get_chip_serial_num(void);
void test_status_led_init(void);
static void turn_on_led(uint8_t led_num);
ErrStatus memory_compare(uint8_t* src,uint8_t* dst,uint16_t length);
int main(void)
{
    /* 四个工作灯的初始化 */
    GpioInit();
    AdcInit(HAL_ADC_PA0);
    AdcInit(HAL_ADC_PA1);
    AdcInit(HAL_ADC_PA2);
    AdcInit(HAL_ADC_PA3);
    AdcInit(HAL_ADC_PA4);
    AdcInit(HAL_ADC_PA5);
    AdcInit(HAL_ADC_PA6);
    AdcInit(HAL_ADC_PA7);
    AdcInit(HAL_ADC_PB0);
    AdcInit(HAL_ADC_PB1);
    AdcInit(HAL_ADC_PC0);
    AdcInit(HAL_ADC_PC1);
    AdcInit(HAL_ADC_PC2);
    AdcInit(HAL_ADC_PC3);
    AdcInit(HAL_ADC_PC4);
    AdcInit(HAL_ADC_PC5);

    SetGpioPinDir(PF0,GPIO_OUTPUT);
    SetGpioPinDir(PF1,GPIO_OUTPUT);
    SetGpioPinDir(PF2,GPIO_OUTPUT);
    SetGpioPinDir(PF3,GPIO_OUTPUT);
    SetGpioPinDir(PB14,GPIO_OUTPUT);
    SetGpioPinDir(PB15,GPIO_OUTPUT);

    SetGpioPinState(PF0,HIGH_LEVEL);
    SetGpioPinState(PF1,HIGH_LEVEL);
    SetGpioPinState(PF2,HIGH_LEVEL);
    SetGpioPinState(PF3,HIGH_LEVEL);
    SetGpioPinState(PB14,LOW_LEVEL);
    SetGpioPinState(PB15,LOW_LEVEL);

    DelayInit();
    LogInit();
    SpiInit(DEF_HAL_SPI1, SPI_FULL_DUPLEX_MASTER, SPI_CPHA0_CPOL0, 1000, USE_SOFT_CS);
    /* value convert  */
    /* value print */
    DacInit(HAL_DAC_PA4);
    DacInit(HAL_DAC_PA5);
    SetDacVol(HAL_DAC_PA4, 2458);
    SetDacVol(HAL_DAC_PA5, 1120);
//    LOG("\n\rGD32303E-EVAL-V1.0 System is Starting up...\n\r");
//    LOG("\n\rGD32303E-EVAL-V1.0 Flash:%dK\n\r",*(__IO uint16_t*)(0x1FFFF7E0));
//
//    get_chip_serial_num();
//
//    LOG("\n\rGD32303E-EVAL-V1.0 The CPU Unique Device ID:[%X-%X-%X]\n\r",int_device_serial[2],int_device_serial[1],int_device_serial[0]);
//    LOG("\n\rGD32303E-EVAL-V1.0 SPI Flash:GD25Q16 configured...\n\r");
//
//    /* get flash id */

//
//    /* flash id is correct */
//    if(SFLASH_ID == flash_id){
//        LOG("\n\r\n\rWrite to tx_buffer:\n\r\n\r");
//
//        /* LOG tx_buffer value */
//        for(i = 0; i < BUFFER_SIZE; i ++){
//            tx_buffer[i] = i + 5 ;
//            LOG("0x%02X ",tx_buffer[i]);
//
//            if(15 == i%16)
//                LOG("\n\r");
//        }
//
//        LOG("\n\r\n\rRead from rx_buffer:\n\r");
//
//        /* erases the specified flash sector */
//        spi_flash_sector_erase(FLASH_WRITE_ADDRESS);
//        /* write tx_buffer data to the flash */
//        spi_flash_buffer_write(tx_buffer,FLASH_WRITE_ADDRESS,TX_BUFFER_SIZE);
//
//        DelayMs(10);
//
//        /* read a block of data from the flash to rx_buffer */
//        spi_flash_buffer_read(rx_buffer,FLASH_READ_ADDRESS,RX_BUFFER_SIZE);
//        /* LOG rx_buffer value */
//        for(i = 0; i <= 255; i ++){
//            LOG("0x%02X ", rx_buffer[i]);
//            if(15 == i%16)
//                LOG("\n\r");
//        }
//
//        if(ERROR == memory_compare(tx_buffer,rx_buffer,256)){
//            LOG("Err:Data Read and Write aren't Matching.\n\r");
//            is_successful = 1;
//        }
//
//        /* spi flash test passed */
//        if(0 == is_successful){
//            LOG("\n\rSPI-GD25Q16 Test Passed!\n\r");
//        }
//    }else{
//        /* spi flash read id fail */
//        LOG("\n\rSPI Flash: Read ID Fail!\n\r");
//    }
    while(1)
    {
        /* ADC软件触发使能 */

        DelayMs(1000);

        LOG(" adcPA0 val 1 is %d", GetAdcValue(HAL_ADC_PA0));
        LOG(" adcPA1 val 1 is %d", GetAdcValue(HAL_ADC_PA1));
        LOG(" adcPA2 val 1 is %d", GetAdcValue(HAL_ADC_PA2));
        LOG(" adcPA3 val 1 is %d", GetAdcValue(HAL_ADC_PA3));
        LOG(" adcPA4 val 1 is %d", GetAdcValue(HAL_ADC_PA4));
        LOG(" adcPA5 val 1 is %d", GetAdcValue(HAL_ADC_PA5));
        LOG(" adcPA6 val 1 is %d", GetAdcValue(HAL_ADC_PA6));
        LOG(" adcPA7 val 1 is %d", GetAdcValue(HAL_ADC_PA7));
        LOG(" adcPB0 val 1 is %d", GetAdcValue(HAL_ADC_PB0));
        LOG(" adcPB1 val 1 is %d", GetAdcValue(HAL_ADC_PB1));
        LOG(" adcPC0 val 1 is %d", GetAdcValue(HAL_ADC_PC0));
        LOG(" adcPC1 val 1 is %d", GetAdcValue(HAL_ADC_PC1));
        LOG(" adcPC2 val 1 is %d", GetAdcValue(HAL_ADC_PC2));
        LOG(" adcPC3 val 1 is %d", GetAdcValue(HAL_ADC_PC3));
        LOG(" adcPC4 val 1 is %d", GetAdcValue(HAL_ADC_PC4));
        LOG(" \n\r", GetAdcValue(HAL_ADC_PC4));


        ToggleGpioPinState(PB14);
        ToggleGpioPinState(PB15);
        ToggleGpioPinState(PF0);
        ToggleGpioPinState(PF1);
        ToggleGpioPinState(PF2);
        ToggleGpioPinState(PF3);
    }
}

ErrStatus memory_compare(uint8_t* src, uint8_t* dst, uint16_t length)
{
    while(length --){
        if(*src++ != *dst++)
            return ERROR;
    }
    return SUCCESS;
}

void get_chip_serial_num(void)
{
    int_device_serial[0] = *(__IO uint32_t*)(0x1FFFF7E0);
    int_device_serial[1] = *(__IO uint32_t*)(0x1FFFF7EC);
    int_device_serial[2] = *(__IO uint32_t*)(0x1FFFF7F0);
}


