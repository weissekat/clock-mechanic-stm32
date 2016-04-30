#include "stm32f0xx.h"
#include "leds.h"

void Leds_Initialize () {
    // enable gpio peripheria
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
    
    // 7-segment led anodes (PA0-PA7)
    GPIOA->MODER |=
        GPIO_MODER_MODER0_0
        | GPIO_MODER_MODER1_0
        | GPIO_MODER_MODER2_0
        | GPIO_MODER_MODER3_0
        | GPIO_MODER_MODER4_0
        | GPIO_MODER_MODER5_0
        | GPIO_MODER_MODER6_0
        | GPIO_MODER_MODER7_0;
    
    // 7-segment led cathodes (PB0-PB2, PB10-PB12)
    GPIOB->MODER |=
        GPIO_MODER_MODER0_0
        | GPIO_MODER_MODER1_0
        | GPIO_MODER_MODER2_0
        | GPIO_MODER_MODER10_0
        | GPIO_MODER_MODER11_0
        | GPIO_MODER_MODER12_0;
        
    // turn off cathodes
    GPIOB->ODR |=
        GPIO_ODR_0
        | GPIO_ODR_1
        | GPIO_ODR_2
        | GPIO_ODR_10
        | GPIO_ODR_11
        | GPIO_ODR_12;
        
    // led (PB13)
    GPIOB->MODER |= GPIO_MODER_MODER13_0;

    // enable timer 3 for dynamic indication ( 8 MHz / 400 = 200 KHz )
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->PSC = 0;
    TIM3->ARR = 400;
    TIM3->DIER |= TIM_DIER_UIE;
    TIM3->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM3_IRQn);
}
