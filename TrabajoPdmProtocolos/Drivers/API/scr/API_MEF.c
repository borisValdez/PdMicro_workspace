/**
 ******************************************************************************
 * @file    API_MEF.c
 * @author  Boris
 * @brief   Implementación de la Máquina de Estados Finitos (MEF).
 * lectura de sensores, procesamiento y actualización de la interfaz LCD.
 *  *  Created on: Apr 14, 2026
 ******************************************************************************
 */

#include <API_IMU.h>
#include <API_LCD.h>
#include <API_MEF.h>
#include "API_delay.h"
#include <stdio.h>
#include <math.h>

/* Estado actual */
static MEFState_t state;

/* Estructura para almacenar datos crudos del sensor */
static imu_raw_data_t imu;

/* Variables de cálculo */
static float pitch;      // Ángulo de inclinación calculado
static char buffer[32];  // Buffer para formateo de texto del LCD

/* Timer no bloqueante */
static delay_t mef_delay;


//Inicializa la MEF en el estado de reposo y configura el timer.
void MEF_init(void)
{
    state = IDLE;
    delayInit(&mef_delay, 200); // 200 ms periodo
}


//Actualización de la lógica de estados.
void MEF_update(void)
{

	float abs_pitch; // Variable auxiliar para la lógica de alarma u
    switch(state)
    {

    /* ===================== */
    case IDLE:
        if (delayRead(&mef_delay))  //Espera activa no bloqueante
        {
            state = RECEIVING;
        }
        break;

    /* ===================== */
    case RECEIVING:  //Adquisición de datos mediante bus SPI

        if (imu_read_raw(&imu) == 0)
        {
            state = PROCESS;
        }
        else
        {
            state = MEF_ERROR;
        }

        break;

    /* ===================== */
    case PROCESS: // Procesamiento de calculos

        /* cálculo pitch usando acelerómetro */
        pitch = atan2f((float)imu.ax,
                       sqrtf((float)(imu.ay * imu.ay + imu.az * imu.az)))
                       * 180.0f / 3.14159f;


        state = EXEC;

        break;

    /* ===================== */
    case EXEC:   // Actualización de LCD

    	 /* Línea 0 con ancho fijo */
    		sprintf(buffer, "Pitch: %-6.1f           ", pitch);// Formateo de cadena con ancho fijo (%6.1f) para evitar restos de caracteres previos
    	    lcd_set_cursor(0,0);
    	    lcd_print(buffer);

    	    abs_pitch = fabsf(pitch);//u
    	    /* Línea 1 sin borrar */
    	    lcd_set_cursor(1,0);

    	    if (pitch > 20)
    	    {
    	        lcd_print("ALARMA           ");
    	    }
    	    else if (pitch > 5)
    	    {
    	        lcd_print("INCLINADO           ");
    	    }
    	    else
    	    {
    	        lcd_print("NIVELADO           ");
    	    }

    	    state = IDLE;  // Retorno a espera para cumplir el ciclo
    	    break;

    /* ===================== */
    case MEF_ERROR:

    	lcd_set_cursor(0,0);
    	lcd_print("           ");
    	lcd_set_cursor(0,0);
    	lcd_print("IMU ERROR           ");

    	/* intento recuperacion */
    	imu_init();

    	state = IDLE;


        break;
    }
}
