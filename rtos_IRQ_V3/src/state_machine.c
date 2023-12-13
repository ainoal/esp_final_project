/*
 * state_machine.c
 *
 *  Created on: 19 Nov 2023
 *      Author: z103112
 */

#include "state_machine.h"
#include "btn_setup.h"
#define CONFIGURATION 0
#define IDLING 1
#define MODULATING 2

//PRIVATE VARIABLES
static int state=0;

// PUBLIC FUNCTIONS
void request_state(int new_state){
	switch(new_state){
	case CONFIGURATION:
		stop_controller(); //first stop the controller
		state=CONFIGURATION; //then change state to configuration (this ensures no inverter parameter changes can occur when the converter is on)
		//set LEDS ON?
		AXI_LED_DATA = 0b0001;
	break;

	case IDLING:
		stop_controller();
		state=IDLING; // no modulation, but configuring inverter parameters not allowed
		//set LEDS OFF?
		AXI_LED_DATA = 0b0010;
	break;

	case MODULATING:
		state=MODULATING; //first change the state (effectively prevent change of kp and ki)
		start_controller();  //then actually start the modulation
		AXI_LED_DATA = 0b0100;
	break;
	}
}

int get_state(){
	return state;
}

void change_state(){
	int new_state = (state + 1) % 3; //change state: conf->idle->mod->conf...
	request_state(new_state);
}
