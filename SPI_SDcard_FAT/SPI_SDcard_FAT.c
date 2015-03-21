/*
 * SPI_SDcard_FAT.c
 *
 * Created: 14.03.2015 20:40:17
 *  Author: Disgust
 */ 


#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>

// just for knowledge
#include <stdarg.h>
#include <stddef.h>
#include <stdalign.h>
#include <stdio.h>
#include <stdfix.h>

#include "AppConfig.h"

#include "SPI/spi_device.h"

#include "SD_Card/SDC.h"

#include "FAT/diskio.h"
#include "FAT/integer.h"
#include "FAT/pff.h"

card_t * pCard01;

// Read and write ring buffers
ring_buffer_t * W_rbuff01;
ring_buffer_t * R_rbuff01;

static uint8_t Wbuff01[BUFFER_SIZE]; // storage for write buffer
static uint8_t Rbuff01[BUFFER_SIZE]; // storage for read buffer

int main(void)
{
	// Prepare CPU and OSC
	;;
		// Power reduction
		// Sleep modes
	
	// Prepare System Timers
	;;
	
	// Prepare LCDisplay
	;;
	
	// Prepare place for SD card buffers
	W_rbuff01= malloc(sizeof(ring_buffer_t));
	R_rbuff01= malloc(sizeof(ring_buffer_t));
	initRBuffer(W_rbuff01, Wbuff01, sizeof(Wbuff01));
	initRBuffer(R_rbuff01, Rbuff01, sizeof(Rbuff01));
	
	// Prepare SPI-bus and SPI-device object
	SPI_Init(SPI_MODE_0);
	spiDevice_t SD_card01= SPI_deviceInit(SD_SpiCtrlPort, SD_CsPin, "Memory Card", 1);
	SPI_deviceAttachBuffer(&SD_card01, *R_rbuff01, 1);
	SPI_deviceAttachBuffer(&SD_card01, *W_rbuff01, 0);
	
	// Prepare SD-card object
	pCard01= initCardObject(&SD_card01, SD_CardCtrlPort, SD_PresPin, SD_PwrPin);
	
	// Prepare File System
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