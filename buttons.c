#include "stm32f0xx.h"
#include "buttons.h"

void Buttons_Initialize() {
    // enable gpio peripheria
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
    
    // buttons (PA8-PA11, switch PA15)
    GPIOA->MODER &= ~(
        GPIO_MODER_MODER8
        | GPIO_MODER_MODER9
        | GPIO_MODER_MODER10
        | GPIO_MODER_MODER11
        | GPIO_MODER_MODER15);
        
    GPIOA->PUPDR |=
        GPIO_PUPDR_PUPDR8_0
        | GPIO_PUPDR_PUPDR9_0
        | GPIO_PUPDR_PUPDR10_0
        | GPIO_PUPDR_PUPDR11_0
        | GPIO_PUPDR_PUPDR15_0;

    EXTI->IMR |= EXTI_IMR_MR8 | EXTI_IMR_MR9;
    EXTI->FTSR |= EXTI_FTSR_TR8 | EXTI_FTSR_TR9;

    NVIC_EnableIRQ(EXTI4_15_IRQn);
}
