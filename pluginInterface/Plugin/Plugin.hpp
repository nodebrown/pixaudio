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

class Plugin {
protected:
    bool active;
public:
    virtual ~Plugin() {};
    virtual bool initialize(int bufferSize, int channelSize) = 0;
    virtual void process(float **input, float **output) = 0;
    virtual PluginMetaData* const getMetaData() = 0;
    virtual std::vector<PluginParameter*>* const getParameters() = 0;
    virtual bool setParameter(int index, float value) = 0;
    void setActive(bool active) { this->active = active; }
    bool isActive() { return this->active; }
};

#endif