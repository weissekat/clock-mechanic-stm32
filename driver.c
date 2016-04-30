#include "stm32f0xx.h"
#include "driver.h"

void Driver_Initialize() {
    // enable gpio peripheria
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    
    // driver
    GPIOB->MODER |= GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0;

    // enable timer 14 for driver delay ( 8 MHz / 400 / 200 = 100 Hz ~ 10 ms )
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
    TIM14->PSC = 400 - 1;
    TIM14->ARR = 200;
    TIM14->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM14_IRQn);

    // disable backup domain protection
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_DBP;
    RCC->APB1ENR &= ~RCC_APB1ENR_PWREN;

    // enable low frequency oscillator
    RCC->BDCR |= RCC_BDCR_RTCEN | RCC_BDCR_LSEON | RCC_BDCR_RTCSEL_0;
    while ((RCC->BDCR & RCC_BDCR_LSERDY) != RCC_BDCR_LSERDY) {};

    // set RTC alarm every second
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    RTC->CR &=~ RTC_CR_ALRAE;
    while ((RTC->ISR & RTC_ISR_ALRAWF) != RTC_ISR_ALRAWF) {};
    RTC->ALRMAR |= RTC_ALRMAR_MSK4 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK1;
    RTC->CR = RTC_CR_ALRAIE | RTC_CR_ALRAE;
    RTC->WPR = 0xFE;
    RTC->WPR = 0x64;

    // enable RTC alarm interrupt
    EXTI->IMR |= EXTI_IMR_MR17;
    EXTI->RTSR |= EXTI_RTSR_TR17;
    NVIC_EnableIRQ(RTC_IRQn);
}
