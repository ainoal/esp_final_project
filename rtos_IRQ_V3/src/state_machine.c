/*
 * state_machine.c
 *
 *  Created on: 19 Nov 2023
 *      Author: z103112
 */

#include "state_machine.h"
#define CONFIGURATION 0
#define IDLING 1
#define MODULATING 2

//PRIVATE VARIABLES
static int state=0;

// PUBLIC FUNCTIONS
void request_state(int new_state){
	switch(new_state){
	case CONFIGURATION:
		stop_controller();
		state=CONFIGURATION;
		//set LEDS ON?
	break;

	case IDLING:
		stop_controller();
		state=IDLING;
		//set LEDS OFF?
	break;

	case MODULATING:
		state=MODULATING;
		start_controller();
	break;
	}
}

int get_state(){
	return state;
}

void change_state(){
	int new_state = (state + 1) % 3;
	request_state(new_state);
}
