#include <gd32f30x.h>
#include "hal_gpio.h"

void GpioInit()
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOF);
}

void SetGpioPinDir(GpioEnum gpio, GpioDirEnum dir)
{
    /*gpio 初始化*/
    (dir == GPIO_INPUT) ?
    gpio_init((GPIO_BASE + 0x00000000U) + gpio.port * 0x00000400U, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ,(((uint32_t)0x01U << (gpio.pin))))
    : gpio_init((GPIO_BASE + 0x00000000U) + gpio.port * 0x00000400U, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,(((uint32_t)0x01U << (gpio.pin))));
    /*复位寄存器使能*/
    GPIO_BC((GPIO_BASE + 0x00000000U) + gpio.port * 0x00000400U) = (((uint32_t)0x01U<<(gpio.pin)));
}

void SetGpioPinState(GpioEnum gpio, GpioStateEnum state)
{
    (state == HIGH_LEVEL) ? ( GPIO_BOP((GPIO_BASE + 0x00000000U) + gpio.port * 0x00000400U) = ((uint32_t)((uint32_t)0x01U << (gpio.pin))) ) :
    ( GPIO_BC((GPIO_BASE + 0x00000000U) + gpio.port * 0x00000400U) = (((uint32_t)0x01U<<(gpio.pin))) );
}

GpioStateEnum GetGpioPinState(GpioEnum gpio)
{
    GpioStateEnum gpio_state;
    gpio_state = gpio_input_bit_get ((GPIO_BASE + 0x00000000U) + gpio.port * 0x00000400U, (((uint32_t)0x01U<<(gpio.pin)))) == RESET ?
     LOW_LEVEL :  HIGH_LEVEL;
    return gpio_state;
}

void ToggleGpioPinState(GpioEnum gpio)
{
    gpio_bit_write((GPIO_BASE + 0x00000000U) + gpio.port * 0x00000400U, (((uint32_t)0x01U<<(gpio.pin))),
                   (bit_status)(1 - gpio_input_bit_get((GPIO_BASE + 0x00000000U) + gpio.port * 0x00000400U,(((uint32_t)0x01U<<(gpio.pin))))));
}