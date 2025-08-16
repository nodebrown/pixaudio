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
        wetness = 0.4;

}

bool DelayPlugin::initialize(int bufferSize, int channelSize, int inIndex, int outIndex) {
    this->bufferSize = bufferSize;
    this->channelSize = channelSize;
    this->inIndex = inIndex;
    this->outIndex = outIndex;

    delayBuffer.resize(channelSize);
    for(int i=0; i<channelSize; i++) {
        delayBuffer[i].resize(delaySamples, 0.0f);
    }

    writeIndex = 20000;
    readIndex = 0;
    std::cout<<"Initialized delay plugin with parameters: "<<std::endl;
    std::cout<<"Buffer size: "<<bufferSize<<std::endl;
    std::cout<<"Channelsize: "<<channelSize<<std::endl;
    std::cout<<"inIndex: "<<inIndex<<std::endl;
    std::cout<<"outIndex: "<<outIndex<<std::endl;
    initialized = true;
    return true;
}

void DelayPlugin::process(float** input, float** output) {
    int tempWriteIndex = writeIndex;
    int tempReadIndex = readIndex;
    for (int i = 0; i < bufferSize; ++i) {
        float inSample0 = 0.0f;
        float inSample1 = 0.0f;

        switch (inIndex)
        {
        case ChannelConfiguration::CH0:
            inSample0 = input[0][i];
            inSample1 = inSample0;
            break;

        case ChannelConfiguration::CH1:
            inSample1 = input[1][i];
            inSample0 = inSample1;
            break;

        case ChannelConfiguration::ALL:
            inSample0 = input[0][i];
            inSample1 = input[1][i];
            break;

        default:
            break;
        }


        float delayedSample0 = delayBuffer[0][tempReadIndex];
        float delayedSample1 = delayBuffer[1][tempReadIndex];

        switch (outIndex)
        {
        case ChannelConfiguration::CH0:
            output[0][i] = delayedSample0 * wetness + (1.0 - wetness) * inSample0;
            break;

        case ChannelConfiguration::CH1:
            output[1][i] = delayedSample1 * wetness + (1.0 - wetness) * inSample1;
            break;
        
        case ChannelConfiguration::ALL:
            output[0][i] = delayedSample0 * wetness + (1.0 - wetness) * inSample0;
            output[1][i] = delayedSample1 * wetness + (1.0 - wetness) * inSample1;
            break;
        
        default:
            break;
        }
        delayBuffer[0][tempWriteIndex] = inSample0 + delayedSample0 * parameters[FEEDBACK]->current;
        delayBuffer[1][tempWriteIndex] = inSample1 + delayedSample1 * parameters[FEEDBACK]->current;
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