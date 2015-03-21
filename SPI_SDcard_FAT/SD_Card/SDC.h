/*
 * SDC.h
 *
 * Created: 14.03.2015 22:31:45
 *  Author: Disgust
 */ 


#ifndef SDC_H_
#define SDC_H_

#define USE_SPI
//#define USE_SDIO

#include "crc7.h"
#include "../r_buffer.h"

/*
CRC7

Generator polynomial: G(x) = x^7 + x^3 + 1.
M(x) = (first bit) * x^n + (second bit) * x^n - 1 +...+ (last bit) * x^0
CRC[6...0] = Remainder [(M(x) * x^7) / G(x)]
*/

#if defined(USE_SPI)
	#include "../SPI/spi_device.h"
#endif

typedef enum {
	INACTIVE,
	IDLE,
	READY,
	IDENTIFICATION,
	STAND_BY,
	TRANSFER,
	SENDING_DATA,
	RECEIVE_DATA,
	PROGRAMMING,
	DISCONNECT
} card_state_t;

typedef enum {
	UNCKNOWN,
	SD_v1,
	SD_v2_SC,
	SD_v2_HC,
	MMC_v3
	} card_type_t;

typedef struct {
	spiDevice_t * spiCard;
	PORT_t CtrlPort;
	uint8_t PwrPin;
	uint8_t PresPin;
	card_type_t type;
	card_state_t state;
	ring_buffer_t readBuffer;
	ring_buffer_t writeBuffer;
	} card_t;

// enum {
// 	CHECK_FUNC= 0,
// 	SWITCH_FUNC= 1
// 	} MODE_FUNC;

// typedef enum {
// 	
// 	} error_t;

	
typedef enum {
	INACTIVE_MODE,
	CARD_IDENTIFICATION_MODE,
	DATA_TRANSFER_MODE
	} card_opmode_t;

#define STUFF 0x00000000

