#include "hal_spi.h"
#include "gd32f30x.h"
#include "hal_delay.h"
uint32_t spi_name[3] = {SPI0, SPI1, SPI2};
SpiModeEnum SpiMode = 0;
SpiCsUseEnum CsMode = 0;

//my
spi_parameter_struct spi_init_struct[3] = {0};;


void SpiInit(HalSpiEnum spi_num, SpiModeEnum mode, SpiClockModeEnum clk_mode, uint32_t bit_rate, SpiCsUseEnum cs_mode)
{
    SpiMode = mode;
    CsMode = cs_mode;

    //使能相应的时钟
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOE);//TODO 开发板特有
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_SPI0);
    rcu_periph_clock_enable(RCU_SPI1);
    rcu_periph_clock_enable(RCU_SPI2);


    switch (spi_num)
    {
        case HAL_SPI1:
            /* 配置SPI 相关引脚 */
            gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_7);
            gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
            /* SPI 片选引脚配置 */
            gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);//TODO 开发板特有
            //gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);//TODO 开发板特有

            break;
        case HAL_SPI2:
            /* 配置SPI 相关引脚 */
            gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13 | GPIO_PIN_15);
            gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_14);
            /* SPI 片选引脚配置 */
            gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
            break;
        case HAL_SPI3:
            /* 配置SPI 相关引脚 */
            gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3 | GPIO_PIN_5);
            gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
            /* SPI 片选引脚配置 */
            gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
            break;
    }

    /* 片选引脚拉高 */
    //SPI_FLASH_CS_HIGH();//TODO 待完善

    switch (mode)
    {
        case SPI_FULL_DUPLEX_MASTER:
            spi_init_struct[spi_num].trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
            spi_init_struct[spi_num].device_mode          = SPI_MASTER;
            break;

        case SPI_FULL_DUPLEX_SLAVE:
            spi_init_struct[spi_num].trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
            spi_init_struct[spi_num].device_mode          = SPI_SLAVE;
            break;

        case SPI_HALF_DUPLEX_MASTER:
            spi_init_struct[spi_num].trans_mode           = SPI_TRANSMODE_BDRECEIVE;
            spi_init_struct[spi_num].device_mode          = SPI_MASTER;
            break;

        case SPI_HALF_DUPLEX_SLAVE:
            spi_init_struct[spi_num].trans_mode           = SPI_TRANSMODE_BDRECEIVE;
            spi_init_struct[spi_num].device_mode          = SPI_SLAVE;
            break;

        case SPI_RECV_ONLY_MASTER:
            spi_init_struct[spi_num].trans_mode           = SPI_TRANSMODE_RECEIVEONLY;
            spi_init_struct[spi_num].device_mode          = SPI_MASTER;
            break;

        default: ;
    }

    switch (clk_mode)
    {
        case SPI_CPHA0_CPOL0:
            spi_init_struct[spi_num].clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
            break;

        case SPI_CPHA0_CPOL1:
            spi_init_struct[spi_num].clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
            break;

        case SPI_CPHA1_CPOL0:
            spi_init_struct[spi_num].clock_polarity_phase = SPI_CK_PL_HIGH_PH_1EDGE;
            break;

        case SPI_CPHA1_CPOL1:
            spi_init_struct[spi_num].clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
            break;

        default: ;
    }

    switch (cs_mode)
    {
        case USE_SOFT_CS:
            spi_init_struct[spi_num].nss                  = SPI_NSS_SOFT;
            break;

        case USE_HARD_CS: //TODO 未采取硬件cs
            spi_init_struct[spi_num].nss                  = SPI_NSS_SOFT;
            break;
        default: ;
    }

    if (bit_rate >= 30000000) { // 大于30Mhz
        spi_init_struct[spi_num].prescale                = (spi_num == HAL_SPI1) ? SPI_PSC_4 : SPI_PSC_2;
    }
    else if (bit_rate >= 15000000) {
        spi_init_struct[spi_num].prescale                = (spi_num == HAL_SPI1) ? SPI_PSC_8 : SPI_PSC_4;
    }
    else if (bit_rate >= 7500000) {
        spi_init_struct[spi_num].prescale                = (spi_num == HAL_SPI1) ? SPI_PSC_16 : SPI_PSC_8;
    }
    else if (bit_rate >= 3750000) {
        spi_init_struct[spi_num].prescale                = (spi_num == HAL_SPI1) ? SPI_PSC_32 : SPI_PSC_16;
    }
    else if (bit_rate >= 1875000) {
        spi_init_struct[spi_num].prescale                = (spi_num == HAL_SPI1) ? SPI_PSC_64 : SPI_PSC_32;
    }
    else if (bit_rate >=  937500) {
        spi_init_struct[spi_num].prescale                = (spi_num == HAL_SPI1) ? SPI_PSC_128 : SPI_PSC_64;
    }
    else if (bit_rate >=  468750) {
        spi_init_struct[spi_num].prescale                = (spi_num == HAL_SPI1) ? SPI_PSC_256 : SPI_PSC_128;
    }
    else { // SPI1最低速为468750Hz，SPI2、SPI3最低速为234375Hz*
        spi_init_struct[spi_num].prescale                = SPI_PSC_256;
    }

    spi_init_struct[spi_num].frame_size                  = SPI_FRAMESIZE_8BIT;
    spi_init_struct[spi_num].endian                      = SPI_ENDIAN_MSB;

    //初始化
    spi_init(spi_name[spi_num], &spi_init_struct[spi_num]);

    /* 设置CRC校验 */
    //spi_crc_polynomial_set(spi_name[spi_num],7);//TODO 暂用 实际无

    /* 使能 SPI0 */
    spi_enable(spi_name[spi_num]);
}

