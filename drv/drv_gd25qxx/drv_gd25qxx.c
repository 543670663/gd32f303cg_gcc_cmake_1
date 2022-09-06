/*!
    \file  gd25qxx.c
    \brief SPI flash gd25qxx driver
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-05-19, V1.0.0, demo for GD32F30x
*/

#include "drv_gd25qxx.h"
#include "gd32f30x.h"
#include "hal_spi.h"
#include <string.h>
#include "drv_log.h"

uint8_t WRITE      =      0x02;     /* write to memory instruction */
uint8_t WRSR       =      0x01;     /* write status register instruction */
uint8_t WREN       =      0x06;     /* write enable instruction */

uint8_t READ       =      0x03;     /* read from memory instruction */
uint8_t RDSR       =      0x05;     /* read status register instruction  */
uint8_t RDID       =      0x9F;     /* read identification */
uint8_t SE         =      0x20;    /* sector erase instruction */
uint8_t BE         =      0xC7;     /* bulk erase instruction */
uint8_t WIP_FLAG   =      0x01;     /* write in progress(wip)flag */
uint8_t DUMMY_BYTE =      0xA5;
uint8_t send_data_2[256] = {0};
uint8_t *rev_data_2;
/*!
    \brief      initialize SPI0 GPIO and parameter
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_flash_init(void)
{
    spi_parameter_struct spi_init_struct;

    //使能相应的时钟
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_SPI0);

    /* 配置SPI 相关引脚 */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_7);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    /* SPI 片选引脚配置 */
    gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);

    /* 片选引脚拉高 */
    SPI_FLASH_CS_HIGH();

    /* SPI0 配置 */
    //SPI传输模式 全双工
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    //主机模式
    spi_init_struct.device_mode          = SPI_MASTER;;
    //数据8bit
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;;
    //相位极性 SPI时钟极性为低电平，相位为第一边缘 也就是mode2
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    //nss 软件控制
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_8 ;
    //传输方式大端字节
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    //初始化
    spi_init(SPI0, &spi_init_struct);

    /* 设置CRC校验 */
    spi_crc_polynomial_set(SPI0,7);
    /* 使能 SPI0 */
    spi_enable(SPI0);
}

/*!
    \brief      erase the specified flash sector 擦除指定的flash扇区
    \param[in]  sector_addr: address of the sector to erase 要删除的扇区地址
    \param[out] none
    \retval     none
*/
void spi_flash_sector_erase(uint32_t sector_addr)
{
    /* send write enable instruction */
    spi_flash_write_enable();

    /* sector erase */
    /* select the flash: chip select low */
    SPI_FLASH_CS_LOW();
    /* send sector erase instruction */
    uint8_t send_data[] = {SE ,(sector_addr & 0xFF0000) >> 16, (sector_addr & 0xFF00) >> 8, sector_addr & 0xFF};
    SpiWriteBuf(DEF_HAL_SPI1, send_data, 4);
    SPI_FLASH_CS_HIGH();
    /* wait the end of flash writing */
    spi_flash_wait_for_write_end();
}

/*!
    \brief      erase the entire flash
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_flash_bulk_erase(void)
{
    /* send write enable instruction */
    spi_flash_write_enable();

    /* bulk erase */
    /* select the flash: chip select low */
    SPI_FLASH_CS_LOW();
    /* send bulk erase instruction  */
    spi_flash_send_byte(BE);
    /* deselect the flash: chip select high */
    SPI_FLASH_CS_HIGH();

    /* wait the end of flash writing */
    spi_flash_wait_for_write_end();
}

/*!
    \brief      write more than one byte to the flash
    \param[in]  pbuffer: pointer to the buffer
    \param[in]  write_addr: flash's internal address to write
    \param[in]  num_byte_to_write: number of bytes to write to the flash
    \param[out] none
    \retval     none
*/
void spi_flash_page_write(uint8_t* pbuffer, uint32_t write_addr, uint16_t num_byte_to_write)
{
    /* enable the write access to the flash */
    spi_flash_write_enable();

    /* select the flash: chip select low */
    SPI_FLASH_CS_LOW();



    /* send "write to memory" instruction */
    uint8_t send_data[] = {WRITE ,(write_addr & 0xFF0000) >> 16, (write_addr & 0xFF00) >> 8, write_addr & 0xFF};
    SpiWriteBuf(DEF_HAL_SPI1, send_data, 4);

    /* while there is data to be written on the flash */
    SpiWriteBuf(DEF_HAL_SPI1, pbuffer, num_byte_to_write);

    /* deselect the flash: chip select high */
    SPI_FLASH_CS_HIGH();

    /* wait the end of flash writing */
    spi_flash_wait_for_write_end();
}

