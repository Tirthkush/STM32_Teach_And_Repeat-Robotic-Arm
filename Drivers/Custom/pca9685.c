/*
 * pca9685.c
 *
 *  Created on: 12-Jul-2026
 *      Author: Tirth Kushwaha
 */

#include "pca9685.h"

static I2C_HandleTypeDef *pca_i2c;

/* Private Functions ---------------------------------------------------------*/

static void WriteReg(uint8_t reg, uint8_t data)
{
    HAL_I2C_Mem_Write(pca_i2c, PCA9685_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
}

static uint8_t ReadReg(uint8_t reg)
{
    uint8_t data;
    HAL_I2C_Mem_Read(pca_i2c, PCA9685_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    return data;
}

/* Public Functions ----------------------------------------------------------*/

void PCA9685_Init(I2C_HandleTypeDef *hi2c)
{
    pca_i2c = hi2c;

    PCA9685_Reset();
    PCA9685_SetPWMFreq(50); // Default 50Hz for standard servos
}

void PCA9685_Reset(void)
{
    WriteReg(MODE1, 0x00);
    HAL_Delay(10);
}

void PCA9685_SetPWMFreq(float freq)
{
    // Calculate prescale value based on 25MHz internal oscillator
    float prescale = 25000000.0f;
    prescale /= 4096.0f;
    prescale /= freq;
    prescale -= 1.0f;

    uint8_t oldmode = ReadReg(MODE1);

    // Go to sleep mode to change the prescaler
    uint8_t sleep = (oldmode & 0x7F) | 0x10;
    WriteReg(MODE1, sleep);

    // Write the rounded prescale value
    WriteReg(PRESCALE, (uint8_t)(prescale + 0.5f));

    // Restore old mode
    WriteReg(MODE1, oldmode);
    HAL_Delay(5);

    // Auto-increment enable (0xA1) and restart
    WriteReg(MODE1, oldmode | 0xA1);
}

void PCA9685_SetPWM(uint8_t channel, uint16_t on, uint16_t off)
{
    uint8_t reg = LED0_ON_L + (4 * channel);
    uint8_t data[4];

    data[0] = on & 0xFF;
    data[1] = (on >> 8) & 0xFF;
    data[2] = off & 0xFF;
    data[3] = (off >> 8) & 0xFF;

    HAL_I2C_Mem_Write(pca_i2c, PCA9685_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, data, 4, 100);
}

void PCA9685_SetServoAngle(uint8_t channel, float angle)
{
    // Constrain angle between 0 and 180 degrees
    if (angle < 0.0f)   angle = 0.0f;
    if (angle > 180.0f) angle = 180.0f;

    // Map the angle to the pulse length (SERVO_MIN to SERVO_MAX should be defined in pca9685.h)
    uint16_t pulse = SERVO_MIN + (uint16_t)(((SERVO_MAX - SERVO_MIN) * angle) / 180.0f);

    PCA9685_SetPWM(channel, 0, pulse);
}
