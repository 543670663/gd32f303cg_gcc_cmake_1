#ifndef __HAL_SPI_H_
#define __HAL_SPI_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEF_HAL_SPI1 0
#define DEF_HAL_SPI2 1
#define DEF_HAL_SPI3 2

typedef enum {
    HAL_SPI1 = DEF_HAL_SPI1,
    HAL_SPI2 = DEF_HAL_SPI2,
    HAL_SPI3 = DEF_HAL_SPI3,
} HalSpiEnum;

typedef enum {
    USE_SOFT_CS = 0,
    USE_HARD_CS,
} SpiCsUseEnum;

typedef enum {
    SPI_FULL_DUPLEX_MASTER = 0, // 全双工主机
    SPI_FULL_DUPLEX_SLAVE,      // 全双工从机
    SPI_HALF_DUPLEX_MASTER,     // 半双工主机
    SPI_HALF_DUPLEX_SLAVE,      // 半双工从机
    SPI_RECV_ONLY_MASTER,       // 主机仅接收
} SpiModeEnum;

typedef enum {
    SPI_CPHA0_CPOL0 = 0,
    SPI_CPHA0_CPOL1,
    SPI_CPHA1_CPOL0,
    SPI_CPHA1_CPOL1,
} SpiClockModeEnum;

void SpiInit(HalSpiEnum spi_num, SpiModeEnum mode, SpiClockModeEnum clk_mode, uint32_t bit_rate, SpiCsUseEnum cs_mode);

/* send a byte through the SPI interface and return the byte received from the SPI bus */
uint8_t spi_flash_send_byte(uint8_t byte);
void SpiWriteBuf(HalSpiEnum spi_num, uint8_t *data, uint16_t size);
void SpiReadBuf(HalSpiEnum spi_num, uint8_t *data, uint16_t size);
uint8_t SpiFlashSendThenRevByte(HalSpiEnum spi_num, uint8_t byte);
uint8_t SpiFlashRevByte(HalSpiEnum spi_num);
uint8_t SpiWriteReadBuf(HalSpiEnum spi_num, uint8_t *rev, uint8_t *Send, uint16_t size);
//void SetSpiIntEnable(HalSpiEnum spi_num, bool en);
//bool SpiWriteBuf(HalSpiEnum spi_num, uint8_t *data, uint16_t len);
//bool SpiReadBuf(HalSpiEnum spi_num, uint8_t *data, uint16_t len);
//uint8_t SpiReadWriteByte_CommWithBoard(HalSpiEnum spi_num, unsigned char byte);
//void SpiWriteByte_CommWithBoard(HalSpiEnum spi_num, unsigned char byte);
//
//void Spi1IntHandler(unsigned char recv_byte);
//void Spi2IntHandler(unsigned char recv_byte);



#ifdef __cplusplus
}
#endif

#endif
