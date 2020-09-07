
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