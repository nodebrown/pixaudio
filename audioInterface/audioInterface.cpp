#include "audioInterface.hpp"

Device::Device(const char* name, int maxinchannel,  int maxoutchannel, double samplerate, int framesPerBuffer, int usingoutchannels) {
    this->name = name;
    this->maxinchannels = maxinchannel;
    this->maxoutchannels = maxoutchannel;
    this->samplerate = samplerate;
    this->framesPerBuffer = framesPerBuffer;
    this->usingoutchannels = usingoutchannels;
}

Device::Device() {}

AudioInterface::AudioInterface() {
    this->sample_rate =  SAMPLE_RATE;
    this->fpb = FPB;
    enumerate();
}

AudioInterface::AudioInterface(double sample_rate) {
    this->sample_rate = sample_rate;
    this->fpb = FPB;
    enumerate();
}

AudioInterface::AudioInterface(long int fpb) {
    this->sample_rate = SAMPLE_RATE;
    this->fpb = fpb;
    enumerate();
}

AudioInterface::AudioInterface(double sample_rate, long int fpb) {
    this->sample_rate = sample_rate;
    this->fpb = fpb;
    enumerate();
}

AudioInterface::~AudioInterface() {
    errorCheck(Pa_Terminate(), "Terminating");
    for(int i=0; i<this->devices.size(); i++) {
        delete this->devices[i];
    }
}

void AudioInterface::setSampleRateAndFPB(double sample_rate, long int fpb)
{
    this->sample_rate = sample_rate;
    this->fpb = fpb;
}

void AudioInterface::enumerate() {
    errorCheck(Pa_Initialize(), "Initializing");
    int numdevices = Pa_GetDeviceCount();
    for(int i=0; i<numdevices; i++) {
        const PaDeviceInfo* deviceinfo = Pa_GetDeviceInfo(i);
        this->devices.push_back(new Device(deviceinfo->name, deviceinfo->maxInputChannels, deviceinfo->maxOutputChannels, this->sample_rate, this->fpb, 0));
    }
}

std::vector<Device*>* AudioInterface::getDevices() {
    return &(this->devices);
}

Device AudioInterface::getDetails() {
    return this->useddevice;
}


int AudioInterface::audioCallback(const void* input, void* output, unsigned long framesPerBuffer, const  PaStreamCallbackTimeInfo* timeInfo,PaStreamCallbackFlags statusFlags, void* userdata) {
    auto* self = static_cast<AudioInterface*>(userdata);
    float** in = (float**)input;
    float** out = (float**)output;
    if(self->callback)
        self->callback(in, out);
	return paContinue;
}

void AudioInterface::openStream(int index) {
    PaStreamParameters inputparams;
    PaStreamParameters outputparams;

    this->useddevice = *(this->devices[index]);

    inputparams.device = index;
    inputparams.channelCount = devices[index]->maxinchannels;
    inputparams.sampleFormat = paFloat32 | paNonInterleaved;
    inputparams.suggestedLatency = Pa_GetDeviceInfo(index)->defaultLowInputLatency;
    inputparams.hostApiSpecificStreamInfo = NULL;

    outputparams.device = index;
    outputparams.channelCount = (devices[index]->maxoutchannels > 1) ? 2 : 1;
    devices[index]->usingoutchannels = outputparams.channelCount;
    outputparams.sampleFormat = paFloat32 | paNonInterleaved;
    outputparams.suggestedLatency = Pa_GetDeviceInfo(index)->defaultLowOutputLatency;
    outputparams.hostApiSpecificStreamInfo = NULL;   

    this->useddevice.framesPerBuffer = this->fpb;
    this->useddevice.samplerate = this->sample_rate;

    errorCheck(Pa_OpenStream(&(this->stream), &inputparams, &outputparams, this->sample_rate, this->fpb, paNoFlag, this->audioCallback, (void*)this), "Opening stream");
}

int AudioInterface::openDevice(int index, std::function<void(float**, float**)> cb) {
    this->callback = cb;
    this->openStream(index);
    errorCheck(Pa_StartStream(this->stream), "Starting stream");
    return 0;
}

int AudioInterface::openDevice(const char *name, std::function<void(float**, float**)> cb) {
    this->callback = cb;
    int index = -1;
    for(int i=0; i<this->devices.size(); i++) {
        if(strcmp(name, this->devices[i]->name) == 0) {
            index = i;
            break;
        }
    }
    if(index == -1)
        return -1;
    this->openStream(index);
    errorCheck(Pa_StartStream(this->stream), "Starting stream");
    return 0;
}

int AudioInterface::closeDevice() {
    errorCheck(Pa_StopStream(this->stream), "Stopping stream");
    errorCheck(Pa_CloseStream(this->stream), "Closing stream");
    return 0;
}


int AudioInterface::errorCheck(PaError error, const char* place) {
	if(error != paNoError) {
		printf("Error occured when %s... Error : %s\n", place, Pa_GetErrorText(error));
		return 1;
	}else {
		printf("%s...\n", place);
		return 0;
	}
}