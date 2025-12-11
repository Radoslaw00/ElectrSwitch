// ------------------------------[ logic.c ]------------------------------
// This file contains the core logic for the Electricity Flow Control System.
// ------------------------------[ INCLUDES ]------------------------------
#include <stdint.h>         // Library for standard integer types
#include <stdbool.h>        // Library for boolean type
#include <stddef.h>         // Library for standard definitions

#include "registers.h"      // Include simulated hardware registers header file
#include "logic.h"          // Include logic header file

#define MAX_TMP 120         // Temperature maximum limits in Celsius
#define MIN_TMP -20         // Temperature minimum limits in Celsius
#define MAX_VOL 240         // Voltage maximum limit in Volts
#define MIN_VOL 180         // Voltage minimum limit in Volts
#define MAX_CYC 640         // For delay loop
#define VOL_CLEAR_HIGH  235 // Must drop below 235V to clear over-voltage fault (was 240)
#define VOL_CLEAR_LOW   185 // Must rise above 185V to clear under-voltage fault (was 180)
#define TEMP_CLEAR_HIGH 115 // Must drop below 115C to clear over-temp fault (was 120)
#define TEMP_CLEAR_LOW  -15 // Must rise above -15C to clear under-temp fault (was -20)

static uint32_t seconds = 0;
static uint32_t tick_20ms = 0;

static int16_t temperature = 0;
static int16_t voltage = 0;

volatile bool emergency_flag = false;
volatile bool over_temp = false;
volatile bool over_voltage = false;

bool temp_range(int16_t temp) { return (temp >= MIN_TMP && temp <= MAX_TMP); }
bool volt_range(int16_t volt) {	return (volt >= MIN_VOL && volt <= MAX_VOL); }

// ===============================================================================

// -----------------------------[ READ SENSOR DATA ]-----------------------------
void read_sensor_data(void) {
	read_simulated_registers(&temperature, &voltage);
}
// ------------------------------[ FAKE REGISTERS ]----------------------------- 
//  REG_DATA = (uint16_t)voltage;   
//  REG_STATUS = 0x01;          


// ------------------------------[ COMPARE SENSOR DATA ]------------------------------
void compare_sensor_data(int16_t temperature, int16_t voltage) {
// ---------------------------[ TEMPERATURE HYSTERESIS ]-----------------------
    if (temperature > MAX_TMP) {
        // High limit: Immediate fault trip of >120C
        over_temp = true;
    }
    else if (over_temp == true && temperature < TEMP_CLEAR_HIGH) {
        // High limit fault clear: Must drop below clear limit of 115C
        over_temp = false;
    }

    else if (temperature < MIN_TMP) {
        // Low limit: Immediate fault trip of <-20C
        over_temp = true;
    }
    else if (over_temp == true && temperature > TEMP_CLEAR_LOW) {
        // Low limit fault clear: Must rise above clear limit of >-15C
        over_temp = false;
    }

// ----------------------------[ VOLTAGE HYSTERESIS ]----------------------------
    if (voltage > MAX_VOL) {
        // Over-voltage trip of >240V
        over_voltage = true;
    }
    else if (over_voltage == true && voltage < VOL_CLEAR_HIGH) {
        // Over-voltage fault clear of <235V
        over_voltage = false;
    }

    else if (voltage < MIN_VOL) {
        // Under-voltage trip of <180V
        over_voltage = true;
    }
    else if (over_voltage == true && voltage > VOL_CLEAR_LOW) {
        // Under-voltage fault clear of >185V
        over_voltage = false;
    }
// -------------------------[ EMERGENCY FLAG UPDATE ]-------------------------
    if (over_temp || over_voltage) {
        emergency_flag = true;
    }
    else {
        emergency_flag = false;
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
void read_sensors(void)			{ read_sensor_data();		}
void cmp_sensor_data(void)		{ compare_sensor_data(temperature, voltage);	}
void electricity_flow(void)		{ elec_ctrl_flow_output();  }
void time_update(void)			{ time();					}
void delay_ms(uint32_t count)	{ del();					}
// ------------------------------[ END OF FILE ]------------------------------
