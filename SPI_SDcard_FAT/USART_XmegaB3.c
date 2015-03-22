/*
 * USART_XmegaB3.c
 *
 * Created: 03.02.2015 0:58:39
 *  Author: Disgust
 */ 

#include "USART_XmegaB3.h"
#include "AppConfig.h"

static USART_t * debugPort= (USART_t *)0x8A0;

 void prepareUSART(unsigned long ubrr) {
	ubrr= ((F_CPU /((ubrr) * 16 ) ) - 1);
	 //      DDRD|= (1 << PIND1);
	 //      DDRD&= ~(1 << PIND0);
	debugPort->BAUDCTRLB= (1 << USART_BSCALE0_bp);
	debugPort->BAUDCTRLA= (LO(ubrr));
	debugPort->BAUDCTRLB= (HI(ubrr));
	debugPort->CTRLC= USART_CMODE_ASYNCHRONOUS_gc|USART_PMODE_DISABLED_gc|(0 << USART_SBMODE_bp)|USART_CHSIZE_8BIT_gc; // Frame format: 8 data, 1 stop, parity no
	debugPort->CTRLB= (1 << USART_RXEN_bp)|(1 << USART_TXEN_bp); 
 }
 
 void sendChar(uint8_t byteToSend)
 {
	 while (!(debugPort->STATUS & (1 << USART_DREIF_bp)));
	 debugPort->DATA= byteToSend;
 }

 uint8_t receiveChar()
 {
	 return debugPort->DATA;
 }

void uartIntOn(uint8_t regValue)
{
	debugPort->CTRLA|= regValue;
}

void uartIntOff(uint8_t regValue)
{
	debugPort->CTRLA&= ~regValue;
}

 
 //TODO: Implement other functions and ISR