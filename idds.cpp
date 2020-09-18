/*
 * idds.cpp
 *
 *  Created on: 14/09/2020
 *      Author: kevin
 */


#include "idds.h"
#define FRAC 16
#define Q16(X) (X * (float)(1 << FRAC))
#define SAMPLE_RATE DDS_SAMPLE_RATE

IWaveForm::IWaveForm(float freq)
{
	frequency_L = freq;
	frequency_R = freq;

}


void IWaveForm::SetFreq(float freq){

	frequency_L = freq;
	frequency_R = freq;

	phaseIncrement_L = Q16(
			(float)frequency_L * TABLE_SIZE / SAMPLE_RATE);
	phaseIncrement_R = Q16(
			(float)frequency_R * TABLE_SIZE / SAMPLE_RATE);

}

float IWaveForm::DDS_calculate_channel_out(uint32_t *phaseAccumulator,
		uint32_t phaseIncrement, uint32_t index)
{
	/* Increment the phase accumulator */
	*phaseAccumulator += phaseIncrement;
	*phaseAccumulator &= TABLE_SIZE * (1 << 16) - 1;
	index = *phaseAccumulator >> 16;

	/* interpolation */
	float v1 = Table[index];
	float v2 = Table[index + 1];
	float fmul = (*phaseAccumulator & 65535) / 65536.0f;
	//		float fmul = fix32_frac(phaseAccumulator, 16);
	float out = v1 + (v2 - v1) * fmul;

	out = out * 32768.0f;
	if (out > 32767)
		out = 32767;
	if (out < -32768)
		out = -32768;

	return (out);
}

void IWaveForm::Calculate(dmabuf_t *buffer, uint16_t buffer_size)
{

	uint32_t index_L = 0;
	uint32_t index_R = 0;


	int i = 0;

	for (i = 0; i < buffer_size; i++)
	{
		/* Left channel */
		buffer[i].LEFT.W16 = (int16_t)DDS_calculate_channel_out(&phaseAccumulator_L, phaseIncrement_L, index_L);
		/* Right channel */
		buffer[i].RIGHT.W16 = (int16_t)DDS_calculate_channel_out(&phaseAccumulator_R, phaseIncrement_R, index_R);
	}
}
