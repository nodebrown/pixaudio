#include "../Plugin/Plugin.hpp"
#include <vector>
#include <exception>
#include <iostream>

class Gain : public Plugin {
private:
    int bufferSize;
    int channelSize;
    PluginMetaData* pluginMetaData;
    std::vector<PluginParameter*> parameters;
    float gain; 
public:
    Gain() {
        pluginMetaData = new PluginMetaData();
        pluginMetaData->author = "Arjun B";
        pluginMetaData->category = "Basic";
        pluginMetaData->name = "AGain";
        pluginMetaData->email = "arjunbchennithala@gmail.com";

        gain = 1.0;


        PluginParameter* gainParameter = new PluginParameter();
        gainParameter->name = "Gain";
        gainParameter->description = "Controls the gain";
        gainParameter->type = ROTARY;
        gainParameter->start = 0.0;
        gainParameter->end = 2.0;
        gainParameter->current = 1.0;
        gainParameter->step = 0.01;

        parameters.push_back(gainParameter);
    }


    ~Gain() {
        delete pluginMetaData;
        for(int i=0; i<parameters.size(); i++) {
            PluginParameter* param =  parameters.at(i);
            delete param;
        }
    }

    bool initialize(int bufferSize, int channelSize) override{
        this->bufferSize = bufferSize;
        this->channelSize = channelSize;
        return true;
    }

    void process(float** input, float**output) override{
        for(int i=0; i<channelSize; i++) {
            for(int j=0; j<bufferSize; j++) {
                output[i][j] = input[i][j] * gain;
            }
        }
    }

    PluginMetaData* const getMetaData() override {
        return pluginMetaData; 
    }

    std::vector<PluginParameter*>* const getParameters() override {
        return &parameters;
    }

    bool setParameter(int index, float value) override {
        PluginParameter* param = parameters.at(index);
        if(value >= param->start && value <= param->end) {
            param->current = value;
            gain = value;
        }
        return true;
    }
};
