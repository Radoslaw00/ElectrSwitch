// ------------------------------[ registers.h ]------------------------------
// This file contains simulated hardware registers for the Electricity Flow Control System.
#ifndef REGISTERS_H
#define REGISTERS_H
// ------------------------------[ INCLUDES ]------------------------------
#include <stdint.h>			//library for standard integer types
#include <stdbool.h>		//library for boolean type
// ------------------------------[ FAKE REGISTERS ]------------------------------
extern volatile uint8_t REG_STATUS;			//Status register
extern volatile uint8_t REG_CONTROL;		//Control register
extern volatile uint16_t REG_DATA;			//Data register
extern volatile uint32_t REG_CONFIG;		//Configuration register
// ------------------------------[ SIMULATED REGISTERS ]------------------------------
void read_simulated_registers(int16_t* temperature, int16_t* voltage);
void set_simulated_registers(int16_t temperature, int16_t voltage);

#endif
// ------------------------------[ END OF FILE ]------------------------------