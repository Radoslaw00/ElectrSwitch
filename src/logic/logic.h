// ------------------------------[ logic.h ]------------------------------
// This file contains the core logic functions and global status flags for the Electricity Flow Control System.
#ifndef LOGIC_H			//Header guard
#define LOGIC_H			//Header guard
// ------------------------------[ INCLUDES ]------------------------------
#include <stdint.h>				//library for standard integer types
#include <stdbool.h>			//library for boolean type
// ------------------------------[ CORE FUNCTIONS ]------------------------------
void read_sensors(void);			// Reads temperature and voltage sensor data
void cmp_sensor_data(void);			// Compares sensor data against thresholds
void time_update(void);				// Updates internal time counters
void electricity_flow(void);		// Controls electricity flow based on sensor data
void del(void);						// Introduces a delay (20ms at 32kHz clock speed)
// -----------------------[ HELPER/RANGE CHECK FUNCTIONS ]------------------------
bool temp_range(int16_t temperature);	// Checks if temperature is within valid range
bool volt_range(int16_t voltage);		// Checks if voltage is within valid range
// ----------------------------[ GLOBAL STATUS FLAGS ]-----------------------------
extern volatile bool emergency_flag;	// Emergency flag for electricity flow control
extern volatile bool over_temp;			// Over-temperature flag
extern volatile bool over_voltage;		// Over-voltage flag

#endif
// ------------------------------[ END OF FILE ]------------------------------