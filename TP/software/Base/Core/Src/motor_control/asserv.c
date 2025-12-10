/*
 * asserv.c
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#include "motor_control/asserv.h"
/*
double speed_pid(void){
	    float error = state - goal;

	    float proportional = Kp * error;

	    state.integrator += 0.5f *Ki * (error + pid->prevError) * pid->T;

	    // Anti-windup (Clamping) : On empêche l'intégrateur de saturer
	    if (pid->integrator > pid->limMaxInt) {
	        pid->integrator = pid->limMaxInt;
	    } else if (pid->integrator < pid->limMinInt) {
	        pid->integrator = pid->limMinInt;
	    }

	    // --- Terme Dérivé (D) ---
	    // Note : On peut dériver sur l'erreur ou sur la mesure (pour éviter les à-coups sur changement de consigne)
	    // Ici, implémentation classique sur l'erreur : (error - prevError) / T
	    float derivative = pid->Kd * (error - pid->prevError) / pid->T;

	    // --- Calcul de la sortie totale ---
	    float output = proportional + pid->integrator + derivative;

	    // Saturation de la sortie (Output Clamping)
	    if (output > pid->limMax) {
	        output = pid->limMax;
	    } else if (output < pid->limMin) {
	        output = pid->limMin;
	    }

	    // --- Mise à jour des variables pour le prochain tour ---
	    pid->prevError = error;
	    pid->prevMeasurement = measurement;

	    return output;
	}
}
 */
