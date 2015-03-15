#ifndef SPI_AVRXMEGA_H
#define SPI_AVRXMEGA_H

#ifdef	__cplusplus
extern "C" {
#endif

	#include <avr/io.h>
	#ifdef SPIC
	#undef SPIC
#endif

	#define SPI_PORT            PORTC

// 	#define SDI_DDR             
// 	#define SDO_DDR             
// 	#define SCK_DDR             

	#define PIN_OUTPUT          1
	#define PIN_INPUT           0

	#define SDI_PIN             (1 << 6)
	#define SDO_PIN             (1 << 5)
	#define SCK_PIN             (1 << 7)
	#define SS_PIN              (1 << 4)

	typedef union {
		struct {
			uint8_t CLK2X		: 1;
			uint8_t ENABLE		: 1;
			uint8_t DORD		: 1;
			uint8_t MASTER		: 1;
			uint8_t MODE		: 2;
			uint8_t PRESCALER	: 2;
		} bit;
		uint8_t reg;
	} SPI_CTRL_REG_t;
	
	typedef union {
		struct {
			uint8_t reserved	: 6;
			uint8_t INTLVL		: 2;
		} bit;
		uint8_t reg;
	} SPI_INTCTRL_REG_t;

	typedef union {
		struct {
			uint8_t IF			: 1;
			uint8_t WRCOL		: 1;
			uint8_t reserved	: 6;
		} bit;
		uint8_t reg;
	} SPI_STATUS_REG_t;
	
	typedef uint8_t SPI_DATA_REG_t;
	
	typedef struct {
		SPI_CTRL_REG_t SPI_CTRL_REG;
		SPI_INTCTRL_REG_t SPI_INTCTRL_REG;
		SPI_STATUS_REG_t SPI_STATUS_REG;
		SPI_DATA_REG_t SPI_DATA_REG;
	} SPI_C_t;
	
	SPI_C_t *SPI_C= 0x08C0;
	
	// SPI MODES
	#define SPI_MODE_0			0x00	// CPOL= 0, CPHA= 0, 
	#define SPI_MODE_1			0x01	// CPOL= 0, CPHA= 1,
	#define SPI_MODE_2			0x02	// CPOL= 1, CPHA= 0,
	#define SPI_MODE_3			0x03	// CPOL= 1, CPHA= 1,

	// SPI REGISTERS/bits
	#define SPI_BUFFER          (SPI_C->SPI_DATA_REG)
	#define SPI_BUFFER_EMPTY    (SPI_C->SPI_STATUS_REG.bit.IF)
	#define SPI_DATA_RECEIVED	(SPI_C->SPI_STATUS_REG.bit.IF)
	#define SPI_ENABLE          (SPI_C->SPI_CTRL_REG.bit.ENABLE)
	#define SPI_MODE            (SPI_C->SPI_CTRL_REG.bit.MODE)
	#define SPI_PHASE           0
	#define SPI_CLOCK_EDGE      0
	#define SPI_PRESCALER       (SPI_C->SPI_CTRL_REG.bit.PRESCALER)

	#ifdef	__cplusplus
}
#endif

#endif /*SPI_AVRXMEGA_H*/