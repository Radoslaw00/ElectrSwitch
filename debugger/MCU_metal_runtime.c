// ------------------------------[ MCU_metal_runtime.c ]------------------------------
// This file acts as a debugger and test board for the ElectrSwitch system.
// It simulates the hardware environment, provides inputs, and verifies outputs.
//
// ------------------------------[ BUILD & RUN ]------------------------------
// Since logic.c has a compilation error (missing arguments), use this PowerShell command:
// (Get-Content src/logic/logic.c) -replace 'compare_sensor_data\(\);', 'compare_sensor_data(temperature, voltage);' | Set-Content debugger/logic_fixed.c; gcc debugger/MCU_metal_runtime.c debugger/logic_fixed.c src/registers/registers.c -I src/logic -I src/registers -o debugger/mcu_runtime.exe; if (Test-Path debugger/mcu_runtime.exe) { ./debugger/mcu_runtime.exe }
// ------------------------------[ INCLUDES ]------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// Include project headers
#include "logic.h"
#include "registers.h"

// ------------------------------[ COLORS ]------------------------------
#define COL_RESET   "\033[0m"
#define COL_RED     "\033[31m"
#define COL_GREEN   "\033[32m"
#define COL_YELLOW  "\033[33m"
#define COL_CYAN    "\033[36m"

// ------------------------------[ HELPER FUNCTIONS ]------------------------------

// Simulate one full cycle of the main loop (excluding infinite loop and delay)
void run_mcu_cycle(void) {
    read_sensors();
    cmp_sensor_data(); 
    time_update();
    electricity_flow();
    // delay_ms() is skipped to run tests instantly
}

// Print the current state of the "Board"
void print_board_state(const char* scenario_name) {
    printf("------------------------------------------------------------\n");
    printf("SCENARIO: %s%s%s\n", COL_CYAN, scenario_name, COL_RESET);
    
    // Read simulated registers to see what the inputs are
    int16_t t, v;
    read_simulated_registers(&t, &v);
    
    printf("INPUTS  | Temp: %d C | Volt: %d V\n", t, v);
    
    // Check Outputs
    bool power_cut = (REG_CONTROL & 0x02);
    
    printf("OUTPUTS | Emergency Flag: [%s%s%s] | Power Flow: [%s%s%s]\n", 
        emergency_flag ? COL_RED : COL_GREEN, 
        emergency_flag ? "TRUE" : "FALSE", 
        COL_RESET,
        power_cut ? COL_RED : COL_GREEN,
        power_cut ? "CUTOFF" : "NORMAL",
        COL_RESET
    );
    
    // Internal Hysteresis State (Inferred from output behavior)
    if (emergency_flag) {
        printf("STATUS  | %sSYSTEM TRIP%s\n", COL_RED, COL_RESET);
    } else {
        printf("STATUS  | %sSYSTEM OK%s\n", COL_GREEN, COL_RESET);
    }
    printf("------------------------------------------------------------\n\n");
}

// ------------------------------[ TEST SCENARIOS ]------------------------------

void test_normal_operation(void) {
    printf("Running: Normal Operation Test...\n");
    set_simulated_registers(25, 220); // 25C, 220V (Safe)
    run_mcu_cycle();
    print_board_state("Normal Operation");
}

void test_overheat_trip(void) {
    printf("Running: Overheat Trip Test...\n");
    set_simulated_registers(125, 220); // 125C (Over 120 limit), 220V
    run_mcu_cycle();
    print_board_state("Overheat Trip (>120C)");
}

void test_overheat_hysteresis(void) {
    printf("Running: Overheat Hysteresis Test...\n");
    // First trip it
    set_simulated_registers(125, 220);
    run_mcu_cycle();
    
    // Now lower it to 118C (Should still be tripped because clear is 115C)
    // BUG REPRODUCTION: Since the user rejected the hysteresis fix, this will likely CLEAR the fault incorrectly.
    set_simulated_registers(118, 220);
    run_mcu_cycle();
    
    print_board_state("Overheat Hysteresis (118C - Expecting TRIP, might CLEAR due to bug)");
}

void test_overheat_clear(void) {
    printf("Running: Overheat Clear Test...\n");
    // Drop below 115C
    set_simulated_registers(110, 220);
    run_mcu_cycle();
    print_board_state("Overheat Clear (<115C)");
}

void test_undervoltage_trip(void) {
    printf("Running: Undervoltage Trip Test...\n");
    set_simulated_registers(25, 170); // 170V (Under 180 limit)
    run_mcu_cycle();
    print_board_state("Undervoltage Trip (<180V)");
}

void test_undervoltage_hysteresis(void) {
    printf("Running: Undervoltage Hysteresis Test...\n");
    // First trip
    set_simulated_registers(25, 170);
    run_mcu_cycle();
    
    // Rise to 182V (Should still be tripped because clear is 185V)
    set_simulated_registers(25, 182);
    run_mcu_cycle();
    
    print_board_state("Undervoltage Hysteresis (182V - Expecting TRIP, might CLEAR due to bug)");
}

// ------------------------------[ MAIN ]------------------------------
int main(void) {
    printf("============================================================\n");
    printf("      ELECTRSWITCH DEBUGGER & TEST BOARD SIMULATION\n");
    printf("============================================================\n\n");

    test_normal_operation();
    test_overheat_trip();
    test_overheat_hysteresis();
    test_overheat_clear();
    test_undervoltage_trip();
    test_undervoltage_hysteresis();

    printf("============================================================\n");
    printf("      TEST SEQUENCE COMPLETED\n");
    printf("============================================================\n");
    
    return 0;
}
// ------------------------------[ END OF FILE ]------------------------------
