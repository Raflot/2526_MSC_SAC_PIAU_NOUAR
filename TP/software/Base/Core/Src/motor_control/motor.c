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
int CURENT_RATIO = 50;

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
	__HAL_TIM_SET_COMPARE(MOTOR_TIMER, PWM_CH1, PWM_MAX/2);
	__HAL_TIM_SET_COMPARE(MOTOR_TIMER, PWM_CH2, PWM_MAX/2);
}

void motor_stop(void)
{
	HAL_TIM_PWM_Stop(MOTOR_TIMER, PWM_CH1);
	HAL_TIM_PWM_Stop(MOTOR_TIMER, PWM_CH2);

	HAL_TIMEx_PWMN_Stop(MOTOR_TIMER, PWM_CH1);
	HAL_TIMEx_PWMN_Stop(MOTOR_TIMER, PWM_CH2);
}

void motor_set_command(int cmd, int* CURENT_RATIO)
{

	if (cmd > PWM_MAX) cmd = PWM_MAX;
	int D = cmd - *CURENT_RATIO;
	if (D>0){
		for (int i=0; i < D/10;i++){
			__HAL_TIM_SET_COMPARE(MOTOR_TIMER, PWM_CH1, PWM_MAX*(*CURENT_RATIO)/100+10*i);
			__HAL_TIM_SET_COMPARE(MOTOR_TIMER, PWM_CH2, PWM_MAX-PWM_MAX*(*CURENT_RATIO)/100-10*i);
			HAL_Delay(1000);
		}
	}
	else
	{
		for (int i=0; i < -D/100;i++){
			__HAL_TIM_SET_COMPARE(MOTOR_TIMER, PWM_CH1, PWM_MAX*(*CURENT_RATIO)/100-10*i);
			__HAL_TIM_SET_COMPARE(MOTOR_TIMER, PWM_CH2, PWM_MAX-PWM_MAX*(*CURENT_RATIO)/100+10*i);
			HAL_Delay(1000);
		}
	}
	CURENT_RATIO = cmd;
}
