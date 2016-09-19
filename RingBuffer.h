#ifndef _RingBuffer_H_
#define _RingBuffer_H_

#include <stdint.h>

#define BUFSIZE 34
#define RINGBUFFSIZE 34
#define RINGBUFFLENGTH (RINGBUFFSIZE + 1)

class RingBuffer {

	public:
		char ringBufferVariable[RINGBUFFLENGTH];
		char temp[34];
		char *readout;
		
		unsigned int head, tail;
		
		RingBuffer(void) {
			readout = temp;
			head = tail = 0;
		}
		
		uint32_t ringBufferWrite(char in);
		uint32_t ringBufferStringWrite(char in[]);
		uint32_t ringBufferRead(char *out);
		uint32_t ringBufferStringRead(char *out);
};

#endif
