/*
 * synth.c
 *
 *  Created on: 22 Apr 2017
 *      Author: dimtass
 */

#include <math.h>
//#include "platform_config.h"
#include <stdint.h>
#include "dds.h"

#define SAMPLE_RATE DDS_SAMPLE_RATE
#define TABLE_SIZE 1024						  // size of wavetable
#define TABLE_FREQ (SAMPLE_RATE / TABLE_SIZE) // frequency of one wavetable period

/* Math definitions */
#define _2PI 6.283185307f
#define _PI 3.14159265f

/* Q16.16 */
#define FRAC 16
#define Q16(X) (X * (float)(1 << FRAC))
#define QFLOAT(X) (X / (float)(1 << FRAC))
#define Q16_TO_UINT(X) ((uint32_t)X / (1 << FRAC))

//static float _sine[TABLE_SIZE+1];	// +1 for interpolation
static float sine[TABLE_SIZE + 1];	  // +1 for interpolation
static uint32_t phaseAccumulator = 0; // fixed-point (16.16) phase accumulator
int32_t triangle[TABLE_SIZE] = {0};

void generateTriangle(int32_t amplitude, int32_t *buffer, uint16_t length)
{
	// Generate a triangle wave signal with the provided amplitude and store it in
	// the provided buffer of size length.
	float delta = (float)amplitude / (float)length;
	for (int i = 0; i < length / 2; ++i)
	{
		buffer[i] = -(amplitude / 2) + delta * i;
	}
	for (int i = length / 2; i < length; ++i)
	{
		buffer[i] = (amplitude / 2) - delta * (i - length / 2);
	}
}

void sine_init(void)
{
	// populate table table[k]=sin(2*pi*k/N)
	int i = 0;
	for (i = 0; i < TABLE_SIZE; i++)
	{
		// calculating sine wave
		sine[i] = sinf(_2PI * ((float)i / TABLE_SIZE));
	}
	/* set the last byte equal to first for interpolation */
	sine[TABLE_SIZE] = sine[0];
	phaseAccumulator = 0;
}

void DDS_Init(void)
{
	sine_init();
}

static inline float DDS_calculate_channel_out(uint32_t *phaseAccumulator,
									   uint32_t phaseIncrement, uint32_t index)
{
	/* Increment the phase accumulator */
	*phaseAccumulator += phaseIncrement;
	*phaseAccumulator &= TABLE_SIZE * (1 << 16) - 1;
	index = *phaseAccumulator >> 16;

	/* interpolation */
	float v1 = sine[index];
	float v2 = sine[index + 1];
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

void DDS_calculate(dmabuf_t *buffer, uint16_t buffer_size,
				   uint32_t *phaseAccumulator_L, float frequency_L,
				   uint32_t *phaseAccumulator_R, float frequency_R)
{
	uint32_t index_L = 0;
	uint32_t index_R = 0;

	uint32_t phaseIncrement_L = Q16(
		(float)frequency_L * TABLE_SIZE / SAMPLE_RATE);
	uint32_t phaseIncrement_R = Q16(
		(float)frequency_R * TABLE_SIZE / SAMPLE_RATE);
	int i = 0;

	for (i = 0; i < buffer_size; i++)
	{
		/* Left channel */
		buffer[i].LEFT.W16 = (int16_t)DDS_calculate_channel_out(
			phaseAccumulator_L, phaseIncrement_L, index_L);
		/* Right channel */
		buffer[i].RIGHT.W16 = (int16_t)DDS_calculate_channel_out(
			phaseAccumulator_R, phaseIncrement_R, index_R);
	}
}
