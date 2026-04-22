/**
 ******************************************************************************
 * @file    API_LCD.c
 * @author  Boris
 * @brief   Driver para pantallas LCD 16x2 mediante el PCF8574.
 * Controla el display en modo de 4 bits para optimizar la transferencia.
 ******************************************************************************
 */

#include "API_LCD.h"
#include "API_I2C.h"
#include "API_delay.h"
#include "main.h"
#include <string.h>

/* La dirección 0x27 es la estándar, se desplaza 1 bit para cumplir con el formato HAL I2C */
#define LCD_ADDR (0x27 << 1)

/* Mapeo de bits del expansor PCF8574 conectado al LCD */
#define LCD_BACKLIGHT 0x08  //!< Control del LED de fondo
#define LCD_ENABLE    0x04  //!< Bit de habilitación (E)
#define LCD_RS        0x01  //!< Selección de Registro (0: Comando, 1: Dato)

/**
 *  Envía un byte físico al expansor I2C .
 */
static void lcd_write(uint8_t data)
{
    uint8_t tmp = data | LCD_BACKLIGHT;
    i2c_write(LCD_ADDR, &tmp, 1); // Uso de la API_I2C modular

}

/**
 *  Genera el pulso de Enable necesario para que el LCD capture el dato.
 */
static void lcd_pulse_enable(uint8_t data)
{
    lcd_write(data | LCD_ENABLE);
    HAL_Delay(1);
    lcd_write(data & ~LCD_ENABLE);
    HAL_Delay(1);
}


 //Envía un nibble (4 bits) al LCD.

static void lcd_write4bits(uint8_t data)
{
    lcd_write(data);
    lcd_pulse_enable(data);
}


 //Envía un comando de control al LCD.

static void lcd_command(uint8_t cmd)
{
    // Se envía primero el nibble alto y luego el bajo
    lcd_write4bits(cmd & 0xF0);
    lcd_write4bits((cmd << 4) & 0xF0);
}

/**
Envía un carácter para ser mostrado en pantalla.
 */
static void lcd_data(uint8_t data)
{
    // Se envía manteniendo el bit RS en alto (Modo Dato)
    lcd_write4bits((data & 0xF0) | LCD_RS);
    lcd_write4bits(((data << 4) & 0xF0) | LCD_RS);
}

/**
 * nicializa el LCD siguiendo la secuencia de arranque del HD44780.
 */
void lcd_init(void)
{
    HAL_Delay(50); // Espera de encendido

    // Secuencia de reset forzado a modo 8 bits (3 veces)
    lcd_write4bits(0x30);
    HAL_Delay(5);
    lcd_write4bits(0x30);
    HAL_Delay(5);
    lcd_write4bits(0x30);
    HAL_Delay(5);

    // Configuración definitiva a modo 4 bits
    lcd_write4bits(0x20);
    HAL_Delay(5);

    /* --- Comandos de configuración --- */
    lcd_command(0x28); // Function Set: 4-bit, 2 líneas, 5x8 puntos
    lcd_command(0x08); // Display Off
    lcd_command(0x01); // Clear Display
    HAL_Delay(5);      //
    lcd_command(0x06); // Entry Mode
    lcd_command(0x0C); // Display On
}

/**
 * Limpia el contenido de la pantalla y vuelve el cursor a (0,0).
 */
void lcd_clear(void)
{
    lcd_command(0x01);
    HAL_Delay(2);
}

/**
 * Ubica el cursor en una posición específica.

 */
void lcd_set_cursor(uint8_t row, uint8_t col)
{
    uint8_t addr;
    switch(row)
    {
        case 0: addr = 0x80 + col; break; // Dirección DDRAM fila 0
        case 1: addr = 0xC0 + col; break; // Dirección DDRAM fila 1
        default: addr = 0x80 + col;
    }
    lcd_command(addr);
}

/**
 *Imprime una cadena de caracteres en el LCD.
 */
void lcd_print(char *str)
{
    while(*str)
    {
        lcd_data(*str++);
    }
}
