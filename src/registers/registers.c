#include "registers.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

//FAKE REGSISTERS
volatile uint8_t REG_STATUS = 0x00;        //Status register
volatile uint8_t REG_CONTROL = 0x00;       //Control register
volatile uint16_t REG_DATA = 0x0000;       //Data register
volatile uint32_t REG_CONFIG = 0x00000000; //Configuration register

//Simulated sensor values
static int16_t simulated_temperature = 25; //Normal temperature
static int16_t simulated_voltage = 200;    //Normal voltage

void read_simulated_registers(int16_t* temperature, int16_t* voltage) {
	if (temperature) *temperature = simulated_temperature;
	if (voltage) *voltage = simulated_voltage;
}

void set_simulated_registers(int16_t temperature, int16_t voltage) {
	simulated_temperature = temperature;
	simulated_voltage = voltage;
}