#include "ApplicationProcessor.hpp"

ApplicationProcessor::ApplicationProcessor() {
    
}

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
    if(inChannelSize == 1) {
        for(int i=0; i<outChannelSize; i++) {
            for(int j=0; j<framesPerBuffer; j++) {
                output[i][j] = input[0][j];
            }
        }
    } else {
        for(int i=0; i<inChannelSize; i++) {
            for(int j=0; j<framesPerBuffer; j++) {
                output[i][j] = input[i][j];
            }
        }
    }

    std::vector<Plugin*>* availablePlugins = getPlugins();
    for(int i=0; i<availablePlugins->size(); i++) {
        Plugin* currentPlugin = getPlugin(i);
        if(currentPlugin) {
            if(currentPlugin->isActive()) {
                currentPlugin->process(output, output);
            }
        }
    }
}

bool ApplicationProcessor::openPlugin(std::string fileName) 
{
    int ret =  loadPlugin(fileName.c_str(), framesPerBuffer, outChannelSize, 0, 0);
    if(ret == 0) {
        std::vector<Plugin*>* availablePlugins = getPlugins();
        if(availablePlugins->size() > 0) {
            Plugin* lastPlugin = getPlugin(availablePlugins->size()-1);
        }
    }
    return ret;
}

Device const ApplicationProcessor::getOpenedDevice()
{
    return getDetails();
}

std::vector<std::vector<float>>* const ApplicationProcessor::getLastOutputSamples()
{
    return &outputSamples;
}

void ApplicationProcessor::updateIndices(int pluginIndex, int inIndex, int outIndex)
{
    Plugin* plugin = getPlugin(pluginIndex);
    plugin->setIndices( inIndex, outIndex );
}

bool ApplicationProcessor::removePluginByIndex(int pluginIndex)
{   
    std::vector<Plugin*>* plugins = getPlugins();
    delete (*plugins)[pluginIndex];
    plugins->erase(plugins->begin() + pluginIndex);
    dlclose(pluginHandles[pluginIndex]);
    pluginHandles.erase(pluginHandles.begin() + pluginIndex);
    return true;
}

bool ApplicationProcessor::openDevice(int index)
{
    std::function<void(float**, float**)> callback = std::bind(&ApplicationProcessor::process, this, std::placeholders::_1, std::placeholders::_2);

    int ret = AudioInterface::openDevice(index, callback);

    Device device = getOpenedDevice();

    this->inChannelSize = device.maxinchannels;
    this->outChannelSize = device.maxoutchannels;
    this->framesPerBuffer = device.framesPerBuffer;

    outputSamples.resize(device.usingoutchannels);
    for(int i=0; i<device.usingoutchannels; i++) {
        outputSamples[i].resize(device.framesPerBuffer, 0.0f);
    }
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
