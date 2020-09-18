
#define TABLE_SIZE 1024
class SawtoothWave : public IWaveForm
{

public:
    SawtoothWave();
    SawtoothWave(float freq);
    ~SawtoothWave(){};
    void Generate();

};
