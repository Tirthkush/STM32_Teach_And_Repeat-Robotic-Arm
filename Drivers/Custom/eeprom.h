/*
 * eeprom.h
 *
 *  Created on: 09-Jul-2026
 *      Author: Tirth Kushwaha
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "stm32f1xx_hal.h"

/* EEPROM I2C Address */
#define EEPROM_ADDRESS    (0x50 << 1)

/* Function Prototypes */
void EEPROM_Init(I2C_HandleTypeDef *hi2c);

HAL_StatusTypeDef EEPROM_WriteByte(uint16_t memAddr,uint8_t data);

HAL_StatusTypeDef EEPROM_ReadByte(uint16_t memAddr,uint8_t *data);

HAL_StatusTypeDef EEPROM_WriteBuffer(uint16_t memAddr,uint8_t *buffer,uint16_t size);

HAL_StatusTypeDef EEPROM_ReadBuffer(uint16_t memAddr,uint8_t *buffer,uint16_t size);

#endif /* INC_EEPROM_H_ */
