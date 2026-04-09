#include "API_debounce.h"
#include "stm32f4xx_hal.h"

#define DEBOUNCE_TIME_MS 40 // tiempo de estabilizaciòn del botòn

typedef enum {
	BUTTON_UP, BUTTON_FALLING, BUTTON_DOWN, BUTTON_RAISING,
} debounceState_t; // estados posibles de la maquina de estados

static debounceState_t state = { }; // Guarda el estado actual de la MEF
static bool_t button_down; // Flag de pulsaciòn
static delay_t delay = { 0 }; // Delay no bloqueante

static void button_released() {
	button_down = false;
	state = BUTTON_UP;
} // Confirma que el botòn no esta pulsado

static void button_pressed() {
	button_down = true;
	state = BUTTON_DOWN;
} // Confirma que el boton se presionò

bool_t readKey() {
	if (button_down) {
		button_down = false;
		return true;
	} // Si el botòn se presiono pone la variable button_down en cero y devuelve true

	return button_down;
}

void debounceFSM_init() {
	button_down = false;
	state = BUTTON_UP;
	delayInit(&delay, DEBOUNCE_TIME_MS);
} // Inicializa, pone el estado en button_up y le pasa los 40ms al delay nobloqueante


void debounceFSM_update() {
	bool pressed = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET;
	switch (state) {
	case BUTTON_UP:
		if (pressed) {
			state = BUTTON_FALLING;
		}
		break;
	case BUTTON_FALLING:
		if (delayRead(&delay)) {
			// Debounce time elapsed, stable read
			if (pressed) {
				button_pressed();
			} else {
				state = BUTTON_UP;
			}
		}
		break;
	case BUTTON_DOWN:
		if (!pressed) {
			state = BUTTON_RAISING;
		}
		break;
	case BUTTON_RAISING:
		if (delayRead(&delay)) {
			// Debounce time elapsed, stable read
			if (pressed) {
				state = BUTTON_DOWN;
			} else {
				button_released();
			}
		}
		break;
	default:
		// TODO: error handling
		break;
	}
} // MEF: Lee el boton, y ejecuta la MEF segùn diagrama solicitado.
