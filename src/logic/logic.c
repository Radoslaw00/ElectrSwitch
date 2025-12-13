// ------------------------------[ logic.c ]------------------------------
// This file contains the core logic for the Electricity Flow Control System.
// ------------------------------[ INCLUDES ]------------------------------
#include <stdint.h>         // Library for standard integer types
#include <stdbool.h>        // Library for boolean type
#include <stddef.h>         // Library for standard definitions

#include "registers.h"      // Include simulated hardware registers header file
#include "logic.h"          // Include logic header file
// ------------------------------[ CONSTANTS ]------------------------------
#define MAX_TMP 120         // Temperature maximum limits in Celsius
#define MIN_TMP -20         // Temperature minimum limits in Celsius
#define MAX_VOL 240         // Voltage maximum limit in Volts
#define MIN_VOL 180         // Voltage minimum limit in Volts
#define MAX_CYC 640         // For delay loop
#define VOL_CLEAR_HIGH  235 // Must drop below 235V to clear over-voltage fault (was 240)
#define VOL_CLEAR_LOW   185 // Must rise above 185V to clear under-voltage fault (was 180)
#define TEMP_CLEAR_HIGH 115 // Must drop below 115C to clear over-temp fault (was 120)
#define TEMP_CLEAR_LOW  -15 // Must rise above -15C to clear under-temp fault (was -20)

static uint32_t seconds = 0;            // Software seconds counter
static uint32_t tick_20ms = 0;          // 20ms tick counter for timekeeping

static int16_t temperature = 0;         // Sensor data storage
static int16_t voltage = 0;             // Sensor data storage

static bool temp_high_fault = false;    // High temperature fault state
static bool temp_low_fault = false;     // Low temperature fault state
static bool volt_high_fault = false;	// High voltage fault state
static bool volt_low_fault = false;     // Low voltage fault state

volatile bool emergency_flag = false;   // Emergency flag for electricity flow control
volatile bool over_temp = false;        // Over-temperature flag
volatile bool over_voltage = false;     // Over-voltage flag
// ------------------------------[ RANGE CHECK FUNCTIONS ]------------------------------
bool temp_range(int16_t temp) { return (temp >= MIN_TMP && temp <= MAX_TMP); }
bool volt_range(int16_t volt) {	return (volt >= MIN_VOL && volt <= MAX_VOL); }
// -----------------------------[ READ SENSOR DATA ]-----------------------------
void read_sensor_data(void) {
	read_simulated_registers(&temperature, &voltage);
}
// ------------------------------[ FAKE REGISTERS ]----------------------------- 
//  REG_DATA = (uint16_t)voltage;       // Simulated voltage register
//  REG_STATUS = 0x01;                  // Simulated status register
// ------------------------------[ COMPARE SENSOR DATA ]------------------------------
void compare_sensor_data(int16_t temperature, int16_t voltage) {
// ---------------------------[ TEMPERATURE HYSTERESIS ]-----------------------
    if (temperature > MAX_TMP) {
        temp_high_fault = true;     // Trip High
    }
    else if (temperature < TEMP_CLEAR_HIGH) {
        temp_high_fault = false;    // Clear High
    }

    if (temperature < MIN_TMP) {
        temp_low_fault = true;      // Trip Low
    }
    else if (temperature > TEMP_CLEAR_LOW) {
        temp_low_fault = false;     // Clear Low
    }
// ---------------------------[ OVER TEMPERATURE FLAG ]--------------------------
    over_temp = temp_high_fault || temp_low_fault;
// ----------------------------[ VOLTAGE HYSTERESIS ]----------------------------
    if (voltage > MAX_VOL) {
        volt_high_fault = true;     // Trip High
    }
    else if (voltage < VOL_CLEAR_HIGH) {
        volt_high_fault = false;    // Clear High
    }

    if (voltage < MIN_VOL) {
        volt_low_fault = true;      // Trip Low
    }
    else if (voltage > VOL_CLEAR_LOW) {
        volt_low_fault = false;     // Clear Low
    }
// ---------------------------[ OVER VOLTAGE FLAG ]---------------------------
    over_voltage = volt_high_fault || volt_low_fault;
// -------------------------[ EMERGENCY FLAG UPDATE ]-------------------------
    if (over_temp || over_voltage) {
		emergency_flag = true;      // Set emergency flag
    }
    else {
		emergency_flag = false;     // Clear emergency flag
    }
}
// --------------------------[ ELECTRICITY FLOW CONTROL ]--------------------------
void elec_cutoff(void)  { REG_CONTROL |= 0x02;  }  //simulate cutting electricity
void elec_flow(void)    { REG_CONTROL &= ~0x02; }  //simulate normal electricity flow
// --------------------------[ ELECTRICITY FLOW OUTPUT ]--------------------------
void elec_ctrl_flow_output(void) {
	if (emergency_flag) {
        //CUTOFF LOGIC
        elec_cutoff(); 		    // Cut off electricity flow
//          REG_CONTROL |= 0x01;
//          Set bit 0 to indicate cutoff
	} else {
		//NORMAL FLOW LOGIC
        elec_flow();            // Restore electricity flow
//          REG_CONTROL &= ~0x01; 
//          Clear bit 0 to indicate normal flow
	}
}
// ------------------------------[ DELAY FUNCTION ]------------------------------
void del(void) {								//Clock speed 32kHz
	volatile uint32_t count = MAX_CYC;			//20ms delay at 32kHz - 640 cycles
	    while (count--) {                       // Decrement count
			__asm__ __volatile__("nop");        // No operation (1 cycle)
		}
}
// ------------------------------[ DELAY CALCULATION ]------------------------------
/*              <--- PORTING DELAY FUNCTION TO OTHER CLOCK SPEEDS --->

Convert kHz to Hz       : 32 kHz = 32.000 Hz (cycles per second).
Convert ms to seconds   : 20 ms = 0.02 seconds.
Calculate counts        : 32.000 cycles / second * 0.02 seconds = 640 cycles.

Do this calculation if you want to change your delay time and you must do it
if you change your hardware and it's not exacly 32.000Hz clock speed.
Changes are also required in time() function.

REMEMBER: When changing the cycle count remember to change LINE 15 in logic.c
"#define MAX_CYC 640         // For delay loop"
MAX_CYC <-- this value must be changed accordingly to your calculations.
*/

// ------------------------------[ TIME UPDATE ]------------------------------
void time(void) {               // Software clock
	tick_20ms++;                // Increment every 20 milliseconds
	if (tick_20ms >= 50) {      // 50*20 miliseconds = 1 second
		tick_20ms = 0;          // Reset 20ms tick counter
		seconds++;              // Increment seconds counter
	}
}																		
// -----------------------------[ RENAME FUNCTIONS ]-----------------------------
void read_sensors(void)			{ read_sensor_data();		                    }
void cmp_sensor_data(void)		{ compare_sensor_data(temperature, voltage);	}
void electricity_flow(void)		{ elec_ctrl_flow_output();                      }
void time_update(void)			{ time();					                    }
void delay_ms(uint32_t count)	{ del();					                    }
// --------------------------------[ END OF FILE ]-------------------------------