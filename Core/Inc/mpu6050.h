#ifndef MPU6050_H
#define MPU6050_H

#include "stm32f1xx_hal.h"

typedef struct {
    int16_t Accel_X;
    int16_t Accel_Y;
    int16_t Accel_Z;
    int16_t Gyro_X;
    int16_t Gyro_Y;
    int16_t Gyro_Z;
} MPU6050_Data;

uint8_t MPU6050_Init(void);
void MPU6050_Read_Accel(MPU6050_Data *dataStruct);
void MPU6050_Read_Gyro(MPU6050_Data *dataStruct);

#endif
