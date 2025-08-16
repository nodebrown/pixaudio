#include <wx/wx.h>
#include "pluginInterface/Plugin/Plugin.hpp"
#include <vector>
#include "ApplicationProcessor.hpp"
#include "ParameterControl.hpp"

class PluginContainer : public wxPanel {
private:
    Plugin* plugin;
    int index;
    std::vector<ParamerControl*> parameterControls; 
    ApplicationProcessor* applicationProcessor;
    wxStaticText* pluginName;
    wxButton* deleteButton;
    int inIndex;
    int outIndex;
public:
    int getIndex() {
        return index;
    }

    void setIndex(int index) {
        this->index = index;
    }

    Plugin* getPlugin() {
        return plugin;
    }

    PluginContainer(wxWindow *parent, int index, Plugin *plugin, ApplicationProcessor *applicationProcessor, wxArrayString *inputChoices, wxArrayString *outputChoices) : wxPanel(parent)
    {
        this->index = index;
        this->plugin = plugin;
        this->applicationProcessor = applicationProcessor;
        wxBoxSizer *headerSizer = new wxBoxSizer(wxHORIZONTAL);
        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
        wxGridSizer *controlSizer = new wxGridSizer(1, 0, 10, 50);
        pluginName = new wxStaticText(this, wxID_ANY, plugin->getMetaData()->name);
        deleteButton = new wxButton(this, wxID_ANY, "Remove");


        Bind(wxEVT_COMMAND_BUTTON_CLICKED, &PluginContainer::OnRemoveClick, this);

        wxChoice *inputChoiceBox = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(100, 30), *inputChoices);
        inputChoiceBox->Select(0);
        wxChoice *outputChoiceBox = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(100, 30), *outputChoices);
        inputChoiceBox->Bind(wxEVT_CHOICE, &PluginContainer::OnInputChannelSelected, this);
        outputChoiceBox->Select(0);
        outputChoiceBox->Bind(wxEVT_CHOICE, &PluginContainer::OnOutputChannelSelected, this);
        inIndex = 0;
        outIndex = 0;
        headerSizer->Add(pluginName, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
        headerSizer->AddStretchSpacer();
        headerSizer->Add(inputChoiceBox);
        headerSizer->AddSpacer(10);
        headerSizer->Add(outputChoiceBox);
        headerSizer->AddSpacer(100);
        headerSizer->Add(deleteButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
        mainSizer->Add(headerSizer, 0, wxEXPAND | wxALL);
        
        this->SetBackgroundColour(*wxLIGHT_GREY);
        std::vector<PluginParameter *> *parameters = plugin->getParameters();
        PluginMetaData *PluginMetaData = plugin->getMetaData();
        controlSizer->AddSpacer(5);
        for (int i = 0; i < parameters->size(); i++)
        {
            ParamerControl* ctrl = new ParamerControl(this, parameters->at(i), applicationProcessor, index, i);
            parameterControls.push_back(ctrl);
            controlSizer->Add(ctrl, 3);
        }
        mainSizer->Add(controlSizer, 1);
        this->SetSizer(mainSizer);
    }

    void OnRemoveClick(wxCommandEvent& evt) {
        evt.SetClientData(this);
        evt.ResumePropagation(1);
        evt.Skip();
    }

    void OnInputChannelSelected(wxCommandEvent& evt) {
        int index = evt.GetInt();
        inIndex = index;
        this->applicationProcessor->updateIndices(this->index, index, outIndex);
    }

    void OnOutputChannelSelected(wxCommandEvent &evt) {
        int index = evt.GetInt();
        outIndex = index;
        this->applicationProcessor->updateIndices(this->index, inIndex, index);
    }

    void updatePlugin(int paramIndex, float paramValue) {
        this->applicationProcessor->updatePlugin(this->index, paramIndex, paramValue);
    }

    bool drawPlugin();
};