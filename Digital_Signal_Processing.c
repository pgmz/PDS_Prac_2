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

int k = 0;
int n = 0;

extern QueueHandle_t ADC_Convertion_Data;

void DSP_task (void *pvParameters){

	double x_n;
	double y_n;

	double y_n_1[7] = {
			0,0,0,0,0,0,0
	};

	double y_n_2[7] = {
			0,0,0,0,0,0,0
	};

	for(;;)
	{
		xQueueReceive(ADC_Convertion_Data, &x_n, portMAX_DELAY);
		DSP_Amplitude(&x_n, 1);
		DSP_Low_Filter(&x_n, &y_n_1[0], 1);
		DSP_High_Filter(&x_n, &y_n_2[0], 1);
		DSP_Add(&y_n, &y_n_1[0], &y_n_2[0]);
	}

}

void DSP_Amplitude (double * x_n, double Amlitude_factor){
	*x_n = Amlitude_factor*(*x_n);
}

void DSP_Low_Filter (double * x_n, double *y_n_1, double Amlitude_factor){

	for(n=0; n<6; n++)
	{
		*(y_n_1 + n) = Amlitude_factor*(*(y_n_1 + (n+1)) + (*x_n)*(h1_n_low_filter[n]));
	}

}

void DSP_High_Filter (double * x_n, double *y_n_2, double Amlitude_factor){

	for(n=0; n<6; n++)
	{
		*(y_n_2 + n) = Amlitude_factor*(*(y_n_2 + (n+1)) + (*x_n)*(h2_n_high_filter[n]));
	}

}

void DSP_Add (double * y_n, double * y_n_1, double *y_n_2){
	*y_n = *(y_n_1) + *(y_n_2);
	*(y_n_1) = 0;
	*(y_n_2) = 0;
}
