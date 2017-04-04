/*
 * ADC_input_process.h
 *
 *  Created on: 04/03/2017
 *      Author: Patricio Gomez
 */

#ifndef SOURCE_ADC_INPUT_PROCESS_H_
#define SOURCE_ADC_INPUT_PROCESS_H_

#include "PIT_sample_frec.h"
#include "Digital_Signal_Processing.h"
#include "DAC_output_process.h"

#include "fsl_adc16.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "timers.h"

void ADC_input_process_init();
void ADC_Convertion_task();


#endif /* SOURCE_ADC_INPUT_PROCESS_H_ */
