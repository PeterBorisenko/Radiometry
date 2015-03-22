/*
 * AppConfig.h
 *
 * Created: 15.03.2015 12:44:00
 *  Author: Disgust
 */ 


#ifndef APPCONFIG_H_
#define APPCONFIG_H_

#define F_CPU 24000000UL
#define BAUDRATE	(9600UL)

// SD-card user definitions
#define SD_SpiCtrlPort	PORTC
#define SD_CsPin		(1 << 0)

#define SD_CardCtrlPort	PORTB
#define SD_PwrPin		(1 << 3)
#define SD_PresPin		(1 << 2)

#define BUFFER_SIZE 512

// Counter definitions


// Display user definitions


// GPS user definitions
#define GPS_SpiCtrlPort PORTG
#define GPS_CsPin	(1 << 0)

#define GPS_CtrlPort	PORTG
#define GPS_Reset		(1 << 1)
#define GPS_Enable		(1 << 2)



#endif /* APPCONFIG_H_ */