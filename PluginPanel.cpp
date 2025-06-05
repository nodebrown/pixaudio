#include "PluginPanel.hpp"

bool PluginPanel::addPlugin(Plugin *plugin)
{
    plugins.push_back(plugin);
    PluginContainer* pluginContainer = new PluginContainer(this, plugins.size()-1, plugins.at(plugins.size() - 1), nullptr);
    pluginContainers.push_back(pluginContainer);
    // drawPlugins();
    return true;
}

bool PluginPanel::drawPlugins()
{
    for(int i=0; i<pluginContainers.size(); i++) {
        pluginContainers.at(i)->drawPlugin();
    }
    return true;
}