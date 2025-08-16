#include "EQPlugin.hpp"
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <cmath>


enum CONTROLS {
    GAIN = 0,
};

EQPlugin::EQPlugin()
       {

        pluginMetaData = new PluginMetaData();
        pluginMetaData->author = "Arjun B";
        pluginMetaData->category = "Basic";
        pluginMetaData->name = "AEQ";
        pluginMetaData->email = "arjunbchennithala@gmail.com";

        char* frequencyBandLabels[] = {"50 Hz", "100 Hz", "500 Hz", "1000 Hz", "2 KHz", "5 KHz", "8 KHz", "10 KHz", "15 KHz", "20 KHz"};

        PluginParameter* gainParam = new PluginParameter();
        gainParam->name = "Gain";
        gainParam->description = "Sets the entire gain of plugin";
        gainParam->start = 0.0f;
        gainParam->current = 1.0f;
        gainParam->end = 1.0f;
        gainParam->type = SLIDER;

        parameters.push_back(gainParam);

        for(int i=0; i<10; i++) {
            PluginParameter* param = new PluginParameter();
            param->name = frequencyBandLabels[i];
            param->type = SLIDER;
            param->start = -20.0f;
            param->end = 20.0f;
            param->current = 0.0f;
            parameters.push_back(param);
        }

        initialized = false;
        active = true;
}

bool EQPlugin::initialize(int bufferSize, int channelSize, int inIndex, int outIndex) {
    this->bufferSize = bufferSize;
    this->channelSize = channelSize;
    this->inIndex = inIndex;
    this->outIndex = outIndex;
    for(int i=0; i<10; i++) {
        bands.push_back(new BiquadFilter(BiquadFilter::FilterType::Peaking, 48000, 50, 0.8, 0.0));
        bands[i]->setFreq(frequencyBands[i]);
    }

    gain = 1.0;

    std::cout<<"Initialization of Equalizer complete"<<std::endl;
    initialized = true;
    return true;
}

void EQPlugin::process(float** input, float** output) {
    for (int i = 0; i < bufferSize; ++i) {

        float inSample = 0.0f;

        switch (inIndex)
        {
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

        default:
            break;
        }

        float out = inSample;
        for(BiquadFilter* filter: bands) {
            out = filter->process(out);
        }

        float outSample = out * gain;

        switch(outIndex) {
            case ChannelConfiguration::CH0:
                output[0][i] = outSample;
                break;
            case ChannelConfiguration::CH1:
                output[1][i] = outSample;
                break;
            case ChannelConfiguration::ALL:
                output[0][i] = outSample;
                output[1][i] = outSample;
                break;
        }
    }
}

PluginMetaData* const EQPlugin::getMetaData() {
    return pluginMetaData; 
}

std::vector<PluginParameter*>* const EQPlugin::getParameters() {
    return &parameters;
}

bool EQPlugin::setParameter(int index, float value) {
    PluginParameter *parameter = parameters.at(index);
    if(!(value >= parameter->start && value <= parameter->end)) {
        return false;
    }
    parameter->current = value;
    switch (index) {
        case GAIN:
            gain = value;
            break;
        default:
            bands[index-1]->setGain(value);
            break;
    }
    return true; 
}


extern "C" Plugin* create_object() {
    return new EQPlugin();
}