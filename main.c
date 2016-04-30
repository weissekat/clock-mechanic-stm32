#include "stm32f0xx.h"

#include "main.h"
#include "leds.h"
#include "buttons.h"
#include "driver.h"

volatile uint16_t upper_num = 123;
volatile uint16_t lower_num = 456;

void RTC_IRQHandler(void) {
    if ((RTC->ISR & RTC_ISR_ALRAF) == RTC_ISR_ALRAF) {
        // clear interrupt flags
        RTC->ISR &= ~RTC_ISR_ALRAF;
        EXTI->PR |= EXTI_PR_PR17;

        // enable driver
        if (((GPIOB->ODR & GPIO_ODR_3) != GPIO_ODR_3) & ((GPIOB->ODR & GPIO_ODR_4) != GPIO_ODR_4)) {
            GPIOB->ODR |= GPIO_ODR_3;
        }
        if (((GPIOB->ODR & GPIO_ODR_3) == GPIO_ODR_3) & ((GPIOB->ODR & GPIO_ODR_4) == GPIO_ODR_4)) {
            GPIOB->ODR &= ~GPIO_ODR_3;
        }

        // enable driver delay timer
        TIM14->CR1 |= TIM_CR1_CEN;
    }
}

void TIM14_IRQHandler(void) {
    // clear interrupt flag
    TIM14->SR &= ~TIM_SR_UIF;

    // disable driver
    if (((GPIOB->ODR & GPIO_ODR_3) == GPIO_ODR_3) & ((GPIOB->ODR & GPIO_ODR_4) != GPIO_ODR_4)) {
        GPIOB->ODR |= GPIO_ODR_4;
    }
    if (((GPIOB->ODR & GPIO_ODR_3) != GPIO_ODR_3) & ((GPIOB->ODR & GPIO_ODR_4) == GPIO_ODR_4)) {
        GPIOB->ODR &= ~GPIO_ODR_4;
    }

    // disable driver delay timer
    TIM14->CR1 &= ~TIM_CR1_CEN;
}

void TIM3_IRQHandler(void) {
    uint8_t i, digit_value[6];

    digit_value[0] = upper_num / 100;
    digit_value[1] = upper_num % 100 / 10;
    digit_value[2] = upper_num % 10;
    digit_value[3] = lower_num / 100;
    digit_value[4] = lower_num % 100 / 10;
    digit_value[5] = lower_num % 10;

    // clear interrupt flag
    TIM3->SR &= ~TIM_SR_UIF;

    // set digits
    for (i = 0; i <= 5; i++) {
        GPIOB->ODR |= digit_all;
        GPIOA->ODR &= ~number_all;

        GPIOB->ODR &= ~digits[i];
        GPIOA->ODR |= numbers[digit_value[i]];
    }

    // clear digits
    GPIOB->ODR |= digit_all;
    GPIOA->ODR &= ~number_all;
}

int main() {
    Driver_Initialize();
    Leds_Initialize();
    Buttons_Initialize();

    __enable_irq();

    while (1) {};
}
