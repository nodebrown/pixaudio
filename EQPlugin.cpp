#include "EQPlugin.hpp"
#include <iostream>
#include <cmath>


enum CONTROLS {
    GAIN = 0,
    LBQ,
    LBG,
    MBQ,
    MBG,
    HBQ,
    HBG
};

EQPlugin::EQPlugin()
       {

        pluginMetaData = new PluginMetaData();
        pluginMetaData->author = "Arjun B";
        pluginMetaData->category = "Basic";
        pluginMetaData->name = "AEQ";
        pluginMetaData->email = "arjunbchennithala@gmail.com";


        PluginParameter* gainParam = new PluginParameter();
        gainParam->name = "Gain";
        gainParam->description = "Sets the entire gain of plugin";
        gainParam->start = 0.0f;
        gainParam->current = 1.0f;
        gainParam->end = 1.0f;
        gainParam->type = SLIDER;

        PluginParameter* lbQ = new PluginParameter();
        lbQ->name = "Low band Q";
        lbQ->description = "Sets the Q factor of lower band";
        lbQ->start = 0.1f;
        lbQ->current = 1.0f;
        lbQ->end = 1.0f;
        lbQ->type = SLIDER;


        PluginParameter* lbG = new PluginParameter();
        lbG->name = "Low band Gain";
        lbG->description = "Sets the gain of low band";
        lbG->start = -20.0f;
        lbG->current = 0.0f;
        lbG->end = 20.0f;
        lbG->type = SLIDER;


        PluginParameter* mbQ = new PluginParameter();
        mbQ->name = "Mid band Q";
        mbQ->description = "Sets the Q factor of mid band";
        mbQ->start = 0.1f;
        mbQ->current = 1.0f;
        mbQ->end = 1.0f;
        mbQ->type = SLIDER;



        PluginParameter* mbG = new PluginParameter();
        mbG->name = "Mid band Gain";
        mbG->description = "Sets the gain of mid band";
        mbG->start = -20.0f;
        mbG->current = 0.0f;
        mbG->end = 20.0f;
        mbG->type = SLIDER;



        PluginParameter* hbQ = new PluginParameter();
        hbQ->name = "High band Q";
        hbQ->description = "Sets the Q factor of high band";
        hbQ->start = 0.1f;
        hbQ->current = 1.0f;
        hbQ->end = 1.0f;
        hbQ->type = SLIDER;



        PluginParameter* hbG = new PluginParameter();
        hbG->name = "High band Gain";
        hbG->description = "Sets the gain of high band";
        hbG->start = -20.0f;
        hbG->current = 0.0f;
        hbG->end = 20.0f;
        hbG->type = SLIDER;

        parameters.push_back(gainParam);
        parameters.push_back(lbQ);
        parameters.push_back(lbG);
        parameters.push_back(mbQ);
        parameters.push_back(mbG);
        parameters.push_back(hbQ);
        parameters.push_back(hbG);

        initialized = false;
        active = true;
}

bool EQPlugin::initialize(int bufferSize, int channelSize, int inIndex, int outIndex) {
    this->bufferSize = bufferSize;
    this->channelSize = channelSize;
    this->inIndex = inIndex;
    this->outIndex = outIndex;


    lb = new BiquadFilter(BiquadFilter::FilterType::Peaking, 48000, 500, 1.0, 0.0);

    mb = new BiquadFilter(BiquadFilter::FilterType::Peaking, 48000, 8000, 1.0, 0.0);

    hb = new BiquadFilter(BiquadFilter::FilterType::Peaking, 48000, 15000, 1.0, 0.0);


    gain = 1.0;

    
    std::cout<<"Initialization of Equalizer complete"<<std::endl;
    initialized = true;
    return true;
}

void EQPlugin::process(float** input, float** output) {
    for (int i = 0; i < bufferSize; ++i) {
        float in = input[inIndex][i];

        float out = lb->process(in);
        out += mb->process(in);
        out += hb->process(in);

        output[outIndex][i] = out * gain;
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
        case LBQ:
            lb->updateParams(value, lb->getgainDB());
            break;
        case LBG:
            lb->updateParams(lb->getQ(), value);
            break;
        case MBQ:
            mb->updateParams(value, mb->getgainDB());
            break;
        case MBG:
            mb->updateParams(mb->getQ(), value);
            break;
        case HBQ:
            hb->updateParams(value, hb->getgainDB());
            break;
        case HBG:
            hb->updateParams(hb->getQ(), value);
            break;
        default:
            break;
    }
    return true; 
}


extern "C" Plugin* create_object() {
    return new EQPlugin();
}