#pragma once
#include "Globals.h"
#include <cmath>
#include <complex>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <vector>

using cd = std::complex<double>;
const double PI = acos(-1);

// Simple struct that holds a magnitude and a phase
typedef struct Pair {
    double magnitude;
    double phase;
} Pair;

// namespace dsp
namespace myDsp {
// db to linear
float dbToLinearZeroMinus60(float dB);
// linear to db
float linearToDb(float linear);
// apply db gain
float applyGain(float value, float gain);
// sinc function
double sinc(const double x);
// InterPolation function, gets starValue, endValue and weight value
float linearInterpolation(float starValue, float endValue,
                          float interpolationWeight);

void fft(std::vector<cd> &a, bool invert);
// Low pass filter window method in the frequency domain
void lowPassFilter(std::vector<cd> &a, double frequency);
// Function that generates a wave table from a set of harmonics
void harmonicsToWave(std::vector<Pair> &harmonics,
                     std::vector<float> &waveTable);
// Function that gets a vector and normalizes it between -1 and 1
void normalize(std::vector<float> &v);

// Class that implements a Biquad filter
class BiquadFilter {
public:
    enum FilterType { LowPass, HighPass, BandPass };

    BiquadFilter();
    ~BiquadFilter();

    void setFilter(FilterType type, float frequency, float Q);

    float process(float input);

    // Play and stop functions
    void play();
    void stop();

    // Return is playing
    bool isPlaying();

private:
    float b0, b1, b2;
    float a0, a1, a2;
    float x1 = 0.0, x2 = 0.0;
    float y1 = 0.0, y2 = 0.0;
    
    bool playing;
    
};
} // namespace dsp
