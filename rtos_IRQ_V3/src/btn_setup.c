#include <xparameters.h>
#include <xgpio.h>
#include "btn_setup.h"


XGpio BTNS_SWTS;
u8 buttons = 0;

void SetupPushButtons()
{
	XGpio_Initialize(&BTNS_SWTS, BUTTONS_AXI_ID);
	XGpio_InterruptEnable(&BTNS_SWTS, 0xF);
	XGpio_InterruptGlobalEnable(&BTNS_SWTS);

	Xil_ExceptionInit();

	// Enable interrupts.
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT, (Xil_ExceptionHandler) XScuGic_InterruptHandler, &xInterruptController);

	/* Defines the PushButtons_Intr_Handler as the FIQ interrupt handler.*/
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_FIQ_INT,
								 (Xil_ExceptionHandler) PushButtons_Intr_Handler,
								 &xInterruptController);
	Xil_ExceptionEnableMask(XIL_EXCEPTION_FIQ);
}

void PushButtons_Intr_Handler(void *data)
//BUTTON SEMAPHORE ACQUIRED
{
	buttons = XGpio_DiscreteRead(&BTNS_SWTS, BUTTONS_channel);
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
	XGpio_InterruptClear(&BTNS_SWTS,0xF);
}
