#ifndef __HAL_DAC_H
#define __HAL_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

typedef enum {
    HAL_DAC_PA4 = 0,
    HAL_DAC_PA5,
} HalDacEnum;

typedef struct {
    unsigned char enumName;
    HalDacEnum dacPin;
} DacDevStruct;

void DacInit(HalDacEnum dac);
void SetDacVol(HalDacEnum dac_pin, uint16_t vol);

#ifdef __cplusplus
}
#endif

#endif