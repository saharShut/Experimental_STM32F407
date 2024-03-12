

#include "lm75a.h"

static I2C_HandleTypeDef I2C_PORT;

I2C_HandleTypeDef* LM75A_Init(void)
{
    // I2C_PORT = port;

    // if(I2C_PORT->Instance == I2C3)
    // {
    //     uint8_t reg = TEMP;
    //     // Preset temp pointer
    //     if(HAL_I2C_Mem_Write(I2C_PORT,(uint16_t) LM75A_ADDRESS, (uint16_t)TEMP, I2C_MEMADD_SIZE_8BIT, &reg, 1, 555) != HAL_OK)
    //     {
    //         return 16;
    //     }
    //     return 1;
    // }

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**I2C3 GPIO Configuration
    PC9     ------> I2C3_SDA
    PA8     ------> I2C3_SCL
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C3_CLK_ENABLE();

    I2C_PORT.Instance = I2C3;
    I2C_PORT.Init.ClockSpeed = 100000;
    I2C_PORT.Init.DutyCycle = I2C_DUTYCYCLE_2;
    I2C_PORT.Init.OwnAddress1 = 0;
    I2C_PORT.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    I2C_PORT.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    I2C_PORT.Init.OwnAddress2 = 0;
    I2C_PORT.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    I2C_PORT.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if(HAL_I2C_Init(&I2C_PORT) == HAL_ERROR)
    {
        return NULL;
    }

    uint8_t reg = TEMP;
    // Preset temp pointer
    if(HAL_I2C_Mem_Write(&I2C_PORT,(uint16_t) LM75A_ADDRESS, (uint16_t)TEMP, I2C_MEMADD_SIZE_8BIT, &reg, 1, 10) != HAL_OK)
    {
        return NULL;
    }

    return &I2C_PORT;
}


int8_t LM75A_Read_Temp(const uint16_t* data)
{
    if(HAL_I2C_Mem_Read(&I2C_PORT, (uint16_t) LM75A_ADDRESS, (uint16_t)TEMP, I2C_MEMADD_SIZE_8BIT, (uint8_t*)data, 2, 10) != HAL_OK)
    {
        return -1;
    }
    return 0;
}

