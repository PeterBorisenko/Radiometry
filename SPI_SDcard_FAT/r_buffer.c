/*
 * CFile1.c
 *
 * Created: 20.03.2015 14:49:27
 *  Author: Disgust
 */ 

#include "r_buffer.h"

void initRBuffer(ring_buffer_t * buf, uint8_t * place, uint8_t size)
{
	buf->buffer= place;
	buf->counter= 0;
	buf->length= size;
}
