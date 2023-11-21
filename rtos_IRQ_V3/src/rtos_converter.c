
#include <xscugic.h>
#include "FreeRTOS.h"
#include "task.h"

#include "timer_setup.h"
#include "uart_setup.h"
#include "btn_setup.h"
#include "pi_controller.h"
#include "converter_model.h"
#include <stdio.h>

extern XScuGic xInterruptController;

void simulate_and_control();
void output_to_user();
void read_UART();
void SetupInterrupts();

int main(void)
{
	AXI_LED_TRI &= ~(0b1111UL);
	AXI_BTN_TRI |= 0xF;

	SetupInterrupts();
	SetupUART();
	//SetupUARTInterrupt();
	SetupTimer();
	SetupTicker();
	SetupPushButtons();

	double Kp_init=0.0024;
	double Ki_init=242.1475;
	converter_init(0.0,0.0,0.0,0.0,0.0,0.0);
	pi_controller_init(Kp_init,Ki_init);


	xTaskCreate(simulate_and_control, "simulate_and_control", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+3, NULL);
	xTaskCreate(output_to_user, "output_to_user", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);
	xTaskCreate(read_UART,"read_UART",configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL);

	vTaskStartScheduler();

	/* As normal, the following line should never be reached. */
	for (;;);

}

void SetupInterrupts()
{
	XScuGic_Config *pxGICConfig;

	/* Ensure no interrupts execute while the scheduler is in an inconsistent
	state.  Interrupts are automatically enabled when the scheduler is
	started. */
	portDISABLE_INTERRUPTS();

	/* Obtain the configuration of the GIC. */
	pxGICConfig = XScuGic_LookupConfig( XPAR_SCUGIC_SINGLE_DEVICE_ID );

	/* Install a default handler for each GIC interrupt. */
	XScuGic_CfgInitialize( &xInterruptController, pxGICConfig, pxGICConfig->CpuBaseAddress );
}

void simulate_and_control() {
	const TickType_t freq = pdMS_TO_TICKS( 20 ); // in ms
	double u;
	struct time_stamp_meas meas;
	TickType_t wakeTime = xTaskGetTickCount();  // only once initialized

	for( ;; ) {
		AXI_LED_DATA ^= 0x01;
		meas=converter_meas();
		//set_PWM_duty_cycle(meas.y);  TO BE IMPLEMENTED
		u=pi_controller_update_state(meas.y);
		converter_state_trans(u);
		vTaskDelayUntil( &wakeTime, freq );
	}
}

void output_to_user() {
	const TickType_t freq = pdMS_TO_TICKS( 2000 ); // in ms
	const char *del = ",";
	// https://www.freertos.org/a00021.html#xTaskGetTickCount
	TickType_t wakeTime = xTaskGetTickCount();  // only once initialized
	struct time_stamp_meas meas;
	double u;

	for( ;; ) {
		AXI_LED_DATA ^= 0x02;
		meas=converter_meas();
		u=pi_controller_get_state();
		printf("%.5f%s%.2f%s%.2f\n",
			meas.time, del, u, del, meas.y);
		// https://www.freertos.org/vtaskdelayuntil.html
		vTaskDelayUntil( &wakeTime, freq );

	}
}



void read_UART(){
	//
	const TickType_t freq = pdMS_TO_TICKS( 100 ); // in ms
	TickType_t wakeTime = xTaskGetTickCount();  // only once initialized
	const char *message[20];
	ParsedData user_data;
	for( ;; ) {
	//char* test=uart_receive();
	//while(test){
	//	printf("%c\n",test);
	//	test=uart_receive();
	//}
	*message=receive_message();
	if (*message){
		printf("%s\n",*message);
		user_data=command_parser(*message);
		//printf("%s\n",user_data.identifier);
		//printf("%.2f\n",user_data.value);

		take_user_actions(user_data); //TO BE IMPLEMENTED



		///////////////////////////////////////////////////////////////////
		if(strcmp(user_data.identifier,"uref")==0){
			pi_controller_update_setpoint(user_data.value);
			//AXI_LED_DATA ^= 0x4;
		}
		else if(strcmp(user_data.identifier,"run_command")==0){
			if(user_data.value==1){
				start_controller();
			}
			else if(user_data.value==0){
				stop_controller();
			}
		}
		/////////////////////////////////////////////////////////////////////
	}


	vTaskDelayUntil( &wakeTime, freq );
	}
}
