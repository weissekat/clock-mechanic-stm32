#include "stm32f0xx.h"
#include "main.h"
#include "leds.h"
#include "buttons.h"
#include "driver.h"

volatile uint16_t upper_num = 123;
volatile uint16_t lower_num = 456;

void RTC_IRQHandler(void) {
    int i;
    if ((RTC->ISR & RTC_ISR_ALRAF) == RTC_ISR_ALRAF) {
        // clear interrupt flags
        RTC->ISR &= ~RTC_ISR_ALRAF;
        EXTI->PR |= EXTI_PR_PR17;

        // tick
        if (((GPIOB->ODR & GPIO_ODR_3) == GPIO_ODR_3) & ((GPIOB->ODR & GPIO_ODR_4) == GPIO_ODR_4)) {
            GPIOB->ODR &= ~GPIO_ODR_3;
            for (i = 0; i <= 10000; i++) { __NOP(); };
            GPIOB->ODR &= ~GPIO_ODR_4;
        } else {
            GPIOB->ODR |= GPIO_ODR_3;
            for (i = 0; i <= 10000; i++) { __NOP(); };
            GPIOB->ODR |= GPIO_ODR_4;
        }
    }
}

void TIM3_IRQHandler(void) {
    uint8_t i, digit_value[6];

    digit_value[0] = upper_num / 100;
    digit_value[1] = upper_num % 100 / 10;
    digit_value[2] = upper_num % 10;
    digit_value[3] = lower_num / 100;
    digit_value[4] = lower_num % 100 / 10;
    digit_value[5] = lower_num % 10;

    TIM3->SR &= ~TIM_SR_UIF;

    for (i = 0; i <= 5; i++) {
        GPIOB->ODR |= digit_all;
        GPIOA->ODR &= ~number_all;

        GPIOB->ODR &= ~digits[i];
        GPIOA->ODR |= numbers[digit_value[i]];
    }

    GPIOB->ODR |= digit_all;
    GPIOA->ODR &= ~number_all;
}

int main() {
    // disable backup domain protection
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_DBP;
    RCC->APB1ENR &= ~RCC_APB1ENR_PWREN;

    // enable low frequency oscillator
    RCC->BDCR |= RCC_BDCR_RTCEN | RCC_BDCR_LSEON | RCC_BDCR_RTCSEL_0;
    while ((RCC->BDCR & RCC_BDCR_LSERDY) != RCC_BDCR_LSERDY) {}; 
    
    // set alarm every second
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    RTC->CR &=~ RTC_CR_ALRAE;
    while ((RTC->ISR & RTC_ISR_ALRAWF) != RTC_ISR_ALRAWF) {};
    RTC->ALRMAR |= RTC_ALRMAR_MSK4 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK1;
    RTC->CR = RTC_CR_ALRAIE | RTC_CR_ALRAE;
    RTC->WPR = 0xFE;
    RTC->WPR = 0x64;

    // enable alarm interrupt
    EXTI->IMR |= EXTI_IMR_MR17;
    EXTI->RTSR |= EXTI_RTSR_TR17;
    NVIC_EnableIRQ(RTC_IRQn);

    // enable timer 3 for dynamic indication ( 8 MHz / 400 = 200 KHz )
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->PSC = 0;
    TIM3->ARR = 400;
    TIM3->DIER |= TIM_DIER_UIE;
    TIM3->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM3_IRQn);

    Leds_Initialize();
    Buttons_Initialize();
    Driver_Initialize();

    __enable_irq();

    while (1) {};
}
