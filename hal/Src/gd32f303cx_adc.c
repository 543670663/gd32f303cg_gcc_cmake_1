#include "hal_adc.h"
#include "hal_gpio.h"
#include "hal_delay.h"
#include <gd32f30x.h>
#include "assert.h"

// �谴ADCͨ���ŵ��� ��˳���ʼ��
static GpioEnum AdcGpio[] = {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
                             PB0, PB1, PC0, PC1, PC2, PC3, PC4, PC5,
                             };
static void SetAdcGpioPinDir(GpioEnum gpio)
{
    /*gpio ��ʼ��*/
    gpio_init((GPIO_BASE + 0x00000000U) + gpio.port * 0x00000400U, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ,(((uint32_t)0x01U << (gpio.pin))));
}

void AdcInit(HalAdcEnum adc)
{

    /* ʹ�� GPIOC clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    /* ����GPIO ����Ϊģ������ */
    SetAdcGpioPinDir(AdcGpio[adc]);
    /* ʹ��ADC0 ʱ��Դ */
    rcu_periph_clock_enable(RCU_ADC1);
    /* ����Ԥ��Ƶ */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);
    /* ADC ɨ��ģʽ�� */
    adc_special_function_config(ADC1,ADC_SCAN_MODE,ENABLE);
    /* ADC ������ʽ���� ����Ҫ�ⲿ���� */
    adc_external_trigger_source_config(ADC1, ADC_INSERTED_CHANNEL, ADC0_1_2_EXTTRIG_INSERTED_NONE);
    /* �����Ҷ��� */
    adc_data_alignment_config(ADC1,ADC_DATAALIGN_RIGHT);
    /* ADC ģʽ���ã�����ADC�������� */
    adc_mode_config(ADC_MODE_FREE);
    /* ADC ת��ͨ����С���ã�2��ͨ�� */
    adc_channel_length_config(ADC1,ADC_INSERTED_CHANNEL,2);

    /* ADC ������ʹ�� */
    adc_external_trigger_config(ADC1,ADC_INSERTED_CHANNEL,ENABLE);
    /* ʹ��ADC */
    adc_enable(ADC1);
    /* ADC У׼ */
    adc_calibration_enable(ADC1);
}

uint16_t GetAdcValue(HalAdcEnum adc_pin)
{
    uint16_t adc_val;
    /* ADC ͨ������ ����ʱ��Ϊ55.5��ʱ������ */
    adc_inserted_channel_config(ADC1, 0, adc_pin, ADC_SAMPLETIME_239POINT5);
    // ����û�в����ⲿ����������ʹ���������ADCת��
    adc_software_trigger_enable(ADC1, ADC_INSERTED_CHANNEL);

    while(!adc_flag_get(ADC1, ADC_FLAG_EOC));                       // �ȴ��������
    adc_flag_clear(ADC1, ADC_FLAG_EOC);                             // ���������־

    DelayMs(10);
    adc_val = ADC_IDATA0(ADC1); //ǰ16λ
    return adc_val;
}

uint16_t GetAdcResolution(void)
{
    return 4096;
}

uint16_t GetAdcVol(HalAdcEnum adc_pin)
{
    return (uint32_t)GetAdcValue(adc_pin) * MCU_VREF / GetAdcResolution();
}