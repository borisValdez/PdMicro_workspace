/*
 * API_MEF.H
 *
 *  Created on: Apr 14, 2026
 *      Author: boris
 */

#ifndef API_INC_API_MEF_H_
#define API_INC_API_MEF_H_

typedef enum {
    IDLE,
    RECEIVING,
    PROCESS,
    EXEC,
	MEF_ERROR
} MEFState_t;

void MEF_init(void);
void MEF_update(void);

#endif /* API_INC_API_MEF_H_ */
