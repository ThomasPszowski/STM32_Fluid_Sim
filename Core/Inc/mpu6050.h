#ifndef MPU6050_H
#define MPU6050_H

#include "stm32f1xx_hal.h"  // dostosuj do swojego układu, jeśli trzeba

#define MPU6050_I2C_ADDR     0x68 << 1  // <<1 bo HAL oczekuje 8-bitowego adresu

extern int debug_info;

typedef struct {
    int16_t Accel_X;
    int16_t Accel_Y;
    int16_t Accel_Z;
    int16_t Gyro_X;
    int16_t Gyro_Y;
    int16_t Gyro_Z;
    int16_t Temp;
} MPU6050_Data;

HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef MPU6050_ReadData(I2C_HandleTypeDef *hi2c, MPU6050_Data *data);

#endif
