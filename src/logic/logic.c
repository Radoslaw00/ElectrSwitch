#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "logic.h"

#define MAX_TMP 120
#define MIN_TMP -40
#define MAX_VOL 240
#define MIN_VOL 180

static uint32_t seconds = 0;
static uint32_t tick_20ms = 0;

volatile bool emergency_flag = false;
volatile bool over_temp = false;
volatile bool over_voltage = false;

bool temp_range(int16_t temp) {
	return (temp >= MIN_TMP && temp <= MAX_TMP);
}
bool volt_range(int16_t volt) {
	return (volt >= MIN_VOL && volt <= MAX_VOL);
}

//Compare sensor data
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

//Time
void time(void)
{
	tick_20ms++;

	if (tick_20ms >= 50) {   //50*20ms = 1s
		tick_20ms = 0;
		seconds++;
	}
}


//RENAME FUNCTIONS (call all from logic.c) (ALL FOR main.c):
void date(void) {}
void read_sensors(void) {}
void cmp_sensor_data(void) {}
void emerg_override(void) {}
void time_update(void) { time(); }
void read_sensors(void) {}