#include <xparameters.h>
#include <xuartps_hw.h>
#include <zynq_registers.h>
#include "matrix_operations.h"
//#include <stdlib.h>
#include <stdio.h>
#include <sleep.h>



// For exact explanations for registers see:
// UG585 - Zynq-7000 SoC Technical Reference Manual - B.33 UART Controller (UART)
// https://www.xilinx.com/support/documentation/user_guides/ug585-Zynq-7000-TRM.pdf

//THIS PROJECT DEMONSTRATES THE SIMULATION OF CONVERTER AND CONTROL
//WITH FOLLOWING FUNCTIONALITIES:
// - A STEP RESPONSE OF CONVERTER IS SIMULATED ONCE
// - NO USER INPUT IS TAKEN
// - CONVERTER OUTPUT IS PRINTED TO TERMINAL

// System matrices and vectors
double A[N][N] = {
	{0.9652, -0.0172, 0.0057, -0.0058, 0.0052, -0.0251},
	{0.7732, 0.1252, 0.2315, 0.0700, 0.1282, 0.7754},
	{0.8278, -0.7522, -0.0956, 0.3299, -0.4855, 0.3915},
	{0.9948, 0.2655, -0.3848, 0.4212, 0.3927, 0.2899},
	{0.7648, -0.4165, -0.4855, -0.3366, -0.0986, 0.7281},
	{1.1056, 0.7587, -0.1179, 0.0748, -0.2192, 0.1491}
};
double B[N][M] = {
	{0.0471},
	{0.0377},
	{0.0404},
	{0.0485},
	{0.0373},
	{0.0539},
};
double C[P][N] = {{0, 0, 0, 0, 0, 1}};


int main(void) {
	AXI_LED_TRI &= 0x0;
	AXI_LED_DATA = 0b0001;
    double x[N] = {0}; // Initial state vector
    double u;      // Input (scalar for simplicity)
    double y;          // Output
    double time;
    double time_step=0.00002;
    double Kp=0.0024;
    double Ki=242.1475;
    double error;
    double ref_sig=1.0;
    double error_int=0;
    // Define the serial output delimiter as a string
	const char *del = ",";

	// Header
	printf("time%su%sy%sx1%sx2%sx3%sx4%sx5%sx6\n", del, del, del, del, del, del, del, del);

    //while(1){
	x[0]=0;
	x[1]=0;
	x[2]=0;
	x[3]=0;
	x[4]=0;
	x[5]=0;
	time=0;
	y=0;
    // Simulation loop
    for (int k = 0; k < 3000; ++k) { // Run for 10 steps as an example
        // Controller
    	error=ref_sig-y;
    	error_int=error_int+error*time_step;
    	u=Kp*error+Ki*error_int;


    	double Ax[N], Bu[N], next_x[N];

        // Calculate A*x
        matrix_vector_multiply(A, x, Ax);

        // Calculate B*u
        for (int i = 0; i < N; ++i) {
            Bu[i] = B[i][0] * u;
        }

        // Calculate x_{k+1} = A*x + B*u
        add_vectors(Ax, Bu, next_x);

        // Copy next_x to x for the next iteration
        for (int i = 0; i < N; ++i) {
            x[i] = next_x[i];
        }

        // Compute output y = C*x
        y = output(C, x);
        time=time+time_step;

        // Data row, print only every tenth simulation sample
        if(k % 10 == 0){
        	printf("%.5f%s%.2f%s%.2f%s%.2f%s%.2f%s%.2f%s%.2f%s%.2f%s%.2f\n",
				time, del, u, del, y, del, x[0], del, x[1], del, x[2], del, x[3], del, x[4], del, x[5]);
        };
		// Print the current state and output
		//printf("Step \t%d: State x = [\t%2f, \t%2f], Output y = \t%2f\n", k, x[0], x[1], y);
        sleep(0.1);
    }

    //}
    return 0;
}
