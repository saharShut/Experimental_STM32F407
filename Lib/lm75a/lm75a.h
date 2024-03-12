#ifndef __LM75A_H__
#define __LM75A_H__


#include "stm32f4xx_hal.h"
/**
 * use PC9 - i2c3_sda
 * use PA8 - i2c3_scl
*/

#define LM75A_ADDRESS 0x90

/////////////////////////////
// Register list
/////////////////////////////
#define CONF 0x01
#define TEMP 0x00
#define TOS  0x03
#define THYST 0x02

I2C_HandleTypeDef* LM75A_Init(void);

int8_t LM75A_Read_Temp(const uint16_t* data);

#endif    //__LM75A_H__