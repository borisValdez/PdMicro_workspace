/**
 ******************************************************************************
 * @file    API_MEF.h
 * @author  Boris
 * @brief   Interfaz de la Máquina de Estados Finitos.
 ******************************************************************************
 */

#ifndef API_INC_API_MEF_H_
#define API_INC_API_MEF_H_


//Enumeración de los estados de la MEF.

typedef enum {
    IDLE,       // Espera de tiempo para el próximo ciclo
    RECEIVING,  //Lectura de datos del sensor
    PROCESS,    //Cálculos trigonométricos
    EXEC,       // Actualización de salida (LCD)
    MEF_ERROR   //!Manejo de fallos de comunicación
} MEFState_t;

/* Funciones de la MEF */
void MEF_init(void);   //Inicializa la máquina y variables
void MEF_update(void); //Ejecuta un ciclo de la máquina

#endif /* API_INC_API_MEF_H_ */
