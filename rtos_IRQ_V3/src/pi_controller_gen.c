/*
 * controller_1.c
 *
 *  Created on: 22 Nov 2023
 *      Author: z103112
 */


#include "pi_controller_gen.h"

// PRIVATE FUNCTIONS
static double saturate(pi_gen_state PI_s, double value) {
    if (value < PI_s.u_range[0]) {
        return PI_s.u_range[0];
    } else if (value > PI_s.u_range[1]) {
        return PI_s.u_range[1];
    } else {
        return value;
    }
}

// PUBLIC
pi_gen_state pi_controller_gen_update_state(pi_gen_state PI_s,  double measurement) {
	if (PI_s.run==1){
		double error = PI_s.setpoint - measurement;
		// ANTI WIND_UP:
		double u_pre=PI_s.kp * error + PI_s.ki * PI_s.integral; // control before integration
		if((u_pre+error*PI_s.time_step*PI_s.ki<PI_s.u_range[1] && u_pre+error*PI_s.time_step*PI_s.ki>PI_s.u_range[0])|| //check if uref would be within u_range AFTER integration
				(abs(PI_s.integral)>abs(PI_s.integral+error*PI_s.time_step))){ //always allow "negative" integration
			PI_s.integral += error*PI_s.time_step;
			u_pre = u_pre+error*PI_s.time_step*PI_s.ki; //computed control before checking the saturation
		}
		else{ // if integration would saturate the control
			if(u_pre+error*PI_s.time_step*PI_s.ki>=PI_s.u_range[1]){
				u_pre=PI_s.u_range[1];
			}else{
				u_pre=PI_s.u_range[0];
			}
		}
		PI_s.u=saturate(PI_s, u_pre); //It is possible that proportional part alone saturates the control. That is taken care of here.
	}
	else{
		PI_s.integral=0.0;
		PI_s.u=0.0;
	}
	return PI_s;
}
