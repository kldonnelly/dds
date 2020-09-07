/*
 * dds_defs.h
 *
 *  Created on: 6 May 2017
 *      Author: dimtass
 */

#ifndef DDS_DEFS_H_
#define DDS_DEFS_H_

#define DDS_BUFF_SIZE 		128
#define DDS_SAMPLE_RATE		44100



typedef union{
	int16_t W16;
	uint8_t  SH16[2];		// 0 - low half-word, 1 - high
} dmasmp_t;


typedef struct __attribute__((__packed__)) {
	 dmasmp_t  LEFT;
	 dmasmp_t  RIGHT;
} dmabuf_t;

#endif /* DDS_DEFS_H_ */
