/*
 * converter_sim_V2.c
 *
 *  Created on: 13 Nov 2023
 *      Author: z103112
 */
// ZYNQ INCLUDES
#include <xparameters.h>
#include <xuartps_hw.h>
#include <zynq_registers.h>
//#include "matrix_operations.h"
//#include <stdlib.h>
#include <stdio.h>
#include <sleep.h>

//OWN HEADERS
#include "converter_model.h"
#include "pi_controller.h"

//MAIN
int main(void){
	double Kp_init=0.0024;
	double Ki_init=242.1475;
	double u;
	double y;
	double time=0.0;
	double time_step=0.00002;
	const char *del = ",";
	converter_init(0.0,0.0,0.0,0.0,0.0,0.0);
	pi_controller_init(Kp_init,Ki_init);

    // Simulation loop
    for (int k = 0; k < 3000; ++k) { // Run for 10 steps as an example

    	// Controller and simulation
        time=time+time_step;
        y=converter_meas();
        if (k==0){
        	start_controller();
        }
        u=pi_controller_update_state(y);
        converter_state_trans(u);

        if(k==100){
        	pi_controller_update_setpoint(1.0);
        	//start_controller();
        }

        if(k==2000){
        	stop_controller();
        }
        // Data row, print only every tenth simulation sample
        if(k % 10 == 0){
        	printf("%.5f%s%.2f%s%.2f\n",
				time, del, u, del, y);
        };
        sleep(0.1);
    }
}






