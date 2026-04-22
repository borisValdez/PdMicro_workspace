/**
 ******************************************************************************
 * @file    API_LCD.c
 * @author  Boris
 * @brief   Driver para pantallas LCD 16x2 mediante el expansor I2C PCF8574.
 * Controla el display en modo de 4 bits para optimizar la transferencia.
 *   Created on: Apr 14, 2026
 ******************************************************************************
 */

#include "API_LCD.h"
#include "API_I2C.h"
#include "API_delay.h"
#include "main.h"
#include <string.h>
#include "stm32f4xx_hal.h"

/* La dirección 0x27 es la estándar, se desplaza 1 bit para cumplir con el formato HAL I2C */
#define LCD_ADDR (0x27 << 1)  //Dirección I2C del LCD

/* Bits del PCF8574 */
#define LCD_BACKLIGHT 0x08  //!< Control del LED de fondo
#define LCD_ENABLE    0x04  //!< Bit de habilitación (E)
#define LCD_RS        0x01  //!< Selección de Registro (0: Comando, 1: Dato)


static void lcd_write(uint8_t data)  // Envia un byte
{
    uint8_t tmp = data | LCD_BACKLIGHT;
    i2c_write(LCD_ADDR, &tmp, 1);  // Uso de la API_I2C modular
}


static void lcd_pulse_enable(uint8_t data)  //Genera el pulso de Enable necesario para que el LCD capture el dato.
{
    lcd_write(data | LCD_ENABLE);
    HAL_Delay(1);
    lcd_write(data & ~LCD_ENABLE);
    HAL_Delay(1);
}


static void lcd_write4bits(uint8_t data) // Envía un nibble
{
    lcd_write(data);
    lcd_pulse_enable(data);
}


static void lcd_command(uint8_t cmd)   //Envía un comando de control al LCD.
{
    lcd_write4bits(cmd & 0xF0);
    lcd_write4bits((cmd << 4) & 0xF0);
}


static void lcd_data(uint8_t data)   //Envia dato
{
    lcd_write4bits((data & 0xF0) | LCD_RS);
    lcd_write4bits(((data << 4) & 0xF0) | LCD_RS);
}

/* Inicialización */
void lcd_init(void)  //Inicializa el LCD siguiendo la secuencia de arranque del HD44780.
{
	HAL_Delay(50);

    lcd_write4bits(0x30);
    HAL_Delay(5);

    lcd_write4bits(0x30);
    HAL_Delay(5);

    lcd_write4bits(0x30);
    HAL_Delay(5);

    lcd_write4bits(0x20); // modo 4 bits
    HAL_Delay(5);

    lcd_command(0x28); // 2 líneas
    lcd_command(0x08); // display off
    lcd_command(0x01); // clear
    HAL_Delay(5);

    lcd_command(0x06);
    lcd_command(0x0C);
}

/* Limpiar pantalla */
void lcd_clear(void)
{
    lcd_command(0x01);
    HAL_Delay(2);
}

/* Posicionar cursor */
void lcd_set_cursor(uint8_t row, uint8_t col) //Ubica el cursor en una posición específica.
{
    uint8_t addr;

    switch(row)
    {
        case 0: addr = 0x80 + col; break;
        case 1: addr = 0xC0 + col; break;
        default: addr = 0x80 + col;
    }

    lcd_command(addr);
}

/* Imprimir string */
void lcd_print(char *str) //Imprime una cadena de caracteres en el LCD.
{
    while(*str)
    {
        lcd_data(*str++);
    }
}
