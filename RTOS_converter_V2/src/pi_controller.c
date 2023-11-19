/*
 * pi_controller.c
 *
 *  Created on: 13 Nov 2023
 *      Author: z103112
 */

#include "pi_controller.h"

static double time_step=0.00002;

// PI controller state (hidden from other modules)
static struct {
    double kp;
    double ki;
    double integral;
    double setpoint;
    int run;
    double u;
    int par_to_conf; // 0:kp, 1:ki
} pi_state;

// Initialize the PI controller
void pi_controller_init(double kp, double ki) {
    pi_state.kp = kp;
    pi_state.ki = ki;
    pi_state.integral = 0.0;
    pi_state.setpoint = 0.0;
    pi_state.run=0;
    pi_state.u=0.0;
    pi_state.par_to_conf=0;
}

// Configure the PI controller
void pi_controller_update_setpoint(double setpoint) {
    pi_state.setpoint=setpoint;
}

void pi_controller_set_Kp(double new_kp){
	pi_state.kp=new_kp;
}

void pi_controller_set_Ki(double new_ki){
	pi_state.ki=new_ki;
}

// Run PI controller
void start_controller(){
	pi_state.run=1;
}

void stop_controller(){
	pi_state.run=0;
}

double pi_controller_update_state(double measurement) {
	if (pi_state.run==1){
		double error = pi_state.setpoint - measurement;
		pi_state.integral += error*time_step;
		pi_state.u = pi_state.kp * error + pi_state.ki * pi_state.integral;
	}
	else{
		pi_state.integral=0.0;
		pi_state.u=0.0;
	}
	return pi_state.u;
}

double pi_controller_get_state(){
	return pi_state.u;
}

void change_par_to_conf(){
	pi_state.par_to_conf=(pi_state.par_to_conf + 1) % 2;
}

void change_setpoint(double delta){
	pi_state.setpoint+=delta;
}

void change_par_value(double delta){
	switch(pi_state.par_to_conf){
	case 0:
		pi_state.kp+=delta;
	case 1:
		pi_state.ki+=delta;
	}
}

// Internal functions (if necessary) should be declared static


