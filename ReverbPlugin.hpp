#include "pluginInterface/Plugin/Plugin.hpp"
#include <vector>

class ReverbPlugin : public Plugin {
    enum PARAMETER_INDICES {
        DECAY_PARAM,
        PREDELAY_PARAM,
        WETNESS_PARAM
    };

private:
    int bufferSize;
    float decay;
    float wetness;
    int predelaySamples;

    struct CombFilter {
        std::vector<float> buffer;
        int index = 0;
        float feedback = 0.7f;
        float process(float input) {
            float output = buffer[index];
            buffer[index] = input + output * feedback;
            index = (index + 1) % buffer.size();
            return output;
        }
    };

    struct AllpassFilter {
        std::vector<float> buffer;
        int index = 0;
        float feedback = 0.5f;
        float process(float input) {
            float bufout = buffer[index];
            float output = -input + bufout;
            buffer[index] = input + bufout * feedback;
            index = (index + 1) % buffer.size();
            return output;
        }
    };

    std::vector<CombFilter> combsL;
    std::vector<CombFilter> combsR;
    std::vector<AllpassFilter> allpassesL;
    std::vector<AllpassFilter> allpassesR;
    std::vector<float> predelayBuffer;
    int predelayIndex;

    std::vector<PluginParameter*> parameters;
    PluginMetaData* pluginMetaData;

public:
    ReverbPlugin();

    bool initialize(int bufferSize, int channelSize, int inIndex, int outIndex) override;
    void process(float** input, float** output) override;
    PluginMetaData* const getMetaData() override;
    std::vector<PluginParameter*>* const getParameters() override;
    bool setParameter(int index, float value) override;
};
