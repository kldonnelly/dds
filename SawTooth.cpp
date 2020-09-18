#include "idds.h"
#include "Sawtooth.h"

/* Math definitions */

#define SAMPLE_RATE DDS_SAMPLE_RATE
/* Q16.16 */
#define FRAC 16
#define Q16(X) (X * (float)(1 << FRAC))

SawtoothWave::SawtoothWave()
{

    Generate();
}

SawtoothWave::SawtoothWave(float freq):IWaveForm(freq)
{

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
    	Table[i] = -(amplitude / 2) + delta * i;
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


