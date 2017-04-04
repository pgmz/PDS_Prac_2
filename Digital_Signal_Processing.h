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
#include "External_mod_process.h"

#define DAC_QUEUE_LENGTH	20
#define DAC_QUEUE_ITEM_SIZE	sizeof(float)

void DSP_task ();
void DSP_Echo (float * x_n);

#endif /* SOURCE_DIGITAL_SIGNAL_PROCESSING_H_ */
