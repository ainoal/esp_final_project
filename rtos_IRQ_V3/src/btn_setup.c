#include <xparameters.h>
#include <xgpio.h>
#include "btn_setup.h"
#include "user_actions.h"


XGpio BTNS_SWTS;
u8 buttons = 0;

/*void SetupPushButtons()
{
	XGpio_Initialize(&BTNS_SWTS, BUTTONS_AXI_ID);
	XGpio_InterruptEnable(&BTNS_SWTS, 0xF);
	XGpio_InterruptGlobalEnable(&BTNS_SWTS);

	Xil_ExceptionInit();

	// Enable interrupts.
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT, (Xil_ExceptionHandler) XScuGic_InterruptHandler, &xInterruptController);

	/* Defines the PushButtons_Intr_Handler as the FIQ interrupt handler.*/
	/*Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_FIQ_INT,
								 (Xil_ExceptionHandler) PushButtons_Intr_Handler,
								 &xInterruptController);
	Xil_ExceptionEnableMask(XIL_EXCEPTION_FIQ);
}*/

void PushButtons_Handler(void *data)
//BUTTON SEMAPHORE ACQUIRED
{
	int isUARTSemaphoreTaken = checkUARTSemaphoreStatus();
	if (isUARTSemaphoreTaken == 0) {
		/* Buttons have an effect only if the semaphore is free */
		buttons = AXI_BTN_DATA;
		//printf("Push buttons\n");
		switch(buttons)
			{
			case 0x01:
				AXI_LED_DATA ^= 0x01;
				change_state();
				break;
			case 0x02:
				AXI_LED_DATA ^= 0x02;
				change_par_to_conf();
				break;
			case 0x04:
				AXI_LED_DATA ^= 0x04;
				if (get_state()!=0){
					change_setpoint(-0.1);}
				else{
					change_par_value(-1);}
				break;
			case 0x08:
				AXI_LED_DATA ^= 0x08;
				if (get_state()!=0){
					change_setpoint(0.1);}
				else{
					change_par_value(1);}
				break;
				break;
			}
	}
	else {
		printf("UART is in conf state\n");
	}

	//XGpio_InterruptClear(&BTNS_SWTS,0xF);
}

int checkUARTSemaphoreStatus(void) {
	int isUARTSemaphoreTaken = 0;
    if (xSemaphoreTake(uart_semaphore, 0) == pdTRUE) {
        xSemaphoreGive(uart_semaphore); // Release the semaphore immediately
        isUARTSemaphoreTaken = 0; 	// Semaphore is available
    } else {
        isUARTSemaphoreTaken = 1; 		// Semaphore is taken
    }
    return isUARTSemaphoreTaken;
}

