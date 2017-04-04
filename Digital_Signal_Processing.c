/*
 * Digital_Signal_Processing.c
 *
 *  Created on: 11/03/2017
 *      Author: Patricio Gomez
 */

#include "Digital_Signal_Processing.h"

float echo_n[R_PARAM_MAX] = {0};
int n = 0;

float y_n;
float x_n;


extern float ADC_data;
extern float *R_param;
extern float *A_param;

void DSP_task (){
	/*Valor leído por ADC, es x_n **/
	x_n = ADC_data - 0x388;
	DSP_Echo(&x_n);
}

void DSP_Echo(float * x_n){
	echo_n[n] = (*A_param) *echo_n[n];
	y_n = (-1)*((*A_param))*(*x_n) + (1 - (((*A_param))*((*A_param))))*(echo_n[n]);
	echo_n[n] += *x_n;
	n = (n > R_PARAM_MAX)?(0):(n+1);

}
