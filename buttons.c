#include "stm32f0xx.h"
#include "buttons.h"

void Buttons_Initialize() {
    // enable gpio peripheria
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
    
    // buttons (PA8-PA11)
    GPIOA->MODER &= ~(
        GPIO_MODER_MODER8
        | GPIO_MODER_MODER9
        | GPIO_MODER_MODER10
        | GPIO_MODER_MODER11);
        
    GPIOA->PUPDR |=
        GPIO_PUPDR_PUPDR8_0
        | GPIO_PUPDR_PUPDR9_0
        | GPIO_PUPDR_PUPDR10_0
        | GPIO_PUPDR_PUPDR11_0;
}
