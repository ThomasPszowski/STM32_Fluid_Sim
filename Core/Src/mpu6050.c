#include "mpu6050.h"
#include "math.h"
#include "string.h"

#define MPU6050_ADDR 0x68 << 1  // Adres z przesunięciem dla HAL (7-bit + 1-bit RW)
#define WHO_AM_I_REG 0x75
#define PWR_MGMT_1   0x6B
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H  0x43

extern I2C_HandleTypeDef hi2c1;

uint8_t MPU6050_Init(void) {
    uint8_t check;
    uint8_t data;

    // Sprawdź ID urządzenia
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, WHO_AM_I_REG, 1, &check, 1, 1000);
    if (check != 0x68) return 1; // Nie znaleziono MPU6050

    // Wybudź czujnik (ustaw 0 w rejestrze zasilania)
    data = 0;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, PWR_MGMT_1, 1, &data, 1, 1000);

    return 0; // OK
}

void MPU6050_Read_Accel(MPU6050_Data *dataStruct) {
    uint8_t rawData[6];
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, ACCEL_XOUT_H, 1, rawData, 6, 1000);

    dataStruct->Accel_X = (int16_t)(rawData[0] << 8 | rawData[1]);
    dataStruct->Accel_Y = (int16_t)(rawData[2] << 8 | rawData[3]);
    dataStruct->Accel_Z = (int16_t)(rawData[4] << 8 | rawData[5]);
}

void MPU6050_Read_Gyro(MPU6050_Data *dataStruct) {
    uint8_t rawData[6];
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, GYRO_XOUT_H, 1, rawData, 6, 1000);

    dataStruct->Gyro_X = (int16_t)(rawData[0] << 8 | rawData[1]);
    dataStruct->Gyro_Y = (int16_t)(rawData[2] << 8 | rawData[3]);
    dataStruct->Gyro_Z = (int16_t)(rawData[4] << 8 | rawData[5]);
}
