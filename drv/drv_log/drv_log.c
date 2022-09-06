#include "drv_log.h"
#include "drv_log_cfg.h"
#include "hal_uart.h"


void LogInit(void)
{
#if defined(DEBUG)
      UartInit(HAL_UART1, 115200);
#endif
    LOG("LOG Init Ok.");
}

void SendLog(void *str)
{
#if defined(DEBUG)
        uint8_t *tmp = (uint8_t *)str;
        for (uint8_t i = 0; i < strlen(str); i++) {
            UartSendByte(LOG_UART, tmp[i]);

            if (tmp[i] == '\0') {
                break;
            }
        }
#endif
}




