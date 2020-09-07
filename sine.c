#include "idds.h"
#include "sine.h"
#include <math.h>
/* Math definitions */
#define _2PI 6.283185307f
#define _PI 3.14159265f
#define SAMPLE_RATE DDS_SAMPLE_RATE
/* Q16.16 */
#define FRAC 16
#define Q16(X) (X * (float)(1 << FRAC))

float DDS_calculate_channel_out(uint32_t *phaseAccumulator,
                                uint32_t phaseIncrement, uint32_t index)
{

}

void Calculate(dmabuf_t *buffer, uint16_t buffer_size)
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