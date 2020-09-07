#include "idds.h"
#include "Sawtooth.h"

/* Math definitions */

#define SAMPLE_RATE DDS_SAMPLE_RATE
/* Q16.16 */
#define FRAC 16
#define Q16(X) (X * (float)(1 << FRAC))

SawtoothWave::SawtoothWave()
{
    frequency_L = 500;
    frequency_R = 500;
    Generate();
}

SawtoothWave::SawtoothWave(float freq)
{
    frequency_L = freq;
    frequency_R = freq;
    Generate();
}

void SawtoothWave::Generate()
{
    // populate table table[k]=sin(2*pi*k/N)
    int i = 0;

    float length = TABLE_SIZE;
    float amplitude = 2;
    float delta = amplitude / length;
    for (i = 0; i < TABLE_SIZE; i++)
    {
        // calculating sine wave
        Sawtooth[i] = -(amplitude / 2) + delta * i;
    }
    /* set the last byte equal to first for interpolation */
    Sawtooth[TABLE_SIZE] = Sawtooth[0];
    phaseAccumulator_L = 0;
    phaseAccumulator_R = 0;
}

float SawtoothWave::DDS_calculate_channel_out(uint32_t *phaseAccumulator,
                                              uint32_t phaseIncrement, uint32_t index)
{
    /* Increment the phase accumulator */
    *phaseAccumulator += phaseIncrement;
    *phaseAccumulator &= TABLE_SIZE * (1 << 16) - 1;
    index = *phaseAccumulator >> 16;

    /* interpolation */
    float v1 = Sawtooth[index];
    float v2 = Sawtooth[index + 1];
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

void SawtoothWave::Calculate(dmabuf_t *buffer, uint16_t buffer_size)
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