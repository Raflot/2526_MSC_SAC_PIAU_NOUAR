/*
 * asserv.c
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */
#include "motor_control/asserv.h"

// Global instance
Asserv asserv_sys = {0};

void asserv_init(void)
{
	asserv_sys.T_current = 0.00005f;
	asserv_sys.target_current = 0.0f;
	asserv_sys.target_speed = 0.0f;
	asserv_sys.current_loop.kp = 2000.0f;

	asserv_sys.current_loop.ki = 10000.0f;
	//asserv_sys.current_loop.kp = 0.007f;
	//asserv_sys.current_loop.ki = 12.0f;
	asserv_sys.current_loop.kd = 0.0f;

	asserv_sys.current_loop.max_integral = 4250.0f;
	asserv_sys.current_loop.min_integral = -4250.0f;
	asserv_sys.current_loop.max_output = 4250.0f;
	asserv_sys.current_loop.min_output = -4250.0f;
	asserv_sys.current_loop.integrator = 0.0f;
	asserv_sys.current_loop.prev_error = 0.0f;
	asserv_sys.current_loop.prev_measurement = 0.0f;

	asserv_sys.speed_loop.kp = 0.015f;
	asserv_sys.speed_loop.ki = 0.06f;
	asserv_sys.speed_loop.kd = 0.0f;

	asserv_sys.speed_loop.max_integral = 8499.0f;
	asserv_sys.speed_loop.min_integral = -8499.0f;
	asserv_sys.speed_loop.max_output = 8499.0f;
	asserv_sys.speed_loop.min_output = -8499.0f;

	asserv_sys.speed_loop.integrator = 0.0f;
	asserv_sys.speed_loop.prev_error = 0.0f;
	asserv_sys.speed_loop.prev_measurement = 0.0f;
}

float update_current_loop(float current_measurement)
{
	float pwm_adjustment = compute_pid(
			&asserv_sys.current_loop,
			current_measurement,
			asserv_sys.target_current,
			asserv_sys.T_current
	);

	return pwm_adjustment;
}

float compute_pid(PID_Controller *pid, float measurement, float target, float dt)
{
	float error = target - measurement;

	float proportional = pid->kp * error;

	pid->integrator += 0.5f * pid->ki * (error + pid->prev_error) * dt;
	if (pid->integrator > pid->max_integral) {
		pid->integrator = pid->max_integral;
	} else if (pid->integrator < pid->min_integral) {
		pid->integrator = pid->min_integral;
	}

	float derivative = 0.0f;
	if (dt > 0.0f) {
		derivative = pid->kd * (error - pid->prev_error) / dt;
	}

	float output = proportional + pid->integrator + derivative;

	if (output > pid->max_output) {
		output = pid->max_output;
	} else if (output < pid->min_output) {
		output = pid->min_output;
	}

	pid->prev_error = error;
	pid->prev_measurement = measurement;

	return output;
}
