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
	float PREV_RATIO;
	float CURRENT_RATIO;
	float TARGET_RATIO;
	float MAX_RATIO;

	int TIME;

} MotorState;
MotorState state = {0};



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
	state.CURRENT_RATIO = 50;
	state.PREV_RATIO = 50;
	state.TARGET_RATIO = 50;
	state.MAX_RATIO = 100;
	state.TIME = 100;
}

void motor_stop(void)
{
	HAL_TIM_PWM_Stop(MOTOR_TIMER, PWM_CH1);
	HAL_TIM_PWM_Stop(MOTOR_TIMER, PWM_CH2);

	HAL_TIMEx_PWMN_Stop(MOTOR_TIMER, PWM_CH1);
	HAL_TIMEx_PWMN_Stop(MOTOR_TIMER, PWM_CH2);
}

void set_speed(void){
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
	if (cmd > state.MAX_RATIO) cmd = state.MAX_RATIO;
	if (cmd < 0) cmd = 0;

	state.TARGET_RATIO = cmd;

}

