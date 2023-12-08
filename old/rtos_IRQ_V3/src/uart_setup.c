
#include "uart_setup.h"
#include <sleep.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pi_controller.h"


# define BUFFER_SIZE 20
static char rx_buf[BUFFER_SIZE];

//static int message_ready = 0;

void SetupUART(){
        uint32_t r = 0; // Temporary value variable

        r = UART_CTRL;
        r &= ~(XUARTPS_CR_TX_EN | XUARTPS_CR_RX_EN); // Clear Tx & Rx Enable
        r |= XUARTPS_CR_RX_DIS | XUARTPS_CR_TX_DIS; // Tx & Rx Disable
        UART_CTRL = r;

        UART_MODE = 0;
        UART_MODE &= ~XUARTPS_MR_CLKSEL; // Clear "Input clock selection" - 0: clock source is uart_ref_clk
        UART_MODE |= XUARTPS_MR_CHARLEN_8_BIT; // Set "8 bits data"
        UART_MODE |= XUARTPS_MR_PARITY_NONE; // Set "No parity mode"
        UART_MODE |= XUARTPS_MR_STOPMODE_1_BIT; // Set "1 stop bit"
        UART_MODE |= XUARTPS_MR_CHMODE_NORM; // Set "Normal mode"

        // baud_rate = sel_clk / (CD * (BDIV + 1) (ref: UG585 - TRM - Ch. 19 UART)
        UART_BAUD_DIV = 6; // ("BDIV")
        UART_BAUD_GEN = 124; // ("CD")
        // Baud Rate = 100Mhz / (124 * (6 + 1)) = 115200 bps

        UART_CTRL |= (XUARTPS_CR_TXRST | XUARTPS_CR_RXRST); // TX & RX logic reset

        r = UART_CTRL;
        r |= XUARTPS_CR_RX_EN | XUARTPS_CR_TX_EN; // Set TX & RX enabled
        r &= ~(XUARTPS_CR_RX_DIS | XUARTPS_CR_TX_DIS); // Clear TX & RX disabled
        UART_CTRL = r;
 }

// Check if UART receive FIFO is not empty and return the new data
char uart_receive() { //receive a single character from UART
	 if ((UART_STATUS & XUARTPS_SR_RXEMPTY) == XUARTPS_SR_RXEMPTY) return 0;
	 return UART_FIFO;
}

ParsedData command_parser(const char* uart_message){
    ParsedData result;

    const char* token = strtok(uart_message, "=");
    if (token != NULL) {
        strncpy(result.identifier, token, sizeof(result.identifier));
        result.identifier[sizeof(result.identifier) - 1] = '\0';  // Ensure null termination
    } else {
        strcpy(result.identifier, "Invalid");
    }

    token = strtok(NULL, "=");
    if (token != NULL) {
        result.value = atof(token);
    } else {
        result.value = 0.0;
    }

    return result;
}

const char* receive_message(){ //receive a string of characters
		int index_str=0;
		char input = uart_receive();
		// If an UART message was received
		while(input){
			if (input == '\r' || input == '\n' || index_str==BUFFER_SIZE){
				rx_buf[index_str] = '\0';
				uart_receive();
				return rx_buf;
			}
			else {
				rx_buf[index_str] = input;
				index_str++;
			}
			input=uart_receive();
		}
		return 0;
}
