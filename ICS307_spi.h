#ifndef SPIXIO_H
#define SPIXIO_H

#include <inttypes.h>

extern uint8_t GSPI_DONE;
extern char GSPI_NAME[500];

/* Application specific macro definations */
#define CHANNEL_TO_OPEN				0	/*0 for first available channel, 1 for next... */
#define SPI_DEVICE_BUFFER_SIZE		256
#define SPI_CLOCK_RATE_HZ			10000

/* Unique for this device */
#define ICS307_NUMBITS 132
#define ICS307_NUMWORD (ICS307_NUMBITS/8 + 1) // MUST be 17 (if 132 bits)
//extern uint8_t ICS307_WORD[ICS307_NUMWORD];

int write_word(uint8_t* data, uint32_t databits);
int spi_init(void);
void spi_close(void);
int clogb2(int value);

#endif // SPIXIO_H
