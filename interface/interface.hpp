#ifndef INTERFACE_INCLUDED
#define INTERFACE_INCLUDED

#include <portaudio.h>
#include <vector>
#include <string.h>
#include <cstdio>

#define SAMPLE_RATE 48000
#define FPB 256

class Device {
public:
    int maxinchannels;
    int maxoutchannels;
    double samplerate;
    int framesPerBuffer;
    const char* name;
    Device();
    Device(const char* name, int maxinchannel,  int maxoutchannel, double samplerate, int framesPerBuffer);
};

typedef std::vector<Device*> devices_list;

class Interface {
private: 
    Device useddevice;
    std::vector<Device*> devices;
    double sample_rate;
    long int fpb;
    PaStream *stream;
    void openStream(int index);
    void (*callback)(float** input, float** output, Device *device);
    void enumerate();
    static int errorCheck(PaError error, const char* place);
    static int audioCallback(const void* input, void* output, unsigned long framesPerBuffer, const  PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userdata);

public:
    Interface();
    Interface(double sample_rate);
    Interface(long fpb);
    Interface(double sample_rate, long fpb);
    ~Interface();
    devices_list getDevices();
    Device getDetails();
    int openDevice(int index, void (*callback)(float** input, float** output, Device *device));
    int openDevice(const char* name, void (*callback)(float** input, float** output, Device *device));
    int closeDevice();
};

#endif