# Core

This folder contains the application firmware generated using STM32CubeIDE and STM32CubeMX.

The source code in this directory is responsible for the main operation of the robotic arm, including peripheral initialization, application logic, and execution flow.

Folder Structure
Core
├── Inc
└── Src
Inc

Contains application header files.

Examples include:

main.h
Peripheral configuration headers
User-defined declarations
Src

Contains the application source files.

Examples include:

main.c
Peripheral initialization
Robotic arm control logic
Motion recording & playback logic
UART debugging
ADC data processing
Responsibilities

The application layer performs the following tasks:

Initializes STM32 peripherals
Reads joystick values using ADC with DMA
Converts ADC values into servo angles
Communicates with the PCA9685 PWM driver
Stores servo positions into EEPROM during recording
Reads stored positions during playback
Processes capacitive touch sensor inputs
Sends debugging information through UART
