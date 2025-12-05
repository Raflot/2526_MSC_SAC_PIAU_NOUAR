/*
 * motor.c
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#include "motor_control/motor.h"
#include "main.h"
#include "tim.h"
#define MOTOR_TIMER    &htim1
#define PWM_CH1        TIM_CHANNEL_1
#define PWM_CH2        TIM_CHANNEL_2
#define PWM_MAX        8499

void motor_init(void)
{
    if (HAL_TIM_PWM_Start(MOTOR_TIMER, PWM_CH1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIMEx_PWMN_Start(MOTOR_TIMER, PWM_CH1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_Start(MOTOR_TIMER, PWM_CH2) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIMEx_PWMN_Start(MOTOR_TIMER, PWM_CH2) != HAL_OK)
    {
        Error_Handler();
    }
    __HAL_TIM_SET_COMPARE(MOTOR_TIMER, PWM_CH1, 0);
    __HAL_TIM_SET_COMPARE(MOTOR_TIMER, PWM_CH2, 0);
}

void motor_set_command(int cmd)
{
    if (cmd > PWM_MAX) cmd = PWM_MAX;
__HAL_TIM_SET_COMPARE(MOTOR_TIMER, PWM_CH1, PWM_MAX*cmd/100);
__HAL_TIM_SET_COMPARE(MOTOR_TIMER, PWM_CH2, 0);
}