// SD Card Commands
#if defined(USE_SPI)

	#define CMD0	0b000000
		#define cmd0_arg		(uint32_t)(STUFF)			// GO_IDLE_STATE - R1
	#define CMD1	0b000001
		#define cmd1_arg(HCS)	(uint32_t)(0x0000|(HCS << 30))		// SEND_OP_COND - R1
	#define CMD6	0b000110
		#define cmd6_arg(MODE_FUNC)	(uint32_t)(0x0000|(MODE_FUNC << 31)|\
									(FUNCGRP_6 << 20)|(FUNCGRP_5 << 16)|\
									(FUNCGRP_4 << 12)|(FUNCGRP_3 << 8)|\
									(FUNCGRP_2 << 4)|FUNCGRP_1)	// SWITCH_FUNC - R1
	#define CMD8	0b001000
		#define cmd8_arg(VHS,CH_PTRN)	(uint32_t)(0x0000|(VHS << 8)|CH_PTRN)	// SEND_IF_COND - R7
	#define CMD9	0b001001
		#define cmd9_arg		(uint32_t)(STUFF)			// SEND_CSD - R1
	#define CMD10	0b001010
		#define cmd10_arg	(uint32_t)(STUFF)			// SEND_CID - R1
	#define CMD12	0b001100
		#define cmd12_arg	(uint32_t)(STUFF)			// STOP_TRANSMISSION - R1b
	#define CMD13	0b001101
		#define cmd13_arg	(uint32_t)(STUFF)			// SEND_STATUS - R2
	#define CMD16	0b010000
		#define cmd16_arg(BLK_LENGTH)	(uint32_t)(BLK_LENGTH)		// SET_BLOCKLEN - R1
	#define CMD17	0b010001
		#define cmd17_arg(ADDR)		(uint32_t)(ADDR)			// READ_SINGLE_BLOCK - R1
	#define CMD18	0b010010
		#define cmd18_arg(ADDR)		(uint32_t)(ADDR)			// READ_MULTIPLE_BLOCK - R1
	#define CMD24	0b011000
		#define cmd24_arg(ADDR)		(uint32_t)(ADDR)			// WRITE_BLOCK - R1
	#define CMD25	0b011001
		#define cmd25_arg(ADDR)		(uint32_t)(ADDR)			// WRITE_MULTIPLE_BLOCK - R1
	#define CMD27	0b011011
		#define cmd27_arg	(uint32_t)(STUFF)			// PROGRAM_CSD - R1
	#define CMD28	0b011100
		#define cmd28_arg(ADDR)		(uint32_t)(ADDR)			// SET_WRITE_PROT - R1b
	#define CMD29	0b011101
		#define cmd29_arg(ADDR)		(uint32_t)(ADDR)			// CLR_WRITE_PROT - R1b
	#define CMD30	0b011110
		#define cmd30_arg(ADDR)		(uint32_t)(ADDR)			// SEND_WRITE_PROT - R1
	#define CMD32	0b100000
		#define cmd32_arg(ADDR)		(uint32_t)(ADDR)			// ERASE_WR_BLK_START - R1
	#define CMD33	0b100001
		#define cmd33_arg(ADDR)		(uint32_t)(ADDR)			// ERASE_WR_BLK_END - R1
	#define CMD38	0b100110
		#define cmd38_arg	(uint32_t)(STUFF)			// ERASE - R1b
	#define CMD42	0b101010
		#define cmd42_arg			(uint32_t)(0x0000)			//LOCK_UNLOCK - R1
	#define CMD55	0b110111
		#define cmd55_arg	(uint32_t)(STUFF)			// APP_CMD - R1
	#define CMD56	0b111000
		#define cmd56_arg(RD_WR)	(uint32_t)((STUFF << 1)|RD_WR)		// GEN_CMD - R1
	#define CMD58	0b111010
		#define cmd58_arg	(uint32_t)(STUFF)			// READ_OCR - R3
	#define CMD59	0b111011
		#define cmd59_arg(CRC_OPT)	(uint32_t)((STUFF << 1)|CRC_OPT) // CRC_ON_OFF - R1
	
	
	// App specific
	#define ACMD13	
		#define acmd13_arg	(uint32_t)(STUFF)			// SD_STATUS - R2
	#define ACMD18
	#define ACMD22	
		#define acmd22_arg	(uint32_t)(STUFF)			// SEND_NUM_WR_BLOCKS - R1
	#define ACMD23	
		#define acmd23_arg(NUM_OF_BLKS)	(uint32_t)((STUFF << 23)|NUM_OF_BLKS)	// SET_WR_BLOCK_ERASE_COUNT - R1
	#define ACMD25
	#define ACMD26
	#define ACMD38
	#define ACMD41	0b101001	
		#define acmd41_arg(x)	(uint32_t)(0x0000|(x << 30))	// APP_SEND_OP_COND - R1
	#define ACMD42	 
		#define acmd42_arg(SET_CD)	(uint32_t)((STUFF << 1)|SET_CD)	// SET_CLR_CARD_DETECT - R1
	#define ACMD43
	#define ACMD44
	#define ACMD45
	#define ACMD46
	#define ACMD47
	#define ACMD48
	#define ACMD49
	#define ACMD51	
		#define acmd51_arg	(uint32_t)(STUFF)				// SEND_SCR - R1
	

#elif defined(USE_SDIO)
	#define CMD2					// ALL_SEND_CID
	#define CMD3					// SEND_RELATIVE_ADDR
	#define CMD4					// SET_DSR
	#define CMD5
	#define CMD7					// SELECT/DESELECT_CARD
	#define CMD11
	#define CMD15					// GO_INACTIVE_STATE
	#define CMD19					// SEND_TUNING_BLOCK
	#define CMD20					// SPEED_CLASS_CONTROL
	#define CMD23					// SET_BLOCK_COUNT
	#define CMD26
	#define CMD39
	#define CMD40
	#define CMD52
	#define CMD53
	#define CMD54
	
	// App specific
	#define ACMD6	// SET_BUS_WIDTH
#endif

// Card registers
typedef struct {
	uint8_t year;
	uint8_t mounth : 4;
	} date_t;

typedef struct  
{
	uint8_t manufacturerId;
	char OEM_applicationId[2];
	char productName[5];
	uint8_t productRevisionH : 4;
	uint8_t productRevisionL : 4;
	uint32_t serialNumber;
	uint8_t reserved : 4;
	date_t manufacturingDate;
	uint8_t crc7 : 7;
	uint8_t bit0 : 1;
} CID_REG_t;

