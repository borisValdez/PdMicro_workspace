/**
 ******************************************************************************
 * @file    API_IMU.h
 * @author  Boris
 * @brief   Interfaz para el sensor ICM-42688.
 ******************************************************************************
 */

#ifndef API_INC_API_IMU_H_
#define API_INC_API_IMU_H_

#include <stdint.h>
#define IMU_CS_LOW()  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET)
#define IMU_CS_HIGH() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET)

/**
  Estructura para almacenar datos crudos de los 6 ejes.
 */
typedef struct {
    int16_t ax, ay, az; //Aceleración en X, Y, Z
    int16_t gx, gy, gz; // Giroscopio en X, Y, Z
} imu_raw_data_t;


void imu_init(void);
int imu_read_raw(imu_raw_data_t *data);
uint8_t IMU_ReadWhoAmI(void);

#endif /* API_INC_API_IMU_H_ */
