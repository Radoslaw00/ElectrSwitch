#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "registers.h"
#include "logic.h"

#define MAX_TMP 120
#define MIN_TMP -20
#define MAX_VOL 240
#define MIN_VOL 180

static uint32_t seconds = 0;
static uint32_t tick_20ms = 0;

static int16_t temperature = 0;
static int16_t voltage = 0;

volatile bool emergency_flag = false;
volatile bool over_temp = false;
volatile bool over_voltage = false;

bool temp_range(int16_t temp) { return (temp >= MIN_TMP && temp <= MAX_TMP); }
bool volt_range(int16_t volt) {	return (volt >= MIN_VOL && volt <= MAX_VOL); }

//READ SENSOR DATA
void read_sensor_data(int16_t* temperature, int16_t* voltage) {
	read_simulated_registers(&temperature, &voltage);
}

//READ SENSOR DATA
// ---------------------------- fake registers ----------------------------
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

//EMERGENCY OVERRIDE
void emergency(void) {
	compare_sensor_data(temperature, voltage);
	if (emergency_flag)
}

//TIME UPDATE
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
void emerg_override(void)	{ emergency();				}
void time_update(void)		{ time();					}
