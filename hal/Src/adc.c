#define __ADC_C__

#include <gd32f30x.h>
#include "adc.h"
#include "hal_delay.h"
void adc_gpio_config(void)
{
	/* 使能 GPIOC clock */
    rcu_periph_clock_enable(RCU_GPIOC);
	
    /* 设置PC3 PC5 引脚为模拟输入 */  
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_MAX, GPIO_PIN_3 | GPIO_PIN_5);
}



void adc_init(void)
{
	//adc 引脚初始化
	adc_gpio_config();

	/* 使能ADC0 时钟源 */
    rcu_periph_clock_enable(RCU_ADC0);
    /* 配置预分频 */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);

	
	/* ADC 扫描模式打开 */
	adc_special_function_config(ADC0,ADC_SCAN_MODE,ENABLE);
	/* ADC 触发方式配置 不需要外部触发 */
	adc_external_trigger_source_config(ADC0, ADC_INSERTED_CHANNEL, ADC0_1_2_EXTTRIG_INSERTED_NONE);
	/* 数据右对齐 */
	adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);
	/* ADC 模式配置，所有ADC独立工作 */
	adc_mode_config(ADC_MODE_FREE);  
	/* ADC 转换通道大小配置，2个通道 */
	adc_channel_length_config(ADC0,ADC_INSERTED_CHANNEL,2);

	/* ADC 通道配置 */
	adc_inserted_channel_config(ADC0, 0, ADC_CHANNEL_13, ADC_SAMPLETIME_239POINT5);
	/* ADC 通道配置 */
	adc_inserted_channel_config(ADC0, 1, ADC_CHANNEL_15, ADC_SAMPLETIME_239POINT5);

	/* ADC 插入组使能 */
	adc_external_trigger_config(ADC0,ADC_INSERTED_CHANNEL,ENABLE);
	
	/* 使能ADC */
	adc_enable(ADC0);
	/* ADC 校准 */
	adc_calibration_enable(ADC0);
}






