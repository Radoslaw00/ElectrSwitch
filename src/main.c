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
		read_sensors();		                        // Read sensor data
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