uint8_t spi_flash_send_byte(uint8_t byte)
{
    /* loop while data register in not emplty */
    while (RESET == spi_i2s_flag_get(SPI0,SPI_FLAG_TBE));

    /* send byte through the SPI0 peripheral */
    spi_i2s_data_transmit(SPI0,byte);

    /* wait to receive a byte */
    while(RESET == spi_i2s_flag_get(SPI0,SPI_FLAG_RBNE));

    /* return the byte read from the SPI bus */
    return(spi_i2s_data_receive(SPI0));
}

uint8_t SpiFlashSendThenRevByte(HalSpiEnum spi_num, uint8_t byte)
{
    /* loop while data register in not emplty */
    while (RESET == spi_i2s_flag_get(spi_name[spi_num],SPI_FLAG_TBE));

    /* send byte through the SPI0 peripheral */
    spi_i2s_data_transmit(spi_name[spi_num], byte);

    /* wait to receive a byte */
    while(RESET == spi_i2s_flag_get(spi_name[spi_num],SPI_FLAG_RBNE));

    /* return the byte read from the SPI bus */
    return(spi_i2s_data_receive(spi_name[spi_num]));
}

void SpiWriteBuf(HalSpiEnum spi_num, uint8_t *data, uint16_t size)//TODO 未测试
{
    for(uint16_t i = 0; i < size; i++)
    {
        SpiFlashSendThenRevByte(spi_num, *(data + i));
    }
}

void SpiReadBuf(HalSpiEnum spi_num, uint8_t *data, uint16_t size)//TODO 未测试
{
    for(uint16_t i = 0; i < size; i++)
    {
        *(data + i) = SpiFlashSendThenRevByte(spi_num, 0x00);
    }
}

uint8_t SpiWriteReadBuf(HalSpiEnum spi_num, uint8_t *rev, uint8_t *Send, uint16_t size)
{
    while(size--){
        /* read a byte from the flash */
        *rev = spi_flash_send_byte(*Send);
        /* point to the next location where the byte read will be saved */
        Send++;
        rev++;
    }

}
//
//bool SpiReadBuf(HalSpiEnum spi_num, uint8_t *data, uint16_t size)
//{
//    if (HAL_SPI_Receive(&hspi[spi_num], data, size, 0xFF) == HAL_OK) {
//        return true;
//    }
//    else {
//        return false;
//    }
//}

//void TempDisableCommSpiIrq(void)
//{
//    NVIC_DisableIRQ(SSP1_IRQn);
//}
//void TempEnableCommSpiIrq(void)
//{
//    NVIC_EnableIRQ(SSP1_IRQn);
//}
//__weak_symbol void Spi1IntHandler(uint8_t recv_byte)
//{
//    WS_ERROR();
//}
//
//__weak_symbol void Spi2IntHandler(uint8_t recv_byte)
//{
//    WS_ERROR();
//}
//
//void SSP0_IRQHandler(void)
//{
//    static volatile uint8_t recv = 0;
//    NVIC_DisableIRQ(SSP0_IRQn);
//    recv = LPC_SSP0->DR;
//    Chip_SSP_ClearIntPending(LPC_SSP0, SSP_INT_CLEAR_BITMASK);
//    Spi1IntHandler(recv);
//    NVIC_EnableIRQ(SSP0_IRQn);
//}
//
//void SSP1_IRQHandler(void)
//{
//    static volatile uint8_t recv = 0;
//    NVIC_DisableIRQ(SSP1_IRQn);
//    recv = LPC_SSP1->DR;
//    Chip_SSP_ClearIntPending(LPC_SSP1, SSP_INT_CLEAR_BITMASK);
//    Spi2IntHandler(recv);
//    NVIC_EnableIRQ(SSP1_IRQn);
//}