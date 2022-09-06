/*!
    \file  systick.c
    \brief the systick configuration file
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-23, V1.0.0, demo for GD32F30x
*/

#include "gd32f30x.h"
#include "hal_delay.h"

volatile static uint32_t delay;

/*!
    \brief      configure systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DelayInit(void)
{
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000000U)){
        /* capture error */
        while (1){
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

// ʵ�����棺
// 1msʱ��  ʵ��1001.5us
// 10msʱ�� ʵ��10.0s
// 500msʱ��ʵ��500.8ms
// 1000msʱ��ʵ��1002.0ms
void DelayMs(uint32_t ms)
{
    delay = ms * 1000U;
    while(0U != delay){
    }
}

// ʵ�����棺
// 1usʱ��  ʵ��4.9us
// 10usʱ�� ʵ��13.95us
// 500usʱ��ʵ��501us
// 1000usʱ��ʵ��998.4us
void DelayUs(uint32_t us)
{
    delay = us;
    while(0U != delay){
    }
}

/*!
    \brief      delay decrement
    \param[in]  none
    \param[out] none
    \retval     none
*/
void delay_decrement(void)
{
    if (0U != delay){
        delay--;
    }
}
