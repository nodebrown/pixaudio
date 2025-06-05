#include <vector>
#include "DelayLine.cpp"
#include "Mixer.cpp"
#include <iostream>
#include <random>

class Diffuser {
private:
    int channelSize;
    std::vector<DelayLine*> delayLines;
    Mixer* mixer;
    int random(int min, int max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(min, max);
        return distrib(gen);
    }
public:
    Diffuser(int channelSize, int timeMs):
        channelSize(channelSize) {
            for(int i=0; i<channelSize; i++) {
                delayLines.push_back(new DelayLine(random(0, timeMs) * 48));
            }
            mixer = new Mixer(channelSize);
    }

    void process(std::vector<float>& input) {
        for(int i=0; i<channelSize; i++) {
            input[i] = delayLines[i]->process(input[i]);
        }
        mixer->process(input);
    }
};