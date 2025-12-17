/*
 * analog_input.c
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#include "acquisition/input_analog.h"
#include "adc.h"


#define ADC_REF_VOLTAGE  3.3f
#define ADC_RESOLUTION   4095.0f
#define CURRENT_OFFSET   1.65f
#define SENSITIVITY      0.05f

void capt_init(void)
{
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
}

float measure_current_pooling(void)
{
    uint32_t raw_value = 0;

    HAL_ADC_Start(&hadc1);

    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
        raw_value = HAL_ADC_GetValue(&hadc1);
    }

    HAL_ADC_Stop(&hadc1);

    float voltage = (raw_value * ADC_REF_VOLTAGE) / ADC_RESOLUTION;
    float current = (voltage - CURRENT_OFFSET) / SENSITIVITY;

    return current;
}
