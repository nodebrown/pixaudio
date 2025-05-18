#ifndef APPLICATION_PROCESSOR
#define APPLICATION_PROCESSOR

#include <vector>
#include <iostream>
#include "pluginInterface/PluginInterface.hpp"
#include "audioInterface/audioInterface.hpp"

class ApplicationProcessor : private PluginInterface, private AudioInterface {
private:
    bool processing;
    bool bypassedAllPlugins;
    void process(float**, float**);
public:
    float sampleValue;
    ApplicationProcessor();

    ~ApplicationProcessor();

    void setProcessing(bool val) { this->processing = true;}
    bool getProcessing() { return this->processing; }

    bool openDevice(int index);
    std::vector<Device*>* const getAudioDevices();

    bool bypassPlugin(int index, bool state);

    bool updatePlugin(int pluginIndex, int paramIndex, float paramValue);

    std::vector<Plugin*>* const getAllPlugins();

    Plugin* const getSinglePlugin(int index);

    bool openPlugin(char* fileName);
};
#endif