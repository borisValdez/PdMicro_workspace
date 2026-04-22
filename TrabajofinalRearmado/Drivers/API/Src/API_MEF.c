/**
 ******************************************************************************
 * @file    API_MEF.c
 * @author  Boris
 * @brief   Implementación de la Máquina de Estados Finitos (MEF).
 * Gestiona el ciclo de vida del sistema: lectura de sensores, procesamiento de la interfaz LCD.
 ******************************************************************************
 */

#include <API_IMU.h>
#include <API_LCD.h>
#include <API_MEF.h>
#include "API_delay.h"
#include <stdio.h>
#include <math.h>

/* Estado actual de la MEF */
static MEFState_t state;

/* Estructura para almacenar datos crudos del sensor */
static imu_raw_data_t imu;

/* Variables de cálculo */
static float pitch;      //!< Ángulo de inclinación calculado
static char buffer[32];  //!< Buffer para formateo de texto del LCD

/* Timer no bloqueante*/
static delay_t mef_delay;

/**
 * @brief  Inicializa la MEF en el estado de reposo y configura el timer.
 */
void MEF_init(void)
{
    state = IDLE;
    delayInit(&mef_delay, 200);
}

/**
 Actualización de la lógica de estados.
 */
void MEF_update(void)
{
    switch(state)
    {
        /* Espera activa no bloqueante */
        case IDLE:
            if (delayRead(&mef_delay))
            {
                state = RECEIVING;
            }
            break;

        /* Adquisición de datos mediante bus SPI */
        case RECEIVING:
            if (imu_read_raw(&imu) == 0)
            {
                state = PROCESS;
            }
            else
            {
                // Si la comunicación falla va a error
                state = MEF_ERROR;
            }
            break;

        /* Procesamiento matemático de los datos crudos */
        case PROCESS:


            pitch = atan2f((float)imu.ax,
                           sqrtf((float)(imu.ay * imu.ay + imu.az * imu.az)))
                           * 180.0f / 3.14159f;

            state = EXEC;
            break;

        /* Actualización de la interfaz de salida (LCD) */
        case EXEC:

            sprintf(buffer, "Pitch:%6.1f             ", pitch);
            lcd_set_cursor(0, 0);
            lcd_print(buffer);

            lcd_set_cursor(1, 0);
            if (pitch > 20)
            {
                lcd_print("ALARMA             "); // Espacios extra para limpiar la línea
            }
            else if (pitch > 5)
            {
                lcd_print("INCLINADO             ");
            }
            else
            {
                lcd_print("NIVELADO              ");
            }

            state = IDLE; // Retorno a espera para cumplir el ciclo
            break;

        /* Gestión de excepciones y recuperación */
        case MEF_ERROR:
            lcd_set_cursor(0, 0);
            lcd_print("                 "); // Limpieza de fila
            lcd_set_cursor(0, 0);
            lcd_print("IMU ERROR");

            /** * Intento de recuperación: Se reinicializa el sensor por si
             * el error fue causado por un colapso del bus SPI.
             */
            imu_init();
            state = IDLE;
            break;

        default:
            state = IDLE;
            break;
    }
}
