/*
 * input_encoder.h
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#ifndef INC_ACQUISITION_INPUT_ENCODER_H_
#define INC_ACQUISITION_INPUT_ENCODER_H_
#include "main.h"

void encoder_init(void);
float position(void);
float speed(void);

void encoder_compute_speed_100Hz(void);

#endif /* INC_ACQUISITION_INPUT_ENCODER_H_ */
