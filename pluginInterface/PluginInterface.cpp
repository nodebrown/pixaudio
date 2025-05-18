#include "PluginInterface.hpp"

int PluginInterface::loadPlugin(char *fileName)
{
    void* handle = dlopen(fileName, RTLD_NOW);
    if(!handle) {
        std::cerr<<"Cannot open plugin file: "<<fileName
                <<" Error: "<<dlerror()<<std::endl;
        return -1;
    } 
    typedef Plugin* (*create_t)();
    create_t create_object = (create_t)dlsym(handle, "create_object");
    if(!create_object) {
        std::cerr<<"Cannot find symbol: create_object"<<std::endl;
        dlclose(handle);
        return -2;
    }

    Plugin* obj = create_object();

    PluginMetaData* metadata = obj->getMetaData();
    if(metadata) {
        std::cout<<"Loaded plugin: "<<metadata->name<<" From file: "<<fileName<<std::endl;
    } else {
        std::cout<<"Plugin loaded from file: "<<fileName<<std::endl;
    }

    plugins.push_back(obj);
    pluginHandles.push_back(handle);
    return 0;
}

bool PluginInterface::loadPlugin(int index, char *fileName)
{
    return false;
}

int PluginInterface::loadPlugin(Plugin* plugin)
{
    
    return 0;
}

bool PluginInterface::removePlugin(int index)
{
    return false;
}

bool PluginInterface::swapPlugins(int index1, int index2) 
{
    return false;
}

Plugin *PluginInterface::getPlugin(int index)
{ 
    return plugins.at(index);
}

std::vector<Plugin *>* PluginInterface::getPlugins()
{
    return &plugins;
}

int PluginInterface::duplicatePlugin(int)
{
    return 0;
}
