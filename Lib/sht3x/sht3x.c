#include "sht3x.h"


static I2C_HandleTypeDef* I2C_PORT;

// Its code for example initialization i2c port must be 
// provide anyware alse
//
// void SHT3X_Init(I2C_HandleTypeDef* port)
// {
//     I2C_PORT = port;

//     if(I2C_PORT->Instance == I2C3)
//     {
//         return 1;
//     }

//     GPIO_InitTypeDef GPIO_InitStruct = {0};

//     __HAL_RCC_GPIOC_CLK_ENABLE();
//     __HAL_RCC_GPIOA_CLK_ENABLE();
//     /**I2C3 GPIO Configuration
//     PC9     ------> I2C3_SDA
//     PA8     ------> I2C3_SCL
//     */
//     GPIO_InitStruct.Pin = GPIO_PIN_9;
//     GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//     GPIO_InitStruct.Pull = GPIO_PULLUP;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//     GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
//     HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//     GPIO_InitStruct.Pin = GPIO_PIN_8;
//     GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//     GPIO_InitStruct.Pull = GPIO_PULLUP;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//     GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//     /* Peripheral clock enable */
//     __HAL_RCC_I2C3_CLK_ENABLE();

//     I2C_PORT->Instance = I2C3;
//     I2C_PORT->Init.ClockSpeed = 100000;
//     I2C_PORT->Init.DutyCycle = I2C_DUTYCYCLE_2;
//     I2C_PORT->Init.OwnAddress1 = 0;
//     I2C_PORT->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//     I2C_PORT->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//     I2C_PORT->Init.OwnAddress2 = 0;
//     I2C_PORT->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//     I2C_PORT->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//     if(HAL_I2C_Init(I2C_PORT) == HAL_ERROR)
//     {
//         return 8;
//     }
// }

/**
 * Registers addresses.
 */
typedef enum
{
	SHT3X_COMMAND_MEASURE_HIGHREP_STRETCH = 0x2c06,
	SHT3X_COMMAND_CLEAR_STATUS = 0x3041,
	SHT3X_COMMAND_SOFT_RESET = 0x30A2,
	SHT3X_COMMAND_HEATER_ENABLE = 0x306d,
	SHT3X_COMMAND_HEATER_DISABLE = 0x3066,
	SHT3X_COMMAND_READ_STATUS = 0xf32d,
	SHT3X_COMMAND_FETCH_DATA = 0xe000,
	SHT3X_COMMAND_MEASURE_HIGHREP_10HZ = 0x2737,
	SHT3X_COMMAND_MEASURE_LOWREP_10HZ = 0x272a
} sht3x_command_t;


static uint8_t calculate_crc(const uint8_t *data, size_t length)
{
	uint8_t crc = 0xff;
	for (size_t i = 0; i < length; i++) {
		crc ^= data[i];
		for (size_t j = 0; j < 8; j++) {
			if ((crc & 0x80u) != 0) {
				crc = (uint8_t)((uint8_t)(crc << 1u) ^ 0x31u);
			} else {
				crc <<= 1u;
			}
		}
	}
	return crc;
}

static uint8_t sht3x_send_command(sht3x_t *handle, sht3x_command_t command)
{
	uint8_t command_buffer[2] = {(command & 0xff00u) >> 8u, command & 0xffu};

	if (HAL_I2C_Master_Transmit(handle->i2c_port, SHT3X_ADDRESS << 1u, command_buffer, sizeof(command_buffer),
	                            SHT3X_I2C_TIMEOUT) != HAL_OK) {
		return 0;
	}

	return 1;
}

static uint16_t uint8_to_uint16(uint8_t msb, uint8_t lsb)
{
	return (uint16_t)((uint16_t)msb << 8u) | lsb;
}

// Initialize sht3x device
// void SHT3X_Init(sht3x_t* device)
// {
//     device->state = undefined;
// }

// Initialize sht3x device and connect it to corresponding i2c_port
uint8_t SHT3X_Init(sht3x_t* device, I2C_HandleTypeDef* port)
{
    device->i2c_port = port;
    device->state = idle;

    uint8_t status_reg_and_checksum[3];
	if (HAL_I2C_Mem_Read(device->i2c_port, SHT3X_ADDRESS << 1u, SHT3X_COMMAND_READ_STATUS, 2, (uint8_t*)status_reg_and_checksum,
					  sizeof(status_reg_and_checksum), SHT3X_I2C_TIMEOUT) != HAL_OK) {
		return 0;
	}

	uint8_t calculated_crc = calculate_crc(status_reg_and_checksum, 2);

	if (calculated_crc != status_reg_and_checksum[2]) {
		return 2;
	}

	return 1;
}

// Cause soft reset
result_state_t reset(sht3x_t* device)
{}

// Proccess measument 
result_state_t get_measure(sht3x_t* device)
{}

// Set mode
result_state_t set_mode(sht3x_t* device, state_t* mode)
{}

// Issue break command
result_state_t break_command(sht3x_t* device)
{}

// Get device status
result_state_t get_status(sht3x_t* device)
{}


result_state_t sht3x_read_temperature_and_humidity(sht3x_t *handle, float *temperature, float *humidity)
{
	sht3x_send_command(handle, SHT3X_COMMAND_MEASURE_HIGHREP_STRETCH);

	HAL_Delay(1);

	uint8_t buffer[6];
	if (HAL_I2C_Master_Receive(handle->i2c_port, SHT3X_ADDRESS << 1u, buffer, sizeof(buffer), SHT3X_I2C_TIMEOUT) != HAL_OK) {
		return fault;
	}

	uint8_t temperature_crc = calculate_crc(buffer, 2);
	uint8_t humidity_crc = calculate_crc(buffer + 3, 2);
	if (temperature_crc != buffer[2] || humidity_crc != buffer[5]) {
		return fault;
	}

	uint16_t temperature_raw = uint8_to_uint16(buffer[0], buffer[1]);
	uint16_t humidity_raw = uint8_to_uint16(buffer[3], buffer[4]);

	*temperature = -45.0f + 175.0f * (float)temperature_raw / 65535.0f;
	*humidity = 100.0f * (float)humidity_raw / 65535.0f;

	return success;
}

// Convert uint16_t to float in celsius
float get_celsius_temperature(const uint16_t* temp)
{
    return 175.0 * (*temp / 0xFFFF) - 45;
}

// Convert uint16_t to float in faringate
float get_faringare_temperature(const uint16_t* temp)
{
    return 315.0 * (*temp / 0xFFFF) - 49;
}

// Convert uint16_t to humidity
float get_humidity(const uint16_t* hum)
{
    return 100.0 * (*(hum) / 0xFFFF);
}