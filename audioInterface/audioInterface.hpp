#ifndef AUDIO_INTERFACE_INCLUDED
#define AUDIO_INTERFACE_INCLUDED

#include <portaudio.h>
#include <vector>
#include <string.h>
#include <cstdio>
#include <functional>

#define SAMPLE_RATE 48000
#define FPB 256

class Device {
public:
    int maxinchannels;
    int maxoutchannels;
    int usingoutchannels;
    double samplerate;
    int framesPerBuffer;
    const char* name;
    Device();
    Device(const char* name, int maxinchannel,  int maxoutchannel, double samplerate, int framesPerBuffer, int usingoutchannels);
};

typedef std::vector<Device*> devices_list;

class AudioInterface {
private: 
    Device useddevice;
    std::vector<Device*> devices;
    double sample_rate;
    long int fpb;
    PaStream *stream;
    void openStream(int index);
    std::function<void(float**, float**)> callback;
    void enumerate();
    static int errorCheck(PaError error, const char* place);
    static int audioCallback(const void* input, void* output, unsigned long framesPerBuffer, const  PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userdata);

public:
    AudioInterface();
    AudioInterface(double sample_rate);
    AudioInterface(long int fpb);
    AudioInterface(double sample_rate, long int fpb);
    ~AudioInterface();
    void setSampleRateAndFPB(double sample_rate, long int fpb);
    std::vector<Device*>* getDevices();
    Device getDetails();
    int openDevice(int index, std::function<void(float**, float**)> callback);
    int openDevice(const char* name, std::function<void(float**, float**)> callback);
    int closeDevice();
};

#endif