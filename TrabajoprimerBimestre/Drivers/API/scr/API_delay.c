
/**
 ******************************************************************************
 * @file    API_delay.c
 * @author  Boris
 * @brief   Implementación de retardos no bloqueantes basados en el SysTick.
 * Proporciona una alternativa a HAL_Delay para no detener la MEF.
 *  Created on: Mar 26, 2026
 ******************************************************************************
 */

#include "API_delay.h"
#include "stm32f4xx_hal.h"


//  Inicializa una estructura de retardo. delay: Puntero a la estructura de tipo delay_t.duration: Tiempo de espera en milisegundos:

void delayInit( delay_t * delay, tick_t duration ){

    if(delay != NULL) {              // Se valida que el puntero no sea nulo antes de operar
        delay->duration = duration;
        delay->running = false;
    }
}


 //Realiza el seguimiento del tiempo transcurrido:

bool_t delayRead( delay_t * delay ){

	if(delay == NULL) return false; // Verificación de seguridad del puntero
		if( delay->running == false ){  // Si el retardo no está activo, se captura el tiempo actual
			delay->startTime = HAL_GetTick();
			delay->running = true;
		}

		if( (HAL_GetTick() - delay->startTime) >= delay->duration ){  // Comprobación de tiempo transcurrido
			delay->running = false;
			return true;
		}

		return false;
}



void delayWrite( delay_t * delay, tick_t duration ){   //Permite cambiar la duración de un retardo existente.
	if(delay != NULL) {   // Verificación de seguridad del puntero
	        delay->duration = duration;
	    }
}

bool_t delayIsRunning(delay_t * delay){  //Consulta si el retardo está actualmente en ejecució
	if(delay == NULL) return false; // Verificación de seguridad del puntero
	return delay->running;
}
