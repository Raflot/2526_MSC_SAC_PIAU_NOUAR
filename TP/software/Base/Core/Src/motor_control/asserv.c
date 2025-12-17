/*
 * asserv.c
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#include "motor_control/asserv.h"

typedef struct {
	float PREV_I;
	float CURRENT_I;
	float TARGET_I;
	float MAX_I;

	float PREV_SPEED;
	float CURRENT_SPEED;
	float TARGET_SPEED;
	float MAX_SPEED;

	float integrator;
	float prevError;
	float prevMeasurement;

	int T;

	float kp_S;
	float ki_S;
	float kd_S;
	float kp_I;
	float ki_I;
	float kd_I;

} Asserv;
Asserv pid = {0};

void asserv_init(void)
{
	pid.PREV_I = 0;
	pid.CURRENT_I = 0;
	pid.TARGET_I = 0;
	pid.MAX_I = 5;

	pid.PREV_SPEED = 0;
	pid.CURRENT_SPEED = 0;
	pid.TARGET_SPEED = 0;
	pid.MAX_SPEED = 2000;

	pid.integrator = 0;
	pid.prevError = 0;
	pid.prevMeasurement = 0;

	pid.T = 0;

	pid.kp_S = 0;
	pid.ki_S = 0;
	pid.kd_S = 0;
	pid.kp_I = 0;
	pid.ki_I = 0;
	pid.kd_I = 0;
}


double get_pid(float measurement,float state,float goal,float kp,float ki,float kd,float MAX,float MIN,float MAX_INT,float MIN_INT){
	    float error = state - goal;

	    float proportional = kp * error;

	    pid.integrator += 0.5f *ki * (error + pid.prevError) * pid.T;

	    if (pid.integrator > MAX_INT) {
	    	pid.integrator = MAX_INT;
	    } else if (pid.integrator < MIN_INT) {
	    	pid.integrator = MIN_INT;
	    }

	    float derivative = kd * (error - pid.prevError) / pid.T;

	    float output = proportional + pid.integrator + derivative;

	    if (output > MAX) {
	        output = MAX;
	    } else if (output < MIN) {
	        output = MIN;
	    }

	    pid.prevError = error;
	    pid.prevMeasurement = measurement;

	    return output;
	}


