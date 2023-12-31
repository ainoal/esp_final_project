/*
 * converter_model.c
 *
 *  Created on: 13 Nov 2023
 *      Author: z103112
 */

#ifndef CONVERTER_MODEL_C
#define CONVERTER_MODEL_C

#include "converter_model.h"

#define N 6 // Dimension of state vector x
#define M 1 // Dimension of input vector u
#define P 1 // Dimension of output vector y

static double x[N] = {0}; // Initial state vector
static double time=0.0;
static double time_step=0.00002;
static double x_new[N] = {0}; // Temporary state vector


static double A[N][N] = { //State transition matrix
		{0.9652, -0.0172, 0.0057, -0.0058, 0.0052, -0.0251},
		{0.7732, 0.1252, 0.2315, 0.0700, 0.1282, 0.7754},
		{0.8278, -0.7522, -0.0956, 0.3299, -0.4855, 0.3915},
		{0.9948, 0.2655, -0.3848, 0.4212, 0.3927, 0.2899},
		{0.7648, -0.4165, -0.4855, -0.3366, -0.0986, 0.7281},
		{1.1056, 0.7587, -0.1179, 0.0748, -0.2192, 0.1491}
	};

static double B[N][M] = { //Control matrix (discrete time)
	{0.0471},
	{0.0377},
	{0.0404},
	{0.0485},
	{0.0373},
	{0.0539},
};


// PRIVATE FUNCTIONS
static void state_trans(double u) { //compute A*x
    for (int i = 0; i < N; ++i) {
    	x_new[i]=0;
    	for (int j = 0; j < N; ++j) {
            x_new[i] += A[i][j] * x[j];
        }
    }
    for (int i = 0; i < N; ++i){
    	x[i]=x_new[i];
    }

}

static void control_system(double u) { //compute the effect of the control (B*u)
	for (int i = 0; i < N; ++i) {
		x[i] += B[i][0] * u;
	}
}

// PUBLIC FUNCTIONS
void converter_init(double x1, double x2, double x3, double x4, double x5, double x6){
	x[0]=x1;
	x[1]=x2;
	x[2]=x3;
	x[3]=x4;
	x[4]=x5;
	x[5]=x6;
}

void converter_state_trans(double u){
    state_trans(u); // Compute x=A*x
    control_system(u); //Compute x+=B*u
    time+=time_step; //increment time
}

struct time_stamp_meas converter_meas(){ //return struct with fields: y=x[5] and time
	struct time_stamp_meas m;
	m.time=time;
	m.y=x[5];
	return m;
}

#endif /* SRC_CONVERTER_MODEL_C_ */
