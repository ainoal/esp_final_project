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
SemaphoreHandle_t button_semaphore;

static int convert_identifier_numeric(char ident[20]){
	int output=(strcmp(ident,"conf")==0)*1+(strcmp(ident,"idle")==0)*2+(strcmp(ident,"mod")==0)*3+
			(strcmp(ident,"uref")==0)*4+(strcmp(ident,"Kp")==0)*5+(strcmp(ident,"Ki")==0)*6;
	return output;
}

void init_button_semaphore(void) {
	/* Create semaphore */
	button_semaphore = xSemaphoreCreateBinary();
	if (button_semaphore != NULL) {
		xSemaphoreGive(button_semaphore);		// Make the semaphore initially available
	} else {
		// Semaphore creation failed, implement error handling
	}
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

	int isButtonSemaphoreTaken = checkButtonSemaphoreStatus();
	if (isButtonSemaphoreTaken != 0) {
		printf("Cannot use UART; button semaphore taken\n");
	}
	if (isButtonSemaphoreTaken == 0) {
		switch(numeric_command){
		case 1:
			//Take semaphore
			if (uart_semaphore != NULL) {
				// See if the semaphore can be obtained. If the semaphore
				// is not available wait 10 ticks to see if it becomes free.

				if (xSemaphoreTake(uart_semaphore, 10) == pdTRUE) {
					// The semaphore was successfully obtained
					request_state(CONFIGURATION); //request state change to conf
					printf("UART semaphore taken\n");
					printf("Entered to configuration mode.\n You can change values of Kp, Ki, or uref by writing command Kp/Ki/uref=value,\n or exit the mode by writing command idle/mod\n");
				}
				else {
					// The semaphore could not be obtained even after waiting 10 ticks
					printf("Error: UART semaphore could not be obtained\n");
				}
			}

		break;

		case 2:
			//RELEASE SEMAPHORE
			xSemaphoreGive(uart_semaphore);
			printf("UART semaphore given\n");
			printf("Entered to idle mode.\n You can change values of uref by writing command uref=value,\n or exit the mode by writing command conf/mod\n");
			request_state(IDLING);//request state change to idle
		break;

		case 3:
			xSemaphoreGive(uart_semaphore);
			printf("UART semaphore given\n");
			printf("Entered to modulation mode.\n You can change values of uref by writing command uref=value,\n or exit the mode by writing command conf/idle\n");
			request_state(MODULATING);//request state change to modulation
		break;

		case 4:
			if (get_state()==IDLING){
				printf("This action not allowed in idle mode.\n");
			}
			else{
				pi_controller_update_setpoint(command.value);
				printf("Set point updated to %.2f\n",command.value);
			}
		break;

		case 5:
			if (get_state()!=CONFIGURATION){
				printf("This action allowed only in configuration state\n");
			}
			else{
				pi_controller_set_Kp(command.value);
				printf("Kp changed to %.4f\n",command.value);
			}
		break;

		case 6:
			if (get_state()!=CONFIGURATION){
				printf("This action allowed only in configuration state\n");
			}
			else{
				pi_controller_set_Ki(command.value);
				printf("Ki changed to %.2f\n",command.value);
			}
		break;

		}
	}
}

int checkButtonSemaphoreStatus(void) {
	int isButtonSemaphoreTaken = 0;
    if (xSemaphoreTake(button_semaphore, 0) == pdTRUE) {
        xSemaphoreGive(button_semaphore); // Release the semaphore immediately
        isButtonSemaphoreTaken = 0; 	// Semaphore is available
    } else {
        isButtonSemaphoreTaken = 1; 		// Semaphore is taken
    }
    return isButtonSemaphoreTaken;
}





