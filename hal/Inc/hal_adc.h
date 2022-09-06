#ifndef __HAL_ADC_H
#define __HAL_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>





#if defined(TARGET_EC)
    #define MCU_VREF 2500
#elif defined(TARGET_VPM)
    #define MCU_VREF 3300
#else
    #define MCU_VREF 3300
#endif

// 需按ADC通道号递增 的顺序排列

typedef enum {
        HAL_ADC_PA0 = 0,
        HAL_ADC_PA1,
        HAL_ADC_PA2,
        HAL_ADC_PA3,
        HAL_ADC_PA4,
        HAL_ADC_PA5,
        HAL_ADC_PA6,
        HAL_ADC_PA7,
        HAL_ADC_PB0,
        HAL_ADC_PB1,
        HAL_ADC_PC0,
        HAL_ADC_PC1,
        HAL_ADC_PC2,
        HAL_ADC_PC3,
        HAL_ADC_PC4,
        HAL_ADC_PC5,
        HAL_ADC_NONE,
} HalAdcEnum;


typedef struct {
    unsigned char enumName;
    HalAdcEnum adcPin;
} AdcDevStruct;

void AdcInit(HalAdcEnum adc);
uint16_t GetAdcValue(HalAdcEnum adc_pin);
uint16_t GetAdcResolution(void);
uint16_t GetAdcVol(HalAdcEnum adc_pin);

#ifdef __cplusplus
}
#endif

#endif