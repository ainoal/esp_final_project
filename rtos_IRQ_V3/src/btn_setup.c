#include <xparameters.h>
#include <xgpio.h>
#include "btn_setup.h"
#include "user_actions.h"
#include "timers.h"
#include <xtime_l.h>

// Use debounce delay to handle possible fluctuations in the signal
// when pressing a button, so that a single button press is not taken as 2
#define DEBOUNCE_DELAY 1000

SemaphoreHandle_t button_semaphore;
TimerHandle_t semaphore_timer;

XGpio BTNS_SWTS;
u8 buttons = 0;

XTime last_debounce_time = 0;
XTime current_time;
int button_pressed = 0;


void vTimerCallback(TimerHandle_t semaphore_timer) {
    xSemaphoreGive(button_semaphore); 	// Give the semaphore when timer runs out
    printf("Button semaphore given\n");
}

void init_semaphore_timer(void) {
	semaphore_timer = xTimerCreate("SemaphoreTimer", pdMS_TO_TICKS(10000), pdFALSE, (void *)0, vTimerCallback);
	if (semaphore_timer != NULL) {
        xTimerStart(semaphore_timer, 0);
        printf("Timer started.\n");
    } else {
    	printf("Error: Timer could not be started.\n");
    }
}

void PushButtons_Handler(void *data)
{
	int isUARTSemaphoreTaken = checkUARTSemaphoreStatus();
	if (isUARTSemaphoreTaken == 0) {
		/* Buttons have an effect only if the semaphore is free */
		buttons = AXI_BTN_DATA;
		switch(buttons)
			{
			case 0x00:
				// No button pressed
				button_pressed = 0;
				break;
			case 0x01:
				take_button_semaphore();
				AXI_LED_DATA ^= 0x01;
				XTime_GetTime(&current_time);
				if (!button_pressed && (current_time - last_debounce_time) >= DEBOUNCE_DELAY) {
					// TODO: state change with the LEDs
					last_debounce_time = current_time;
					button_pressed = 1;
					change_state();
					printf("STATE CHANGE\n");
				}

				break;
			case 0x02:
				take_button_semaphore();
				AXI_LED_DATA ^= 0x02;
				XTime_GetTime(&current_time);
				if (!button_pressed && (current_time - last_debounce_time) >= DEBOUNCE_DELAY) {
					last_debounce_time = current_time;
					button_pressed = 1;
					change_par_to_conf();
				}
				break;
			case 0x04:
				take_button_semaphore();
				AXI_LED_DATA ^= 0x04;
				if (get_state()!=0){
					change_setpoint(-0.1);}
				else{
					change_par_value(-1);}
				break;
			case 0x08:
				take_button_semaphore();
				AXI_LED_DATA ^= 0x08;
				if (get_state()!=0){
					change_setpoint(0.1);}
				else{
					change_par_value(1);}
				break;
			}
	}
	else {
		//printf("UART is in conf state\n");
	}
}

int take_button_semaphore() {
	if (button_semaphore != NULL) {
		/* See if the semaphore can be obtained. If the semaphore
		 * is not available wait 10 ticks to see if it becomes free. */

		if (xSemaphoreTake(button_semaphore, 10) == pdTRUE) {
			/* The semaphore was successfully obtained so the shared
			 * resource can be accessed safely. */
			printf("Button semaphore taken\n");
			init_semaphore_timer();
		}
		else {
			/* The semaphore could not be obtained even after waiting 10 ticks, so
			 the shared resource cannot be accessed. */
			// TODO: IMPLEMENT EXCEPTION HANDLING
		}
	}
}

int checkUARTSemaphoreStatus(void) {
	int isUARTSemaphoreTaken = 0;
    if (xSemaphoreTake(uart_semaphore, 0) == pdTRUE) {
        xSemaphoreGive(uart_semaphore); // Release the semaphore immediately
        isUARTSemaphoreTaken = 0; 		// Semaphore is available
    } else {
        isUARTSemaphoreTaken = 1; 		// Semaphore is taken
    }
    return isUARTSemaphoreTaken;
}

