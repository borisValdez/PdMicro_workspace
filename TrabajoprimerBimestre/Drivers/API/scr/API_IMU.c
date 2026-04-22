
/**
 ******************************************************************************
 * @file    API_IMU.c
 * @author  Boris
 * @brief   Driver para el sensor ICM-42688-P mediante bus SPI.
 * Proporciona funciones de inicialización y lectura de datos crudos
 * de acelerómetro y giroscopio.
 *  *  Created on: Apr 14, 2026
 ******************************************************************************
 */

#include "API_IMU.h"
#include "API_SPI.h"
#include "API_delay.h"
#include "main.h"


#define IMU_DATA_LENGTH 12 // Largo de trama para leer Accel (6 bytes) + Gyro (6 bytes)


extern SPI_HandleTypeDef hspi1;

/* --- Registros del ICM-42688 (según Datasheet) --- */
#define REG_PWR_MGMT0      0x4E  //!< Control de energía y sensores
#define IMU_ACCEL_GYRO_ON  0x0F  //!< Activa Accel y Gyro en Low Noise Mode
#define REG_ACCEL_DATA_X1  0x1F  //!< Registro base de datos de salida
#define REG_DEVICE_CONFIG  0x11  //!< Configuración de reset y bus
#define REG_WHO_AM_I       0x75  //!< Identificación del dispositivo (0x47)


static void imu_write_reg(uint8_t reg, uint8_t value)  //Escribe un valor en un registro específico del sensor.
{
    uint8_t tx[2];

    tx[0] = reg & 0x7F;   // MSB en 0 indica operación de ESCRITURA
    tx[1] = value;

    IMU_CS_LOW();   //selecciono la IMU
    spi_tx(tx, 2);  // envío registro y valor
    IMU_CS_HIGH();  // termino comunicación
}


static void imu_read_regs(uint8_t reg, uint8_t *data, uint8_t len)  //Lee múltiples registros consecutivos del sensor.
{
   // uint8_t tx[1 + len];
    //uint8_t rx[1 + len];

    // Tamaño total: comando de dirección + bytes de datos
        uint8_t tx[13] = {0}; // 1 + IMU_DATA_LENGTH
        uint8_t rx[13] = {0};
        tx[0] = reg | 0x80;   // MSB en 1 indica operación de LECTURA

    IMU_CS_LOW();
    spi_tx_rx(tx, rx, 1 + len);
    IMU_CS_HIGH();

    // Mapeo de datos recibidos desplazando el primer byte de comando
     for(int i = 0; i < len; i++) {
           data[i] = rx[i + 1];
     }
}


void imu_init(void)
{
    HAL_Delay(50); // Tiempo de estabilización tras encendido
    // RESET DEVICE
    imu_write_reg(REG_DEVICE_CONFIG, 0x01);  // // Realiza un Soft Reset del sensor para asegurar estado limpio
    HAL_Delay(50); // Tiempo necesario para que el sensor reinicie su lógica
    imu_write_reg(REG_PWR_MGMT0, IMU_ACCEL_GYRO_ON); //Activa acelerómetro y giroscopio
    HAL_Delay(50);
}

int imu_read_raw(imu_raw_data_t *data)  //Lee los datos crudos (raw) de los 6 ejes.
{
    uint8_t buf[IMU_DATA_LENGTH];   // 6 valores de 16 bits = 12 bytes
    imu_read_regs(REG_ACCEL_DATA_X1, buf, IMU_DATA_LENGTH); // Lectura de todos los ejes

    // Reconstrucción de valores de 16 bits
    data->ax = (int16_t)((buf[0] << 8) | buf[1]);
    data->ay = (int16_t)((buf[2] << 8) | buf[3]);
    data->az = (int16_t)((buf[4] << 8) | buf[5]);

    data->gx = (int16_t)((buf[6] << 8) | buf[7]);
    data->gy = (int16_t)((buf[8] << 8) | buf[9]);
    data->gz = (int16_t)((buf[10] << 8) | buf[11]);

    return 0;
}

//Verifica la identidad del sensor (Who Am I).ID del sensor (0x47 esperado).
uint8_t IMU_ReadWhoAmI(void)
{
    uint8_t tx[2] = { REG_WHO_AM_I | 0x80, 0x00 }; // Byte 1: Dirección + Read bit, Byte 2: Dummy
    uint8_t rx[2] = { 0, 0 };


    IMU_CS_LOW();

    // 2. Usar tu API de SPI
    if (spi_tx_rx(tx, rx, 2) != 0) {
    	IMU_CS_HIGH();
        return 0xEE; // Error de comunicación
    }

    // 3. Desactivar sensor (CS HIGH)
    IMU_CS_HIGH();

    return rx[1];
}
