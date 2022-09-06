#ifndef __LOG_H_
#define __LOG_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>

void LogInit(void);
void SendLog(void *str);

#if defined(DEBUG)

#define LOG_BUF_SIZE    128

#define LOGI(format, args...)   do                                  \
                                {                                   \
                                    char str[LOG_BUF_SIZE] = {0};   \
                                    sprintf(str, format, ##args);   \
                                    SendLog(str);                   \
                                    SendLog("\r\n");                \
                                } while (0)
#define LOG(format, args...)    do                                  \
                                {                                   \
                                    char str[LOG_BUF_SIZE] = {0};   \
                                    sprintf(str, format, ##args);   \
                                    SendLog(str);                   \
                                    SendLog("\r\n");                \
                                } while (0)

// 不创建新行
#define LOG_N(format, args...)  do                                  \
                                {                                   \
                                    char str[LOG_BUF_SIZE] = {0};   \
                                    sprintf(str, format, ##args);   \
                                    SendLog(str);                   \
                                } while (0)

#define LOG_I(format, args...)
#define LOG_E(format, args...)
#define LOG_D(format, args...)
#elif defined(RELEASE)
#define LOGI(format, args...)
#define LOG(format, args...)
#define LOG_N(format, args...)

#define LOG_I(format, args...)
#define LOG_E(format, args...)
#define LOG_D(format, args...)
#endif


#ifdef __cplusplus
}
#endif

#endif