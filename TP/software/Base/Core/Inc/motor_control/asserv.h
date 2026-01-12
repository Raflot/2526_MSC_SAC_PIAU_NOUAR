/*
 * asserv.h
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#ifndef INC_MOTOR_CONTROL_ASSERV_H_
#define INC_MOTOR_CONTROL_ASSERV_H_

typedef struct {
    float kp;
    float ki;
    float kd;
    float max_output;
    float min_output;
    float max_integral;
    float min_integral;
    float integrator;
    float prev_error;
    float prev_measurement;
} PID_Controller;

typedef struct {
    PID_Controller current_loop;
    PID_Controller speed_loop;
    float T_current;
    float target_current;
    float target_speed;
} Asserv;

extern Asserv asserv_sys;

void asserv_init(void);
float update_current_loop(float current_measurement);
float compute_pid(PID_Controller *pid, float measurement, float target, float dt);

#endif /* INC_MOTOR_CONTROL_ASSERV_H_ */
