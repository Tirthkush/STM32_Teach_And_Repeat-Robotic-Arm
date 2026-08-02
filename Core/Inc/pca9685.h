/*
 * pca9685.h
 *
 *  Created on: 12-Jul-2026
 *      Author: Tirth Kushwaha
 */

#ifndef INC_PCA9685_H_
#define INC_PCA9685_H_

#include "stm32f1xx_hal.h"

/* PCA9685 I2C Address */
#define PCA9685_ADDRESS (0x40 << 1)

/* Registers */
#define MODE1       0x00
#define MODE2       0x01
#define PRESCALE    0xFE

#define LED0_ON_L   0x06

/* Servo Pulse Limits (50Hz) */
#define SERVO_MIN   102
#define SERVO_MID   307
#define SERVO_MAX   512

/* Functions */
void PCA9685_Init(I2C_HandleTypeDef *hi2c);

void PCA9685_Reset(void);

void PCA9685_SetPWMFreq(float freq);

void PCA9685_SetPWM(uint8_t channel,uint16_t on,uint16_t off);

void PCA9685_SetServoAngle(uint8_t channel,float angle);

#endif /* INC_PCA9685_H_ */
