#ifndef __HAL_GPIO_H_
#define __HAL_GPIO_H_

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f30x.h"

typedef enum {
    GPIO_INPUT  = 0,
    GPIO_OUTPUT,
} GpioDirEnum;

typedef enum {
    LOW_LEVEL  = 0,
    HIGH_LEVEL,
} GpioStateEnum;

typedef struct {
    unsigned char port;
    unsigned char pin;
} GpioEnum;
#define PA0   ((GpioEnum) {0, 0})
#define PA1   ((GpioEnum) {0, 1})
#define PA2   ((GpioEnum) {0, 2})
#define PA3   ((GpioEnum) {0, 3})
#define PA4   ((GpioEnum) {0, 4})
#define PA5   ((GpioEnum) {0, 5})
#define PA6   ((GpioEnum) {0, 6})
#define PA7   ((GpioEnum) {0, 7})
#define PA8   ((GpioEnum) {0, 8})
#define PA9   ((GpioEnum) {0, 9})
#define PA10  ((GpioEnum) {0, 10})
#define PA11  ((GpioEnum) {0, 11})
#define PA12  ((GpioEnum) {0, 12})
#define PA13  ((GpioEnum) {0, 13})
#define PA14  ((GpioEnum) {0, 14})
#define PA15  ((GpioEnum) {0, 15})


#define PB0   ((GpioEnum) {1, 0})
#define PB1   ((GpioEnum) {1, 1})
#define PB2   ((GpioEnum) {1, 2})
#define PB3   ((GpioEnum) {1, 3})
#define PB4   ((GpioEnum) {1, 4})
#define PB5   ((GpioEnum) {1, 5})
#define PB6   ((GpioEnum) {1, 6})
#define PB7   ((GpioEnum) {1, 7})
#define PB8   ((GpioEnum) {1, 8})
#define PB9   ((GpioEnum) {1, 9})
#define PB10  ((GpioEnum) {1, 10})
#define PB11  ((GpioEnum) {1, 11})
#define PB12  ((GpioEnum) {1, 12})
#define PB13  ((GpioEnum) {1, 13})
#define PB14  ((GpioEnum) {1, 14})
#define PB15  ((GpioEnum) {1, 15})

#define PC0   ((GpioEnum) {2, 0})
#define PC1   ((GpioEnum) {2, 1})
#define PC2   ((GpioEnum) {2, 2})
#define PC3   ((GpioEnum) {2, 3})
#define PC4   ((GpioEnum) {2, 4})
#define PC5   ((GpioEnum) {2, 5})
#define PC6   ((GpioEnum) {2, 6})
#define PC7   ((GpioEnum) {2, 7})
#define PC8   ((GpioEnum) {2, 8})
#define PC9   ((GpioEnum) {2, 9})
#define PC10  ((GpioEnum) {2, 10})
#define PC11  ((GpioEnum) {2, 11})
#define PC12  ((GpioEnum) {2, 12})
#define PC13  ((GpioEnum) {2, 13})
#define PC14  ((GpioEnum) {2, 14})
#define PC15  ((GpioEnum) {2, 15})

#define PD0   ((GpioEnum) {3, 0})
#define PD1   ((GpioEnum) {3, 1})
#define PD2   ((GpioEnum) {3, 2})
#define PD3   ((GpioEnum) {3, 3})
#define PD4   ((GpioEnum) {3, 4})
#define PD5   ((GpioEnum) {3, 5})
#define PD6   ((GpioEnum) {3, 6})
#define PD7   ((GpioEnum) {3, 7})
#define PD8   ((GpioEnum) {3, 8})
#define PD9   ((GpioEnum) {3, 9})
#define PD10  ((GpioEnum) {3, 10})
#define PD11  ((GpioEnum) {3, 11})
#define PD12  ((GpioEnum) {3, 12})
#define PD13  ((GpioEnum) {3, 13})
#define PD14  ((GpioEnum) {3, 14})
#define PD15  ((GpioEnum) {3, 15})

#define PD0   ((GpioEnum) {3, 0})
#define PD1   ((GpioEnum) {3, 1})
#define PD2   ((GpioEnum) {3, 2})
#define PD3   ((GpioEnum) {3, 3})
#define PD4   ((GpioEnum) {3, 4})
#define PD5   ((GpioEnum) {3, 5})
#define PD6   ((GpioEnum) {3, 6})
#define PD7   ((GpioEnum) {3, 7})
#define PD8   ((GpioEnum) {3, 8})
#define PD9   ((GpioEnum) {3, 9})
#define PD10  ((GpioEnum) {3, 10})
#define PD11  ((GpioEnum) {3, 11})
#define PD12  ((GpioEnum) {3, 12})
#define PD13  ((GpioEnum) {3, 13})
#define PD14  ((GpioEnum) {3, 14})
#define PD15  ((GpioEnum) {3, 15})

#define PE0   ((GpioEnum) {4, 0})
#define PE1   ((GpioEnum) {4, 1})
#define PE2   ((GpioEnum) {4, 2})
#define PE3   ((GpioEnum) {4, 3})
#define PE4   ((GpioEnum) {4, 4})
#define PE5   ((GpioEnum) {4, 5})
#define PE6   ((GpioEnum) {4, 6})
#define PE7   ((GpioEnum) {4, 7})
#define PE8   ((GpioEnum) {4, 8})
#define PE9   ((GpioEnum) {4, 9})
#define PE10  ((GpioEnum) {4, 10})
#define PE11  ((GpioEnum) {4, 11})
#define PE12  ((GpioEnum) {4, 12})
#define PE13  ((GpioEnum) {4, 13})
#define PE14  ((GpioEnum) {4, 14})
#define PE15  ((GpioEnum) {4, 15})

#define PF0   ((GpioEnum) {5, 0})
#define PF1   ((GpioEnum) {5, 1})
#define PF2   ((GpioEnum) {5, 2})
#define PF3   ((GpioEnum) {5, 3})
#define PF4   ((GpioEnum) {5, 4})
#define PF5   ((GpioEnum) {5, 5})
#define PF6   ((GpioEnum) {5, 6})
#define PF7   ((GpioEnum) {5, 7})
#define PF8   ((GpioEnum) {5, 8})
#define PF9   ((GpioEnum) {5, 9})
#define PF10  ((GpioEnum) {5, 10})
#define PF11  ((GpioEnum) {5, 11})
#define PF12  ((GpioEnum) {5, 12})
#define PF13  ((GpioEnum) {5, 13})
#define PF14  ((GpioEnum) {5, 14})
#define PF15  ((GpioEnum) {5, 15})



void GpioInit();
void SetGpioPinDir(GpioEnum gpio ,GpioDirEnum dir);
void SetGpioPinState(GpioEnum gpio, GpioStateEnum state);
GpioStateEnum GetGpioPinState(GpioEnum gpio);
void ToggleGpioPinState(GpioEnum gpio);

#ifdef cplusplus
}
#endif

#endif
