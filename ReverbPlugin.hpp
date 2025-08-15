#pragma once
#include "pluginInterface/Plugin/Plugin.hpp"
#include <vector>

class ReverbPlugin : public Plugin {
    enum PARAMETER_INDICES {
        ROOM_SIZE,
        DAMPING,
        WETNESS_PARAM
    };

private:
    int bufferSize;
    int channelSize;

    // Comb filter buffers
    std::vector<std::vector<std::vector<float>>> combBuffers; 
    std::vector<int> combIndices;

    // All-pass filter buffers
    std::vector<std::vector<std::vector<float>>> allpassBuffers; 
    std::vector<int> allpassIndices;

    std::vector<int> combDelays;
    std::vector<int> allpassDelays;

    float roomSize;
    float damping;
    float wetness;

    std::vector<PluginParameter*> parameters;
    PluginMetaData* pluginMetaData;

public:
    ReverbPlugin(float roomSize = 0.5f, float damping = 0.3f, float wetness = 0.3f);

    bool initialize(int bufferSize, int channelSize, int inIndex, int outIndex) override;
    void process(float** input, float** output) override;
    PluginMetaData* const getMetaData() override;
    std::vector<PluginParameter*>* const getParameters() override;
    bool setParameter(int index, float value) override;
};
