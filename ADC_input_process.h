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

#define ADC_Convert_Volt(ADC_Data)	(double)((3.3/4095)*(ADC_Data))

#define ADC_QUEUE_LENGTH	20
#define ADC_QUEUE_ITEM_SIZE	sizeof(double)

void ADC_input_process_init();
void ADC_Convertion_task(void *pvParameters);


#endif /* SOURCE_ADC_INPUT_PROCESS_H_ */
