#ifndef API_INC_API_LCD_H_
#define API_INC_API_LCD_H_

#include <stdint.h>

void lcd_init(void);
void lcd_clear(void);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_print(char *str);

#endif
