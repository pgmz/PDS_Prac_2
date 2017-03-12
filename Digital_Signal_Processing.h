/*
 * Digital_Signal_Processing.h
 *
 *  Created on: 11/03/2017
 *      Author: Patricio Gomez
 */

#ifndef SOURCE_DIGITAL_SIGNAL_PROCESSING_H_
#define SOURCE_DIGITAL_SIGNAL_PROCESSING_H_

#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

void DSP_task (void *pvParameters);
void DSP_Amplitude (double * x_n, double Amlitude_factor);
void DSP_Low_Filter (double * x_n, double *y_n_1, double Amlitude_factor);
void DSP_High_Filter (double * x_n, double *y_n_2, double Amlitude_factor);
void DSP_Add (double * y_n, double * y_n_1, double *y_n_2);

#endif /* SOURCE_DIGITAL_SIGNAL_PROCESSING_H_ */
