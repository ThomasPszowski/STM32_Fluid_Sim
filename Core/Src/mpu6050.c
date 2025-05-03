#include "mpu6050.h"

#define MPU6050_PWR_MGMT_1   0x6B
#define MPU6050_ACCEL_XOUT_H 0x3B

HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t data = 0;
    return HAL_I2C_Mem_Write(hi2c, MPU6050_I2C_ADDR, MPU6050_PWR_MGMT_1, 1, &data, 1, HAL_MAX_DELAY);
}
uint8_t raw[15];
HAL_StatusTypeDef MPU6050_ReadData(I2C_HandleTypeDef *hi2c, MPU6050_Data *data) {

    debug_info = 60;
    if (HAL_I2C_Mem_Read(hi2c, MPU6050_I2C_ADDR, MPU6050_ACCEL_XOUT_H, 1, raw, 14, HAL_MAX_DELAY) != HAL_OK) {
        return HAL_ERROR;
    }
    debug_info = 61;
    data->Accel_X = (int16_t)(raw[0] << 8 | raw[1]);
    data->Accel_Y = (int16_t)(raw[2] << 8 | raw[3]);
    data->Accel_Z = (int16_t)(raw[4] << 8 | raw[5]);
    data->Temp    = (int16_t)(raw[6] << 8 | raw[7]);
    data->Gyro_X  = (int16_t)(raw[8] << 8 | raw[9]);
    data->Gyro_Y  = (int16_t)(raw[10] << 8 | raw[11]);
    data->Gyro_Z  = (int16_t)(raw[12] << 8 | raw[13]);
    debug_info = 62;
    return HAL_OK;
}

