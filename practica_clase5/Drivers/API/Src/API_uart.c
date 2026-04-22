#include "API_uart.h"
#include "stm32f4xx_hal.h"
#include "string.h"

extern UART_HandleTypeDef huart2;

#define UART_TIMEOUT 100


static UART_HandleTypeDef * uartHandle = &huart2;  //Puntero  interno para uart



bool_t uartInit(void)  //  Inicialización UART
{
    HAL_StatusTypeDef status;
    const char * msg = "UART initialized\r\n";

    status = HAL_UART_Transmit(
                uartHandle,
                (uint8_t*)msg,
                strlen(msg),
                UART_TIMEOUT);

    if(status != HAL_OK)
    {
        return false;
    }

    return true;
}



void uartSendString(uint8_t * pstring)  // funciòn oara enviar string
{
    if(pstring == NULL)
    {
        return;
    }

    uint16_t size = strlen((char*)pstring);

    HAL_StatusTypeDef status;

    status = HAL_UART_Transmit(
                uartHandle,
                pstring,
                size,
                UART_TIMEOUT);

    if(status != HAL_OK)
    {
        // fallò
    }
}



void uartSendStringSize(uint8_t * pstring, uint16_t size) // funciòn para enviar string y tamaño
{
    if(pstring == NULL|| size == 0 || size > 256)  // verifico que el string tenga sentido y que el tamaño no sea mayor que lo que puede enviar la uart
    {
        return;
    }

    HAL_StatusTypeDef status;

    status = HAL_UART_Transmit(
                uartHandle,
                pstring,
                size,
                UART_TIMEOUT);

    if(status != HAL_OK)   // corroboro si fallo la transmisiòn
    {
    	// fallò
    }
}


bool_t uartReceiveStringSize(uint8_t * pstring, uint16_t size)
{
    if(pstring == NULL)
        return false;

    if(size == 0 || size > 256)
        return false;

    HAL_StatusTypeDef status;

    status = HAL_UART_Receive(
                uartHandle,
                pstring,
                size,
                1);
    if(status == HAL_OK)
        return true;

    return false;
}