typedef struct
{
	uint8_t CSD_STRUCTURE : 2;
	uint8_t reserved1 : 6;
	uint8_t TAAC;
	uint8_t NSAC;
	uint8_t TRAN_SPEED;
	uint16_t CCC : 12;
	uint8_t READ_BL_LEN : 4;
	uint8_t READ_BL_PARTIAL : 1;
	uint8_t WRITE_BLK_MISALIGN : 1;
	uint8_t READ_BLK_MISALIGN : 1;
	uint8_t DSR_IMP : 1;
	uint8_t reserved2 : 2;
	uint16_t C_SIZE : 12;
	uint8_t VDD_R_CURR_MIN : 3;
	uint8_t VDD_R_CURR_MAX : 3;
	uint8_t VDD_W_CURR_MIN : 3;
	uint8_t VDD_W_CURR_MAX : 3;
	uint8_t C_SIZE_MULT : 3;
	uint8_t ERASE_BLK_EN : 1;
	uint8_t SECTOR_SIZE : 7;
	uint8_t WP_GRP_SIZE : 7;
	uint8_t WP_GRP_ENABLE : 1;
	uint8_t reserved3 : 3;
	uint8_t R2W_FACTOR : 3;
	uint8_t WRITE_BL_LEN : 4;
	uint8_t WRITE_BL_PARTIAL : 1;
	uint8_t reserved4 : 5;
	uint8_t FILE_FORMAT_GRP : 1;
	uint8_t COPY : 1;
	uint8_t PERM_WRITE_PROTECT : 1;
	uint8_t TMP_WRITE_PROTECT : 1;
	uint8_t FILE_FORMAT : 2;
	uint8_t reserved5 : 2;
	uint8_t CRC_sum : 7;
	uint8_t bit0 : 1;
} CSD_v1_REG_t;

typedef struct
{
	uint8_t CSD_STRUCTURE : 2;
	uint8_t reserved1 : 6;
	uint8_t TAAC;
	uint8_t NSAC;
	uint8_t TRAN_SPEED;
	uint16_t CCC : 12;
	uint8_t READ_BL_LEN : 4;
	uint8_t READ_BL_PARTIAL : 1;
	uint8_t WRITE_BLK_MISALIGN : 1;
	uint8_t READ_BLK_MISALIGN : 1;
	uint8_t DSR_IMP : 1;
	uint8_t reserved2 : 6;
	uint32_t C_SIZE : 22;
	uint8_t reserved3 : 1;
	uint8_t ERASE_BLK_EN : 1;
	uint8_t SECTOR_SIZE : 7;
	uint8_t WP_GRP_SIZE : 7;
	uint8_t WP_GRP_ENABLE : 1;
	uint8_t reserved4 : 2;
	uint8_t R2W_FACTOR : 3;
	uint8_t WRITE_BL_LEN : 4;
	uint8_t WRITE_BL_PARTIAL : 1;
	uint8_t reserved5 : 5;
	uint8_t FILE_FORMAT_GRP : 1;
	uint8_t COPY : 1;
	uint8_t PERM_WRITE_PROTECT : 1;
	uint8_t TMP_WRITE_PROTECT : 1;
	uint8_t FILE_FORMAT : 2;
	uint8_t reserved6 : 2;
	uint8_t CRC_sum : 7;
	uint8_t bit0 : 1;
} CSD_v2_REG_t;

typedef struct {
	uint8_t SCR_Struct : 4;
	uint8_t SD_Spec : 4;
	uint8_t DATA_STAT_AFTER_ERASE : 1;
	uint8_t SD_SECURITY : 3;
	uint8_t SD_BUS_WIDTHS : 4;
	uint8_t SD_SPEC3: 1;
	uint8_t EX_SECURITY : 4;
	uint8_t SD_SPEC4 :1;
	uint8_t reserved1 : 6;
	uint8_t CMD_SUPPORT : 4;
	uint32_t reserved2;
	} SCR_REG_t;
	

// Command token
typedef union {
	 struct {
		uint8_t start_bit : 1; // always is 0
		uint8_t transmission_bit : 1; // always is 1
		uint8_t cmd : 6;
		uint32_t arg;
		uint8_t crc : 7;
		uint8_t end_bit :1; // always is 1
	} bit;
	uint8_t reg[6];
} Command_t;

Command_t newCmd(uint8_t cmd, uint32_t arg);

// Data packet
typedef struct {
	uint8_t token;
	uint8_t * data;
	uint16_t crc;
	}SDC_Packet_t;
	

typedef union {
	struct {
		Command_t cmd;
		SDC_Packet_t packet;
		};
		
	}SDC_Message_t;

// Data response tokens

