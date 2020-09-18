

#define TABLE_SIZE 1024
class TriangleWave : public IWaveForm
{

public:
    TriangleWave();
    TriangleWave(float freq);
    ~TriangleWave(){};
    void Generate();

};
