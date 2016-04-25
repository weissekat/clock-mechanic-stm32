#include "stm32f0xx.h"

void RTC_IRQHandler(void)
{
    int i = 0;
    if ((RTC->ISR & (RTC_ISR_ALRAF)) == (RTC_ISR_ALRAF))
    {
        RTC->ISR &= ~RTC_ISR_ALRAF;
        
        EXTI->PR |= EXTI_PR_PR17;
        
        GPIOB->ODR ^= GPIO_ODR_13;
        
        if ((GPIOB->ODR & (GPIO_ODR_3)) == (GPIO_ODR_3))
        {
            GPIOB->ODR &= ~GPIO_ODR_3;
            GPIOB->ODR |= GPIO_ODR_4;
            for (i = 0; i <= 10000; i++ ) { __NOP(); };
            
            return;
        }
        
        if ((GPIOB->ODR & (GPIO_ODR_4)) == (GPIO_ODR_4))
        {
            GPIOB->ODR &= ~GPIO_ODR_4;
            GPIOB->ODR |= GPIO_ODR_3;
            for (i = 0; i <= 10000; i++ ) { __NOP(); };
            
            return;
        }
        
        // fail!
        GPIOB->ODR |= GPIO_ODR_3;
        GPIOB->ODR &= ~GPIO_ODR_4;               
    }
}

int main() {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
    
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    
    PWR->CR |= PWR_CR_DBP;
    
    RCC->APB1ENR &= ~RCC_APB1ENR_PWREN;
    
    RCC->BDCR |= RCC_BDCR_RTCEN | RCC_BDCR_LSEON | RCC_BDCR_RTCSEL_0;
    
    while ((RCC->BDCR & RCC_BDCR_LSERDY) != RCC_BDCR_LSERDY) {}; 
    
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    RTC->CR &=~ RTC_CR_ALRAE;
    while ((RTC->ISR & RTC_ISR_ALRAWF) != RTC_ISR_ALRAWF) {};
        
    RTC->ALRMAR |= RTC_ALRMAR_MSK4 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK1;
        
    RTC->CR = RTC_CR_ALRAIE | RTC_CR_ALRAE;
        
    RTC->WPR = 0xFE;
    RTC->WPR = 0x64;
  
    EXTI->IMR |= EXTI_IMR_MR17;
    
    EXTI->RTSR |= EXTI_RTSR_TR17;
    
    NVIC_SetPriority(RTC_IRQn, 0);
    NVIC_EnableIRQ(RTC_IRQn);
    
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
        
    GPIOB->ODR |=
        GPIO_ODR_0
        | GPIO_ODR_1
        | GPIO_ODR_2
        | GPIO_ODR_10
        | GPIO_ODR_11
        | GPIO_ODR_12;
        
    // led (PB13)
    GPIOB->MODER |=
        GPIO_MODER_MODER13_0;
        
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
        
    // driver
    GPIOB->MODER |=
        GPIO_MODER_MODER3_0
        | GPIO_MODER_MODER4_0;

    __enable_irq();
    
    while(1) {};
}
