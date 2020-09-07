
#include <stdint.h>
#include "dds_defs.h"
class IWaveForm
{

public:
    IWaveForm() {}
    virtual ~IWaveForm() {}
    virtual void Generate() = 0; // "= 0" part makes this method pure virtual, and
                                 // also makes this class abstract.
    virtual void Calculate(dmabuf_t *buffer, uint16_t buffer_size) = 0;
};

typedef long CalculatePtr(dmabuf_t *buffer, uint16_t buffer_size);

#define TABLE_SIZE 1024
typedef struct
{
    uint32_t phaseAccumulator_L;
    uint32_t phaseAccumulator_R;
    uint32_t phaseIncrement_L;
    uint32_t phaseIncrement_R;
    float frequency_L;
    float frequency_R;
    float table[TABLE_SIZE + 1]; // +1 for interpolation
    float DDS_calculate_channel_out(uint32_t *phaseAccumulator,
                                    uint32_t phaseIncrement, uint32_t index);
    CalculatePtr *Calculate;

} IWaveForm;
