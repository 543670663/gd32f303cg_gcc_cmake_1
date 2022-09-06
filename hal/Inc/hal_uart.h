#ifndef __HAL_UART_H_
#define __HAL_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32f30x.h"
#include <stdio.h>

#define MAX_UART_NUM    5

typedef enum {
    HAL_UART1 = 0,
    HAL_UART2,
    HAL_UART3,
    HAL_UART4,
    HAL_UART5,
} HalUartEnum;

typedef enum {
    UART_NONE_PARITY = 0,   // ��У��
    UART_ODD_PARITY,        // ��У��
    UART_EVEN_PARITY,       // żУ��
} UartParityEnum;

void UartInit(HalUartEnum uart_num, unsigned int baudrate);
//void UartSetParity(HalUartEnum uart_num, UartParityEnum parity);
//void UartIntInit(HalUartEnum uart_num);
//void SetUartIntEn(HalUartEnum uart_num, bool en);
//
//unsigned int UartSendRingBuf(HalUartEnum uart_num, const void *str, unsigned int count);
//unsigned int UartReadRingBuf(HalUartEnum uart_num, void *data, unsigned int num);
//unsigned int UartReadBufCount(HalUartEnum uart_num);
//bool IsUartBufEmpty(HalUartEnum uart_num);
//unsigned char UartRecvDataRegIsEmpty(HalUartEnum uart_num);
//unsigned char UartReadByte(HalUartEnum uart_num);
bool UartSendByte(HalUartEnum uart_num, unsigned char byte);
//
//bool UartSendBuf(HalUartEnum uart_num, void *buf, uint16_t len);
//bool UartReadBuf(HalUartEnum uart_num, void *buf, uint16_t len, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif