#ifndef PLUGIN_INCLUDED
#define PLUGIN_INCLUDED

#include "PluginMetaData.hpp"
#include "PluginParameter.hpp"
#include <vector>

enum PARAMETER_CONTROL_TYPE {
    ROTARY,
    SLIDER,
    TOGGLE,
    SWITCH
};

enum ChannelConfiguration {
    NONE = 0,
    CH0,
    CH1,
    ALL
};

class Plugin {
protected:
    bool active;
    int inIndex;
    int outIndex;
    bool initialized;
public:
    virtual ~Plugin() {};
    virtual bool initialize(int bufferSize, int channelSize, int inIndex, int outIndex) = 0;
    virtual void process(float **input, float **output) = 0;
    virtual PluginMetaData* const getMetaData() = 0;
    virtual std::vector<PluginParameter*>* const getParameters() = 0;
    virtual bool setParameter(int index, float value) = 0;
    void setActive(bool active) { this->active = active; }
    bool isActive() { return this->active; }
    bool isInitialized() { return this->active; }
    void setIndices(int inIndex, int outIndex) {this->inIndex = inIndex; this->outIndex = outIndex; }
    int getInIndex() { return this->inIndex; }
    int getOutIndex() { return this->outIndex; }
};

#endif