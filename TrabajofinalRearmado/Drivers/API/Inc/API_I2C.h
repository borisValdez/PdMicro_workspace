#ifndef API_INC_API_I2C_H_
#define API_INC_API_I2C_H_

#include <stdint.h>

void i2c_init(void);
int i2c_write(uint8_t addr, uint8_t *data, uint16_t len);
int i2c_read(uint8_t addr, uint8_t *data, uint16_t len);


#endif
