#include "ApplicationProcessor.hpp"

ApplicationProcessor::ApplicationProcessor() {}

ApplicationProcessor::~ApplicationProcessor() 
{
    std::vector<Plugin*>* availablePlugins = getPlugins();
    for(int i=0; i<availablePlugins->size(); i++) {
        delete availablePlugins->at(i);
        dlclose(pluginHandles.at(i));
    }
}

void ApplicationProcessor::process(float** input, float** output) 
{
    std::vector<Plugin*>* availablePlugins = getPlugins();
    for(int i=0; i<availablePlugins->size(); i++) {
        Plugin* currentPlugin = getPlugin(i);
        if(currentPlugin) {
            if(currentPlugin->isActive()) {
                currentPlugin->process(input, output);
            }
        }
    
    }
}

bool ApplicationProcessor::openPlugin(char* fileName) 
{
    int ret =  loadPlugin(fileName);
    std::vector<Plugin*>* availablePlugins = getPlugins();
    if(availablePlugins->size() > 0) {
        Plugin* lastPlugin = getPlugin(availablePlugins->size()-1);
        lastPlugin->initialize(256, 2);
    }
    return ret;
}

bool ApplicationProcessor::openDevice(int index)
{
    std::function<void(float**, float**)> callback = std::bind(&ApplicationProcessor::process, this, std::placeholders::_1, std::placeholders::_2);

    int ret = AudioInterface::openDevice(index, callback);
    return ret;
}

std::vector<Device *>* const ApplicationProcessor::getAudioDevices()
{
    return getDevices();
}

bool ApplicationProcessor::bypassPlugin(int index, bool state)
{
    Plugin* p = getPlugin(index);
    p->setActive(state);
    return true;
}

bool ApplicationProcessor::updatePlugin(int pluginIndex, int paramIndex, float paramValue)
{
    Plugin* plugin = getPlugin(pluginIndex);
    return plugin->setParameter(paramIndex, paramValue);
}

std::vector<Plugin *> *const ApplicationProcessor::getAllPlugins()
{
    return getPlugins();
}

Plugin *const ApplicationProcessor::getSinglePlugin(int index)
{
    return getPlugin(index);
}
