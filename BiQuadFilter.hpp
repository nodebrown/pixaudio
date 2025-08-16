#include <cmath>
#include <iostream>

class BiquadFilter {
public:
    enum FilterType { Peaking };

    BiquadFilter(FilterType type, double sampleRate, double freq, double Q, double gainDB)
        : type(type), fs(sampleRate), f0(freq), Q(Q), gainDB(gainDB) {
        calculateCoefficients();
    }

    double process(double x) {
        double y = b0 * x + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;

        x2 = x1;
        x1 = x;
        y2 = y1;
        y1 = y;

        return y;
    }

    void updateParams(double Q, double gainDB) {
        this->Q = Q;
        this->gainDB = gainDB;
        calculateCoefficients();
    }

    double getQ() {
        return this->Q;
    }

    double getFreq() {
        return this->f0;
    }

    double getgainDB() {
        return this->gainDB;
    }

    void setQ(double q) {
        this->Q = q;
        calculateCoefficients();
    }

    void setFreq(double freq) {
        this->f0 = freq;
        calculateCoefficients();
    }

    void setGain(double gain) {
        this->gainDB = gain;
        calculateCoefficients();
    }

private:
    FilterType type;
    double fs, f0, Q, gainDB;

    double b0 = 0, b1 = 0, b2 = 0, a1 = 0, a2 = 0;

    double x1 = 0, x2 = 0, y1 = 0, y2 = 0;

    void calculateCoefficients() {
        double A = std::pow(10, gainDB / 40.0);
        double omega = 2.0 * M_PI * f0 / fs;
        double alpha = std::sin(omega) / (2.0 * Q);
        double cosw = std::cos(omega);

        if (type == Peaking) {
            double a0 = 1 + alpha / A;
            b0 = (1 + alpha * A) / a0;
            b1 = (-2 * cosw) / a0;
            b2 = (1 - alpha * A) / a0;
            a1 = (-2 * cosw) / a0;
            a2 = (1 - alpha / A) / a0;
        }
    }
};