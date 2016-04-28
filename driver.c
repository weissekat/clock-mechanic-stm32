#include "stm32f0xx.h"
#include "driver.h"

void Driver_Initialize() {
    // enable gpio peripheria
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    
    // driver
    GPIOB->MODER |=
        GPIO_MODER_MODER3_0
        | GPIO_MODER_MODER4_0;
}
