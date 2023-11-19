/*
 * pi_controller.h
 *
 *  Created on: 13 Nov 2023
 *      Author: z103112
 */

#ifndef PI_CONTROLLER_H
#define PI_CONTROLLER_H

// Public function to initialize the PI controller
void pi_controller_init(double kp, double ki);

// Public functions to update the PI controller
void pi_controller_update_setpoint(double setpoint);
//void pi_controller_increment_Kp(double kp_change);
//void pi_controller_increment_Ki(double ki_change);
void change_par_to_conf();
void change_setpoint(double delta);
void change_par_value(double delta);


void pi_controller_set_Kp(double new_kp);
void pi_controller_set_Ki(double new_ki);
void start_controller();
void stop_controller();
double pi_controller_update_state(double measurement);
double pi_controller_get_state();

#endif // PI_CONTROLLER_H
