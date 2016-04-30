#include "stm32f0xx.h"

#include "main.h"
#include "leds.h"
#include "buttons.h"
#include "driver.h"
#include "timer.h"

//volatile uint16_t upper_num = 8;
volatile uint16_t lower_num = 0;

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
    uint8_t i, digit_value[6] = {10, 10, 10, 10, 10, 10};

    uint16_t hours, minutes_and_seconds, minutes;

    // timer
    uint8_t timer_dot;
    hours = timer_value / 3600;
    minutes_and_seconds = timer_value % 3600;
    minutes = minutes_and_seconds / 60;

    if (hours > 9) {
        digit_value[0] = hours / 10;
        digit_value[1] = hours % 10;
        digit_value[2] = minutes % 10;
        timer_dot = 1;
    } else {
        digit_value[0] = hours;
        digit_value[1] = minutes / 10;
        digit_value[2] = minutes % 10;
        timer_dot = 0;
    }

    // digit_value[0] = upper_num / 100;
    // digit_value[1] = upper_num % 100 / 10;
    // digit_value[2] = upper_num % 10;

    // digit_value[3] = lower_num / 100;
    // digit_value[4] = lower_num % 100 / 10;
    // digit_value[5] = lower_num % 10;

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
        // countdown
        if (timer_value > 0) {
            // tick
            timer_value--;
        } else {
            // beep
            GPIOA->ODR &= ~GPIO_ODR_12;
        }
    } else {
        // lock beep
        GPIOA->ODR |= GPIO_ODR_12;
    }
}

void EXTI4_15_IRQHandler(void) {
    if ((EXTI->PR & EXTI_PR_PR8) == EXTI_PR_PR8) {
        EXTI->PR |= EXTI_PR_PR8;
        if (timer_value >= 60)
            timer_value -= 60;
        else
            timer_value = 0;
    }

    if ((EXTI->PR & EXTI_PR_PR9) == EXTI_PR_PR9) {
        EXTI->PR |= EXTI_PR_PR9;
        if (timer_value < 14400)
            timer_value += 60;
        else
            timer_value = 0;
    }

    // debounce
}

int main() {
    Driver_Initialize();
    Leds_Initialize();
    Buttons_Initialize();
    Timer_Initialize();

    __enable_irq();

    while (1) {};
}
