#include "pluginInterface/Plugin/Plugin.hpp"
#include <vector>
#include "Diffuser.cpp"
#include "FeedBackDelay.cpp"

class ReverbPlugin : public Plugin {

private:
    int bufferSize;
    std::vector<PluginParameter*> parameters;
    PluginMetaData* pluginMetaData;
    int channelSize;
    std::vector<float> splitted;
    std::vector<Diffuser*> diffusers;
    FeedBackDelay* feedBackDelay;
    Mixer* mixer;

    float wetness;

public:
    ReverbPlugin(int bufferSize);
    bool initialize(int bufferSize, int channelSize, int inIndex, int outIndex) override;
    void process(float** input, float** output) override;
    PluginMetaData* const getMetaData() override;
    std::vector<PluginParameter*>* const getParameters() override;
    bool setParameter(int index, float value) override;
};