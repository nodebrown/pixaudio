#include <vector>
#include "Mixer.cpp"
#include <iostream>

class FeedBackDelay {
public:
    std::vector<std::vector<float>> delayLine;
    std::vector<int> writeIndex;
    std::vector<int> readIndex;
    int m_sampleSize;
    float m_feedBackGain;
    int m_channelSize;
    Mixer* mixer;

public:
    FeedBackDelay(int sampleSize, std::vector<int> delayTimes, int channelSize) :
         m_sampleSize(sampleSize) {
            delayLine.resize(channelSize);
            writeIndex.resize(channelSize, 0);
            readIndex.resize(channelSize, 0);
            for(int i=0; i<channelSize; i++) {
                delayLine[i].resize(m_sampleSize, 0.0f);
                writeIndex[i] = delayTimes[i];
            }
            m_feedBackGain = 0.5f;
            m_channelSize = channelSize;
            mixer = new Mixer(channelSize);
        }

    void process(std::vector<float> inputAndOutput) {
        for(int ch=0; ch<m_channelSize; ch++) {
            int tempReadIndex = readIndex[ch];
            int tempWriteIndex = writeIndex[ch];
            float delayedSample = delayLine[ch][tempReadIndex];
            delayLine[ch][tempWriteIndex] = inputAndOutput[ch] + (delayedSample * m_feedBackGain);
            inputAndOutput[ch] = delayedSample;
            writeIndex[ch] = (tempWriteIndex + 1) % m_sampleSize;
            readIndex[ch] = (tempReadIndex + 1) % m_sampleSize;
        }
        mixer->process(inputAndOutput);
    }
};