#include "pluginInterface/Plugin/Plugin.hpp"
#include <vector>

class DelayPlugin : public Plugin {
enum PARAMETER_INDICES {
    FEEDBACK,
    TIME
};
private:
    int bufferSize;
    int channelSize;
    int delaySamples;
    float feedback;
    std::vector<std::vector<float>> delayBuffer;
    int writeIndex;
    int readIndex;
    std::vector<PluginParameter*> parameters;
    PluginMetaData* pluginMetaData;

public:
    DelayPlugin(int delaySamples = 48000, float feedback = 0.5f);

    bool initialize(int bufferSize, int channelSize) override;
    void process(float** input, float** output) override;
    PluginMetaData* const getMetaData() override;
    std::vector<PluginParameter*>* const getParameters() override;
    bool setParameter(int index, float value) override;
};