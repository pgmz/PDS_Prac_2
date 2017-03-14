/*
 * Digital_Signal_Processing.c
 *
 *  Created on: 11/03/2017
 *      Author: Patricio Gomez
 */

#include "Digital_Signal_Processing.h"

const static double h1_n_low_filter[7] = {
		0.07840464525404556,
		0.17707825519483075,
		0.22014353249171387,
		0.2759015644497544,
		0.22014353249171387,
		0.17707825519483075,
		0.17707825519483075
};

const static double h2_n_high_filter[7] = {
		-0.08857280384687653,
		-0.20001387909943527,
		-0.13289448474069163,
		0.7755518089951376,
		-0.13289448474069163,
		-0.20001387909943527,
		-0.08857280384687653
};

int n = 0;

double y_n;
double x_n;
double y_n_1[7] = {
		0,0,0,0,0,0,0
};

double y_n_2[7] = {
		0,0,0,0,0,0,0
};
extern double ADC_data;
extern double *Amp_General;
extern double *Amp_Low_Filter;
extern double *Amp_High_Filter;

void DSP_task (){
	x_n = 2*ADC_data - 1;
	DSP_Amplitude(&x_n, (*Amp_General));
	DSP_Filter(&x_n, &y_n_1[0], &h1_n_low_filter[0], (*Amp_Low_Filter));
	DSP_Filter(&x_n, &y_n_2[0], &h2_n_high_filter[0], (*Amp_High_Filter));
	DSP_Add(&y_n, &y_n_1[0], &y_n_2[0]);
}

void DSP_Amplitude (double * x_n, double Amplitude_factor){
	*x_n = Amplitude_factor*(*x_n);
}

void DSP_Filter (double * x_n, double *y_n, const double *h_n, double Amplitude_factor){

	for(n=0; n<7; n++)
	{
		*(y_n + n) = Amplitude_factor*(*(y_n + (n+1)) + (*x_n)*(h_n[n]));
	}

}

void DSP_Add (double * y_n, double * y_n_1, double *y_n_2){
	*y_n = (*(y_n_1) + *(y_n_2))/2 + 1;
	*(y_n_1) = 0;
	*(y_n_2) = 0;
}
