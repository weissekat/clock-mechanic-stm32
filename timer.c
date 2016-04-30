#include "stm32f0xx.h"
#include "timer.h"

void Timer_Initialize() {
    // enable gpio peripheria
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;

    // buzzer
    GPIOA->MODER |= GPIO_MODER_MODER12_0;

    // turn off buzzer
    GPIOA->ODR |= GPIO_ODR_12;

    // enable timer 16
    RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
    TIM16->PSC = 8000 - 1;
    TIM16->ARR = 1000;
    TIM16->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM16_IRQn);

    TIM16->CR1 |= TIM_CR1_CEN;
}
