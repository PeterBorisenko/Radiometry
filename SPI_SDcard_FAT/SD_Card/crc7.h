#ifndef CRC7_H
#define CRC7_H
#include <avr/pgmspace.h>
	uint8_t crc7_byte(uint8_t crc, uint8_t datum);
	uint8_t crc7_arr(uint8_t crc, uint8_t len, const uint8_t * data);
#endif