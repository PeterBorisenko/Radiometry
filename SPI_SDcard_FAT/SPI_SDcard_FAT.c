/*
 * SPI_SDcard_FAT.c
 *
 * Created: 14.03.2015 20:40:17
 *  Author: Disgust
 */ 


#include <avr/io.h>

#include "AppConfig.h"

#include "SPI/spi_device.h"

#include "SD_Card/SDC.h"

#include "FAT/diskio.h"
#include "FAT/integer.h"
#include "FAT/pff.h"

card_t * pCard01;

int main(void)
{
	SPI_Init(SPI_MODE_0);
	spiDevice_t SD_card01= SPI_deviceInit(SD_SpiCtrlPort, (1 << SD_CsPin), "Memory Card", 1);
	pCard01= initCardObject(&SD_card01, SD_CardCtrlPort, SD_PresPin, SD_PwrPin);
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}