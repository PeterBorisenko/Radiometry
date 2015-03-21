/* 
 * File:   spi.h
 * Author: Peter Borisenko / AWSMTEK.COM
 * For using with ATXmega and ATSAM devices
 * Created on 9 ???? 2015 ?., 22:42
 */

#ifndef SPI_DEVICE_H
#define	SPI_DEVICE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <avr/io.h>
#include <stdint.h>

#if defined(_AVR_IOXXX_H_)
	#include "spi_avrXmega.h"
#endif

#define SPI_MASTER			1

#define MUTEX_CAPTURED		1
#define MUTEX_FREE			0

// TODO: Implement Read, Write and Flush ring buffer

typedef uint8_t spiMutex_t;

typedef struct {
    unsigned char name[8];
	PORT_t devPort;
    uint8_t csPin;
    uint8_t priority;
    spiMutex_t activity;
} spiDevice_t;

enum {
	SPI_READ,
	SPI_WRITE
} SPI_State;

    // SPI core functions
    void SPI_Init(uint8_t mode);
    spiDevice_t SPI_deviceInit(PORT_t devPort, uint8_t csPin, unsigned char * name, uint8_t pr);
    
    void SPI_WriteByte(uint8_t);
    uint8_t SPI_ReadByte();
	void SPI_WriteReg(spiDevice_t *, uint8_t, uint8_t);
	uint8_t SPI_ReadReg(spiDevice_t *, uint8_t);
    void SPI_WriteArray(spiDevice_t *, uint8_t, uint8_t *, uint8_t);
    void SPI_ReadArray(spiDevice_t *, uint8_t, uint8_t *, uint8_t);

    uint8_t chipSelect(spiDevice_t *);
    uint8_t chipRelease(spiDevice_t *);
    void waitForSPI(void);
	void SPI_FlushBuffer();
	
	// Ring buffer using
	void SPI_ISR_Handler (ring_buffer_t *);

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_DEVICE_H */