/*!
    \brief      write block of data to the flash
    \param[in]  pbuffer: pointer to the buffer
    \param[in]  write_addr: flash's internal address to write
    \param[in]  num_byte_to_write: number of bytes to write to the flash
    \param[out] none
    \retval     none
*/
void spi_flash_buffer_write(uint8_t* pbuffer, uint32_t write_addr, uint16_t num_byte_to_write)
{
    uint8_t num_of_page = 0, num_of_single = 0, addr = 0, count = 0, temp = 0;

    addr          = write_addr % SPI_FLASH_PAGE_SIZE;
    count         = SPI_FLASH_PAGE_SIZE - addr;
    num_of_page   = num_byte_to_write / SPI_FLASH_PAGE_SIZE;
    num_of_single = num_byte_to_write % SPI_FLASH_PAGE_SIZE;

    /* write_addr is SPI_FLASH_PAGE_SIZE aligned  */
    if(0 == addr){
        /* num_byte_to_write < SPI_FLASH_PAGE_SIZE */
        if(0 == num_of_page)
            spi_flash_page_write(pbuffer,write_addr,num_byte_to_write);
            /* num_byte_to_write > SPI_FLASH_PAGE_SIZE */
        else{
            while(num_of_page--){
                spi_flash_page_write(pbuffer,write_addr,SPI_FLASH_PAGE_SIZE);
                write_addr += SPI_FLASH_PAGE_SIZE;
                pbuffer += SPI_FLASH_PAGE_SIZE;
            }
            spi_flash_page_write(pbuffer,write_addr,num_of_single);
        }
    }else{
        /* write_addr is not SPI_FLASH_PAGE_SIZE aligned  */
        if(0 == num_of_page){
            /* (num_byte_to_write + write_addr) > SPI_FLASH_PAGE_SIZE */
            if(num_of_single > count){
                temp = num_of_single - count;
                spi_flash_page_write(pbuffer,write_addr,count);
                write_addr += count;
                pbuffer += count;
                spi_flash_page_write(pbuffer,write_addr,temp);
            }else
                spi_flash_page_write(pbuffer,write_addr,num_byte_to_write);
        }else{
            /* num_byte_to_write > SPI_FLASH_PAGE_SIZE */
            num_byte_to_write -= count;
            num_of_page = num_byte_to_write / SPI_FLASH_PAGE_SIZE;
            num_of_single = num_byte_to_write % SPI_FLASH_PAGE_SIZE;

            spi_flash_page_write(pbuffer,write_addr, count);
            write_addr += count;
            pbuffer += count;

            while(num_of_page--){
                spi_flash_page_write(pbuffer,write_addr,SPI_FLASH_PAGE_SIZE);
                write_addr += SPI_FLASH_PAGE_SIZE;
                pbuffer += SPI_FLASH_PAGE_SIZE;
            }

            if(0 != num_of_single)
                spi_flash_page_write(pbuffer,write_addr,num_of_single);
        }
    }
}

/*!
    \brief      read a block of data from the flash
    \param[in]  pbuffer: pointer to the buffer that receives the data read from the flash
    \param[in]  read_addr: flash's internal address to read from
    \param[in]  num_byte_to_read: number of bytes to read from the flash
    \param[out] none
    \retval     none
*/
void spi_flash_buffer_read(uint8_t* pbuffer, uint32_t read_addr, uint16_t num_byte_to_read)
{
    /* select the flash: chip slect low */
    SPI_FLASH_CS_LOW();

    uint8_t send_data[] = {READ ,(read_addr & 0xFF0000) >> 16, (read_addr & 0xFF00) >> 8, read_addr & 0xFF};
    SpiWriteBuf(DEF_HAL_SPI1, send_data, 4);
    rev_data_2 = pbuffer;
    for(int i = 0; i < num_byte_to_read; i++)
    {
        send_data_2[i] = DUMMY_BYTE;
    }

    SpiWriteReadBuf(DEF_HAL_SPI1, pbuffer, send_data_2, num_byte_to_read);
    /* deselect the flash: chip select high */
    SPI_FLASH_CS_HIGH();
}

