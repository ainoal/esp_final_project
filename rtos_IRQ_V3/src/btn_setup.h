
#include <xscugic.h>
#include "zynq_registers.h"
#include "pi_controller.h"
#include "state_machine.h"
#include "user_actions.h"

#ifndef SRC_BTN_SETUP_H_
#define SRC_BTN_SETUP_H_

#define BUTTONS_channel		2
#define BUTTONS_AXI_ID		XPAR_AXI_GPIO_SW_BTN_DEVICE_ID

void init_semaphore_timer(void);
void PushButtons_Handler();
int checkUARTSemaphoreStatus(void);
void take_button_semaphore(void);


#endif /* SRC_BTN_SETUP_H_ */
