/*
 * PIT_sample_frec.h
 *
 *  Created on: 13/03/2017
 *      Author: Patricio Gomez
 */

#ifndef SOURCE_PIT_SAMPLE_FREC_H_
#define SOURCE_PIT_SAMPLE_FREC_H_

#include "fsl_pit.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

void PIT_sample_frec_init();
void PIT_sample_frec_start();

#endif /* SOURCE_PIT_SAMPLE_FREC_H_ */
