/*
 * External_mod_process.h
 *
 *  Created on: 12/03/2017
 *      Author: Patricio Gomez
 */

#ifndef SOURCE_EXTERNAL_MOD_PROCESS_H_
#define SOURCE_EXTERNAL_MOD_PROCESS_H_

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_pit.h"
#include "board.h"

typedef enum {
	R_PARAM,
	A_PARAM
}Mod_State_type;

#define R_PARAM_MAX	13500
#define R_PARAM_MIN	100
#define A_PARAM_MAX	0.9
#define A_PARAM_MIN 0.1

typedef struct {
	Mod_State_type Mod_State;
	Mod_State_type Mod_Next;
	float Modifier;
	float Modifier_inc;
	float Modifier_max;
	float Modifier_min;
	void (*ftpr)(uint8_t);
	uint8_t led1 :1;
	uint8_t led2 :1;
	uint8_t led3 :1;
}Mod_SM_type;



void External_mod_process_init();

void Modify_amplitude(uint8_t Increment);

void Modify_sampling(uint8_t Increment);

#endif /* SOURCE_EXTERNAL_MOD_PROCESS_H_ */
