/*!
    \file  systick.h
    \brief the header file of systick
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-23, V1.0.0, demo for GD32F30x
*/

#ifndef SYS_TICK_H
#define SYS_TICK_H

#include <stdint.h>

/* configure systick */
void DelayInit(void);
/* delay a time in milliseconds */
void DelayMs(uint32_t ms);
void DelayUs(uint32_t us);
/* delay decrement */
void delay_decrement(void);

#endif /* SYS_TICK_H */
