#ifndef INTERFACE_INCLUDED
#define INTERFACE_INCLUDED

#include <portaudio.h>
#include <vector>
#include <string.h>

#define SAMPLE_RATE = 48000
#define FPB = 256

class Device {
    char* name;
    int maxinchannels;
    int maxoutchannels;
    double samplerate;
public:
    Device(char* name, int maxinchannel,  int maxoutchannel, double samplerate);
};

class Interface {
private:
    
    Device useddevice;
    std::vector<Device*> devices;
    void (*callback)(float** input, float** output, Device *device);
    PaStream *stream;
    void openStream(int index;);
public:
    Audio();
    ~Audio();
    std::vector<Device*> getDevices();
    int openDevice(int index, void (*callback)(float** input, float** output, Device *device));
    int openDevice(char* name, void (*callback)(float** input, float** output, Device *device));
    int closeDevice();
};

#endif