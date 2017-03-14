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
#include "semphr.h"
#include "timers.h"

#define DAC_QUEUE_LENGTH	20
#define DAC_QUEUE_ITEM_SIZE	sizeof(float)

void DSP_task ();
void DSP_Amplitude (float * x_n, float Amplitude_factor);
void DSP_Filter (float * x_n, float *y_n, const float *h_n, float Amplitude_factor);
void DSP_Add (float * y_n, float * y_n_1, float *y_n_2);

#endif /* SOURCE_DIGITAL_SIGNAL_PROCESSING_H_ */
