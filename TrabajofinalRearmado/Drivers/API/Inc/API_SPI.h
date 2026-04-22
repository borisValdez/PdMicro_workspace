#ifndef API_INC_API_SPI_H_
#define API_INC_API_SPI_H_

#include <stdint.h>

void spi_init(void);
int spi_tx(uint8_t *data, uint16_t len);
int spi_rx(uint8_t *data, uint16_t len);
int spi_tx_rx(uint8_t *tx, uint8_t *rx, uint16_t len);

#endif
