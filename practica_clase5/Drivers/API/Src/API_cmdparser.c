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
    uint8_t rxChar;
    bool_t newChar = uartReceiveStringSize(&rxChar, 1);

    switch (state)
    {

        case CMD_IDLE:

            if (newChar && rxChar != '\r' && rxChar != '\n')
            {
                cmdIndex = 0;
                cmdLine[0] = '\0';
                cmdLine[cmdIndex++] = rxChar;
                state = CMD_RECEIVING;
            }
            break;


        case CMD_RECEIVING:

            if (newChar)
            {
                if (rxChar == '\r')
                {
                    cmdLine[cmdIndex] = '\0';
                    state = CMD_PROCESS;
                }
                else if (cmdIndex < (CMD_MAX_LINE - 1))
                {
                    cmdLine[cmdIndex++] = rxChar;
                }
                else
                {
                    state = CMD_ERROR;
                }
            }
            break;


        case CMD_PROCESS:

            if (parseLine() == CMD_OK)
            {
                state = CMD_EXEC;
            }
            else
            {
                state = CMD_ERROR;
            }
            break;


        case CMD_EXEC:

            executeCommand();
            cmdIndex = 0;
            cmdLine[0] = '\0';
            state = CMD_IDLE;
            break;


        case CMD_ERROR:

            uartSendString((uint8_t*)"Error\r\n");
            cmdIndex = 0;
            cmdLine[0] = '\0';
            state = CMD_IDLE;
            break;
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
