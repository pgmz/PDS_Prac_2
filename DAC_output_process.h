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
#include "semphr.h"

#define DAC_Convert_Volt(DAC_Data)		((uint16_t)((4095/3.3)*(DAC_Data)))

void DAC_output_process_init();
void DAC_output_task();


#endif /* SOURCE_DAC_OUTPUT_PROCESS_H_ */
