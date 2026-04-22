/**
 ******************************************************************************
 * @file    API_SPI.c
 * @author  Boris
 * @brief   Capa de abstracción para el periférico SPI1.
 * Proporciona servicios de transmisión y recepción de datos.
 *  *  Created on: Apr 14, 2026
 ******************************************************************************
 */

#include "API_SPI.h"
#include "main.h"


extern SPI_HandleTypeDef hspi1; // Referencia al handle de la HAL definido en main.c

void spi_init(void) //Inicializa el periférico SPI.
{

}


int spi_tx(uint8_t *data, uint16_t len) //Envía datos
{
    HAL_StatusTypeDef status;
    if (data == NULL) return -1;   // Validación de seguridad de punteros
    status = HAL_SPI_Transmit(&hspi1, data, len, HAL_MAX_DELAY);
    if (status != HAL_OK)
    {
        return -1;
    }

    return 0;
}



int spi_rx(uint8_t *data, uint16_t len)  //Recibe datos
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


int spi_tx_rx(uint8_t *tx, uint8_t *rx, uint16_t len) //Transmisión y recepción simultánea
{
    if (tx == NULL || rx == NULL || len == 0) return -1;  // Validación de parámetros
    if (HAL_SPI_TransmitReceive(&hspi1, tx, rx, len, HAL_MAX_DELAY) != HAL_OK)
    {
        return -1;
    }
    return 0;
}
























