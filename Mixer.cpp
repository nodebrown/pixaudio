#ifndef MIXER_INCLUDED

#define MIXER_INCLUDED

#include <vector>
#include <cmath>
#include <cassert>

class Mixer {
private:
    int channelSize;


    void fwht(std::vector<float>& data) {
        int n = data.size();
        for (int len = 1; len < n; len *= 2) {
            for (int i = 0; i < n; i += 2 * len) {
                for (int j = 0; j < len; ++j) {
                    float u = data[i + j];
                    float v = data[i + j + len];
                    data[i + j] = u + v;
                    data[i + j + len] = u - v;
                }
            }
        }
    }

public:
    Mixer(int channelSize) {
        this->channelSize = channelSize;
    }


    void process(std::vector<float>& input) {
        // float ch0 = 1 * input[0] + 1 * input[1] + 1 * input[2] + 1 * input[3] + 1 * input[4] + 1 * input[5] + 1 * input[6] + 1 * input[7];
        // float ch1 = 1 * input[0] + -1 * input[1] + 1 * input[2] + -1 * input[3] + 1 * input[4] + -1 * input[5] + 1 * input[6] + -1 * input[7];
        // float ch2 = 1 * input[0] + 1 * input[1] + -1 * input[2] + -1 * input[3] + 1 * input[4] + 1 * input[5] + -1 * input[6] + -1 * input[7];
        // float ch3 = 1 * input[0] + -1 * input[1] + -1 * input[2] + 1 * input[3] + 1 * input[4] + -1 * input[5] + -1 * input[6] + 1 * input[7];
        // float ch4 = 1 * input[0] + 1 * input[1] + 1 * input[2] + 1 * input[3] + -1 * input[4] + -1 * input[5] + -1 * input[6] + -1 * input[7];
        // float ch5 = 1 * input[0] + -1 * input[1] + 1 * input[2] + -1 * input[3] + -1 * input[4] + 1 * input[5] + -1 * input[6] + 1 * input[7];
        // float ch6 = 1 * input[0] + 1 * input[1] + -1 * input[2] + -1 * input[3] + -1 * input[4] + -1 * input[5] + 1 * input[6] + 1 * input[7];
        // float ch7 = 1 * input[0] + -1 * input[1] + -1 * input[2] + 1 * input[3] + -1 * input[4] + 1 * input[5] + 1 * input[6] + -1 * input[7];

        // input[0] = ch0;
        // input[1] = ch1;
        // input[2] = ch2;
        // input[3] = ch3;
        // input[4] = ch4;
        // input[5] = ch5;
        // input[6] = ch6;
        // input[7] = ch7;

        fwht(input);

        // float a = input[0]
        // size_t N = channelSize;
        // assert(input.size() == channelSize);
        //     assert((N & (N - 1)) == 0 && "Hadamard transform requires power-of-2 size");

        //     for (size_t len = 1; len < N; len *= 2)
        //     {
        //         for (size_t i = 0; i < N; i += 2 * len)
        //         {
        //             for (size_t j = 0; j < len; ++j)
        //             {
        //                 // float a = input.at(i + j);
        //                 // float b = input.at(i + j + len);
        //                 // input.at(i + j) =  a + b;
        //                 // input.at(i + j + len) =  a - b;

        //                 float a = input[i + j];
        //                 float b = input[i + j + len];
        //                 input[i + j] = a + b;
        //                 input[i + j + len] = a - b;

        //             }
        //         }
        //     }
            float scale = 1.0f / std::sqrt(input.size());
            for (float& v : input) {
                v *= scale;
            }
        // // In-place fast Hadamard transform for a vector of size N = power of 2
    }
};


#endif