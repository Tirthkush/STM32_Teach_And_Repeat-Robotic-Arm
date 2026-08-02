# STM32 Teach-and-Repeat Robotic Arm

A 4-DOF Teach-and-Repeat Robotic Arm built using the **STM32F103C8T6 (Blue Pill)**. The system enables real-time control of four servo motors using dual analog joysticks, records arm movements into an external EEPROM, and autonomously replays the recorded motion using capacitive touch sensors.

This project demonstrates embedded firmware development by integrating multiple STM32 peripherals including **ADC with DMA**, **I²C**, **UART**, **GPIO**, and custom-developed drivers for the **PCA9685 PWM Servo Driver** and **24LC256 EEPROM**.

---

# Project Overview

The robotic arm operates in two different modes:

### Manual Mode

The arm is controlled in real time using two analog joysticks. STM32 continuously samples joystick positions using ADC with DMA and updates the servo positions through the PCA9685 PWM controller.

### Teach-and-Repeat Mode

The complete movement of the robotic arm is recorded into an external EEPROM. A playback command reproduces the recorded sequence automatically without requiring joystick input.

---

# Firmware Architecture

The firmware follows a **modular driver-based architecture**, separating hardware-specific functions from the application logic.

Two reusable custom device drivers were developed for this project.

## PCA9685 Driver

A custom driver was written to communicate with the PCA9685 PWM controller over the I²C bus.

Functions provided include:

* Device Initialization
* PWM Frequency Configuration
* Individual PWM Channel Control
* Servo Angle Mapping
* Multi-Servo Control

---

## EEPROM Driver

A custom driver was developed for the external 24LC256 EEPROM using the I²C protocol.

Functions provided include:

* EEPROM Initialization
* Single Byte Read
* Single Byte Write
* Buffer Read
* Buffer Write
* Motion Frame Storage
* Motion Frame Retrieval

---

The application layer communicates only with these drivers, making the firmware modular, reusable, and easier to maintain.

---

# Features

* 4-DOF Servo Motor Control
* Dual Analog Joystick Interface
* ADC with DMA for Continuous Sampling
* PCA9685 16-Channel PWM Driver
* External 24LC256 EEPROM Storage
* Teach-and-Repeat Motion Recording
* Capacitive Touch Record & Playback
* UART Debugging Interface
* Custom PCA9685 Driver Development
* Custom EEPROM Driver Development
* Modular Driver-Based Firmware Architecture
* Clean Separation of Driver and Application Layers

---

# Hardware Used

| Component                      | Quantity |
| ------------------------------ | -------: |
| STM32F103C8T6 Blue Pill        |        1 |
| PCA9685 PWM Driver             |        1 |
| 24LC256 I²C EEPROM             |        1 |
| Analog Joystick Module         |        2 |
| SG90 / MG90S Servo Motor       |        4 |
| TTP223 Capacitive Touch Sensor |        2 |
| External 5V Power Supply       |        1 |
| ST-Link V2 Programmer          |        1 |

---

# Software & Development Tools

* STM32CubeIDE
* STM32CubeMX
* STM32CubeProgrammer
* STM32 HAL Library
* Embedded C

---

# Working Principle

1. Two analog joysticks generate analog voltages corresponding to the desired arm position.

2. STM32 continuously samples all joystick channels using **ADC with DMA**, reducing CPU overhead.

3. ADC values are converted into servo angles ranging from **0° to 180°**.

4. The STM32 communicates with the PCA9685 over the I²C bus, which generates accurate PWM signals for four servo motors.

5. During recording mode, every set of servo angles is stored sequentially inside the external EEPROM.

6. During playback mode, the stored motion frames are read from EEPROM and reproduced automatically to mimic the recorded movement.

---

# STM32 Peripherals Used

| Peripheral | Purpose                             |
| ---------- | ----------------------------------- |
| ADC1       | Analog Joystick Input               |
| DMA        | Continuous ADC Transfer             |
| I²C1       | Communication with PCA9685 & EEPROM |
| UART1      | Serial Debugging                    |
| GPIO       | Touch Sensor Inputs                 |

---



# Demonstration

### Manual Control

* Move the joysticks
* STM32 reads joystick positions using ADC with DMA
* Servo motors respond instantly

### Recording

* Touch the Record Sensor
* Servo positions are continuously stored inside EEPROM

### Playback

* Touch the Playback Sensor
* STM32 reads the recorded frames from EEPROM
* The robotic arm reproduces the complete recorded motion

---

# Future Improvements

* Adjustable Playback Speed
* OLED Display Interface
* Inverse Kinematics
* Bluetooth/Wi-Fi Remote Control
* Mobile Application Support

---

# Learning Outcomes

This project strengthened practical knowledge in:

* Embedded C Programming
* STM32 HAL Driver Development
* Custom Embedded Driver Development
* Modular Firmware Architecture
* ADC with DMA
* I²C Communication
* UART Debugging
* External EEPROM Interfacing
* PWM Servo Control
* Motion Recording Algorithms
* Embedded System Integration

---

# Author

**Tirth**

Electronics & Telecommunication Engineering

Embedded Systems • IoT • Robotics • Firmware Development

---

