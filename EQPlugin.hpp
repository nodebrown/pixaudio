#include "pluginInterface/Plugin/Plugin.hpp"
#include <vector>
#include "BiQuadFilter.hpp"

class EQPlugin : public Plugin {

private:
    std::vector<PluginParameter*> parameters;
    PluginMetaData* pluginMetaData;
    int channelSize;
    int bufferSize;
    std::vector<BiquadFilter*> bands; 
    float gain;
    double frequencyBands[10] = {50, 100, 500, 1000, 2000, 5000, 8000, 10000, 150000, 20000};

public:
    EQPlugin();
    bool initialize(int bufferSize, int channelSize, int inIndex, int outIndex) override;
    void process(float** input, float** output) override;
    PluginMetaData* const getMetaData() override;
    std::vector<PluginParameter*>* const getParameters() override;
    bool setParameter(int index, float value) override;
};