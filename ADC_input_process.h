/*
 * ADC_input_process.h
 *
 *  Created on: 04/03/2017
 *      Author: Patricio Gomez
 */

#ifndef SOURCE_ADC_INPUT_PROCESS_H_
#define SOURCE_ADC_INPUT_PROCESS_H_

#include "fsl_adc16.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"


#define QUEUE_LENGTH	5
#define QUEUE_ITEM_SIZE	sizeof(uint32_t)

void ADC_input_process_init();
void ADC_Convertion_task(void *pvParameters);


#endif /* SOURCE_ADC_INPUT_PROCESS_H_ */
