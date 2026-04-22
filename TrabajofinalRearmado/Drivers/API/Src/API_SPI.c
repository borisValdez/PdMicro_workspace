/**
 ******************************************************************************
 * @file    API_SPI.c
 * @author  Boris
 * @brief   Capa de abstracción para el periférico SPI1.
 * Proporciona servicios de transmisión y recepción de datos.
 ******************************************************************************
 */

#include "API_SPI.h"
#include "main.h"

/* Referencia al handle de la HAL definido en main.c */
extern SPI_HandleTypeDef hspi1;

/**
 * Inicializa el periférico SPI.
 */
void spi_init(void)
{
    //
}

/**
 * Envía datos de forma unidireccional.
 */
int spi_tx(uint8_t *data, uint16_t len)
{
    HAL_StatusTypeDef status;

    // Validación de seguridad de punteros
    if (data == NULL) return -1;

    status = HAL_SPI_Transmit(&hspi1, data, len, HAL_MAX_DELAY);

    if (status != HAL_OK)
    {
        return -1;
    }

    return 0;
}

/**
 * Recibe datos de forma unidireccional .
 */
int spi_rx(uint8_t *data, uint16_t len)
{
    HAL_StatusTypeDef status;

    if (data == NULL) return -1;

    status = HAL_SPI_Receive(&hspi1, data, len, HAL_MAX_DELAY);

    if (status != HAL_OK)
    {
        return -1;
    }

    return 0;
}

/**
 *  Transmisión y recepción simultánea .
 */
int spi_tx_rx(uint8_t *tx, uint8_t *rx, uint16_t len)
{
    // Validación de parámetros
    if (tx == NULL || rx == NULL || len == 0) return -1;

    // Llamada a la HAL para comunicación Full Duplex
    if (HAL_SPI_TransmitReceive(&hspi1, tx, rx, len, HAL_MAX_DELAY) != HAL_OK)
    {
        return -1;
    }
    return 0;
}
