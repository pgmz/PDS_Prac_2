/*
 * DAC_output_process.h
 *
 *  Created on: 11/03/2017
 *      Author: Patricio Gomez
 */

#ifndef SOURCE_DAC_OUTPUT_PROCESS_H_
#define SOURCE_DAC_OUTPUT_PROCESS_H_

#include "fsl_dac.h"
#include "FreeRTOS.h"
#include "queue.h"

#define DAC_QUEUE_LENGTH	5
#define DAC_QUEUE_ITEM_SIZE	sizeof(double)


#endif /* SOURCE_DAC_OUTPUT_PROCESS_H_ */
