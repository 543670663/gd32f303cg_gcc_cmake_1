#define __ADC_C__

#include <gd32f30x.h>
#include "adc.h"
#include "hal_delay.h"
void adc_gpio_config(void)
{
	/* ʹ�� GPIOC clock */
    rcu_periph_clock_enable(RCU_GPIOC);
	
    /* ����PC3 PC5 ����Ϊģ������ */  
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_MAX, GPIO_PIN_3 | GPIO_PIN_5);
}



void adc_init(void)
{
	//adc ���ų�ʼ��
	adc_gpio_config();

	/* ʹ��ADC0 ʱ��Դ */
    rcu_periph_clock_enable(RCU_ADC0);
    /* ����Ԥ��Ƶ */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);

	
	/* ADC ɨ��ģʽ�� */
	adc_special_function_config(ADC0,ADC_SCAN_MODE,ENABLE);
	/* ADC ������ʽ���� ����Ҫ�ⲿ���� */
	adc_external_trigger_source_config(ADC0, ADC_INSERTED_CHANNEL, ADC0_1_2_EXTTRIG_INSERTED_NONE);
	/* �����Ҷ��� */
	adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);
	/* ADC ģʽ���ã�����ADC�������� */
	adc_mode_config(ADC_MODE_FREE);  
	/* ADC ת��ͨ����С���ã�2��ͨ�� */
	adc_channel_length_config(ADC0,ADC_INSERTED_CHANNEL,2);

	/* ADC ͨ������ */
	adc_inserted_channel_config(ADC0, 0, ADC_CHANNEL_13, ADC_SAMPLETIME_239POINT5);
	/* ADC ͨ������ */
	adc_inserted_channel_config(ADC0, 1, ADC_CHANNEL_15, ADC_SAMPLETIME_239POINT5);

	/* ADC ������ʹ�� */
	adc_external_trigger_config(ADC0,ADC_INSERTED_CHANNEL,ENABLE);
	
	/* ʹ��ADC */
	adc_enable(ADC0);
	/* ADC У׼ */
	adc_calibration_enable(ADC0);
}






