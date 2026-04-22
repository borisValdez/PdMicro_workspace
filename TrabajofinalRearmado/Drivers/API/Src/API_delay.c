/**
 ******************************************************************************
 * @file    API_delay.c
 * @author  Boris
 * @brief   Implementación de retardos no bloqueantes basados en el SysTick.
 ******************************************************************************
 */

#include "API_delay.h"
#include "stm32f4xx_hal.h"

// Inicializa una estructura de retardo.

void delayInit( delay_t * delay, tick_t duration ){
    // Se valida que el puntero no sea nulo antes de operar
    if(delay != NULL) {
        delay->duration = duration;
        delay->running = false;
    }
}


// Realiza el seguimiento del tiempo transcurrido

bool_t delayRead( delay_t * delay ){
    // Verificación de seguridad del puntero
    if(delay == NULL) return false;

    // Si el retardo no está activo, se captura el tiempo actual
    if( delay->running == false ){
        delay->startTime = HAL_GetTick();
        delay->running = true;
    }

    // Comprobación de tiempo transcurrido
    if( (HAL_GetTick() - delay->startTime) >= delay->duration ){
        delay->running = false;
        return true;
    }

    return false;
}

//Permite cambiar la duración de un retardo existente

void delayWrite( delay_t * delay, tick_t duration ){
    if(delay != NULL) {
        delay->duration = duration;
    }
}


 //Consulta si el retardo está actualmente en ejecución

bool_t delayIsRunning(delay_t * delay){
    if(delay == NULL) return false;
    return delay->running;
}
