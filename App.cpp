#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "audioInterface/audioInterface.hpp"
#include "pluginInterface/Plugin/Plugin.hpp"
#include "ApplicationProcessor.hpp"
#include "PluginContainer.hpp"
#include <string>

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};
 
class MyFrame : public wxFrame
{
public:
    MyFrame();
 
protected:
    ApplicationProcessor* applicationProcessor;
    std::vector<PluginContainer*> pluginContainers;
    wxGridSizer* sizer;
    void OnRemovePluginClick(wxCommandEvent& event);

private:
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnPluginLoad(wxCommandEvent& event);
    void OnDeviceStart(wxCommandEvent& event);
    wxArrayString *inputChoices;
    wxArrayString *outputChoices;
};

enum
{
    ID_PLUGIN_OPEN = 1,
    ID_DEVICE_START,
    ID_PLUGIN_REMOVE
};
 
wxIMPLEMENT_APP(MyApp);
 
bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}
 
MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "PixAudio")
{

    applicationProcessor = new ApplicationProcessor();
    applicationProcessor->setProcessing(false);

    inputChoices = new wxArrayString();

    inputChoices->Add("IN1");
    inputChoices->Add("IN2");


    outputChoices = new wxArrayString();

    outputChoices->Add("OUT1");
    outputChoices->Add("OUT2");
    
    this->SetSize(1000, 1000);
    wxMenu *menuFile = new wxMenu();
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
 
    wxMenu *menuHelp = new wxMenu();
    menuHelp->Append(wxID_ABOUT);

    wxMenu *menuPlugin = new wxMenu();
    menuPlugin->Append(ID_PLUGIN_OPEN, "&Load..\tCtrl-L");

    wxMenu *deviceMenu = new wxMenu();
    
    devices_list* deviceList = applicationProcessor->getAudioDevices();

    for(int i=0; i<deviceList->size(); i++) {
        int id = wxID_HIGHEST + 1 + i;
        deviceMenu->Append(id, deviceList[0][i]->name);
        Bind(wxEVT_MENU, &MyFrame::OnDeviceStart, this, id);
    }
 
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    menuBar->Append(menuPlugin, "&Plugin");
    menuBar->Append(deviceMenu, "&Device");
 
    SetMenuBar( menuBar );

    this->SetBackgroundColour(*wxBLACK);
    sizer = new wxGridSizer(0, 1, wxSize(10, 10));

    this->SetSizer(sizer);

    Bind(wxEVT_BUTTON, &MyFrame::OnRemovePluginClick, this);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MyFrame::OnPluginLoad, this, ID_PLUGIN_OPEN);
    Bind(wxEVT_MENU, &MyFrame::OnDeviceStart
        , this, ID_DEVICE_START);
}
 
void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
 
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a sample window for PixAudio app",
                 "About PixAudio", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnPluginLoad(wxCommandEvent& event) 
{
    wxFileDialog* fileDialog = new wxFileDialog(this);

    fileDialog->ShowModal();

    wxString path = fileDialog->GetPath();

    this->applicationProcessor->openPlugin(path.ToStdString());

    Plugin* plugin = (this->applicationProcessor->getAllPlugins())->at(this->applicationProcessor->getAllPlugins()->size()-1);

    pluginContainers.push_back(new PluginContainer(this, applicationProcessor->getAllPlugins()->size()-1, plugin, applicationProcessor, inputChoices, outputChoices));
    sizer->Add(pluginContainers[pluginContainers.size()-1], 1, wxEXPAND);

    sizer->Layout();
}

void MyFrame::OnDeviceStart(wxCommandEvent& event) 
{
    int id = event.GetId();
    int deviceIndex = id - (wxID_HIGHEST + 1);
    if(applicationProcessor->getProcessing()) {
        this->applicationProcessor->closeDevice();
        applicationProcessor->setProcessing(false);
    }
    this->applicationProcessor->openDevice(deviceIndex);
    applicationProcessor->setProcessing(true);
}


void MyFrame::OnRemovePluginClick(wxCommandEvent& event) 
{
    int pluginIndex = ((PluginContainer*)(event.GetClientData()))->getIndex();
    PluginContainer* container = pluginContainers[pluginIndex];
    this->applicationProcessor->removePluginByIndex(pluginIndex);
    this->sizer->Detach(pluginIndex);
    container->Destroy();
    pluginContainers.erase(pluginContainers.begin() + pluginIndex);

    int index = 0;
    for(PluginContainer* pluginContainer: pluginContainers) {
        pluginContainer->setIndex(index);
        index++;
    }
    this->sizer->Layout();
}