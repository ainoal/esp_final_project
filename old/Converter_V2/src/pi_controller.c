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
} pi_state;

// Initialize the PI controller
void pi_controller_init(double kp, double ki) {
    pi_state.kp = kp;
    pi_state.ki = ki;
    pi_state.integral = 0.0;
    pi_state.setpoint = 0.0;
    pi_state.run=0;
}

// Configure the PI controller
void pi_controller_update_setpoint(double setpoint) {
    pi_state.setpoint=setpoint;
}

void pi_controller_increment_Kp(double kp_change){
	pi_state.kp+=kp_change;
}

void pi_controller_increment_Ki(double ki_change){
	pi_state.ki+=ki_change;
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
		double output = pi_state.kp * error + pi_state.ki * pi_state.integral;
		return output;
	}
	else{
		pi_state.integral=0;
		return 0.0;
	}
}
// Internal functions (if necessary) should be declared static


