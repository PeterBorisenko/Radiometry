/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2014      */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "../SD_Card/SDC.h"

extern card_t * pCard01;

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (void)
{
	DSTATUS stat;
	
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Partial Sector                                                   */
/*-----------------------------------------------------------------------*/

DRESULT disk_readp (
	BYTE* buff,		/* Pointer to the destination object */
	DWORD sector,	/* Sector number (LBA) */
	UINT offset,	/* Offset in the sector */
	UINT count		/* Byte count (bit15:destination) */
)
{
	DRESULT res;
	res= SD_blockRead(pCard01, sector*512);
	if (res)
	{
		do {
			*buff++ = *(pCard01->readBuffer + offset++);
		} while (--count); // read only needed count of bytes
	}
	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Partial Sector                                                  */
/*-----------------------------------------------------------------------*/

DRESULT disk_writep (
	BYTE* buff,		/* Pointer to the data to be written, NULL:Initiate/Finalize write operation */
	DWORD sc		/* Sector number (LBA) or Number of bytes to send */
)
{
	DRESULT res= RES_ERROR;
	DWORD wc;

	if (!buff) {
		if (sc) {/* Initiate sector write process */
			if (!(pCard01->type= SD_v2_HC)) sc *= 512;	/* Convert to byte address if needed */
				chipSelect(pCard01->spiCard);
				sendCom(CMD24, sc);		/* WRITE_SINGLE_BLOCK */
				SPI_WriteByte(0xFF); SPI_WriteByte(0xFE);		/* Data block header */
				chipRelease(pCard01->spiCard);
				SPI_FlushBuffer();
				wc = 512;							/* Set byte counter */
				res = RES_OK;
		} else {/* Finalize sector write process */
			wc+= 2;
			chipSelect(pCard01->spiCard);
			while (wc--) SPI_WriteByte(0);	/* Fill left bytes and CRC with zeros */
			if ((SPI_ReadByte() & 0x1F) == 0x05) {	/* Receive data resp and wait for end of write process in timeout of 500ms */
				int i;
				for (i = 5000; SPI_ReadByte() != 0xFF && i; i--) {
					_delay_us(100);
				/* Wait ready */
				}
				if (i) res = RES_OK;
			}
			chipRelease(pCard01->spiCard);
			SPI_FlushBuffer();
		}
	} else {
		pCard01->writeBuffer= buff;
		if (SD_blockWrite(pCard01, sc*512)) { // TODO: test this (maybe we need to check card type and use another
														// type of write if it is a block - enabled card)
			res= RES_OK;
		}
	}

	return res;
}

