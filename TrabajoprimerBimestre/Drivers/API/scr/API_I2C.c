/**
 ******************************************************************************
 * @file    API_I2C.c
 * @author  Boris
 * @brief   Capa de abstracción para periférico I2C1.
 * Proporciona funciones de lectura y escritura simplificadas para
 * dispositivos externos.
 ******************************************************************************
 */


#include "main.h"
#include "API_I2C.h"


extern I2C_HandleTypeDef hi2c1;  // Referencia al handle de la HAL

void i2c_init(void) // Inicialización I2C
{

}


//Transmisión de datos en modo Maestro por bus I2C;addr: Dirección del dispositivo esclavo

int i2c_write(uint8_t addr, uint8_t *data, uint16_t len)
{
    HAL_StatusTypeDef status;
    if (data == NULL || len == 0) return -1; // Validación básica de parámetros
    status = HAL_I2C_Master_Transmit(&hi2c1,
                                     addr,
                                     data,
                                     len,
                                     HAL_MAX_DELAY);

    /* Verificación del estado HAL */
    if (status != HAL_OK)
    {
        return -1;
    }

    return 0;
}


//Recepción de datos en modo Maestro por bus I2C.addr: Dirección del dispositivo esclavo.

int i2c_read(uint8_t addr, uint8_t *data, uint16_t len)
{
    HAL_StatusTypeDef status;

    if (data == NULL || len == 0) return -1; // Validación de parámetros
    status = HAL_I2C_Master_Receive(&hi2c1,
                                    addr,
                                    data,
                                    len,
                                    HAL_MAX_DELAY);

    /* Verificación del estado HAL */
    if (status != HAL_OK)
    {
        return -1;  // error
    }

    return 0;       // ok
}
