/*
 * input_encoder.h
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#ifndef INC_ACQUISITION_INPUT_ENCODER_H_
#define INC_ACQUISITION_INPUT_ENCODER_H_
#include "main.h"

extern volatile int32_t encoder_position;
extern volatile int32_t encoder_turns;

void encoder_init(void);

#endif /* INC_ACQUISITION_INPUT_ENCODER_H_ */
