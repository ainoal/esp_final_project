/*
 * state_machine.h
 *
 *  Created on: 19 Nov 2023
 *      Author: z103112
 */

#ifndef SRC_STATE_MACHINE_H_
#define SRC_STATE_MACHINE_H_

#include "pi_controller.h"


void request_state(int new_state);
int get_state();
void change_state();

#endif /* SRC_STATE_MACHINE_H_ */
