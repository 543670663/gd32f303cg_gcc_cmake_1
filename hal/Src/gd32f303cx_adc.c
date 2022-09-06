#include "hal_adc.h"
#include "hal_gpio.h"
#include "hal_delay.h"
#include <gd32f30x.h>
#include "assert.h"

// 需按ADC通道号递增 的顺序初始化
static GpioEnum AdcGpio[] = {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
                             PB0, PB1, PC0, PC1, PC2, PC3, PC4, PC5,
                             };
static void SetAdcGpioPinDir(GpioEnum gpio)
{
    /*gpio 初始化*/
    gpio_init((GPIO_BASE + 0x00000000U) + gpio.port * 0x00000400U, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ,(((uint32_t)0x01U << (gpio.pin))));
}

void AdcInit(HalAdcEnum adc)
{

    /* 使能 GPIOC clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    /* 设置GPIO 引脚为模拟输入 */
    SetAdcGpioPinDir(AdcGpio[adc]);
    /* 使能ADC0 时钟源 */
    rcu_periph_clock_enable(RCU_ADC1);
    /* 配置预分频 */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);
    /* ADC 扫描模式打开 */
    adc_special_function_config(ADC1,ADC_SCAN_MODE,ENABLE);
    /* ADC 触发方式配置 不需要外部触发 */
    adc_external_trigger_source_config(ADC1, ADC_INSERTED_CHANNEL, ADC0_1_2_EXTTRIG_INSERTED_NONE);
    /* 数据右对齐 */
    adc_data_alignment_config(ADC1,ADC_DATAALIGN_RIGHT);
    /* ADC 模式配置，所有ADC独立工作 */
    adc_mode_config(ADC_MODE_FREE);
    /* ADC 转换通道大小配置，2个通道 */
    adc_channel_length_config(ADC1,ADC_INSERTED_CHANNEL,2);

    /* ADC 插入组使能 */
    adc_external_trigger_config(ADC1,ADC_INSERTED_CHANNEL,ENABLE);
    /* 使能ADC */
    adc_enable(ADC1);
    /* ADC 校准 */
    adc_calibration_enable(ADC1);
}

uint16_t GetAdcValue(HalAdcEnum adc_pin)
{
    uint16_t adc_val;
    /* ADC 通道配置 采样时间为55.5个时钟周期 */
    adc_inserted_channel_config(ADC1, 0, adc_pin, ADC_SAMPLETIME_239POINT5);
    // 由于没有采用外部触发，所以使用软件触发ADC转换
    adc_software_trigger_enable(ADC1, ADC_INSERTED_CHANNEL);

    while(!adc_flag_get(ADC1, ADC_FLAG_EOC));                       // 等待采样完成
    adc_flag_clear(ADC1, ADC_FLAG_EOC);                             // 清除结束标志

    DelayMs(10);
    adc_val = ADC_IDATA0(ADC1); //前16位
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