// ------------------------------[ DISCLAIMER ]------------------------------
// This software is pureley theoretical and is not intended for practical use.
// This code should not used in any real MCUs or embedded systems.
// This code is provided "as is" without any warranties or my support.
// Use at your own risk.
// ------------------------------[ TARGET MCU ]------------------------------
// 
// This code is intended to run on bare metal microcontrollers without an RTOS or any OS.
// Clock speed: 32.000Hz (32kHz)
// Memory: 8KB Flash, 1KB RAM
// ------------------------------[ PROJECT ]------------------------------
// Electricity Flow Control System.
// Controls electricity flow based on temperature and voltage sensor data.
// Could be used in UPS, Power Supplies, Solar Inverters, etc.
// ------------------------------[ LICENSE ]------------------------------
// This code is licensed under the MIT License.
// ------------------------------[ INCLUDES ]------------------------------
#include "logic/logic.h"
#include "registers/registers.h"
// ------------------------------[ MAIN EVENT LOOP ]------------------------------
int main(void) {

	while (1) {
		read_sensors(&temperature, &voltage);		// Read sensor data
		cmp_sensor_data();							// Compare sensor data
		time_update();								// Update time
		electricity_flow();							// Control electricity flow

		delay_ms();									// 20 milisecond delay on 32kHz
	}
}
// ------------------------------[ END OF FILE ]------------------------------
