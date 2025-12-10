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
typedef struct {
	int CURRENT_RATIO;
	int TARGET_RATIO;
} MotorState;

MotorState state = {50, 50};

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

void set_speed(){
	if (state.CURRENT_RATIO < state.TARGET_RATIO) {
		state.CURRENT_RATIO++;
	}
	else if (state.CURRENT_RATIO > state.TARGET_RATIO) {
		state.CURRENT_RATIO--;
	}
	else {
		return;
	}
	int ratio = state.CURRENT_RATIO;
	__HAL_TIM_SET_COMPARE(MOTOR_TIMER, PWM_CH1, PWM_MAX * ratio / 100);
	__HAL_TIM_SET_COMPARE(MOTOR_TIMER, PWM_CH2, PWM_MAX - (PWM_MAX * ratio / 100));
}

void motor_set_command(int cmd)
{
	if (cmd > 100) cmd = 100;
	if (cmd < 0) cmd = 0;

	state.TARGET_RATIO = cmd;

}


