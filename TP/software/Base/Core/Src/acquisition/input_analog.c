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
#define SENSITIVITY      0.08f

volatile uint16_t adc_dma_buffer[1];
float current_offset = 1.65f;

void capt_init(void)
{
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_dma_buffer, 1);
    HAL_Delay(100);
    uint32_t raw_value = adc_dma_buffer[0];
    current_offset = ((float)raw_value * ADC_REF_VOLTAGE) / ADC_RESOLUTION;
}

float measure_current(void)
{
    uint32_t raw_value = adc_dma_buffer[0];
    float voltage = ((float)raw_value * ADC_REF_VOLTAGE) / ADC_RESOLUTION;
    float current = (voltage - current_offset) / SENSITIVITY;

    return current;
}
