/*
 * user_actions.c
 *
 *  Created on: 19 Nov 2023
 *      Author: z103112
 */

#include "user_actions.h"

static int convert_identifier_numeric(char ident[20]){
	int output=(strcmp(ident,"conf")==0)*1+(strcmp(ident,"idle")==0)*2+(strcmp(ident,"mod")==0)*3+
			(strcmp(ident,"uref")==0)*4+(strcmp(ident,"Kp")==0)*5+(strcmp(ident,"Ki")==0)*6;
	return output;
}

void take_user_actions(ParsedData command){

int numeric_command=convert_identifier_numeric(command.identifier);

switch(numeric_command){
case 1:
	//request state change to conf
break;

case 2:
	//request state change to idle
break;

case 3:
	//request state change to modulation
break;

case 4:
	//PSEUDOCODE:
	// if get_state~=conf(1)
		//printf("This action allowed only in configuration state")
	//else
		//pi_controller_update_setpoint(command.value);
break;

case 5:
	//PSEUDOCODE:
	// if get_state~=conf(1)
		//printf("This action allowed only in configuration state")
	//else
		//pi_controller_set_Kp(command.value);
break;

case 6:
	//PSEUDOCODE:
	// if get_state~=conf(1)
		//printf("This action allowed only in configuration state");
	//else
		//pi_controller_set_Ki(command.value);
break;

}

}





