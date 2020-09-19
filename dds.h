/*
 * synth.h
 *
 *  Created on: 22 Apr 2017
 *      Author: dimtass
 */

#ifndef DDS_SYNTH_H_
#define DDS_SYNTH_H_
#include <stdint.h>
#include <dds_defs.h>



void DDS_Init(void);
void DDS_calculate(dmabuf_t * buffer, uint16_t buffer_size,
					uint32_t * phaseAccumulator_L, float frequency_L,
					uint32_t * phaseAccumulator_R, float frequency_R);

#endif /* SYNTH_SYNTH_H_ */
