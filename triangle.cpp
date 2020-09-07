#include "idds.h"
#include "triangle.h"
#include <math.h>
/* Math definitions */
#define _2PI 6.283185307f
#define _PI 3.14159265f
#define SAMPLE_RATE DDS_SAMPLE_RATE
/* Q16.16 */
#define FRAC 16
#define Q16(X) (X * (float)(1 << FRAC))

TriangleWave::TriangleWave()
{
    frequency_L = 500;
    frequency_R = 500;
    Generate();
}

TriangleWave::TriangleWave(float freq)
{
    frequency_L = freq;
    frequency_R = freq;
    Generate();
}

void TriangleWave::Generate()
{

    float length = TABLE_SIZE;
    float amplitude = 2;
    float delta = amplitude * 2 / length;
    for (int i = 0; i < TABLE_SIZE / 2; i++)
    {
        Triangle[i] = -(amplitude / 2) + delta * (float)i;
    }
    for (int i = TABLE_SIZE / 2; i < TABLE_SIZE; i++)
    {
        Triangle[i] = (amplitude / 2) - delta * ((float)i - length / 2);
    }
    /* set the last byte equal to first for interpolation */
    Triangle[TABLE_SIZE] = Triangle[0];
    phaseAccumulator_L = 0;
    phaseAccumulator_R = 0;
}

float TriangleWave::DDS_calculate_channel_out(uint32_t *phaseAccumulator,
                                              uint32_t phaseIncrement, uint32_t index)
{
    /* Increment the phase accumulator */
    *phaseAccumulator += phaseIncrement;
    *phaseAccumulator &= TABLE_SIZE * (1 << 16) - 1;
    index = *phaseAccumulator >> 16;

    /* interpolation */
    float v1 = Triangle[index];
    float v2 = Triangle[index + 1];
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

void TriangleWave::Calculate(dmabuf_t *buffer, uint16_t buffer_size)
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
        buffer[i].LEFT.W16 = (int16_t)DDS_calculate_channel_out(&phaseAccumulator_L, phaseIncrement_L, index_L);
        /* Right channel */
        buffer[i].RIGHT.W16 = (int16_t)DDS_calculate_channel_out(&phaseAccumulator_R, phaseIncrement_R, index_R);
    }
}