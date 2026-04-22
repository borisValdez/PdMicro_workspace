/*
 * API_IMU.H
 *
 *  Created on: Apr 14, 2026
 *      Author: boris
 */

#ifndef API_IMU_H
#define API_IMU_H

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "main.h" // Esto es vital para que reconozca los nombres de arriba

#define IMU_CS_LOW()  HAL_GPIO_WritePin(IMU_CS_GPIO_Port, IMU_CS_Pin, GPIO_PIN_RESET)
#define IMU_CS_HIGH() HAL_GPIO_WritePin(IMU_CS_GPIO_Port, IMU_CS_Pin, GPIO_PIN_SET)

typedef struct {
    int16_t ax;   // aceleración eje X
    int16_t ay;   // aceleración eje Y
    int16_t az;   // aceleración eje Z
    int16_t gx;   // velocidad angular eje X
    int16_t gy;   // velocidad angular eje Y
    int16_t gz;   // velocidad angular eje Z
} imu_raw_data_t;

void imu_init(void);  // Inicia la IMU
int imu_read_raw(imu_raw_data_t *data);  //Lee datos del sensor
uint8_t IMU_ReadWhoAmI(void);

#endif