typedef union {
	struct {
		uint8_t bit7 : 1;				// always 0
		uint8_t parameter_err : 1;
		uint8_t address_err : 1;
		uint8_t erase_sequence_err : 1;
		uint8_t comm_crc_err : 1;
		uint8_t illegal_command : 1;
		uint8_t erase_rst : 1;
		uint8_t in_idle_state : 1;
	} bit;
	uint8_t reg;
} R1_t;

typedef union {
	struct {
		uint16_t reserved1		: 15;
		uint8_t volt_2v7_2v8	: 1;
		uint8_t volt_2v8_2v9	: 1;
		uint8_t volt_2v9_3v0	: 1;
		uint8_t volt_3v0_3v1	: 1;
		uint8_t volt_3v1_3v2	: 1;
		uint8_t volt_3v2_3v3	: 1;
		uint8_t volt_3v3_3v4	: 1;
		uint8_t volt_3v4_3v5	: 1;
		uint8_t volt_3v5_3v6	: 1;
		uint8_t s18a			: 1;
		uint8_t reserved2		: 4;
		uint8_t uhs2_stat		: 1;
		uint8_t ccs				: 1;
		uint8_t power_up_stat	: 1;
	} bit;
	uint8_t reg[4];
} ocr_reg_t;

	// R1 format
R1_t R1_response;

	// R1b format
R1_t R1b_response; // bit7 is a BUSY bit

	// R2 format
union {
	struct {
		R1_t r1;
		uint8_t out_of_range_csd_overwrite : 1;
		uint8_t erase_param : 1;
		uint8_t wp_violation : 1;
		uint8_t card_ecc_failed : 1;
		uint8_t CC_err : 1;
		uint8_t error : 1;
		uint8_t wp_erase_skip_lock_unlock_cmd_failed : 1;
		uint8_t card_is_locked : 1;
	} bit;
	uint8_t reg[2];
} R2_response;

	// R3 format  -  This response token is sent by the card when a READ_OCR command is received
union {
	struct {
		R1_t r1;
		ocr_reg_t OCR;
	} part;
	uint8_t reg[5];
} R3_response;

	// R4 & R5 are in the SDIO mode
	
	// R7 format  -  This response token is sent by the card when a SEND_IF_COND command (CMD8) is received.	
union {
	struct {
		R1_t r1;
		uint8_t comm_version : 4;
		const uint16_t reserved : 1;
		uint8_t voltage_accepted : 4;
		uint8_t check_pattern;
		}parts;
	uint8_t reg[5];
	} R7_response;


// COntrol tokens
	//Data response token
struct {
	uint8_t not_used : 3;
	uint8_t bit4 : 1;
	uint8_t status : 3;
	uint8_t bit0 : 1;
	} DT_resp_tk;

#define DATA_ACCEPTED	(0b010)
#define CRC_ERROR		(0b101)
#define WRITE_ERROR		(0b110)



// Data start tokens
#define CMD17_18_24_TK		0b1111110
#define CMD25_TK			0b1111100
#define CMD25_ST_TK			0b1111101 // Stop tran for com25

//Error token
typedef union {
	struct{
		uint8_t not_used : 4;
		uint8_t out_of_range : 1;
		uint8_t ecc_failed : 1;
		uint8_t cc_err : 1;
		uint8_t error : 1;
	} bit;
	uint8_t reg;
} ErrorTk_t;






// Function definitions

card_t * initCardObject(spiDevice_t *, PORT_t, uint8_t, uint8_t);
void cardAttachBuffer(card_t * dev, ring_buffer_t buf, uint8_t R_W);
void cardDetachBuffer(card_t * dev, uint8_t R_W);

void cardPowerUp(card_t *);
void cardPowerDwn(card_t *);
void cardTurnOff(card_t *);
void cardDenied(card_t *);

uint8_t cardCheck(card_t *);
uint16_t cardIDread();

void SD_SoftReset();
void SD_init();

uint8_t SD_blockRead(card_t *, uint32_t);
uint8_t SD_multBlockRead(card_t *, uint32_t, uint8_t, uint8_t *, uint8_t);
uint8_t SD_blockWrite(card_t *, uint32_t);
uint8_t SD_multBlockWrite();

void SD_CRCon();
void SD_CRCoff();

uint8_t readR1Response();
uint8_t readR2Response();
uint8_t readR3Response();
uint8_t readR7Response();
uint8_t readDataResponse();

#endif /* SDC_H_ */