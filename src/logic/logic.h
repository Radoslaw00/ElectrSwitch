// ------------------------------[ logic.h ]------------------------------
// This file contains the core logic functions and global status flags for the Electricity Flow Control System.
#ifndef LOGIC_H
#define LOGIC_H
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
bool temp_range(int16_t temperature);
bool volt_range(int16_t voltage);
// ----------------------------[ GLOBAL STATUS FLAGS ]-----------------------------
extern volatile bool emergency_flag;
extern volatile bool over_temp;
extern volatile bool over_voltage;

#endif
// ------------------------------[ END OF FILE ]------------------------------