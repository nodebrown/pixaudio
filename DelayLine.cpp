#include <vector>
#include <iostream>

class DelayLine {
private:
    std::vector<float> delayLine;
    int delaySamples;
    int sampleSize;
    int writeIndex;
    int readIndex;

public:
    DelayLine(int delaySamples) {
        this->delaySamples = delaySamples;
        this->sampleSize = 48000;
        this->writeIndex = delaySamples;
        this->readIndex = 0;
        try
        {
            delayLine.resize(sampleSize, 0.0f);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Exception during resize: " << e.what() << std::endl;
        }
    }

    float process(float input) {
        float delayed = delayLine[readIndex];
        delayLine[writeIndex] = input;
        writeIndex = (writeIndex + 1) % sampleSize;
        readIndex = (readIndex + 1) % sampleSize;
        return delayed;
    }
};