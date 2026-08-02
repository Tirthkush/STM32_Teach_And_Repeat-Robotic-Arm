# Custom Drivers

This folder contains the custom drivers developed specifically for the **STM32 Teach-and-Repeat Robotic Arm** project.

## Included Drivers

### PCA9685 Driver

A custom I²C driver for the PCA9685 16-channel PWM controller.

Features:

- Initialize the PCA9685
- Configure PWM frequency (50 Hz for servo motors)
- Control servo position using angle values (0°–180°)
- Support multiple servo channels

---

### EEPROM Driver

A custom I²C driver for the external AT24Cxx EEPROM.

Features:

- Initialize EEPROM communication
- Read a single byte
- Write a single byte
- Read multiple bytes (buffer)
- Write multiple bytes (buffer)

---

## Purpose

These custom drivers provide a simple hardware abstraction layer, allowing the main application to communicate with external peripherals using easy-to-use APIs while keeping the application code clean and modular.
