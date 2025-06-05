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
        feedbackParam->name = "Feedback";
        parameters.push_back(feedbackParam);

        pluginMetaData = new PluginMetaData();
        pluginMetaData->author = "Arjun B";
        pluginMetaData->category = "Basic";
        pluginMetaData->name = "ADelay";
        pluginMetaData->email = "arjunbchennithala@gmail.com";


        PluginParameter* timeParam = new PluginParameter();
        timeParam->current = 20000;
        timeParam->start = 0.0;
        timeParam->end = 48000;
        timeParam->name = "Time";
        parameters.push_back(timeParam);



        PluginParameter* wetnessParam = new PluginParameter();
        wetnessParam->current = 0.4;
        wetnessParam->start = 0.0;
        wetnessParam->end = 1.0;
        wetnessParam->name = "Wetness";
        parameters.push_back(wetnessParam);

        active = true;

}

bool DelayPlugin::initialize(int bufferSize, int channelSize, int inIndex, int outIndex) {
    this->bufferSize = bufferSize;
    this->channelSize = channelSize;
    this->inIndex = inIndex;
    this->outIndex = outIndex;

    delayBuffer.resize(delaySamples, 0.0f);

    writeIndex = 20000;
    readIndex = 0;

    initialized = true;
    return true;
}

void DelayPlugin::process(float** input, float** output) {
    int tempWriteIndex = writeIndex;
    int tempReadIndex = readIndex;
    for (int i = 0; i < bufferSize; ++i) {
        float inSample = input[inIndex][i];
        float delayedSample = delayBuffer[tempReadIndex];

        output[outIndex][i] = delayedSample + inSample;

        delayBuffer[tempWriteIndex] = inSample + delayedSample * parameters[FEEDBACK]->current;
        tempWriteIndex = (tempWriteIndex + 1) % delaySamples;
        tempReadIndex = (tempReadIndex + 1) % delaySamples;
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
    case WETNESS_PARAM:
        wetness = value;
        break;
    default:
        break;
    }
    return true; 
}


extern "C" Plugin* create_object() {
    return new DelayPlugin();
}