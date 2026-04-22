/**
 ******************************************************************************
 * @file    API_delay.h
 * @author  Boris
 * @brief   Interfaz para el manejo de retardos no bloqueantes.
 ******************************************************************************
 */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


typedef uint32_t tick_t;
typedef bool bool_t;

/**
 Estructura de control para retardos no bloqueantes.
 */
typedef struct {
   tick_t startTime; // Marca de tiempo inicial
   tick_t duration;  // Tiempo de espera en ms
   bool_t running;   // Estado del temporizador
} delay_t;

/* Prototipos de funciones */
void delayInit( delay_t * delay, tick_t duration );
bool_t delayRead( delay_t * delay );
void delayWrite( delay_t * delay, tick_t duration );
bool_t delayIsRunning(delay_t * delay);

#endif /* API_INC_API_DELAY_H_ */
