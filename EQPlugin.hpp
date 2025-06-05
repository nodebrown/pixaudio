#include "pluginInterface/Plugin/Plugin.hpp"
#include <vector>
#include "BiQuadFilter.hpp"

class EQPlugin : public Plugin {

private:
    std::vector<PluginParameter*> parameters;
    PluginMetaData* pluginMetaData;
    int channelSize;
    int bufferSize;
    BiquadFilter* lb;
    BiquadFilter* mb;
    BiquadFilter* hb;
    float gain;

public:
    EQPlugin();
    bool initialize(int bufferSize, int channelSize, int inIndex, int outIndex) override;
    void process(float** input, float** output) override;
    PluginMetaData* const getMetaData() override;
    std::vector<PluginParameter*>* const getParameters() override;
    bool setParameter(int index, float value) override;
};