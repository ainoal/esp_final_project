/*
 * controller_1.h
 *
 *  Created on: 22 Nov 2023
 *      Author: z103112
 */

#ifndef SRC_PI_CONTROLLER_GEN_H_
#define SRC_PI_CONTROLLER_GEN_H_
#include <stdlib.h>

typedef struct {
    double kp;
    double ki;
    double integral;
    double setpoint;
    int run;
    double u;
    double u_range[2];
    int par_to_conf; // 0:kp, 1:ki
    double time_step;
} pi_gen_state;


pi_gen_state pi_controller_gen_update_state(pi_gen_state PI_s,  double measurement);

#endif /* SRC_PI_CONTROLLER_GEN_H_ */
