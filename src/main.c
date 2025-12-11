// ------------------------------[ main.c ]------------------------------
//  This file contains the main event loop for the Electricity Flow Control System.
// ------------------------------[ DISCLAIMER ]------------------------------
// This software is pureley theoretical and is not intended for practical use.
// This code should not used in any real MCUs or embedded systems.
// This code is provided "as is" without any warranties or my support.
// Use it at your own risk.
// I am not responsible for any damage or issues caused by this code.
// You are free to modify and distribute this code.
// ------------------------------[ TARGET MCU ]------------------------------
// This code is intended to run on bare metal microcontrollers without an RTOS or any OS.
// Clock speed: 32.000Hz (32kHz)
// Memory: 8KB Flash, 1KB RAM
// --------------------------------[ PROJECT ]--------------------------------
// Electricity Flow Control System.
// Controls electricity flow based on temperature and voltage sensor data.
// Could be used in UPS, Power Supplies, Solar Inverters, etc.
// ------------------------------[ FILE STRUCTURE ]------------------------------
// Files:	/src/main.c
//			/src/logic/logic.c | h
//			/src/registers/registers.c | h
// ---------------------------------[ LICENSE ]---------------------------------
// This code is licensed under the MIT License.
// ----------------------------------[ AUTHOR ]----------------------------------
// Author: Radoslaw00
// Link: https://github.com/Radoslaw00
// Link to ElectrSwitch project: https://github.com/Radoslaw00/ElectrSwitch
// Discord: ideqe
// ---------------------------------[ INCLUDES ]---------------------------------
#include "logic/logic.h"			// Include core logic header file
#include "registers/registers.h"	// Include simulated hardware registers header file
// ------------------------------[ MAIN EVENT LOOP ]------------------------------
int main(void) {									// Initialization

	while (1) {										// Infinite loop
		read_sensors();		// Read sensor data
		cmp_sensor_data();							// Compare sensor data
		time_update();								// Update time
		electricity_flow();							// Control electricity flow

		delay_ms();									// 20 milisecond delay on 32kHz
	}                                               // Go back to the beginning of the loop
	return 0;                                       // Never reaches here

    //Note why return: return 0 is unreachable in this no-os infinite-loop design, but kept
    //for completeness/compiler satisfaction if compiled in a non-embedded environment.
}                                                   // End of 'main' function
// ------------------------------[ END OF FILE ]------------------------------
// ------------------------------[ CONTROL FLOW ]------------------------------
/*
MAIN {
- read_sensors
- read_sensor_data
- read_simulated_registers
- cmp_sensor_data
- compare_sensor_data
- IF temperature > MAX_TMP
- over_temp = true
- ELIF over_temp IS true AND temperature < TEMP_CLEAR_HIGH
    - over_temp = false
    - ELIF temperature < MIN_TMP
    - over_temp = true
    - ELIF over_temp IS true AND temperature > TEMP_CLEAR_LOW
    - over_temp = false
    - IF voltage > MAX_VOL
    - over_voltage = true
    - ELIF over_voltage IS true AND voltage < VOL_CLEAR_HIGH
    - over_voltage = false
    - ELIF voltage < MIN_VOL
    - over_voltage = true
    - ELIF over_voltage IS true AND voltage > VOL_CLEAR_LOW
    - over_voltage = false
    - IF over_temp OR over_voltage
    - emergency_flag = true
    - ELSE
    - emergency_flag = false
    - time_update
    - time
    - tick_20ms++
    - IF tick_20ms >= 50
    - tick_20ms = 0
    - seconds++
    - electricity_flow
    - elec_ctrl_flow_output
    - IF emergency_flag
    - elec_cutoff
    - Manipulate REG_CONTROL(Cut Power)
    - ELSE
    - elec_flow
    - Manipulate REG_CONTROL(Maintain Power)
    - delay_ms
    - del
    - Execute 640 - cycle NOP loop }                                    */