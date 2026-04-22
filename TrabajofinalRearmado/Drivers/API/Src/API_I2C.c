/**
 ******************************************************************************
 * @file    API_I2C.c
 * @author  Boris
 * @brief   Capa de abstracción para periférico I2C1.
 * Proporciona funciones de lectura y escritura simplificadas para
 * dispositivos externos
 ******************************************************************************
 */
#include "API_I2C.h"
#include "main.h"

/* Referencia al handle de la HAL generado en main.c */
extern I2C_HandleTypeDef hi2c1;

// Inicializa el periférico I2C.
void i2c_init(void)
{
    //
}


// Transmisión de datos en modo Maestro por bus I2C.

int i2c_write(uint8_t addr, uint8_t *data, uint16_t len)
{
    HAL_StatusTypeDef status;

    // Validación parámetros
    if (data == NULL || len == 0) return -1;

    status = HAL_I2C_Master_Transmit(&hi2c1,
                                     addr,
                                     data,
                                     len,
                                     HAL_MAX_DELAY);

    /* Verificación del estado de la transacción */
    if (status != HAL_OK)
    {
        return -1;
    }

    return 0;
}

// Recepción de datos en modo Maestro por bus I2C.

int i2c_read(uint8_t addr, uint8_t *data, uint16_t len)
{
    HAL_StatusTypeDef status;

    // Validación de parámetros
    if (data == NULL || len == 0) return -1;

    status = HAL_I2C_Master_Receive(&hi2c1,
                                    addr,
                                    data,
                                    len,
                                    HAL_MAX_DELAY);

    /* Verificación del estado de la transacción */
    if (status != HAL_OK)
    {
        return -1;
    }

    return 0;
}
