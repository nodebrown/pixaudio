#ifndef PLUGIN_PARAMETER
#define PLUGIN_PARAMETER

class PluginParameter {
public:
    float start;
    float end;
    float step;
    float current;
    char* name;
    char* description;
    int type;
};

#endif