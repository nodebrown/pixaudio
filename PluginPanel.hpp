#include <wx/wx.h>
#include "pluginInterface/Plugin/Plugin.hpp"
#include <vector>
#include <iostream>
#include "PluginContainer.hpp"

class PluginPanel: public wxPanel {
private:
    std::vector<Plugin*> plugins;
    std::vector<PluginContainer*> pluginContainers;
    wxWindow* parent;
public:
    PluginPanel(wxWindow* parent) 
        : wxPanel(parent){
            this->parent  = parent;
    }

    bool addPlugin(Plugin* plugin);

    bool drawPlugins();
};