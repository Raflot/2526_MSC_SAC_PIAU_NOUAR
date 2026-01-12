/*
 * input_encoder.c
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#include "acquisition/input_encoder.h"
#define ENC_A_PIN    GPIO_PIN_6
#define ENC_A_PORT   GPIOA
#define ENC_B_PIN    GPIO_PIN_4
#define ENC_B_PORT   GPIOA
#define ENC_Z_PIN    GPIO_PIN_8
#define ENC_Z_PORT   GPIOC
#define SAMPLING_PERIOD_S  0.01f

volatile int32_t encoder_position = 0;
volatile int32_t encoder_turns = 0;

volatile int32_t encoder_sens = 200;

static float _speed_rad_s = 0.0f;
static float _prev_position_rad = 0.0f;

void encoder_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitStruct.Pin = ENC_A_PIN | ENC_B_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = ENC_Z_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == ENC_A_PIN)
	{
		if (HAL_GPIO_ReadPin(ENC_B_PORT, ENC_B_PIN) == GPIO_PIN_RESET) {
			encoder_position++;
		} else {
			encoder_position--;
		}
	}

	else if (GPIO_Pin == ENC_B_PIN)
	{
		if (HAL_GPIO_ReadPin(ENC_A_PORT, ENC_A_PIN) == GPIO_PIN_SET) {
			encoder_position++;
		} else {
			encoder_position--;
		}
	}

	else if (GPIO_Pin == ENC_Z_PIN)
		{
			int32_t threshold = 200;

			if (encoder_position > threshold || encoder_position < -threshold)
			{
				encoder_sens = (encoder_position > 0) ? encoder_position : -encoder_position;

				encoder_position = 0;
				encoder_turns++;
			}
		}
	}

float position(void)
{
    if (encoder_sens == 0) return 0.0f;

    return ((float)encoder_position * (2.0f * 3.14159265359f)) / (float)encoder_sens;
}

int sens(void)
{
	return encoder_sens;
}

int turn(void)
{
	return encoder_turns;
}
void encoder_compute_speed_100Hz(void)
{
    float current_pos = position();
    float delta_pos = current_pos - _prev_position_rad;
    _speed_rad_s = delta_pos / SAMPLING_PERIOD_S;
    _prev_position_rad = current_pos;
}

float speed(void)
{
    return abs(_speed_rad_s);
}
