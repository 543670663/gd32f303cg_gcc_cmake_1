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

// 实测误差报告：
// 1ms时：  实测1001.5us
// 10ms时： 实测10.0s
// 500ms时：实测500.8ms
// 1000ms时：实测1002.0ms
void DelayMs(uint32_t ms)
{
    delay = ms * 1000U;
    while(0U != delay){
    }
}

// 实测误差报告：
// 1us时：  实测4.9us
// 10us时： 实测13.95us
// 500us时：实测501us
// 1000us时：实测998.4us
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
