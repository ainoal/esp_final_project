#include <xparameters.h>
#include <xgpio.h>
#include "btn_setup.h"
#include "timers.h"
#include <xtime_l.h>
#include "state_machine.h"
#include <stdio.h>

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
	semaphore_timer = xTimerCreate("SemaphoreTimer", pdMS_TO_TICKS(5000), pdFALSE, (void *)0, vTimerCallback);
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
		// Buttons have an effect only if the semaphore is free
		buttons = AXI_BTN_DATA;
		int state;
		switch(buttons)
			{
			case 0x00:
				// No button pressed
				button_pressed = 0;
				break;
			case 0x01:
				take_button_semaphore();
				//AXI_LED_DATA ^= 0x01;
				XTime_GetTime(&current_time);
				// Handle long button presses
				if (!button_pressed && (current_time - last_debounce_time) >= DEBOUNCE_DELAY) {
					last_debounce_time = current_time;
					button_pressed = 1;
					change_state();
					state=get_state();
					switch(state){
					case CONFIGURATION:
						if (get_par_to_conf()==conf_kp){
							printf("Entered to configuration mode.\n You can decrease or increase the value of Kp/Ki with buttons 2 and 3, respectively,\n change the configurable parameter with button 1 (currently Kp) or enter idle mode with button 0\n");}
						else{
							printf("Entered to configuration mode.\n You can decrease or increase the value of Kp/Ki with buttons 2 and 3, respectively,\n change the configurable parameter with button 1 (currently Ki) or enter idle mode with button 0\n");}
					break;
					case IDLING:
						printf("Entered to idle mode.\n You can enter modulation mode with button 0\n");
					break;
					case MODULATING:
						printf("Entered to modulation mode.\n You can decrease or increase the value of uref with buttons 2 and 3, respectively,\nor enter configuration mode with button 0\n");
					break;
					}
				}

				break;
			case 0x02:
				take_button_semaphore();
				//AXI_LED_DATA ^= 0x02;
				XTime_GetTime(&current_time);
				// Handle long button presses
				if (!button_pressed && (current_time - last_debounce_time) >= DEBOUNCE_DELAY) {
					last_debounce_time = current_time;
					button_pressed = 1;
					int par_to_conf=change_par_to_conf();
					if (par_to_conf==conf_kp){
						printf("Configurable parameter changed to Kp\n");
					}else{printf("Configurable parameter changed to Ki\n");}
				}
				break;
			case 0x04:
				take_button_semaphore();
				//AXI_LED_DATA ^= 0x04;
				if (get_state()==MODULATING){
					double setpoint_val=change_setpoint(-0.1);
					//printf("New set point = %.2f\n", setpoint_val);
				}
				else if(get_state()==CONFIGURATION){
					double par_value;
					if (get_par_to_conf()==conf_kp){
						par_value=change_par_value(-0.0001);
						printf("Kp changed to %.4f\n", par_value);
					}else{
						par_value=change_par_value(-1.0);
						printf("Ki changed to %.2f\n", par_value);
					}
				}

				break;
			case 0x08:
				take_button_semaphore();
				//AXI_LED_DATA ^= 0x08;
				if (get_state()==MODULATING){
					double setpoint_val=change_setpoint(0.1);
					//printf("New set point = %.2f\n", setpoint_val);
				}
				else if(get_state()==CONFIGURATION){
					double par_value;
					if (get_par_to_conf()==conf_kp){
						par_value=change_par_value(0.0001);
						printf("Kp changed to %.4f\n", par_value);
					}else{
						par_value=change_par_value(1.0);
						printf("Ki changed to %.2f\n", par_value);
					}
				}
				break;
			}
	}
	else {
	}
}

void take_button_semaphore() {
	if (button_semaphore != NULL) {
		// See if the semaphore can be obtained. If the semaphore
		// is not available wait 10 ticks to see if it becomes free.

		if (xSemaphoreTake(button_semaphore, 10) == pdTRUE) {
			// The semaphore was successfully obtained
			printf("Button semaphore taken\n");
			init_semaphore_timer();
		}
		else {
			// Button semaphore is already taken
		}
	}
}

int checkUARTSemaphoreStatus(void) {
	// Check if semaphore can be obtained by trying to take it
	// and if successful, releasing the semaphore immediately
	int isUARTSemaphoreTaken = 0;

    if (xSemaphoreTake(uart_semaphore, 0) == pdTRUE) {
        xSemaphoreGive(uart_semaphore);
        isUARTSemaphoreTaken = 0; 		// Semaphore is available
    } else {
        isUARTSemaphoreTaken = 1; 		// Semaphore is taken
    }
    return isUARTSemaphoreTaken;
}

