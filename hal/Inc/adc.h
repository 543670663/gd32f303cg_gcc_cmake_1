#ifndef __ADC_H__
#define __ADC_H__

#ifdef __ADC_C__
#define __ADC_EXT__ 
#else
#define __ADC_EXT__ extern
#endif


#include <gd32f30x.h>


__ADC_EXT__ uint32_t adc_value[2];

void adc_timer_init(void);
void adc_dma_init(void);
void adc_gpio_config(void);
void adc_init(void);



#endif

