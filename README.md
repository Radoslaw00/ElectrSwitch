# ElectrSwitch
<div class="header">
        <h1><span class="emoji">⚡</span> ElectrSwitch: Electricity Flow Control System</h1>
        <p><strong>A tiny and extremely fast electricity state switches manager for a bare-metal embedded system.</strong></p>
        <p>This software is a <strong>purely theoretical</strong> implementation designed to manage and control the flow of electricity based on real-time sensor data from temperature and voltage inputs. Its architecture is optimized for minimal resources and high-speed, direct hardware interaction.</p>
    </div>

    <h2><span class="emoji">⚠️</span> Disclaimer</h2>
    <div class="disclaimer">
        <p><strong>THIS SOFTWARE IS PURELY THEORETICAL AND IS NOT INTENDED FOR PRACTICAL USE.</strong></p>
        <ul>
            <li><strong>Non-Practical:</strong> This code is a conceptual exercise and is **not** to be used in any real Microcontrollers (MCUs) or embedded systems for live control of electricity.</li>
            <li><strong>As Is:</strong> This code is provided "as is" without any warranties, express or implied, or support from the author.</li>
            <li><strong>Risk:</strong> Use (even for simulation/conceptual study) at your own risk. The author assumes no responsibility for any resulting damages or issues.</li>
        </ul>
    </div>

    <hr>

    <h2><span class="emoji">🎯</span> Target System & Constraints</h2>
    <p>This project is designed to run on a <strong>bare-metal</strong> microcontroller environment without an Operating System (OS) or Real-Time Operating System (RTOS), requiring highly efficient code.</p>

    <table>
        <thead>
            <tr>
                <th>Parameter</th>
                <th>Specification</th>
                <th>Note</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td><strong>Architecture</strong></td>
                <td>Bare Metal</td>
                <td>No OS dependency.</td>
            </tr>
            <tr>
                <td><strong>Clock Speed</strong></td>
                <td>32.000 Hz (32 kHz)</td>
                <td>Extremely low frequency, demanding high optimization.</td>
            </tr>
            <tr>
                <td><strong>Flash Memory</strong></td>
                <td>8 KB</td>
                <td>Strict limitation for program size.</td>
            </tr>
            <tr>
                <td><strong>RAM</strong></td>
                <td>1 KB</td>
                <td>Minimal memory available for variables and stack.</td>
            </tr>
        </tbody>
    </table>

    <hr>

    <h2><span class="emoji">💡</span> Project Overview: How It Works</h2>
    <p>The <strong>ElectrSwitch</strong> software acts as a decision-making core, continuously monitoring analog sensor inputs to determine the safe and necessary state of the electrical flow.</p>

    <h3>Core Functionality</h3>
    <ul>
        <li><strong>Sensor Polling:</strong> High-speed polling of dedicated analog-to-digital converter (ADC) channels for **Voltage** and **Temperature** sensors.</li>
        <li><strong>Threshold Checking:</strong> Compares the measured values against predefined safe operating thresholds (e.g., maximum temperature, over/under-voltage limits).</li>
        <li><strong>Flow Control:</strong> Based on threshold checks, it directly manipulates General-Purpose Input/Output (GPIO) pins connected to relays or solid-state switches (the "switches manager").</li>
        <li><strong>Fail-Safe:</strong> Implements immediate cut-off logic (e.g., in assembly language or highly optimized C) in case of critical over-limit conditions.</li>
    </ul>

    <p>This is the conceptual design for the system:</p>
    

    <h3>Potential Applications (Theoretical)</h3>
    <ul>
        <li><strong>Uninterruptible Power Supplies (UPS):</strong> Managing switchover between mains and battery power based on mains voltage stability.</li>
        <li><strong>Solar Inverters:</strong> Controlling the connection of solar panels/batteries to the grid/load.</li>
        <li><strong>Power Supplies:</strong> Implementing over-voltage/over-temperature protection circuits.</li>
    </ul>

    <hr>

    <h2><span class="emoji">🛠️</span> Build and Dependencies</h2>
    <p>The code is intended to be compiled using a standard embedded C toolchain (e.g., GCC for ARM/AVR) and linked directly against the target MCU's register definitions (headers).</p>
    
    <ul>
        <li><strong>Compiler:</strong> <code>&lt;Specify Toolchain, e.g., arm-none-eabi-gcc&gt;</code></li>
        <li><strong>Target Device:</strong> <code>&lt;Specify MCU Series, e.g., STM32F0, PIC16F&gt;</code></li>
        <li><strong>Build System:</strong> <code>&lt;Specify Build System, e.g., Makefile, PlatformIO&gt;</code></li>
        <li><strong>Dependencies:</strong> Bare-metal standard C libraries (<code>stdint.h</code>, etc.) and the target MCU's header file (<code>&lt;MCU_HEADER_FILE&gt;.h</code>).</li>
    </ul>

    <hr>

    <h2><span class="emoji">📄</span> License</h2>
    <p>This code is licensed under the **MIT License**.</p>
    <p>See the <code>LICENSE</code> file for full details.</p>