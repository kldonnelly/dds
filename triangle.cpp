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

TriangleWave::TriangleWave(float freq):IWaveForm(freq)
{

	  Generate();
}

void TriangleWave::Generate()
{

    float length = TABLE_SIZE;
    float amplitude = 2;
    float delta = amplitude * 2 / length;
    for (int i = 0; i < TABLE_SIZE / 2; i++)
    {
    	Table[i] = -(amplitude / 2) + delta * (float)i;
    }
    for (int i = TABLE_SIZE / 2; i < TABLE_SIZE; i++)
    {
    	Table[i] = (amplitude / 2) - delta * ((float)i - length / 2);
    }
    /* set the last byte equal to first for interpolation */
    Table[TABLE_SIZE] = Table[0];
    phaseAccumulator_L = 0;
    phaseAccumulator_R = 0;

    phaseIncrement_L = Q16(
       (float)frequency_L * TABLE_SIZE / SAMPLE_RATE);
    phaseIncrement_R = Q16(
       (float)frequency_R * TABLE_SIZE / SAMPLE_RATE);
}
