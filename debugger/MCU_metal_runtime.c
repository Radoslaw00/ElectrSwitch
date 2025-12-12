// ------------------------------[ MCU_metal_runtime.c ]------------------------------
// This file acts as a debugger and test board for the ElectrSwitch system.
// It simulates the hardware environment, provides inputs, and verifies outputs.
//
// ------------------------------[ BUILD & RUN ]------------------------------
// Since logic.c is now fixed, you can compile directly:
// gcc debugger/MCU_metal_runtime.c src/logic/logic.c src/registers/registers.c -I src/logic -I src/registers -o debugger/mcu_runtime.exe; if ($?) { ./debugger/mcu_runtime.exe }
// ------------------------------[ INCLUDES ]------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <windows.h> // For Sleep()
#include <conio.h>   // For _kbhit(), _getch()

// Include project headers
#include "logic.h"
#include "registers.h"

// ------------------------------[ COLORS ]------------------------------
#define COL_RESET   "\033[0m"
#define COL_RED     "\033[31m"
#define COL_GREEN   "\033[32m"
#define COL_YELLOW  "\033[33m"
#define COL_CYAN    "\033[36m"
#define COL_WHITE   "\033[37m"

// ------------------------------[ HELPER FUNCTIONS ]------------------------------

// Simulate one full cycle of the main loop (excluding infinite loop and delay)
void run_mcu_cycle(void) {
    read_sensors();
    cmp_sensor_data(); 
    time_update();
    electricity_flow();
}

void hide_cursor(void) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);
}

void reset_cursor(void) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};
    SetConsoleCursorPosition(hOut, coord);
}

// ------------------------------[ LIVE SIMULATION ]------------------------------

void live_simulation_mode(void) {
    int16_t sim_temp = 25;
    int16_t sim_volt = 220;
    bool running = true;
    
    // Initial Setting
    set_simulated_registers(sim_temp, sim_volt);
    
    system("cls"); // Clear screen once at start
    hide_cursor(); // Hide blinking cursor
    
    printf("Initializing Live Board Simulation...\n");
    Sleep(1000);

    while (running) {
        // 1. Check User Input (Human Interface)
        if (_kbhit()) {
            int key = _getch();
            if (key == 0xE0) { // Arrow keys match
                key = _getch();
                switch (key) {
                    case 72: sim_temp += 1; break; // UP Arrow
                    case 80: sim_temp -= 1; break; // DOWN Arrow
                    case 77: sim_volt += 5; break; // RIGHT Arrow
                    case 75: sim_volt -= 5; break; // LEFT Arrow
                }
            } else if (key == 'q' || key == 'Q') {
                running = false;
            } else if (key == 'r' || key == 'R') {
                sim_temp = 25; sim_volt = 220; // Reset
            }
            
            // Update the "Physical World"
            set_simulated_registers(sim_temp, sim_volt);
        }

        // 2. Run MCU Logic (Simulate 50Hz Cycle)
        run_mcu_cycle();

        // 3. Update Status Screen (Dashboard)
        reset_cursor(); // Move to top-left instead of clearing
        printf("============================================================\n");
        printf("      ELECTRSWITCH LIVE SIMULATION (32kHz Board Emulation)\n");
        printf("============================================================\n");
        printf("CONTROLS: [UP/DOWN] Temp (+/-1)  [LEFT/RIGHT] Volt (+/-5)  [Q] Quit\n");
        printf("------------------------------------------------------------\n");
        
        // Inputs (Use fixed width %3d to avoid artifacts)
        printf("\n   SENSORS:\n");
        printf("   [ TEMPERATURE ]  %3d C  ", sim_temp);
        if (sim_temp > 120) printf("%s(CRITICAL HIGH)%s   ", COL_RED, COL_RESET);
        else if (sim_temp < -20) printf("%s(CRITICAL LOW)%s    ", COL_RED, COL_RESET);
        else printf("%s(NORMAL)%s          ", COL_GREEN, COL_RESET);

        printf("\n   [ VOLTAGE     ]  %3d V  ", sim_volt);
        if (sim_volt > 240) printf("%s(CRITICAL HIGH)%s   ", COL_RED, COL_RESET);
        else if (sim_volt < 180) printf("%s(CRITICAL LOW)%s    ", COL_RED, COL_RESET);
        else printf("%s(NORMAL)%s          ", COL_GREEN, COL_RESET);

        // Outputs (Pad strings to overwrite previous state fully)
        bool power_cut = (REG_CONTROL & 0x02);
        printf("\n\n   SYSTEM STATUS:\n");
        printf("   [ EMERGENCY   ]  %s%s%s\n", 
            emergency_flag ? COL_RED : COL_GREEN, 
            emergency_flag ? "TRIPPED" : "SAFE   ", 
            COL_RESET);
        
        printf("   [ POWER FLOW  ]  %s%s%s\n", 
            power_cut ? COL_RED : COL_GREEN, 
            power_cut ? "CUTOFF " : "ON     ", 
            COL_RESET);

        // Warning about known bugs if they manifest
        // Example logic to detect hysteresis failure:
        // If Temp is 118C (Dangerous) but Emergency is FALSE (Logic Bug)
        if (emergency_flag == false) {
             if ((sim_temp > 115 && sim_temp <= 120) && over_temp) { // Logic specific knowledge
                 // It's hard to reliably detect "it SHOULD be tripped but isn't" without knowing previous state
                 // But we can warn if we are in the hysteresis danger zone
             }
        }
        
        printf("\n============================================================\n");

        // 4. Timing control (20ms wait = 50Hz)
        Sleep(20); 
    }
}

// ------------------------------[ TEST SCENARIOS (Batch) ]-----------------------
// Kept for regression testing
void print_board_state(const char* scenario_name) {
    int16_t t, v;
    read_simulated_registers(&t, &v);
    bool power_cut = (REG_CONTROL & 0x02);
    printf("SCENARIO: %s | In: T=%d V=%d | Out: Emer=%d Pwr=%s\n", 
        scenario_name, t, v, emergency_flag, power_cut ? "OFF" : "ON");
}

void run_batch_tests(void) {
    printf("Running Batch Tests...\n");
    // Normal
    set_simulated_registers(25, 220); run_mcu_cycle(); print_board_state("Normal");
    // Overheat
    set_simulated_registers(125, 220); run_mcu_cycle(); print_board_state("Overheat Trip");
    // Hysteresis Bug Check
    set_simulated_registers(118, 220); run_mcu_cycle(); print_board_state("Overheat Hys (118)");
    
    printf("Batch Tests Complete. Launching Live Mode...\n");
    Sleep(2000);
}

// ------------------------------[ MAIN ]------------------------------
int main(void) {
    // Optional: Run batch tests first to verify integrity
    // run_batch_tests(); 

    live_simulation_mode();
    
    return 0;
}
// ------------------------------[ END OF FILE ]------------------------------