/*!
    \brief      read flash identification
    \param[in]  none
    \param[out] none
    \retval     flash identification
*/
uint32_t spi_flash_read_id(void)
{
    uint32_t temp = 0;
    SPI_FLASH_CS_LOW();
    uint8_t send_data[4] = {0x9F, DUMMY_BYTE, DUMMY_BYTE, DUMMY_BYTE};
    uint8_t rev_data[4] = {0};
    SpiWriteReadBuf(DEF_HAL_SPI1, rev_data, send_data, 4);
    temp = (rev_data[1] << 16) | (rev_data[2] << 8) | rev_data[3];
    SPI_FLASH_CS_HIGH();
    return temp;
}

/*!
    \brief      initiate a read data byte (read) sequence from the flash
    \param[in]  read_addr: flash's internal address to read from
    \param[out] none
    \retval     none
*/
void spi_flash_start_read_sequence(uint32_t read_addr)
{
    /* select the flash: chip select low */
    SPI_FLASH_CS_LOW();

    /* send "read from memory " instruction */
    spi_flash_send_byte(READ);

    /* send the 24-bit address of the address to read from */
    /* send read_addr high nibble address byte */
    spi_flash_send_byte((read_addr & 0xFF0000) >> 16);
    /* send read_addr medium nibble address byte */
    spi_flash_send_byte((read_addr& 0xFF00) >> 8);
    /* send read_addr low nibble address byte */
    spi_flash_send_byte(read_addr & 0xFF);
}

/*!
    \brief      read a byte from the SPI flash
    \param[in]  none
    \param[out] none
    \retval     byte read from the SPI flash
*/
uint8_t spi_flash_read_byte(void)
{
    return(spi_flash_send_byte(DUMMY_BYTE));
}

/*!
    \brief      send a byte through the SPI interface and return the byte received from the SPI bus
    \param[in]  byte: byte to send
    \param[out] none
    \retval     the value of the received byte
*/


/*!
    \brief      send a half word through the SPI interface and return the half word received from the SPI bus
    \param[in]  half_word: half word to send
    \param[out] none
    \retval     the value of the received byte
*/
uint16_t spi_flash_send_halfword(uint16_t half_word)
{
    /* loop while data register in not emplty */
    while(RESET == spi_i2s_flag_get(SPI0,SPI_FLAG_TBE));

    /* send half word through the SPI0 peripheral */
    spi_i2s_data_transmit(SPI0,half_word);

    /* wait to receive a half word */
    while(RESET == spi_i2s_flag_get(SPI0,SPI_FLAG_RBNE));

    /* return the half word read from the SPI bus */
    return spi_i2s_data_receive(SPI0);
}

/*!
    \brief      enable the write access to the flash
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_flash_write_enable(void)
{
    /* select the flash: chip select low */
    SPI_FLASH_CS_LOW();

    /* send "write enable" instruction */
    // spi_flash_send_byte(WREN);
    SpiWriteBuf(DEF_HAL_SPI1, &WREN, 1);
    /* deselect the flash: chip select high */
    SPI_FLASH_CS_HIGH();
}

/*!
    \brief      poll the status of the write in progress(wip) flag in the flash's status register 轮询flash状态寄存器中正在写(wip)标志的状态
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_flash_wait_for_write_end(void)
{
    uint8_t flash_status = 0;

    /* select the flash: chip select low */
    SPI_FLASH_CS_LOW();

    /* send "read status register" instruction发送“读取状态寄存器”指令 */
    SpiWriteBuf(DEF_HAL_SPI1, &RDSR, 1);

    /* loop as long as the memory is busy with a write cycle 只要内存忙于写循环，就进行循环*/
    do{
        /* send a dummy byte to generate the clock needed by the flash
        and put the value of the status register in flash_status variable */
        SpiWriteReadBuf(DEF_HAL_SPI1, &flash_status, &DUMMY_BYTE, 1);
    }while((flash_status & WIP_FLAG) == SET);

    /* deselect the flash: chip select high */
    SPI_FLASH_CS_HIGH();
}
