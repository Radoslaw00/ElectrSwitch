#include "registers.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// ------------------------------[ FAKE REGISTERS ]------------------------------
volatile uint8_t REG_STATUS = 0x05;        //Status register
volatile uint8_t REG_CONTROL = 0x06;       //Control register
volatile uint16_t REG_DATA = 0x0067;       //Data register
volatile uint32_t REG_CONFIG = 0x00213769; //Configuration register

// ------------------------------ [SIMULATED REGISTERS ]------------------------------
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