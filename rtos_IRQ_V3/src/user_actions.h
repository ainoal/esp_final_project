/*
 * user_actions.h
 *
 *  Created on: 19 Nov 2023
 *      Author: z103112
 */

#ifndef SRC_USER_ACTIONS_H
#define SRC_USER_ACTIONS_H

#include "uart_setup.h"
#include "semphr.h"

void init_uart_semaphore(void);
void take_user_actions(ParsedData user_command);
extern SemaphoreHandle_t uart_semaphore;
extern SemaphoreHandle_t button_semaphore;

#endif /* SRC_USER_ACTIONS_H */
