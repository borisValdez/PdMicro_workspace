#ifndef __API_DEBOUNCE_H
#define __API_DEBOUNCE_H

#include <API_delay.h>


void debounceFSM_init(); // Inicializa la MEF


void debounceFSM_update(); // Actualiza la MEF



bool_t readKey();

#endif
