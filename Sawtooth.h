
#define TABLE_SIZE 1024
class SawtoothWave : public IWaveForm
{
    uint32_t phaseAccumulator_L;
    uint32_t phaseAccumulator_R;
    float frequency_L;
    float frequency_R;
    float  Sawtooth[TABLE_SIZE + 1]; // +1 for interpolation
    float DDS_calculate_channel_out(uint32_t *phaseAccumulator,
                                    uint32_t phaseIncrement, uint32_t index);

public:
    SawtoothWave();
    SawtoothWave(float freq);
    ~SawtoothWave(){};
    void Generate();
    void Calculate(dmabuf_t *buffer, uint16_t buffer_size);
};