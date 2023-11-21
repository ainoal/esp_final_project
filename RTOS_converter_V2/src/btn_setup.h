
#include <xscugic.h>
#include "zynq_registers.h"
#include "pi_controller.h"
#include "state_machine.h"

#ifndef SRC_BTN_SETUP_H_
#define SRC_BTN_SETUP_H_

#define BUTTONS_channel		2
#define BUTTONS_AXI_ID		XPAR_AXI_GPIO_SW_BTN_DEVICE_ID

extern XScuGic xInterruptController;	// Interrupt controller instance

void SetupPushButtons();
void PushButtons_Intr_Handler();


#endif /* SRC_BTN_SETUP_H_ */
