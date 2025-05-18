#ifndef PLUGIN_INTERFACE
#define PLUGIN_INTERFACE

#include "Plugin/Plugin.hpp"
#include <vector>
#include <dlfcn.h>
#include <iostream>

class PluginInterface {
private:
    std::vector<Plugin*> plugins;
protected:
    std::vector<void*> pluginHandles;
public:
    PluginInterface() {}
    int loadPlugin(char* fileName); 
    bool loadPlugin(int index, char *fileName);
    int loadPlugin(Plugin* plugin);
    bool removePlugin(int);
    bool swapPlugins(int, int);
    Plugin* getPlugin(int);
    std::vector<Plugin*>* getPlugins();
    int duplicatePlugin(int);
};

#endif