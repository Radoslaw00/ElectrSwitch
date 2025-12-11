#ifndef LOGIC_H
#define LOGIC_H

#include <stdint.h>
#include <stdbool.h>

void read_sensors(void);
int16_t temp_range(void);
int16_t volt_range(void);

void cmp_sensor_data(int16_t temperature, int16_t voltage);
extern volatile bool emergency_flag;

void time(void);

void electricity_flow(void);
void delay_ms(uint32_t ms);

#endif