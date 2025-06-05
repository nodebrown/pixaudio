#include <wx/wx.h>
#include "pluginInterface/Plugin/Plugin.hpp"
#include "ApplicationProcessor.hpp"
#include <iostream>

class ParamerControl : public wxPanel {
private: 
    wxControl *control;
    wxStaticText *paramName;
    wxWindow* parent;
    float minValue;
    float maxValue;
    int index;
    int pluginIndex;
    ApplicationProcessor* applicationProcessor;
    wxGridSizer* gridSizer;

    int getCurrentValue(PluginParameter* parameter) {
        return 100.0f * (parameter->current - parameter->start) / (parameter->end - parameter->start);
    }

public:
    ParamerControl(wxWindow* parent, PluginParameter* parameter, ApplicationProcessor* applicationProcessor, int pluginIndex, int index) : wxPanel(parent) {
        this->parent = parent;
        this->applicationProcessor = applicationProcessor;
        this->pluginIndex = pluginIndex;
        this->index = index;
        this->gridSizer = new wxGridSizer(2, 1, 0, 0);

        switch(parameter->type) {
                case ROTARY:
                    control = new wxSlider(this, wxID_ANY, getCurrentValue(parameter), 0, 100, wxDefaultPosition, wxSize(10, 100), wxSL_VERTICAL | wxSL_INVERSE, wxDefaultValidator);
                    control->Bind(wxEVT_COMMAND_SLIDER_UPDATED, &ParamerControl::OnSliderChange, this);
                    paramName = new wxStaticText(this, wxID_ANY, parameter->name);
                    this->minValue = parameter->start;
                    this->maxValue = parameter->end;
                    break;
                case SLIDER:
                    control = new wxSlider(this, wxID_ANY, getCurrentValue(parameter), 0, 100, wxDefaultPosition, wxSize(10, 100), wxSL_VERTICAL | wxSL_INVERSE, wxDefaultValidator);
                    control->Bind(wxEVT_COMMAND_SLIDER_UPDATED, &ParamerControl::OnSliderChange, this);
                    paramName = new wxStaticText(this, wxID_ANY, parameter->name);
                    this->minValue = parameter->start;
                    this->maxValue = parameter->end;
                    break;
                default:
                    break;
        }
        gridSizer->Add(control, 1);
        gridSizer->Add(paramName, 1);
        SetSizer(gridSizer);
    }

    void OnSliderChange(wxCommandEvent& event) {
        int value = event.GetInt();
        float diff = this->maxValue - this->minValue;
        float change = diff / 100;
        float newValue = change * value + this->minValue;

        applicationProcessor->updatePlugin(pluginIndex, index, newValue);
    }
};