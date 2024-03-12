#ifndef __SHT3X_H__
#define __SHT3X_H__

#include "stm32f4xx_hal.h"

#define SHT3X_ADDRESS 0x44
#define SHT3X_I2C_TIMEOUT 300


typedef enum {
    undefined,
    idle,
    single_shot_mode,
    periodic_mode,
} state_t;

typedef enum {
    enable,
    disable,
} clock_stretching_t;

typedef enum {
    high,
    medium,
    low,
} repeatability_t;

typedef enum {
    mps0_5,
    mps1,
    mps2,
    mps4,
    mps10,
} mps_t;

typedef enum {
    success,
    fault,
    timeout,
} result_state_t;

typedef struct 
{
    uint16_t temperature;
    uint16_t humidity;
    uint8_t temp_crc;
    uint8_t hum_crc;
    uint16_t status;
    state_t state;
    I2C_HandleTypeDef* i2c_port;
} sht3x_t;

// Initialize sht3x device
// void SHT3X_Init(sht3x_t* device);

// Initialize sht3x device and connect it to corresponding i2c_port
uint8_t SHT3X_Init(sht3x_t* device, I2C_HandleTypeDef* port);

// Cause soft reset
result_state_t reset(sht3x_t* device);

// Proccess measument 
result_state_t get_measure(sht3x_t* device);

// Set mode
result_state_t set_mode(sht3x_t* device, state_t* mode);

// Issue break command
result_state_t break_command(sht3x_t* device);

// Get device status
result_state_t get_status(sht3x_t* device);

// Convert uint16_t to float in celsius
float get_celsius_temperature(const uint16_t* temp);

// Convert uint16_t to float in faringate
float get_faringare_temperature(const uint16_t* temp);

// Convert uint16_t to humidity
float get_humidity(const uint16_t* hum);

// function from github
result_state_t sht3x_read_temperature_and_humidity(sht3x_t *handle, float *temperature, float *humidity);

#endif // __SHT3X_H__