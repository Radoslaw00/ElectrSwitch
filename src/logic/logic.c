#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
// DEFINED VALUES AND FLAGS above and below
#include "registers.h"
#include "logic.h"

#define MAX_TMP 120
#define MIN_TMP -20
#define MAX_VOL 240
#define MIN_VOL 180
#define MAX_DEL 1001

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
//READ SENSOR DATA
void read_sensor_data(int16_t* temperature, int16_t* voltage) {
	read_simulated_registers(&temperature, &voltage);
}

//READ SENSOR DATA
// ------------------------------[ fake registers ]------------------------------
// REG_DATA = (uint16_t)voltage;
// REG_STATUS = 0x01;
// ------------------------------------------------------------------------

//COMPARE SENSOR DATA
void compare_sensor_data(int16_t temperature, int16_t voltage) {
	if (!temp_range(temperature)) {
		over_temp = true;
	} else {
		over_temp = false;
	}
	if (!volt_range(voltage)) {
		over_voltage = true;
	} else {
		over_voltage = false;
	}
	if (over_temp || over_voltage) {
		emergency_flag = true;
	} else {
		emergency_flag = false;
	}
}

void elec_cutoff(void) {
	//REG_CONTROL |= 0x02;  // simulate cutting electricity
}
void elec_flow(void) {
	//REG_CONTROL &= ~0x02; // simulate normal electricity flow
}

void elec(void) {
	if (over_temp || over_voltage) {
		//Simulate cutting off electricity flow
		//REG_CONTROL |= 0x01; // Set bit 0 to indicate cutoff
		elec_cutoff();
	} else {
		//Simulate normal electricity flow
		//REG_CONTROL &= ~0x01; // Clear bit 0 to indicate normal flow
		elec_flow();
	}
}

//TIME UPDATE && DELAY
void del(void) {								//Clock speed 32kHz
	volatile uint32_t count = 0;
	for (count = 0; count < MAX_DEL; count++);
	count = 640;								//20ms delay at 32kHz
		while (count--) {
			__asm__("nop");
		}
}

/*
Google search explanation about 20ms at 32kHz:

To count for 20ms at 32kHz, you'll count 640 cycles
(or counts) because 32kHz means 32,000 counts per second,
and in 20 milliseconds (0.02 seconds), you just multiply the frequency
by the time: 32,000 cycles/second * 0.02 seconds = 640 counts. 
Here's the breakdown:
Convert kHz to Hz : 32 kHz = 32, 000 Hz(cycles per second).
Convert ms to seconds : 20 ms = 0.02 seconds.
Calculate counts : 32, 000 cycles / second * 0.02 seconds = 640 cycles.		*/

void time(void)
{
	tick_20ms++;

	if (tick_20ms >= 50) {   //50*20ms = 1s
		tick_20ms = 0;
		seconds++;
	}
}																		

//RENAME FUNCTIONS
void read_sensors(void)		{ read_sensor_data();		}
void cmp_sensor_data(void)	{ compare_sensor_data();	}
void electricity_flow(void)	{ elec();					}
void time_update(void)		{ time();					}
void delay_ms(uint32_t count)	{ del();					}
