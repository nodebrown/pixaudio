#include "interface.hpp"

Device::Device(char* name, int maxinchannel,  int maxoutchannel, double samplerate) {
    this->name = name;
    this->maxinchannels = maxinchannel;
    this->maxoutchannels = maxoutchannel;
    this->samplerate = samplerate;
}


Interface::Interface() {
    Pa_Initialize();
    int numdevices = Pa_GetDeviceCount();
    for(int i=0; i<numdevices; i++) {
        PaDeviceInfo* deviceinfo = Pa_GetDeviceInfo(i);
        this->devices.push_back(new Device(deviceinfo->name, deviceinfo->maxInputChannels, deviceinfo->maxOutputChannels, SAMPLE_RATE));
    }
}

Interface::~Interface() {
    Pa_Destroy();
    for(int i=0; i<this->numdevices; i++) {
        delete this->devices[i];
    }
}

std::vector<Device*> Interface::getDevices() {
    return this->devices;
}

void Interface::openStream(int index) {
    PaStreamParameters inputparams;
    PaStreamParameters outputparams;

    inputparams.device = index;
    inputparams.channelCount = 2;
    inputparams.sampleFormat = paFloat32 | paNonInterleaved;
    inputparams.suggestedLatency = Pa_GetDeviceInfo(index)->defaultLowInputLatency;
    inputparams.hostApiSpecificStreamInfo = NULL;

    outputparams.device = index;
    outputparams.channelCount = 2;
    outputparams.sampleFormat = paFloat32 | paNonInterleaved;
    oututparams.suggestedLatency = Pa_GetDeviceInfo(index)->defaultLowOutputLatency;
    outputparams.hostApiSpecificStreamInfo = NULL;   

    Pa_OpenStream(&(this->stream), &inputparams, &outputparams, SAMPLE_RATE, FPB, paNoFlag, this->callback, NULL);
}

int Interface::openDevice(int index, void (*callback)(float** input, float** output, Device *device)) {
    this->callback = callback;
    this->openStream(index);
    Pa_StartStream(this->stream);
    return 0;
}

int Interface::openDevice(char *name, void (*callback)(float** input, float** output, Device *device)) {
    this->callback = callback;
    int index = -1;
    for(int i=0; i<this->numdevices; i++) {
        if(strcmp(name, this->devices[i]->name) == 0) {
            index = i;
            break;
        }
    }
    if(index == -1)
        return -1;
    this->openStream(index);
    Pa_StartStream(this->stream);
    return 0;
}

int Interface::closeDevice() {
    Pa_StopStream(this->stream);
    Pa_CloseStream(this->stream);
    return 0;
}