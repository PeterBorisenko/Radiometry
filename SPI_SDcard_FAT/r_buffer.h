#pragma once

#include <stdint.h>

typedef struct {
	uint8_t * buffer;	// storage address
	uint8_t length;		// buffer length (max counter = length - 1)
	uint8_t counter;	// next empty element (accessing : *(buffer+counter))
} ring_buffer_t;

void initRBuffer(ring_buffer_t *, uint8_t *, uint8_t);

