# ⚡ ElectrSwitch: Electricity Flow Control System

**A tiny and extremely fast electricity state switches manager for a bare-metal embedded system.**

This software is a **purely theoretical** implementation designed to manage and control the flow of electricity based on real-time sensor data from temperature and voltage inputs. Its architecture is optimized for minimal resources and high-speed, direct hardware interaction.

---

## ⚠️ Disclaimer

> **THIS SOFTWARE IS PURELY THEORETICAL AND IS NOT INTENDED FOR PRACTICAL USE.**
>
> * **Non-Practical:** This code is a conceptual exercise and is **not** to be used in any real Microcontrollers (MCUs) or embedded systems for live control of electricity.
> * **As Is:** This code is provided "as is" without any warranties, express or implied, or support from the author.
> * **Risk:** Use (even for simulation/conceptual study) at your own risk. The author assumes no responsibility for any resulting damages or issues.

---

## 🎯 Target System & Constraints

This project is designed to run on a **bare-metal** microcontroller environment without an Operating System (OS) or Real-Time Operating System (RTOS), requiring highly efficient code. 

| Parameter | Specification | Note |
| :--- | :--- | :--- |
| **Architecture** | Bare Metal | No OS dependency. |
| **Clock Speed** | 32.000 Hz (32 kHz) | Extremely low frequency, demanding highly optimized C and assembly (`nop`) timing. |
| **Flash Memory** | 8 KB | Strict limitation for program size. |
| **RAM** | 1 KB | Minimal memory available for variables and stack. |

---

## 💡 Project Overview: How It Works

The **ElectrSwitch** software acts as a decision-making core, continuously monitoring analog sensor inputs to determine the safe and necessary state of the electrical flow.

### Core Functionality & Reliability

The system is built around a highly reliable, deterministic main loop: 

* **Fixed Polling Cycle:** The entire control loop executes exactly every **$20\text{ms}$** (a $50\text{Hz}$ rate). This timing is achieved using a **$640$-cycle NOP loop** rather than a hardware timer, minimizing complexity.
* **Hysteresis Logic:** The threshold checking (`cmp_sensor_data`) implements **Hysteresis** for both temperature and voltage. This prevents contact "chattering" or rapid state toggling when sensor readings fluctuate near critical limits. 
* **Fail-Safe Control:** The `electricity_flow` function relies entirely on the central **`emergency_flag`** to make a rapid decision (Cutoff or Flow), directly manipulating the `REG_CONTROL` register.

### Execution Flow (Main Loop)

The main loop (`main.c`) defines the strict $20\text{ms}$ execution sequence:

1.  **Read Inputs:** `read_sensors()` fetches current V/T data.
2.  **Process Logic:** `cmp_sensor_data()` checks thresholds and sets the `emergency_flag`.
3.  **Update Time:** `time_update()` increments the software $20\text{ms}$ tick counter.
4.  **Control Outputs:** `electricity_flow()` switches the simulated relay based on the `emergency_flag`.
5.  **Enforce Timing:** `delay_ms()` executes the mandatory $640$-cycle pause.

### Potential Applications (Theoretical)

* **Uninterruptible Power Supplies (UPS):** Managing switchover based on mains voltage stability using hysteresis.
* **Solar Inverters:** Controlling the connection of power sources based on temperature and voltage limits.
* **Power Supplies:** Implementing robust over-voltage/over-temperature protection circuits.

---

## 🛠️ Build and Dependencies

The code is intended to be compiled using a standard embedded C toolchain (e.g., GCC for ARM/AVR) and linked directly against the target MCU's register definitions (headers).

* **Compiler:** `<Specify Toolchain, e.g., arm-none-eabi-gcc>`
* **Target Device:** `<Specify MCU Series, e.g., STM32F0, PIC16F>`
* **Build System:** `<Specify Build System, e.g., Makefile, PlatformIO>`
* **Dependencies:** Bare-metal standard C libraries (`stdint.h`, etc.) and the target MCU's header file (`<MCU_HEADER_FILE>.h`).

---

## 📄 License

This code is licensed under the **MIT License**.

See the `LICENSE` file for full details.