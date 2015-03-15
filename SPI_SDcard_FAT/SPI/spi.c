#include "spi_device.h"
#include <string.h>

volatile static spiMutex_t spiMutex;

// TODO: To make this lib universal we need to use funcs for pin/ports management but implement it in other file

void SPI_Init(uint8_t mode) {
	
	// SPI I/O Ports Init
	#ifdef SPI_MASTER
	PORTC_DIRSET= (SDO_PIN|SCK_PIN);
	PORTC_DIRCLR= SDI_PIN;
	#else
	PORTC_DIRSET= (SDO_PIN|SCK_PIN);
	PORTC_DIRCLR= SDI_PIN;
	#endif
	
	// SPI Control & Status Registers Init
	SPI_ENABLE= 1;
	SPI_MODE= mode;
}

spiDevice_t SPI_deviceInit(PORT_t devPort, unsigned char csPin, unsigned char * buffer, uint8_t pr) {
	devPort.DIRSET= csPin;
	spiDevice_t spd;
	strcpy(spd.name, buffer);
	spd.priority= pr;
	spd.devPort= devPort;
	spd.csPin= csPin;
	spd.devPort.OUTSET= csPin;
	spd.readBuffer.buffer= NULL;
	spd.writeBuffer.buffer= NULL;
    return spd;
}

void SPI_deviceAttachBuffer(spiDevice_t * dev, uint8_t * buf, uint8_t length, uint8_t R_W) {
	
}

void SPI_deviceDetachBuffer(spiDevice_t * dev, uint8_t R_W) {
	
}

uint8_t chipSelect(spiDevice_t * spd){
    if (spiMutex == MUTEX_FREE) {
        spiMutex= MUTEX_CAPTURED;
        spd->activity= spiMutex;
        spd->devPort.OUTCLR= spd->csPin;
        return 1;
    }
    else {
        return 0;
    }
}

uint8_t chipRelease(spiDevice_t * spd){
    if (spd->activity == MUTEX_CAPTURED) {
        spd->devPort.OUTSET= spd->csPin;
        spiMutex= MUTEX_FREE;
        spd->activity= spiMutex;
        return 1;
    }
    else {
        return 0;
    }
}

void SPI_WriteByte(uint8_t data)
{
   SPI_BUFFER = data;
   while(!SPI_BUFFER_EMPTY);
}

uint8_t SPI_ReadByte() {
	while(!SPI_DATA_RECEIVED);
	return SPI_BUFFER;
}

uint8_t SPI_ReadReg(spiDevice_t * spd, uint8_t addr)
{
	chipSelect(spd);
	uint8_t report;
	SPI_BUFFER = addr;
	while(!SPI_BUFFER_EMPTY);
	report = SPI_BUFFER;
	chipRelease(spd);
	return report;
}

void SPI_WriteReg(spiDevice_t * spd, uint8_t command, uint8_t dat)
{
	chipSelect(spd);
	SPI_WriteByte(command);
	while(!SPI_BUFFER_EMPTY);
	SPI_BUFFER = dat;
	while(!SPI_BUFFER_EMPTY);
	chipRelease(spd);
}

void SPI_WriteArray(spiDevice_t * spd, uint8_t command, uint8_t * buffer, uint8_t num)
{
	chipSelect(spd);
   SPI_WriteByte(command);
   while(!SPI_BUFFER_EMPTY);
   while(num--){
      SPI_BUFFER = *buffer++;
      while(!SPI_BUFFER_EMPTY);
   }
   chipRelease(spd);
}

void SPI_ReadArray(spiDevice_t * spd, uint8_t command, uint8_t * buffer, uint8_t num)
{
	chipSelect(spd);
   SPI_WriteByte(command);
   while(!SPI_BUFFER_EMPTY);
   while(num--){
      //SPI_BUFFER = *buffer;
      while(!SPI_BUFFER_EMPTY);
      *buffer++ = SPI_BUFFER;
   }
   chipRelease(spd);
}

void waitForSPI(){
    while(!SPI_BUFFER_EMPTY);
}

void SPI_FlushBuffer() {
	while (SPI_BUFFER) {
		uint8_t i= SPI_BUFFER;
	}
}

void SPI_ISR_Handler (uint8_t * buffer, uint8_t length, uint8_t counter) {
	switch (SPI_State) {
		case SPI_READ:
			*(buffer+counter) = SPI_BUFFER;
			if (++counter >= length)
			{
				counter= 0;
			}
			break;
		case SPI_WRITE:
			SPI_BUFFER= *(buffer+counter);
			if (++counter >= length)
			{
				counter= 0;
			}
			break;
		default:
			break;
	};
}

typedef struct {
	uint8_t * buffer;	// storage address
	uint8_t length;		// buffer length (max counter = length - 1)
	uint8_t counter;	// next empty element (accessing : *(buffer+counter))
	} ring_buffer_t;
	
	// TODO: Implement Read, Write and Flush ring buffer