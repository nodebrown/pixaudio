#ifndef APPLICATION_PROCESSOR
#define APPLICATION_PROCESSOR

#include <vector>
#include <iostream>
#include "pluginInterface/PluginInterface.hpp"
#include "audioInterface/audioInterface.hpp"

class ApplicationProcessor : public PluginInterface, public AudioInterface {
private:
    bool processing;
    bool bypassedAllPlugins;
    void process(float**, float**);
    std::vector<std::vector<float>> outputSamples;
    std::vector<Plugin*> gainPlugin;
    std::vector<Plugin*> mapperPlugin;
    int inChannelSize;
    int outChannelSize;
    int framesPerBuffer;
public:
    ApplicationProcessor();

    ~ApplicationProcessor();

    void setProcessing(bool val) { this->processing = val;}
    bool getProcessing() { return this->processing; }

    bool openDevice(int index);
    std::vector<Device*>* const getAudioDevices();

    bool bypassPlugin(int index, bool state);

    bool updatePlugin(int pluginIndex, int paramIndex, float paramValue);

    std::vector<Plugin*>* const getAllPlugins();

    Plugin* const getSinglePlugin(int index);

    bool openPlugin(std::string fileName);

    Device const getOpenedDevice();

    std::vector<std::vector<float>>* const getLastOutputSamples();

    void updateIndices(int pluginIndex, int inIndex, int outIndex);

    bool removePluginByIndex(int pluginIndex);
};
#endif