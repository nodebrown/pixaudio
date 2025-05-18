#include "DelayPlugin.hpp"
#include <iostream>

DelayPlugin::DelayPlugin(int delaySamples, float feedback)
    :
      delaySamples(delaySamples)
       {

        PluginParameter* feedbackParam = new PluginParameter();
        feedbackParam->current = feedback;
        feedbackParam->start = 0.0;
        feedbackParam->end = 1.0;
        parameters.push_back(feedbackParam);

        pluginMetaData = new PluginMetaData();
        pluginMetaData->author = "Arjun B";
        pluginMetaData->category = "Basic";
        pluginMetaData->name = "ADelay";
        pluginMetaData->email = "arjunbchennithala@gmail.com";

        active = true;

        PluginParameter* timeParam = new PluginParameter();
        timeParam->current = 20000;
        timeParam->start = 0.0;
        timeParam->end = 48000;
        parameters.push_back(timeParam);

}

bool DelayPlugin::initialize(int bufferSize, int channelSize) {
    this->bufferSize = bufferSize;
    this->channelSize = channelSize;

    delayBuffer.resize(channelSize);
    for (int ch = 0; ch < channelSize; ++ch) {
        delayBuffer[ch].resize(delaySamples, 0.0f);
    }

    writeIndex = 20000;
    readIndex = 0;

    return true;
}

void DelayPlugin::process(float** input, float** output) {
    int tempWriteIndex = 0;
    int tempReadIndex = 0;
    for (int ch = 0; ch < channelSize; ++ch) {
        tempWriteIndex = writeIndex;
        tempReadIndex = readIndex;
        for (int i = 0; i < bufferSize; ++i) {
            
            float inSample = input[ch][i];
            float delayedSample = delayBuffer[ch][tempReadIndex];

            output[ch][i] = delayedSample + inSample;

            delayBuffer[ch][tempWriteIndex] = inSample + delayedSample * parameters[FEEDBACK]->current;
            tempWriteIndex = (tempWriteIndex + 1) % delaySamples;
            tempReadIndex = (tempReadIndex + 1) % delaySamples;
        }
    }

    writeIndex = tempWriteIndex;
    readIndex = tempReadIndex;

}

PluginMetaData* const DelayPlugin::getMetaData() {
    return pluginMetaData; 
}

std::vector<PluginParameter*>* const DelayPlugin::getParameters() {
    return &parameters;
}

bool DelayPlugin::setParameter(int index, float value) {
    PluginParameter *parameter = parameters.at(index);
    if(!(value >= parameter->start && value <= parameter->end)) {
        return false;
    }
    parameter->current = value;
    switch (index)
    {
    case FEEDBACK:
        break;
    case TIME:
        writeIndex = value;
        readIndex = 0;
        break;
    default:
        break;
    }
    return true; 
}


extern "C" Plugin* create_object() {
    return new DelayPlugin();
}