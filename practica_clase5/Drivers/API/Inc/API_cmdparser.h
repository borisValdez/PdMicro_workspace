/*
 * API_cmdparser.h
 *
 *  Created on: Apr 8, 2026
 *      Author: boris
 */

#ifndef API_CMDPARSER_H
#define API_CMDPARSER_H

#include "stdint.h"
#include "stdbool.h"

#define CMD_MAX_LINE 64
#define CMD_MAX_TOKENS 3

typedef enum {
    CMD_OK = 0,
    CMD_ERR_OVERFLOW,
    CMD_ERR_SYNTAX,
    CMD_ERR_UNKNOWN,
    CMD_ERR_ARG
} cmd_status_t;


void cmdParserInit(void);


void cmdPoll(void);


void cmdPrintHelp(void);


#endif /* API_INC_API_CMDPARSER_H_ */
