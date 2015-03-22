/*
 * SPI_SDcard_FAT.c
 *
 * Created: 14.03.2015 20:40:17
 *  Author: Disgust
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// just for knowledge
#include <stdarg.h>
#include <stddef.h>
#include <stdalign.h>
#include <stdfix.h>

#include "AppConfig.h"

#include "USART_XmegaB3.h"
#include "SPI/spi_device.h"

#include "SD_Card/SDC.h"

#include "FAT/diskio.h"
#include "FAT/integer.h"
#include "FAT/pff.h"

volatile static card_t * pCard01;

// Read and write buffers
static uint8_t Wbuff01[BUFFER_SIZE]; // storage for write buffer
static uint8_t Rbuff01[BUFFER_SIZE]; // storage for read buffer

static uint8_t dbgSendChar(char c, FILE *stream);

static FILE debugout= FDEV_SETUP_STREAM(dbgSendChar, NULL, _FDEV_SETUP_WRITE);

static uint8_t dbgSendChar(char c, FILE *stream) {
	if (c == '\n')
	dbgSendChar('\r', stream);
	sendChar(c);
	return 0;
}

int main(void)
{
	// Prepare UART for Debugging
	prepareUSART(BAUDRATE);
	stdout= &debugout;
	printf("USART Initialized\n");
	
	// Prepare CPU and OSC
	;;
		// Power reduction
		// Sleep modes
	printf("OCS not calibrated\n");
	
	// Prepare System Timers
	;;
	printf("System Timers not used!\n");
	// Prepare LCDisplay
	;;
	printf("LCD not used!\n");
	
	// Prepare SPI-bus and SPI-device objects
	SPI_Init(SPI_MODE_0);
	spiDevice_t SD_card01= SPI_deviceInit(SD_SpiCtrlPort, SD_CsPin, "Memory Card", 1);
	spiDevice_t GPS_mdl01= SPI_deviceInit(GPS_SpiCtrlPort, GPS_CsPin, "A2200 GPS Module", 2);
	
	// Prepare SD-card object
	pCard01= initCardObject(&SD_card01, SD_CardCtrlPort, SD_PresPin, SD_PwrPin);
	cardAttachBuffer(&pCard01, *Rbuff01, 1);
	cardAttachBuffer(&pCard01, *Wbuff01, 0);
	
	// Prepare File System
	;;
	
	// Prepare GPS Module
	;;
	
	// Prepare Buttons
	;;
	
	// Prepare Sound
	;;
	
	// Prepare USB
	;;
	
	// Prepare Interrupts
	;;
	
	// Prepare Tubes
	;;
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}