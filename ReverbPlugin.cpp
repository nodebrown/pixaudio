#include "ReverbPlugin.hpp"
#include <iostream>


#define DIFFUSER_COUNT 8
#define INTERNAL_CHANNEL_COUNT 8

enum CONTROLS {
    WETNESS_PARAM = 0
};

ReverbPlugin::ReverbPlugin(int bufferSize)
    :
      bufferSize(bufferSize)
       {

        pluginMetaData = new PluginMetaData();
        pluginMetaData->author = "Arjun B";
        pluginMetaData->category = "Basic";
        pluginMetaData->name = "AReverb";
        pluginMetaData->email = "arjunbchennithala@gmail.com";
        initialized = false;
        active = true;
        wetness = 0.4;


        PluginParameter* wetnessParameter = new PluginParameter();
        wetnessParameter->name = "Wetness";
        wetnessParameter->description = "This plugin sets the wetness";
        wetnessParameter->start = 0.0;
        wetnessParameter->end = 1.0;
        wetnessParameter->current = 0.4;

        parameters.push_back(wetnessParameter);
}

bool ReverbPlugin::initialize(int bufferSize, int channelSize, int inIndex, int outIndex) {
    this->bufferSize = bufferSize;
    this->channelSize = channelSize;
    this->inIndex = inIndex;
    this->outIndex = -1;
    for(int i=0; i<DIFFUSER_COUNT; i++) {
        Diffuser* diffuser = new Diffuser(INTERNAL_CHANNEL_COUNT,  200);
        diffusers.push_back(diffuser);
    }
    splitted.resize(INTERNAL_CHANNEL_COUNT, 0.0f);
    std::vector<int> delayTimes = {4799, 5039, 5227, 5471, 5527, 5749, 5801, 6037};
    // std::vector<int> delayTimes = {5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000};
    feedBackDelay = new FeedBackDelay(48000, delayTimes, INTERNAL_CHANNEL_COUNT);
    std::cout<<"Initialization of reverb complete"<<std::endl;
    initialized = true;
    return true;
}

void mixDownToStero(std::vector<float> input, float& chL, float& chR) {
    float panning[] = {-1.0, -0.7, -0.4, -0.1, 0.1, 0.4, 0.7, 1.0};
    chL = 0.0f;
    chR = 0.0f;

    for(int i=0; i<8; i++) {
        float angle = (panning[i] + 1.0f) * 0.25f * M_PI;
        float gainL = sinf(angle);
        float gainR = cosf(angle);

        chL += gainL * input[i];
        chR += gainR * input[i];
    }

    float norm = 1.0 / sqrt(8.0f);
    
    chL *= norm;
    chR *= norm;
}


float combine(std::vector<float>& inputs) {
    float sum = 0.0f;
    for(int i=0; i<inputs.size(); i++) {
        sum += inputs.at(i);
    }   
    float avg = sum / static_cast<float>(inputs.size());
    return avg;
}

void ReverbPlugin::process(float** input, float** output) {
    for (int i = 0; i < bufferSize; ++i) {
        for(int j=0; j<splitted.size(); j++) {
            splitted.at(j) =  input[inIndex][i];   
        }
        for(Diffuser* diffuser: diffusers) {
            diffuser->process(splitted);
        }

        feedBackDelay->process(splitted);

        float sampL;
        float sampR;


        mixDownToStero(splitted, sampL, sampR);

        output[0][i] = sampL;
        output[1][i] = sampR;
    }
}

PluginMetaData* const ReverbPlugin::getMetaData() {
    return pluginMetaData; 
}

std::vector<PluginParameter*>* const ReverbPlugin::getParameters() {
    return &parameters;
}

bool ReverbPlugin::setParameter(int index, float value) {
    PluginParameter *parameter = parameters.at(index);
    if(!(value >= parameter->start && value <= parameter->end)) {
        return false;
    }
    parameter->current = value;
    switch (index) {
        case WETNESS_PARAM:
            wetness = value;
        default:
            break;
    }
    return true; 
}


extern "C" Plugin* create_object() {
    return new ReverbPlugin(256);
}