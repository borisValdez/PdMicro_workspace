/*
 * API_cmdparser.c
 *
 *  Created on: Apr 8, 2026
 *      Author: boris
 */
#include "API_cmdparser.h"
#include "API_uart.h"
#include <string.h>


typedef enum { 					// MEF SOLICITADA POR TP
    CMD_IDLE,
    CMD_RECEIVING,
    CMD_PROCESS,
    CMD_EXEC,
    CMD_ERROR
} parserState_t;


static parserState_t state = CMD_IDLE; // Variable que guarda el estado de la mef
static char cmdLine[CMD_MAX_LINE];     // Variable para ir acumulando lo que se recibe caracter a caracter
static uint8_t cmdIndex = 0;           // Este indice me indica en que posiciòn del buffer va el proximo caracter
static char * tokens[CMD_MAX_TOKENS];
static uint8_t tokenCount = 0;


static cmd_status_t parseLine(void);
static void executeCommand(void);

void ledToggle(void);
uint8_t ledStatus(void);
void ledOn(void);
void ledOff(void);

void cmdParserInit(void)    // Inicializo en IDLE, apuntando a la primer posiciòn de buffer y
{
    state = CMD_IDLE;
    cmdIndex = 0;
    cmdLine[0] = '\0';
}

void cmdPoll(void)				//Funciòn que sera llamada constantemente por main.c
{
    uint8_t rxChar; 			// variable donde se guarda el byte

    if (uartReceiveStringSize(&rxChar, 1) == true)
    {
        switch (state)
        {
            case CMD_IDLE:							// En caso IDEL

                if (rxChar != '\r' && rxChar != '\n')		// Si estoy recibiendo algo y aun no llego el /n
                {
                	cmdIndex = 0;
                	cmdLine[0] = '\0';
                    cmdLine[cmdIndex++] = rxChar;
                    state = CMD_RECEIVING;					// paso a estado recibiendo
                }

                break;


            case CMD_RECEIVING:				//En caso del state recibiendo

                if (rxChar == '\r' || rxChar == '\n')		//si recibio el /n
                {
                    cmdLine[cmdIndex] = '\0';
                    state = CMD_PROCESS;
                }
                else if (cmdIndex < (CMD_MAX_LINE - 1))
                {
                    cmdLine[cmdIndex++] = rxChar;		//sino guarda en rxchar en el siguiente indice de Line
                }
                else
                {
                    state = CMD_ERROR;				// si no hay espacio en buffer, error
                }

                break;


            case CMD_PROCESS:		//procesar el comando recibido

                if (parseLine() == CMD_OK)		// si parseo es ok
                {
                    state = CMD_EXEC;			// ejecuto
                }
                else
                {
                    state = CMD_ERROR;		//sino error
                }

                break;


            case CMD_EXEC:		// Estado: ejecutar comando

                executeCommand();		// Ejecuta el comando (help, led, etc.)
                cmdIndex = 0;
                cmdLine[0] = '\0'; // y limpio el buffer
                state = CMD_IDLE;     // Vuelvo a dejar el estado IDLE para esperar otro comando

                break;


            case CMD_ERROR:

                uartSendString((uint8_t*)"Error\r\n");
                cmdIndex = 0;
                cmdLine[0] = '\0';
                state = CMD_IDLE;

                break;
        }
    }
}




static cmd_status_t parseLine(void)			//
{
    tokenCount = 0;

    char * token = strtok(cmdLine, " ");		//

    while (token != NULL && tokenCount < CMD_MAX_TOKENS)
    {
        tokens[tokenCount++] = token;
        token = strtok(NULL, " ");
    }

    return CMD_OK;
}


static void executeCommand(void)
{
    if (tokenCount == 0)
        return;

    /* ===== HELP ===== */
    if (strcasecmp(tokens[0], "help") == 0)
    {
        cmdPrintHelp();
    }

    /* ===== LED ===== */
    else if (strcasecmp(tokens[0], "led") == 0)
    {
        if (tokenCount < 2)
        {
            uartSendString((uint8_t*)"Falta parametro\r\n");
            return;
        }

        if (strcasecmp(tokens[1], "on") == 0)
        {
            ledOn();
        }
        else if (strcasecmp(tokens[1], "off") == 0)
        {
            ledOff();
        }
        else if (strcasecmp(tokens[1], "toggle") == 0)
        {
            ledToggle();
        }
        else
        {
            uartSendString((uint8_t*)"Parametro invalido\r\n");
        }
    }

    /* ===== STATUS ===== */
    else if (strcasecmp(tokens[0], "status") == 0)
    {
        if (ledStatus())
        {
            uartSendString((uint8_t*)"LED is ON\r\n");
        }
        else
        {
            uartSendString((uint8_t*)"LED is OFF\r\n");
        }
    }

    else
    {
        uartSendString((uint8_t*)"Comando desconocido\r\n");
    }
}





void cmdPrintHelp(void)
{
    uartSendString((uint8_t*)"\r\nComandos disponibles:\r\n");
    uartSendString((uint8_t*)"HELP\r\n");
    uartSendString((uint8_t*)"LED ON\r\n");
    uartSendString((uint8_t*)"LED OFF\r\n");
    uartSendString((uint8_t*)"LED TOGGLE\r\n");
    uartSendString((uint8_t*)"STATUS\r\n");
}
