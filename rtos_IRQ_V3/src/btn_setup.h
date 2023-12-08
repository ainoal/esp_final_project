
#include <xscugic.h>
#include "zynq_registers.h"
#include "pi_controller.h"
#include "state_machine.h"
//#include "semphr.h"

#ifndef SRC_BTN_SETUP_H_
#define SRC_BTN_SETUP_H_

#define BUTTONS_channel		2
#define BUTTONS_AXI_ID		XPAR_AXI_GPIO_SW_BTN_DEVICE_ID

//extern XScuGic xInterruptController;	// Interrupt controller instance

//extern SemaphoreHandle_t button_semaphore;

//void init_button_semaphore(void)
void SetupPushButtons();
void PushButtons_Handler();
int checkUARTSemaphoreStatus(void);
int take_button_semaphore(void);


#endif /* SRC_BTN_SETUP_H_ */
