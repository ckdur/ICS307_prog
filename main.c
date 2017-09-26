#include <stdio.h>
#include <math.h>
#include "ICS307_spi.h"

// Compile with:
// gcc main.c ICS307_spi.c -I./ -o ICS307_spi -lftd2xx -lMPSSE -lm

// CKDUR: Radix you want to display (only valid 2, 8, 10 and 16)
#define RADIX 2

// CKDUR: Modify me if you want another configuration for your ICS307

// 100MHz ? (Amaya told me this) (29FFDC0010A00000000000000000EBFEE)
//uint8_t ICS307_WORD[ICS307_NUMWORD] = {0xEE, 0xBF, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x01, 0xC0, 0xFD, 0x9F, 0x2};    

// 5MHz
uint8_t ICS307_WORD[ICS307_NUMWORD] = {0xF2, 0x9F, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDA, 0xAE, 0xC2, 0x57, 0x85, 0x3};  

// The one Amaya told me if 5MHz fails
//uint8_t ICS307_WORD[ICS307_NUMWORD] = {0xF2, 0x9F, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDA, 0xAE, 0xC2, 0x57, 0x81, 0x3};  

// The original one Students used
//uint8_t ICS307_WORD[ICS307_NUMWORD] = {0xE6, 0xDF, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAE, 0xC3, 0xC7, 0x13, 0x03}; 

// Capture program for aamaya's chip
int get_ndig(int word, int radix)
{
    return (int)(log((double)((1<<word)-1))/log((double)radix)) + 1;
}

void sprint_radix(char* chbuf2, uint32_t nvalue, int ndig, int radix)
{
    int i;
    if(radix == 16) sprintf(chbuf2, "%.*x", ndig, nvalue);
    if(radix == 10) sprintf(chbuf2, "%u", (unsigned int)nvalue);
    if(radix == 8) sprintf(chbuf2, "%.*o", ndig, nvalue);
    if(radix == 2) {
        for(i = 0; i < ndig; i++)
        {
            chbuf2[i] = ((nvalue >> (ndig - i - 1)) & 0x1)?'1':'0';
        }
        chbuf2[i] = 0;
    }
}

int main(int argc, char *argv[])
{
	printf("Welcome to ICS307 programmer!\n");
	printf("Compilation date:, " __DATE__ ", " __TIME__ "\n");
		
	if(!spi_init()) 
	{
		fprintf(stderr, "Failed to init SPI (File: %s, Line %d)\n", __FILE__, __LINE__);
		return 1;
	}
	
	// Just write it... a couple of times if u want
	write_word(ICS307_WORD, ICS307_NUMBITS);
	//write_word(ICS307_WORD, ICS307_NUMBITS);
	//write_word(ICS307_WORD, ICS307_NUMBITS);
    getchar();
    spi_close();
	
	return 0;
}

