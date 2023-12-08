/*
 * converter_model.h
 *
 *  Created on: 13 Nov 2023
 *      Author: z103112
 */

#ifndef CONVERTER_MODEL_H
#define CONVERTER_MODEL_H

void converter_init(double x1, double x2, double x3, double x4, double x5, double x6);
void converter_state_trans(double u);
double converter_meas();


#endif /* SRC_CONVERTER_MODEL_H_ */
