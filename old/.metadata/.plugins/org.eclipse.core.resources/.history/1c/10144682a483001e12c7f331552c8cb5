/*
 * This state machine demo demonstrates the state changes when
 * the "BTN0" button is pressed. It visualizes the state with
 * lighting up the respective LED (not the final functionality;
 * feel free to modify and build on top of this)
 */

// Move definitions to a header file in the final project?
#define CONFIGURATION 0
#define IDLING 1
#define MODULATING 2

// Use debounce delay to handle possible fluctuations in the signal
// when pressing a button, so that a single button press is not taken as 2
#define DEBOUNCE_DELAY 1000

#include <zynq_registers.h>
#include <xtime_l.h>

int main(void) {
	int state = CONFIGURATION;

	AXI_LED_TRI &= ~0x0; 	// Set LEDs as output (0)
	AXI_BTN_TRI &= ~0xF;	// Set buttons as input (1)

	XTime last_debounce_time = 0;
	XTime current_time;
	int button_pressed = 0;

	while(1) {
		switch(state) {
		case CONFIGURATION:
			AXI_LED_DATA = 0b0001;	// Show state with LED
			if (AXI_BTN_DATA == 0b0001) {
				XTime_GetTime(&current_time);
				if (!button_pressed && (current_time - last_debounce_time) >= DEBOUNCE_DELAY) {
					last_debounce_time = current_time;
					button_pressed = 1;
					state = IDLING;
				}
			} else {
				button_pressed = 0;
			}
			break;
		case IDLING:
			AXI_LED_DATA = 0b0010;	// Show state with LED
			if (AXI_BTN_DATA == 0b0001) {
				XTime_GetTime(&current_time);
				if (!button_pressed && (current_time - last_debounce_time) >= DEBOUNCE_DELAY) {
					last_debounce_time = current_time;
					button_pressed = 1;
					state = MODULATING;
				}
			} else {
				button_pressed = 0;
			}
			break;
		case MODULATING:
			AXI_LED_DATA = 0b0100;	// Show state with LED
			if (AXI_BTN_DATA == 0b0001) {
				XTime_GetTime(&current_time);
				if (!button_pressed && (current_time - last_debounce_time) >= DEBOUNCE_DELAY) {
					last_debounce_time = current_time;
					button_pressed = 1;
					state = CONFIGURATION;
				}
			} else {
				button_pressed = 0;
			}
			break;
		default:
			break;
		}
	}
}
