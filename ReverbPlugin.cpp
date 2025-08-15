#include "ReverbPlugin.hpp"
#include <iostream>

ReverbPlugin::ReverbPlugin()
    : decay(0.7f), wetness(0.4f), predelaySamples(0)
{
    // Parameters
    auto decayParam = new PluginParameter();
    decayParam->current = decay;
    decayParam->start = 0.0f;
    decayParam->end = 0.99f;
    decayParam->name = "Decay";
    parameters.push_back(decayParam);

    auto predelayParam = new PluginParameter();
    predelayParam->current = 0;
    predelayParam->start = 0;
    predelayParam->end = 2000; // samples
    predelayParam->name = "PreDelay";
    parameters.push_back(predelayParam);

    auto wetParam = new PluginParameter();
    wetParam->current = wetness;
    wetParam->start = 0.0f;
    wetParam->end = 1.0f;
    wetParam->name = "Wetness";
    parameters.push_back(wetParam);

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
    this->inIndex = inIndex;
    this->outIndex = outIndex;

    predelayBuffer.assign(parameters[1]->end, 0.0f);
    predelayIndex = 0;


    // Example comb filter lengths (prime numbers for smoother response)
    int combLengthsL[] = {1116, 1188, 1277, 1356};
    int combLengthsR[] = {1139, 1211, 1300, 1379};

    combsL.clear();
    combsR.clear();
    for (int len : combLengthsL) {
        CombFilter c;
        c.buffer.assign(len, 0.0f);
        c.feedback = decay;
        combsL.push_back(c);
    }
    for (int len : combLengthsR) {
        CombFilter c;
        c.buffer.assign(len, 0.0f);
        c.feedback = decay;
        combsR.push_back(c);
    }

    // Example allpass filter lengths
    int allpassLengths[] = {556, 441, 341};
    allpassesL.clear();
    allpassesR.clear();
    for (int len : allpassLengths) {
        AllpassFilter a;
        a.buffer.assign(len, 0.0f);
        allpassesL.push_back(a);
        allpassesR.push_back(a);
    }

    initialized = true;
    return true;
}

void ReverbPlugin::process(float** input, float** output) {
    for (int i = 0; i < bufferSize; ++i) {
        float inSample = 0.0f;
        switch(inIndex) {
            case ChannelConfiguration::CH0:
                inSample = input[0][i];
                break;
            case ChannelConfiguration::CH1:
                inSample = input[1][i];
                break;
            case ChannelConfiguration::ALL:
                inSample = input[0][i] + input[1][i];
                inSample = inSample / 2;
                break;
        }


        predelayBuffer[predelayIndex] = inSample;

        // Read delayed signal
        float delayedInput = predelayBuffer[(predelayIndex - predelaySamples + 2000) % 2000];

        // Advance index
        predelayIndex = (predelayIndex + 1) % 2000;

        inSample = delayedInput;

        // Process comb filters
        float outL = 0.0f;
        float outR = 0.0f;
        for (auto& c : combsL) outL += c.process(inSample);
        for (auto& c : combsR) outR += c.process(inSample);

        // Normalize
        outL /= combsL.size();
        outR /= combsR.size();

        // Process allpass filters
        for (auto& a : allpassesL) outL = a.process(outL);
        for (auto& a : allpassesR) outR = a.process(outR);

        switch (outIndex) {
            case ChannelConfiguration::CH0:
                output[0][i] = wetness * outL + (1.0f - wetness) * inSample;
                break;
            case ChannelConfiguration::CH1:
                output[1][i] = wetness * outR + (1.0f - wetness) * inSample;
                break;
            case ChannelConfiguration::ALL:
                output[0][i] = wetness * outL + (1.0f - wetness) * inSample;
                output[1][i] = wetness * outR + (1.0f - wetness) * inSample;
                break;
        }

        // Mix dry/wet
    
    }
}

PluginMetaData* const ReverbPlugin::getMetaData() {
    return pluginMetaData;
}

std::vector<PluginParameter*>* const ReverbPlugin::getParameters() {
    return &parameters;
}

bool ReverbPlugin::setParameter(int index, float value) {
    auto param = parameters.at(index);
    if (!(value >= param->start && value <= param->end)) return false;

    param->current = value;
    switch (index) {
        case DECAY_PARAM:
            decay = value;
            for (auto& c : combsL) c.feedback = decay;
            for (auto& c : combsR) c.feedback = decay;
            break;
        case PREDELAY_PARAM:
            predelaySamples = (int)value;
            break;
        case WETNESS_PARAM:
            wetness = value;
            break;
    }
    return true;
}

extern "C" Plugin* create_object() {
    return new ReverbPlugin();
}
