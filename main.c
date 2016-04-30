#include "stm32f0xx.h"

#include "main.h"
#include "leds.h"
#include "buttons.h"
#include "driver.h"
#include "timer.h"

volatile uint16_t timer_value = 0;

void RTC_IRQHandler(void) {
    if ((RTC->ISR & RTC_ISR_ALRAF) == RTC_ISR_ALRAF) {
        // clear interrupt flags
        RTC->ISR &= ~RTC_ISR_ALRAF;
        EXTI->PR |= EXTI_PR_PR17;

        // enable driver
        if (((GPIOB->ODR & GPIO_ODR_3) != GPIO_ODR_3) && ((GPIOB->ODR & GPIO_ODR_4) != GPIO_ODR_4)) {
            GPIOB->ODR |= GPIO_ODR_3;
        }
        if (((GPIOB->ODR & GPIO_ODR_3) == GPIO_ODR_3) && ((GPIOB->ODR & GPIO_ODR_4) == GPIO_ODR_4)) {
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
    if (((GPIOB->ODR & GPIO_ODR_3) == GPIO_ODR_3) && ((GPIOB->ODR & GPIO_ODR_4) != GPIO_ODR_4)) {
        GPIOB->ODR |= GPIO_ODR_4;
    }
    if (((GPIOB->ODR & GPIO_ODR_3) != GPIO_ODR_3) && ((GPIOB->ODR & GPIO_ODR_4) == GPIO_ODR_4)) {
        GPIOB->ODR &= ~GPIO_ODR_4;
    }

    // disable driver delay timer
    TIM14->CR1 &= ~TIM_CR1_CEN;
}

void TIM3_IRQHandler(void) {
    uint8_t i;
    uint8_t digit_value[6] = {10, 10, 10, 10, 10, 10};

    uint16_t hours = timer_value / 3600;
    uint16_t minutes = timer_value % 3600 / 60;

    uint8_t timer_enabled = (GPIOA->IDR & GPIO_IDR_15) != GPIO_IDR_15;
    uint8_t dot = timer_enabled && timer_value % 2;

    // scale time
    if (hours > 9) {
        digit_value[0] = (hours / 10) + 1;
        digit_value[1] = hours % 10 + 10; // dot
        digit_value[2] = minutes % 10 + dot * 11;
    } else {
        digit_value[0] = (hours + 10) + 1; // dot
        digit_value[1] = minutes / 10;
        digit_value[2] = minutes % 10 + dot * 11;
    }

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

void TIM16_IRQHandler(void) {
    TIM16->SR &= ~TIM_SR_UIF;

    // if switch up
    if ((GPIOA->IDR & GPIO_IDR_15) != GPIO_IDR_15) {
        if (timer_value > 0) {
            timer_value--;
        } else {
            GPIOA->ODR &= ~GPIO_ODR_12; // beep
        }
    } else {
        GPIOA->ODR |= GPIO_ODR_12; // lock beep
    }
}

void EXTI4_15_IRQHandler(void) {
    if ((EXTI->PR & EXTI_PR_PR8) == EXTI_PR_PR8) {
        EXTI->PR |= EXTI_PR_PR8;
        if (timer_value >= 60)
            timer_value -= 60;
    }

    if ((EXTI->PR & EXTI_PR_PR9) == EXTI_PR_PR9) {
        EXTI->PR |= EXTI_PR_PR9;
        if (timer_value <= 35940)
            timer_value += 60;
    }

    // truncate timer_value to least minute
    if ((timer_value % 60) != 0)
        timer_value = timer_value - (timer_value % 60);
}

int main() {
    Driver_Initialize();
    Leds_Initialize();
    Buttons_Initialize();
    Timer_Initialize();

    __enable_irq();

    while (1) {};
}
