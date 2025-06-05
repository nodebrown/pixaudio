#include "ApplicationProcessor.hpp"
#include <iostream>

int main() {
    ApplicationProcessor* applicationProcessor = new ApplicationProcessor();
    std::vector<Device*>* devs = applicationProcessor->getAudioDevices();

    for(int i=0; i<devs->size(); i++) {
        std::cout<<"Device name: "<<devs->at(i)->name<<std::endl;
    }
    return 0;
}
