#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>
#include <stdbool.h>

// ------------------------------[ FAKE REGISTERS ]------------------------------
extern volatile uint8_t REG_STATUS;			//Status register
extern volatile uint8_t REG_CONTROL;		//Control register
extern volatile uint16_t REG_DATA;			//Data register
extern volatile uint32_t REG_CONFIG;		//Configuration register

#endif