/*
 * eeprom.c
 *
 *  Created on: 09-Jul-2026
 *      Author: Tirth Kushwaha
 */

#include "eeprom.h"

static I2C_HandleTypeDef *eeprom_i2c;

/*--------------------------------*/
void EEPROM_Init(I2C_HandleTypeDef *hi2c)
{
    eeprom_i2c = hi2c;
}

/*--------------------------------*/
HAL_StatusTypeDef EEPROM_WriteByte(uint16_t memAddr,uint8_t data)
{
    HAL_StatusTypeDef status;
    status = HAL_I2C_Mem_Write(eeprom_i2c,EEPROM_ADDRESS,memAddr,I2C_MEMADD_SIZE_16BIT,&data,1,100);
    HAL_Delay(5);
    return status;
}

/*--------------------------------*/
HAL_StatusTypeDef EEPROM_ReadByte(uint16_t memAddr,uint8_t *data)
{
    return HAL_I2C_Mem_Read(eeprom_i2c,EEPROM_ADDRESS,memAddr,I2C_MEMADD_SIZE_16BIT,data,1,100);
}

/*--------------------------------*/
HAL_StatusTypeDef EEPROM_WriteBuffer(uint16_t memAddr,uint8_t *buffer,uint16_t size)
{
    HAL_StatusTypeDef status;
    status = HAL_I2C_Mem_Write(eeprom_i2c,EEPROM_ADDRESS,memAddr,I2C_MEMADD_SIZE_16BIT,buffer,size,100);
    HAL_Delay(5);
    return status;
}

/*--------------------------------*/
HAL_StatusTypeDef EEPROM_ReadBuffer(uint16_t memAddr,uint8_t *buffer,uint16_t size)
{
    return HAL_I2C_Mem_Read(eeprom_i2c,EEPROM_ADDRESS,memAddr,I2C_MEMADD_SIZE_16BIT,buffer,size,100);
}
