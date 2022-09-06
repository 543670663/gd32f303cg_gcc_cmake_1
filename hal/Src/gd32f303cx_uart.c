#include "hal_uart.h"
#include <gd32f30x.h>
//#include "assert.h"
//#include "ring_buffer.h"
//#include "common_func.h"
#include <stdlib.h>
#include "gd32f30x.h"

//#define UART_SRB_SIZE 256
//#define UART_RRB_SIZE 512
//
//static RingBufStruct TxRing[MAX_UART_NUM];
//static RingBufStruct RxRing[MAX_UART_NUM];
//
//static uint8_t *pRxBuff[MAX_UART_NUM] = {NULL};
//static uint8_t *pTxBuff[MAX_UART_NUM] = {NULL};
//
typedef struct __UART_HandleTypeDef
{
    uint32_t                 Instance;        /*!< UART registers base address        */
} UART_HandleTypeDef;

static UART_HandleTypeDef uart[MAX_UART_NUM] = {0};



void UartInit(HalUartEnum uart_num, unsigned int baudrate)
{
    switch (uart_num)
    {
        case HAL_UART1: uart[uart_num].Instance = USART0;
            rcu_periph_clock_enable(RCU_GPIOA);
            rcu_periph_clock_enable(RCU_USART0);
            gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
            gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
            break;
        case HAL_UART2: uart[uart_num].Instance = USART1;
            rcu_periph_clock_enable(RCU_GPIOA);
            rcu_periph_clock_enable(RCU_USART1);
            gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
            gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
            break;
        case HAL_UART3: uart[uart_num].Instance = USART2;
            rcu_periph_clock_enable(RCU_GPIOA);
            rcu_periph_clock_enable(RCU_USART2);
            gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
            gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
            break;
        case HAL_UART4: uart[uart_num].Instance = UART3;
            rcu_periph_clock_enable(RCU_GPIOC);
            rcu_periph_clock_enable(RCU_UART3);
            gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
            gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
            break;
        case HAL_UART5: uart[uart_num].Instance = UART4;
            rcu_periph_clock_enable(RCU_GPIOC);
            rcu_periph_clock_enable(RCU_GPIOD);
            rcu_periph_clock_enable(RCU_UART4);
            gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
            gpio_init(GPIOD, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
            break;
        default: ;
    }
    usart_deinit(uart[uart_num].Instance);//复位USART0
    //--------串口基本配置-------
    usart_baudrate_set(uart[uart_num].Instance, (uint32_t)baudrate);              //串口和波特率设置
    usart_receive_config(uart[uart_num].Instance, USART_RECEIVE_ENABLE);  //使能接收器(REN=1)
    usart_transmit_config(uart[uart_num].Instance, USART_TRANSMIT_ENABLE);//使能发送器(TEN=1)
    usart_enable(uart[uart_num].Instance);                                        //使能USART(UEN=1)

    //--------过采样方式设置-------
    usart_flag_clear(uart[uart_num].Instance,USART_FLAG_TC);   //初始化完成最后清零TC标志，为下次使用做准备
}

bool UartSendByte(HalUartEnum uart_num, unsigned char byte)
{
   while(!usart_flag_get(uart[uart_num].Instance,USART_FLAG_TBE)){}    //等待数据为0位TBE硬件置1
   usart_data_transmit(uart[uart_num].Instance,byte);                       //向TDATA寄存器写入字节数据

   while(!usart_flag_get(uart[uart_num].Instance,USART_FLAG_TC)){}     //等待发送完成位TC硬件置1
   usart_flag_clear(uart[uart_num].Instance,USART_FLAG_TC);	        //清除TC标志，为下次使用准备
}




//void UartSetParity(HalUartEnum uart_num, UartParityEnum parity)
//{
//    switch (parity)
//    {
//        case UART_NONE_PARITY:
//            uart[uart_num].Init.WordLength = UART_WORDLENGTH_8B;
//            uart[uart_num].Init.Parity = UART_PARITY_NONE;
//            break;
//
//        case UART_ODD_PARITY:
//            uart[uart_num].Init.WordLength = UART_WORDLENGTH_9B;
//            uart[uart_num].Init.Parity = UART_PARITY_ODD;
//            break;
//
//        case UART_EVEN_PARITY:
//            uart[uart_num].Init.WordLength = UART_WORDLENGTH_9B;
//            uart[uart_num].Init.Parity = UART_PARITY_EVEN;
//            break;
//
//        default: WS_ERROR();
//    }
//    if (HAL_UART_Init(&uart[uart_num]) != HAL_OK) {
//        WS_ERROR();
//    }
//}
//
//// ����STM32 �ٷ�HAL����Ҫ�Ļص�����
//void HAL_UART_MspInit(UART_HandleTypeDef* huart)
//{
//    GPIO_InitTypeDef gpio_cfg = {0};
//
//    if (huart->Instance == USART1) {
//        __HAL_RCC_USART1_CLK_ENABLE();
//        __HAL_RCC_GPIOA_CLK_ENABLE();
//
//        gpio_cfg.Pin = GPIO_PIN_9;
//        gpio_cfg.Mode = GPIO_MODE_AF_PP;
//        gpio_cfg.Speed = GPIO_SPEED_FREQ_HIGH;
//        HAL_GPIO_Init(GPIOA, &gpio_cfg);
//
//        gpio_cfg.Pin = GPIO_PIN_10;
//        gpio_cfg.Mode = GPIO_MODE_INPUT;
//        gpio_cfg.Pull = GPIO_NOPULL;
//        HAL_GPIO_Init(GPIOA, &gpio_cfg);
//    }
//    else if (huart->Instance == USART2) {
//        __HAL_RCC_USART2_CLK_ENABLE();
//        __HAL_RCC_GPIOA_CLK_ENABLE();
//
//        gpio_cfg.Pin = GPIO_PIN_2;
//        gpio_cfg.Mode = GPIO_MODE_AF_PP;
//        gpio_cfg.Speed = GPIO_SPEED_FREQ_HIGH;
//        HAL_GPIO_Init(GPIOA, &gpio_cfg);
//
//        gpio_cfg.Pin = GPIO_PIN_3;
//        gpio_cfg.Mode = GPIO_MODE_INPUT;
//        gpio_cfg.Pull = GPIO_NOPULL;
//        HAL_GPIO_Init(GPIOA, &gpio_cfg);
//
//    }
//    else if (huart->Instance == USART3) {
////        __HAL_RCC_USART3_CLK_ENABLE();
////        __HAL_RCC_GPIOB_CLK_ENABLE();
////
////        gpio_cfg.Pin = GPIO_PIN_10;
////        gpio_cfg.Mode = GPIO_MODE_AF_PP;
////        gpio_cfg.Speed = GPIO_SPEED_FREQ_HIGH;
////        HAL_GPIO_Init(GPIOB, &gpio_cfg);
////
////        gpio_cfg.Pin = GPIO_PIN_11;
////        gpio_cfg.Mode = GPIO_MODE_INPUT;
////        gpio_cfg.Pull = GPIO_NOPULL;
////        HAL_GPIO_Init(GPIOB, &gpio_cfg);
//
//        __HAL_RCC_USART3_CLK_ENABLE();
//        __HAL_RCC_GPIOD_CLK_ENABLE();
//        __HAL_AFIO_REMAP_USART3_ENABLE();
//        /**USART3 GPIO Configuration
//        PD8     ------> USART3_TX
//        PD9     ------> USART3_RX
//        */
//        gpio_cfg.Pin = GPIO_PIN_8;
//        gpio_cfg.Mode = GPIO_MODE_AF_PP;
//        gpio_cfg.Speed = GPIO_SPEED_FREQ_HIGH;
//        HAL_GPIO_Init(GPIOD, &gpio_cfg);
//
//        gpio_cfg.Pin = GPIO_PIN_9;
//        gpio_cfg.Mode = GPIO_MODE_INPUT;
//        gpio_cfg.Pull = GPIO_NOPULL;
//        HAL_GPIO_Init(GPIOD, &gpio_cfg);
//        __HAL_AFIO_REMAP_USART3_ENABLE();
//
//    }
//    else if(huart->Instance == UART4) {
//        __HAL_RCC_UART4_CLK_ENABLE();
//        __HAL_RCC_GPIOC_CLK_ENABLE();
//
//        gpio_cfg.Pin = GPIO_PIN_10;
//        gpio_cfg.Mode = GPIO_MODE_AF_PP;
//        gpio_cfg.Speed = GPIO_SPEED_FREQ_HIGH;
//        HAL_GPIO_Init(GPIOC, &gpio_cfg);
//
//        gpio_cfg.Pin = GPIO_PIN_11;
//        gpio_cfg.Mode = GPIO_MODE_INPUT;
//        gpio_cfg.Pull = GPIO_NOPULL;
//        HAL_GPIO_Init(GPIOC, &gpio_cfg);
//    }
//    else if(huart->Instance == UART5) {
//        __HAL_RCC_UART5_CLK_ENABLE();
//        __HAL_RCC_GPIOC_CLK_ENABLE();
//        __HAL_RCC_GPIOD_CLK_ENABLE();
//
//        gpio_cfg.Pin = GPIO_PIN_12;
//        gpio_cfg.Mode = GPIO_MODE_AF_PP;
//        gpio_cfg.Speed = GPIO_SPEED_FREQ_HIGH;
//        HAL_GPIO_Init(GPIOC, &gpio_cfg);
//
//        gpio_cfg.Pin = GPIO_PIN_2;
//        gpio_cfg.Mode = GPIO_MODE_INPUT;
//        gpio_cfg.Pull = GPIO_NOPULL;
//        HAL_GPIO_Init(GPIOD, &gpio_cfg);
//    }
//}
//
//void UartIntInit(HalUartEnum uart_num)
//{
//    if (pRxBuff[uart_num] == NULL) {
//        pRxBuff[uart_num] = (uint8_t *)malloc(UART_RRB_SIZE);
//    }
//    if (pTxBuff[uart_num] == NULL) {
//        pTxBuff[uart_num] = (uint8_t *)malloc(UART_SRB_SIZE);
//    }
//
//    WS_ASSERT(pRxBuff[uart_num] != NULL);
//    WS_ASSERT(pTxBuff[uart_num] != NULL);
//
//    RingBufInit(&RxRing[uart_num], pRxBuff[uart_num], sizeof(pRxBuff[uart_num][0]), UART_RRB_SIZE);
//    RingBufInit(&TxRing[uart_num], pTxBuff[uart_num], sizeof(pTxBuff[uart_num][0]), UART_SRB_SIZE);
//
//    __HAL_UART_ENABLE_IT(&uart[uart_num], UART_IT_TXE);
//    __HAL_UART_ENABLE_IT(&uart[uart_num], UART_IT_RXNE);
//
//    SetUartIntEn(uart_num, true);
//}
//
//void SetUartIntEn(HalUartEnum uart_num, bool en)
//{
//    if (en) {
//        switch (uart_num) {
//            case HAL_UART1: {
//                HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
//                HAL_NVIC_EnableIRQ(USART1_IRQn);
//                break;
//            }
//            case HAL_UART2: {
//                HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
//                HAL_NVIC_EnableIRQ(USART2_IRQn);
//                break;
//            }
//            case HAL_UART3: {
//                HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
//                HAL_NVIC_EnableIRQ(USART3_IRQn);
//                break;
//            }
//            case HAL_UART4: {
//                HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
//                HAL_NVIC_EnableIRQ(UART4_IRQn);
//                break;
//            }
//            case HAL_UART5: {
//                HAL_NVIC_SetPriority(UART5_IRQn, 0, 0);
//                HAL_NVIC_EnableIRQ(UART5_IRQn);
//                break;
//            }
//            default:
//                WS_ERROR();
//        }
//        return;
//    }
//
//    switch (uart_num) {
//        case HAL_UART1: {
//            HAL_NVIC_DisableIRQ(USART1_IRQn);
//            break;
//        }
//        case HAL_UART2: {
//            HAL_NVIC_DisableIRQ(USART2_IRQn);
//            break;
//        }
//        case HAL_UART3: {
//            HAL_NVIC_DisableIRQ(USART3_IRQn);
//            break;
//        }
//        case HAL_UART4: {
//            HAL_NVIC_DisableIRQ(UART4_IRQn);
//            break;
//        }
//        case HAL_UART5: {
//            HAL_NVIC_DisableIRQ(UART5_IRQn);
//            break;
//        }
//        default:
//            WS_ERROR();
//    }
//}
//
//unsigned int UartReadLineState(HalUartEnum uart_num)
//{
//    return uart[uart_num].Instance->SR;
//}
//
//unsigned int UartSendRingBuf(HalUartEnum uart_num, const void *str, unsigned int count)
//{
//    uint8_t ch;
//    uint32_t ret;
//    uint8_t *p8 = (uint8_t *)str;
//    count = MIN(count, UART_SRB_SIZE);
//
//    /* Don't let UART transmit ring buffer change in the UART IRQ handler */
//    __HAL_UART_DISABLE_IT(&uart[uart_num], UART_IT_TXE);
//
//    /* Move as much data as possible into transmit ring buffer */
//    ret = RingBufInsertMult(&TxRing[uart_num], p8, count);
//
//    /* Fill FIFO until full or until TX ring buffer is empty */
//    while (__HAL_UART_GET_FLAG(&uart[uart_num], UART_FLAG_TXE) &&
//           RingBufPop(&TxRing[uart_num], &ch)) {
//        uart[uart_num].Instance->DR = ch;
//    }
//
//    /* Add additional data to transmit ring buffer if possible */
//    ret += RingBufInsertMult(&TxRing[uart_num], (p8 + ret), (count - ret));
//
//    /* Enable UART transmit interrupt */
//    __HAL_UART_ENABLE_IT(&uart[uart_num], UART_IT_TXE);
//
//    return ret;
//}
//
//unsigned int UartReadRingBuf(HalUartEnum uart_num, void *data, unsigned int num)
//{
//    return RingBufPopMult(&RxRing[uart_num], (uint8_t *)data, MIN(num, UART_RRB_SIZE));
//}
//
//unsigned int UartReadBufCount(HalUartEnum uart_num)
//{
//    return GetRingBufCount(&RxRing[uart_num]);
//}
//
//bool IsUartBufEmpty(HalUartEnum uart_num)
//{
//    return IsRingBufEmpty(&RxRing[uart_num]);
//}
//
//
//
//unsigned char UartReadByte(HalUartEnum uart_num)
//{
//    if (uart[uart_num].Init.Parity == UART_PARITY_NONE)
//    {
//        return (uint8_t)(uart[uart_num].Instance->DR & (uint8_t)0x00FF);
//    }
//    else
//    {
//        return (uint8_t)(uart[uart_num].Instance->DR & (uint8_t)0x007F);
//    }
//}
//
//bool UartSendByte(HalUartEnum uart_num, unsigned char byte)
//{
//    while (!__HAL_UART_GET_FLAG(&uart[uart_num], UART_FLAG_TC)); // �ȴ��������
//    uart[uart_num].Instance->DR = byte;
//    return true;
//}
//
//
//bool UartSendBuf(HalUartEnum uart_num, void *buf, uint16_t len)
//{
//    if (HAL_UART_Transmit(&uart[uart_num], (uint8_t *)buf, len, HAL_MAX_DELAY) == HAL_OK) {
//        return true;
//    }
//    return false;
//}
//
//bool UartReadBuf(HalUartEnum uart_num, void *buf, uint16_t len, uint32_t timeout)
//{
//    if (HAL_UART_Receive(&uart[uart_num], (uint8_t *)buf, len, timeout) == HAL_OK) {
//        return true;
//    }
//    return false;
//}
//
//static void UartRingBufProcess(HalUartEnum uart_num)
//{
//    if (__HAL_UART_GET_IT_SOURCE(&uart[uart_num], UART_IT_TXE)) {
//        uint8_t ch;
//        /* Fill FIFO until full or until TX ring buffer is empty */
//        while (__HAL_UART_GET_FLAG(&uart[uart_num], UART_FLAG_TXE)
//            && RingBufPop(&TxRing[uart_num], &ch)) {
//            uart[uart_num].Instance->DR = ch;
//        }
//
//        /* Disable transmit interrupt if the ring buffer is empty */
//        if (IsRingBufEmpty(&TxRing[uart_num])) {
//            __HAL_UART_DISABLE_IT(&uart[uart_num], UART_IT_TXE);
//        }
//    }
//
//    /* Handle receive interrupt */
//    while (__HAL_UART_GET_FLAG(&uart[uart_num], UART_FLAG_RXNE)) {
////        UartSendByte(uart_num, 't')
//        uint8_t ch = UartReadByte(uart_num);
//        RingBufInsert(&RxRing[uart_num], &ch);
//    }
//}
//
//void USART1_IRQHandler(void)
//{
//    UartRingBufProcess(HAL_UART1);
//}
//
//void USART2_IRQHandler(void)
//{
//    UartRingBufProcess(HAL_UART2);
//}
//
//void USART3_IRQHandler(void)
//{
//    UartRingBufProcess(HAL_UART3);
//}
//
//void UART4_IRQHandler(void)
//{
//    UartRingBufProcess(HAL_UART4);
//}
//
//void UART5_IRQHandler(void)
//{
//    UartRingBufProcess(HAL_UART5);
//}