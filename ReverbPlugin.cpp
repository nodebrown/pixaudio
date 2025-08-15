#include "ReverbPlugin.hpp"
#include <iostream>
#include <cmath>

ReverbPlugin::ReverbPlugin(float roomSize, float damping, float wetness)
    : roomSize(roomSize), damping(damping), wetness(wetness) 
{
    // Parameters
    PluginParameter* roomParam = new PluginParameter();
    roomParam->current = roomSize;
    roomParam->start = 0.0;
    roomParam->end = 1.0;
    roomParam->name = "Room Size";
    parameters.push_back(roomParam);

    PluginParameter* dampingParam = new PluginParameter();
    dampingParam->current = damping;
    dampingParam->start = 0.0;
    dampingParam->end = 1.0;
    dampingParam->name = "Damping";
    parameters.push_back(dampingParam);

    PluginParameter* wetnessParam = new PluginParameter();
    wetnessParam->current = wetness;
    wetnessParam->start = 0.0;
    wetnessParam->end = 1.0;
    wetnessParam->name = "Wetness";
    parameters.push_back(wetnessParam);

    // Metadata
    pluginMetaData = new PluginMetaData();
    pluginMetaData->author = "Arjun B";
    pluginMetaData->category = "Reverb";
    pluginMetaData->name = "SimpleReverb";
    pluginMetaData->email = "arjunbchennithala@gmail.com";

    active = true;
}

bool ReverbPlugin::initialize(int bufferSize, int channelSize, int inIndex, int outIndex) {
    this->bufferSize = bufferSize;
    this->channelSize = channelSize;
    this->inIndex = inIndex;
    this->outIndex = outIndex;

    // Example comb & all-pass delay times (samples)
    combDelays = {1116, 1188, 1277, 1356};
    allpassDelays = {556, 441};

    combBuffers.resize(channelSize);
    combIndices.resize(combDelays.size(), 0);
    for (int ch = 0; ch < channelSize; ch++) {
        combBuffers[ch].resize(combDelays.size());
        for (size_t i = 0; i < combDelays.size(); i++) {
            combBuffers[ch][i].assign(combDelays[i], 0.0f);
        }
    }

    allpassBuffers.resize(channelSize);
    allpassIndices.resize(allpassDelays.size(), 0);
    for (int ch = 0; ch < channelSize; ch++) {
        allpassBuffers[ch].resize(allpassDelays.size());
        for (size_t i = 0; i < allpassDelays.size(); i++) {
            allpassBuffers[ch][i].assign(allpassDelays[i], 0.0f);
        }
    }

    std::cout << "Initialized reverb plugin" << std::endl;
    initialized = true;
    return true;
}

void ReverbPlugin::process(float** input, float** output) {
    for (int i = 0; i < bufferSize; i++) {
        float inL = 0.0f, inR = 0.0f;
        switch (inIndex) {
            case ChannelConfiguration::CH0: inL = input[0][i]; inR = inL; break;
            case ChannelConfiguration::CH1: inR = input[1][i]; inL = inR; break;
            case ChannelConfiguration::ALL: inL = input[0][i]; inR = input[1][i]; break;
        }

        // Process per channel
        float wetL = 0.0f, wetR = 0.0f;
        for (int ch = 0; ch < channelSize; ch++) {
            float inp = (ch == 0 ? inL : inR);
            float combOut = 0.0f;

            // Comb filters
            for (size_t c = 0; c < combDelays.size(); c++) {
                auto& buf = combBuffers[ch][c];
                int& idx = combIndices[c];
                float y = buf[idx];
                buf[idx] = inp + (y * (roomSize * 0.9f));
                combOut += y;
                idx = (idx + 1) % combDelays[c];
            }

            // All-pass filters
            float allpassOut = combOut;
            for (size_t a = 0; a < allpassDelays.size(); a++) {
                auto& buf = allpassBuffers[ch][a];
                int& idx = allpassIndices[a];
                float bufOut = buf[idx];
                float inputVal = allpassOut;
                buf[idx] = inputVal + bufOut * 0.5f;
                allpassOut = bufOut - inputVal * 0.5f;
                idx = (idx + 1) % allpassDelays[a];
            }

            if (ch == 0) wetL = allpassOut;
            else wetR = allpassOut;
        }

        // Mix dry/wet
        switch (outIndex) {
            case ChannelConfiguration::CH0:
                output[0][i] = wetL * wetness + inL * (1.0f - wetness);
                break;
            case ChannelConfiguration::CH1:
                output[1][i] = wetR * wetness + inR * (1.0f - wetness);
                break;
            case ChannelConfiguration::ALL:
                output[0][i] = wetL * wetness + inL * (1.0f - wetness);
                output[1][i] = wetR * wetness + inR * (1.0f - wetness);
                break;
        }
    }
}

PluginMetaData* const ReverbPlugin::getMetaData() {
    return pluginMetaData;
}

std::vector<PluginParameter*>* const ReverbPlugin::getParameters() {
    return &parameters;
}

bool ReverbPlugin::setParameter(int index, float value) {
    PluginParameter* parameter = parameters.at(index);
    if (!(value >= parameter->start && value <= parameter->end)) return false;
    parameter->current = value;
    switch (index) {
        case ROOM_SIZE: roomSize = value; break;
        case DAMPING: damping = value; break;
        case WETNESS_PARAM: wetness = value; break;
    }
    return true;
}

extern "C" Plugin* create_object() {
    return new ReverbPlugin();
}
