/*
 * SDC.c
 *
 * Created: 14.03.2015 22:31:33
 *  Author: Disgust
 */ 

#include <stdlib.h>
#include "../AppConfig.h"
#include "SDC.h"

card_t * initCardObject(spiDevice_t * spiDev, PORT_t cardCtrl, uint8_t presPin, uint8_t pwrPin) {
	static card_t card01;
	card01.spiCard= spiDev;
	card01.CtrlPort= cardCtrl;
	card01.PresPin= presPin;
	card01.PwrPin= pwrPin;
	card01.readBuffer= NULL;
	card01.writeBuffer= NULL;
	return &card01;
}

void cardAttachBuffer(card_t * dev, uint8_t * buf, uint8_t R_W) {
	R_W?(dev->readBuffer= buf):(dev->writeBuffer= buf);
}

void cardDetachBuffer(card_t * dev, uint8_t R_W) {
	R_W?(dev->readBuffer= NULL):(dev->writeBuffer= NULL);
}

void cardPowerUp(card_t * card) {
	card->CtrlPort.OUTSET= card->PwrPin;
	card->state= IDLE;
}

void cardPowerDwn(card_t * card) {
	
}

void cardTurnOff(card_t * card)
{
	
}


void cardDenied(card_t * card) {
	card->state= INACTIVE;
	card->type= UNCKNOWN;
	cardPowerDwn(card);
}

void sendCom(uint8_t cmd, uint32_t arg) {
	
	Command_t msg= newCmd(cmd, arg);
	for (int i= 0; i < 5; i++)
	{
		SPI_WriteByte(msg.reg[i]);
	}
}

void sendAppCom(uint8_t cmd, uint32_t arg) {
	Command_t msg= newCmd(CMD55, 0);
	for (int i= 0; i < 5; i++)
	{
		SPI_WriteByte(msg.reg[i]);
	}
	SPI_FlushBuffer();
	sendCom(cmd, arg);
}

uint8_t cardCheck(card_t * card) // if CARD_PRESENCE
{
#ifdef USE_SPI

	cardPowerUp(card);
	chipSelect(card->spiCard);
	sendCom(CMD0, cmd0_arg);
	readR1Response();
	if (R1_response.reg != 0x01) { // if any bit except the first...
		cardDenied(card);
		return 0;
	}
	sendCom(CMD8, cmd8_arg(0x1, 0xAA));
	readR7Response();
	
	if (R7_response.parts.r1.reg >> 1 /*or if timeout occured*/)
	{
		//goto checking SD v1
			//then MMC v3
	}
	else if ((R7_response.parts.check_pattern != 0xAA)||(R7_response.parts.voltage_accepted != 0x01))
	{
		cardDenied(card);
		return 0;
	}
// 	sendCom(CMD58, cmd58_arg(0)); // It's not mandatory
// 	readR3Response();
// 	if (R3_response.part.OCR.bit.volt_2v7_2v8)
// 	{
// 		// Check supported voltage range
// 	}
	do {
		sendAppCom(ACMD41, acmd41_arg(1));
		readR1Response();
		if (R1_response.reg >> 1 /*or if timeout occured*/) {
			cardDenied(card);
			return 0;
		}
	} while (R1_response.reg != 0x00);
	SD_CRCoff();
	sendCom(CMD58, cmd58_arg);
	readR3Response();
	if (R3_response.part.OCR.bit.ccs)
	{
		card->type= SD_v2_HC;
		card->state= READY;
		return 1;
	}
	else {
		card->type= SD_v2_SC;
		card->state= READY;
		sendCom(CMD16, cmd16_arg(512)); // Force block size to 512 bytes for work with FAT systems
		return 1;
	}
	return 0;
#else
	cardDenied(card);
	return 0;
#endif
}

uint8_t readR1Response() {
	
	R1_response.reg= SPI_ReadByte();
	return 1;
}

uint8_t readR2Response() {
	
	R2_response.reg[1]= SPI_ReadByte();
	R2_response.reg[0]= SPI_ReadByte();
	return 1;
}

uint8_t readR3Response() {
	
	for (uint8_t i= 4; i >= 0; i--)
	{
		R3_response.reg[i]= SPI_ReadByte();
	}
	return 1;
}

uint8_t readR7Response() {
	
	for (uint8_t i= 4; i >= 0; i--)
	{
		R7_response.reg[i]= SPI_ReadByte();
	}
	return 1;
}

uint8_t readDataResponse() {
	uint8_t a= SPI_ReadByte();
	DT_resp_tk= *((struct resp_str *)&a);
	return 1;
}

void SD_SoftReset(card_t * card)
{
	chipSelect(card->spiCard);
	SPI_WriteByte(0x40); // CMD0 + start bits
	SPI_WriteByte(0x0);
	SPI_WriteByte(0x0);
	SPI_WriteByte(0x0);
	SPI_WriteByte(0x0);
	SPI_WriteByte(0x95); // CRC
	// TODO: change to command type sending
	card->state= IDLE;
}

void SD_CRCon() {
	sendCom(CMD59, cmd59_arg(1));
}

void SD_CRCoff() {
	sendCom(CMD59, cmd59_arg(0));
}

uint8_t SD_blockRead(card_t * card, uint32_t addr) {
	chipSelect(card->spiCard);
	sendCom(CMD17, cmd17_arg(addr));
	readR1Response();
	if (R1_response.reg) {
		ErrorTk_t errorTk;
		errorTk.reg= SPI_ReadByte();
		chipRelease(card->spiCard);
		return 0;
	}
	else {
		/*READ 512 byte block to buffer*/
		/*check it's CRC*/
		chipSelect(card->spiCard);
		for (int i= 0; i < BUFFER_SIZE; i++) {
			*(card->readBuffer++)= SPI_ReadByte();
		}
		chipRelease(card->spiCard);
		return 1;
	}
	chipRelease(card->spiCard);
	return 0;
}

uint8_t SD_multBlockRead(card_t * card, uint32_t addr, uint8_t numBlocks, uint8_t * buffer, uint8_t length) {
	
	
	
	
	return 0;
}

uint8_t SD_blockWrite( card_t * card, uint32_t addr)
{
	chipSelect(card->spiCard);
	sendCom(CMD24, cmd24_arg(addr));
	readR1Response();
	if (R1_response.reg) {
		ErrorTk_t errorTk;
		errorTk.reg= SPI_ReadByte();
		chipRelease(card->spiCard);
		return 0;
	}
	else {
		/*send start block token*/
		SPI_WriteByte(CMD17_18_24_TK);
		for (int i= 0; i < BUFFER_SIZE; i++) {
			SPI_WriteByte(*(card->writeBuffer++));
		}
		readDataResponse();
		if (DT_resp_tk.status != DATA_ACCEPTED)
		{
			sendCom(CMD13, cmd13_arg);
			readR2Response();
			chipRelease(card->spiCard);
			return 0;
		}
		
		chipRelease(card->spiCard);
		return 1;
	}
	chipRelease(card->spiCard);
	return 0;	
}


Command_t newCmd( uint8_t cmd, uint32_t arg )
{
	Command_t msg;
	msg.bit.start_bit= 0;
	msg.bit.transmission_bit= 1;
	msg.bit.cmd= cmd;
	msg.bit.arg= arg;
	msg.bit.end_bit= 0;
	msg.bit.crc= crc7_arr(msg.bit.crc, 6, msg.reg);
	return msg;
}


