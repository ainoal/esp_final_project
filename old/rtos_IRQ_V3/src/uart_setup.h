
#ifndef SRC_UART_SETUP_H_
#define SRC_UART_SETUP_H_

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include <xscugic.h>
#include "zynq_registers.h"
#include <xuartps_hw.h>

#define UARTPS_1_INTR        82U

typedef struct {
    char identifier[20];
    float value;
} ParsedData;

void SetupUART();
//void SetupUARTInterrupt();
//void UartIRQHandler();
char uart_receive();
const char* receive_message();
ParsedData command_parser(const char* uart_message);

extern XScuGic xInterruptController;	// Interrupt controller instance

#endif /* SRC_UART_SETUP_H_ */
