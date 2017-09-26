/* Include libMPSSE header */
/* Include D2XX header*/
#include <stdio.h>
#include <string.h>
#include "ICS307_spi.h"
#include "WinTypes.h"
#include "ftd2xx.h"
#include "libMPSSE_spi.h"

static FT_HANDLE ftHandle;
static uint8 buffer[SPI_DEVICE_BUFFER_SIZE] = {0};

#define APP_CHECK_STATUS(exp) {if(exp!=FT_OK){printf("%s:%d:%s(): status(0x%x) \
!= FT_OK\n",__FILE__, __LINE__, __FUNCTION__,exp);return 0;}else{;}};
#define APP_CHECK_STATUS_INV(exp) {if(exp!=FT_OK){printf("%s:%d:%s(): status(0x%x) \
!= FT_OK\n",__FILE__, __LINE__, __FUNCTION__,exp);return 1;}else{;}};
#define CHECK_NULL(exp){if(exp==NULL){printf("%s:%d:%s():  NULL expression \
encountered \n",__FILE__, __LINE__, __FUNCTION__);return 0;}else{;}};
#define CHECK_NULL_INV(exp){if(exp==NULL){printf("%s:%d:%s():  NULL expression \
encountered \n",__FILE__, __LINE__, __FUNCTION__);return 1;}else{;}};

uint8_t  GSPI_DONE;
uint32_t GSPI_SPI_ADDR_BITS;
char GSPI_NAME[500];

int clogb2(int value)
{
	int 	i;
	int clogb = 0;
	for(i = 0; (1<<i) < value; i = i + 1)
		clogb = i + 1;
	return clogb;
}

int write_word(uint8_t* data, uint32_t databits)
{
	uint32 sizeToTransfer = 0;
	uint32 sizeTransfered;
	FT_STATUS status;
	int i, n, s;

	/* CS_High + Write command + Address */
	sizeToTransfer=ICS307_NUMBITS;
	sizeTransfered=0;
	memset(buffer, 0, SPI_DEVICE_BUFFER_SIZE);
	for(i = 0; i < databits; i++)
	{
	    // ICS307 accepts the MSB first. This cable needs that first bit is in buffer[0]b7. Reacomodate this
	    int i_inv = (ICS307_NUMBITS - i - 1);
	    int nByteFrom = i_inv / 8;
	    int nBitFrom = i_inv % 8;
	    int nByteTo = i / 8;
	    int nBitTo = 7 - (i % 8);
	    buffer[nByteTo] |= ((data[nByteFrom] >> nBitFrom) & 0x1) << nBitTo;
	}
	
	status = SPI_Write(ftHandle, buffer, sizeToTransfer, &sizeTransfered,
		SPI_TRANSFER_OPTIONS_SIZE_IN_BITS|
		SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE|
		SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
	APP_CHECK_STATUS(status);

	return 1;
}

int spi_init(void)
{
    strcpy(GSPI_NAME, "No Device");
    GSPI_DONE = 0;
    FT_STATUS status = FT_OK;
	FT_DEVICE_LIST_INFO_NODE devList = {0};
	ChannelConfig channelConf = {0};
	uint32 channels = 0;
	uint8 latency = 255;
	uint32 i;
	uint32 dev_to_open;

    channelConf.ClockRate = SPI_CLOCK_RATE_HZ;
	channelConf.LatencyTimer = latency;
	channelConf.configOptions = SPI_CONFIG_OPTION_MODE0 | SPI_CONFIG_OPTION_CS_DBUS3 | SPI_CONFIG_OPTION_CS_ACTIVELOW;
	channelConf.Pin = 0x00000000;	/*According to manual, this is not needed because directions of pins are overriden in SPI*/

    /* init library */
#ifdef _MSC_VER
	Init_libMPSSE();
#endif
	status = SPI_GetNumChannels(&channels);
	APP_CHECK_STATUS(status);
	printf("Number of available SPI channels = %d\n",(int)channels);

	if(channels>0)
	{
		// NO, WE ARENT GOING TO FUCKING OPEN THE FIRST AVAILABLE
		// WE'LL SEARCH OUR PROGRAMMER
		dev_to_open = 0xFFFFFFFF;
		for(i = 0; i < channels; i++){
			status = SPI_GetChannelInfo (i,&devList);
			APP_CHECK_STATUS(status);

			if(/*devList.Flags == 0x2 && devList.Type == 0x8 && */devList.ID == 0x4036014)
			{
				dev_to_open = i;
				break;
			}

		}
		if(dev_to_open == 0xFFFFFFFF) return 0;

		status = SPI_OpenChannel(dev_to_open,&ftHandle);
		APP_CHECK_STATUS(status);

		status = SPI_InitChannel(ftHandle,&channelConf);
	}
	else return 0;

    GSPI_DONE = 1;
    sprintf(GSPI_NAME, "SPI(%u) 0x%x 0x%x 0x%x 0x%x (0x%x), %s(%s) \n",
        i,devList.Flags,devList.Type,devList.ID,devList.LocId,devList.ftHandle,devList.Description,devList.SerialNumber);
    return 1;
}

void spi_close(void)
{
    SPI_CloseChannel(ftHandle);

#ifdef _MSC_VER
	Cleanup_libMPSSE();
#endif
}
