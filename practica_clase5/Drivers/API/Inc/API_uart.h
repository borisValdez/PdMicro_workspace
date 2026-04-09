
#ifndef API_UART_H
#define API_UART_H


#include <stdint.h>
#include <stdbool.h>

typedef bool bool_t;


bool_t uartInit(void);
void uartSendString(uint8_t * pstring);
void uartSendStringSize(uint8_t * pstring, uint16_t size);
bool_t uartReceiveStringSize(uint8_t * pstring, uint16_t size);


#endif
