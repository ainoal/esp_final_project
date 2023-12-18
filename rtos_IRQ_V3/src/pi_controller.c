/*
 * pi_controller.c
 *
 *  Created on: 13 Nov 2023
 *      Author: z103112
 */

#include "pi_controller.h"
#include <stdlib.h>


static pi_gen_state pi_state;


// Initialize the PI controller

void pi_controller_init(double kp, double ki) {
    pi_state.kp = kp;
    pi_state.ki = ki;
    pi_state.integral = 0.0;
    pi_state.setpoint = 0.0;
    pi_state.run=0;
    pi_state.u=0.0;
    pi_state.par_to_conf=0;
    pi_state.u_range[0]=-5.0;
    pi_state.u_range[1]=5.0;
    pi_state.time_step=0.00002;
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
	pi_state=pi_controller_gen_update_state(pi_state, measurement);
	return pi_state.u;
}

double pi_controller_get_state(){
	return pi_state.u;
}

int change_par_to_conf(){
	pi_state.par_to_conf=(pi_state.par_to_conf + 1) % 2; //this parameter indicates if the buttons (+/-) change kp or ki values
	return pi_state.par_to_conf;
}

int get_par_to_conf(){
	return pi_state.par_to_conf; //this parameter indicates if the buttons (+/-) change kp or ki values
}

//this function changes value of uref
double change_setpoint(double delta){
	pi_state.setpoint+=delta;
	return pi_state.setpoint;
}

//this function changes value of kp or ki depending on the state of par_to_conf
double change_par_value(double delta){
	switch(pi_state.par_to_conf){
	case conf_kp:
		pi_state.kp+=delta;
		return pi_state.kp;
	case conf_ki:
		pi_state.ki+=delta;
		return pi_state.ki;
	}
}


