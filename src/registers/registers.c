//fake registers for an embedded system
//simulated registers for an embedded system

#include "registers.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

//define registers
volatile uint8_t REG_STATUS = 0x00;				// Status register
volatile uint8_t REG_CONTROL = 0x00;			// Control register
volatile uint16_t REG_DATA = 0x0000;			// Data register
volatile uint32_t REG_CONFIG = 0x00000000;		// Configuration register

