/*
 * user_actions.c
 *
 *  Created on: 19 Nov 2023
 *      Author: z103112
 */

#include "user_actions.h"
#include "state_machine.h"
#include "pi_controller.h"
#include "semphr.h"
#include <stdio.h>
#define CONFIGURATION 0
#define IDLING 1
#define MODULATING 2

SemaphoreHandle_t uart_semaphore;

static int convert_identifier_numeric(char ident[20]){
	int output=(strcmp(ident,"conf")==0)*1+(strcmp(ident,"idle")==0)*2+(strcmp(ident,"mod")==0)*3+
			(strcmp(ident,"uref")==0)*4+(strcmp(ident,"Kp")==0)*5+(strcmp(ident,"Ki")==0)*6;
	return output;
}

void init_uart_semaphore(void) {
	/* Create semaphore */
	uart_semaphore = xSemaphoreCreateBinary();
	if (uart_semaphore != NULL) {
		xSemaphoreGive(uart_semaphore);		// Make the semaphore initially available
	} else {
		// Semaphore creation failed, implement error handling
	}
}

void take_user_actions(ParsedData command){

	int numeric_command=convert_identifier_numeric(command.identifier);

	switch(numeric_command){
	case 1:
		//ACQUIRE SEMAPHORE
		if (uart_semaphore != NULL) {
			/* See if the semaphore can be obtained. If the semaphore
			 * is not available wait 10 ticks to see if it becomes free. */

			if (xSemaphoreTake(uart_semaphore, 10) == pdTRUE) {
				/* The semaphore was successfully obtained so the shared
				 * resource can be accessed safely. */
				request_state(CONFIGURATION);//request state change to conf
				printf("UART semaphore taken\n");
			}
			else {
				/* The semaphore could not be obtained even after waiting 10 ticks, so
				 the shared resource cannot be accessed. */
				// TODO: IMPLEMENT EXCEPTION HANDLING
			}
		}

	break;

	case 2:
		//RELEASE SEMAPHORE
		xSemaphoreGive(uart_semaphore);
		printf("UART semaphore given\n");
		request_state(IDLING);//request state change to idle
	break;

	case 3:
		//RELEASE SEMAPHORE
		xSemaphoreGive(uart_semaphore);
		printf("UART semaphore given\n");
		request_state(MODULATING);//request state change to modulation
	break;

	case 4:
		pi_controller_update_setpoint(command.value);
	break;

	case 5:
		if (get_state()!=CONFIGURATION){
			printf("This action allowed only in configuration state\n");
		}
		else{
			printf("Kp changed\n");
		}
	break;

	case 6:
		if (get_state()!=CONFIGURATION){
			printf("This action allowed only in configuration state\n");
		}
		else{
			printf("Ki changed\n");
		}
	break;

	}

}





