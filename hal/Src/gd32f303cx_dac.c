#include "hal_dac.h"
#include "hal_gpio.h"
#include "hal_delay.h"
#include <gd32f30x.h>

uint32_t DacName[2] = {DAC0, DAC1};

void DacInit(HalDacEnum dac)
{
    switch (dac)
    {
        case HAL_DAC_PA4:
            /* enable the clock of peripherals */
            rcu_periph_clock_enable(RCU_GPIOA);
            rcu_periph_clock_enable(RCU_DAC);
            /* once enabled the DAC, the corresponding GPIO pin is connected to the DAC converter automatically */
            gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_4);

            /* configure the DAC0 */
            dac_trigger_disable(DacName[dac]);
            dac_wave_mode_config(DacName[dac], DAC_WAVE_DISABLE);
            dac_output_buffer_enable(DacName[dac]);

            /* enable DAC0 and set data */
            dac_enable(DacName[dac]);
            break;

        case HAL_DAC_PA5: //TODO Î´²ÉÈ¡Ó²¼þcs
            /* enable the clock of peripherals */
            rcu_periph_clock_enable(RCU_GPIOA);
            rcu_periph_clock_enable(RCU_DAC);
            /* once enabled the DAC, the corresponding GPIO pin is connected to the DAC converter automatically */
            gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
            /* configure the DAC0 */
            dac_trigger_disable(DacName[dac]);
            dac_wave_mode_config(DacName[dac], DAC_WAVE_DISABLE);
            dac_output_buffer_enable(DacName[dac]);

            /* enable DAC0 and set data */
            dac_enable(DacName[dac]);
            break;
        default: ;
    }
}

void SetDacVol(HalDacEnum dac_pin, uint16_t vol)
{
    dac_data_set(DacName[dac_pin], DAC_ALIGN_12B_R, (uint32_t)vol * 4095 / 3300);
}
