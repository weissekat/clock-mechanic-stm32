#ifndef __MAIN_H
#define __MAIN_H

#define segment_a GPIO_ODR_0
#define segment_b GPIO_ODR_1
#define segment_c GPIO_ODR_2
#define segment_d GPIO_ODR_3
#define segment_e GPIO_ODR_4
#define segment_f GPIO_ODR_5
#define segment_g GPIO_ODR_6
#define segment_dot GPIO_ODR_7

#define number_0 (segment_a | segment_b | segment_c | segment_d | segment_e | segment_f)
#define number_1 (segment_b | segment_c)
#define number_2 (segment_a | segment_b | segment_d | segment_e | segment_g)
#define number_3 (segment_a | segment_b | segment_c | segment_d | segment_g)
#define number_4 (segment_b | segment_c | segment_f | segment_g)
#define number_5 (segment_a | segment_c | segment_d | segment_f | segment_g)
#define number_6 (segment_a | segment_c | segment_d | segment_e | segment_f | segment_g)
#define number_7 (segment_a | segment_b | segment_c)
#define number_8 (segment_a | segment_b | segment_c | segment_d | segment_e | segment_f | segment_g)
#define number_9 (segment_a | segment_b | segment_c | segment_d | segment_f | segment_g)

#define number_all (segment_a | segment_b | segment_c | segment_d | segment_e | segment_f | segment_g | segment_dot)
#define number_none ((uint16_t) 0)

#define digit_0 (GPIO_ODR_0)
#define digit_1 (GPIO_ODR_1)
#define digit_2 (GPIO_ODR_2)
#define digit_3 (GPIO_ODR_10)
#define digit_4 (GPIO_ODR_11)
#define digit_5 (GPIO_ODR_12)
#define digit_all (digit_0 | digit_1 | digit_2 | digit_3 | digit_4 | digit_5)

uint16_t numbers[] = {number_0, number_1, number_2, number_3, number_4, number_5, number_6, number_7, number_8, number_9, number_none};
uint16_t digits[] = {digit_0, digit_1, digit_2, digit_3, digit_4, digit_5};
    
#endif
