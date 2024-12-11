#include "AuxDsp.h"
#include "Globals.h"

// Get the linear value of a dB value
float myDsp::dbToLinearZeroMinus60(float dB) {
    if (dB <= -60.0f)
        return 0.0f;
    return pow(10.0f, dB / 20.0f);
}

float myDsp::linearToDb(float linear) {
    linear = std::abs(linear);
    if (linear == 0.0f) {
        return -60.0f;
    }
    return 20.0f * log10(linear);
}

// Apply gain to a sample function
float myDsp::applyGain(float value, float gain) {
    return value * dbToLinearZeroMinus60(gain);
}

// Sinc function
double myDsp::sinc(const double x) {
    if (x == 0)
        return 1;
    return sin(M_PI * x) / (M_PI * x);
}

// InterPolation function, gets starValue, endValue and weight value
float myDsp::linearInterpolation(float starValue, float endValue,
                               float interpolationWeight) {
    return (1.0f - interpolationWeight) * starValue +
           interpolationWeight * endValue;
}

// FFT function
void myDsp::fft(std::vector<cd> &a, bool invert) {
    int n = a.size();

    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;

        if (i < j)
            swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len / 2; j++) {
                cd u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (cd &x : a)
            x /= n;
    }
}

// Function that gets harmonics and their phases and builts the waveTable for
// that harmonics
void myDsp::harmonicsToWave(std::vector<Pair> &harmonics,
                          std::vector<float> &waveTable) {
    // Getting the waveTableSize and creating a complex buffer of the same size
    // to perform the ifft
    const int waveTableSize = waveTable.size();
    std::vector<cd> bufferComplex(waveTableSize, 0.0);

    // Iterate through each harmonic and allocate in the complex buffer
    // It's shifted pi/2 by default in order to get a perfect sine wave
    // and not a cosine wave (result of ifft)
    // Also check if all magnitudes are 0, if so, fill waveTable with 0 and
    // return
    bool allZero = true;
    for (int i = 0; i < harmonics.size(); i++) {
        if (harmonics[i].magnitude != 0)
            allZero = false;
        bufferComplex[i + 1] =
            cd(harmonics[i].magnitude * cos(PI / 2 + harmonics[i].phase),
               harmonics[i].magnitude * sin(PI / 2 + harmonics[i].phase));
    }

    if (allZero) {
        for (int i = 0; i < waveTableSize; i++) {
            waveTable[i] = 0;
        }
        return;
    }

    // Enforce conjugate symmetry
    for (int i = waveTableSize / 2; i < waveTableSize; ++i) {
        bufferComplex[i] = std::conj(bufferComplex[waveTableSize - i]);
    }

    // Perform ifft
    fft(bufferComplex, true);

    // Copy real part of ifft to waveTable and normalize from -1 to 1
    waveTable[0] = static_cast<float>(bufferComplex[0].real());
    float min = waveTable[0], max = waveTable[0];
    for (int i = 1; i < waveTableSize; i++) {
        waveTable[i] = static_cast<float>(bufferComplex[i].real());
        if (waveTable[i] < min)
            min = waveTable[i];
        else if (waveTable[i] > max)
            max = waveTable[i];
    }

    if (max - min == 0) {
        for (int i = 0; i < waveTableSize; i++) {
            waveTable[i] = 0;
        }
        return;
    }

    // Normalize the vector
    for (int i = 0; i < waveTableSize; i++) {
        waveTable[i] = ((waveTable[i] - min) * 2 / (max - min)) - 1;
    }
}

void myDsp::normalize(std::vector<float> &v) {
    // Get the min and max value of the vector
    float min = v[0], max = v[0];
    for (int i = 1; i < v.size(); i++) {
        if (v[i] < min)
            min = v[i];
        else if (v[i] > max)
            max = v[i];
    }

    // If min and max are the same, fill the vector with 0

    if (max - min == 0) {
        for (int i = 0; i < v.size(); i++) {
            v[i] = 0;
        }
        return;
    }

    // Normalize the vector
    for (int i = 0; i < v.size(); i++) {
        v[i] = ((v[i] - min) * 2 / (max - min)) - 1;
    }
}

myDsp::BiquadFilter::BiquadFilter(): playing(false) {}

myDsp::BiquadFilter::~BiquadFilter() {}

void myDsp::BiquadFilter::setFilter(FilterType type, float frequency, float Q) {
    float omega = 2.0 * M_PI * frequency / Globals::sampleRate;
    float alpha = sin(omega) / (2.0 * Q);
    float cos_omega = cos(omega);

    switch (type) {
    case LowPass:
        b0 = (1.0 - cos_omega) / 2.0;
        b1 = 1.0 - cos_omega;
        b2 = (1.0 - cos_omega) / 2.0;
        a0 = 1.0 + alpha;
        a1 = -2.0 * cos_omega;
        a2 = 1.0 - alpha;
        break;

    case HighPass:
        b0 = (1.0 + cos_omega) / 2.0;
        b1 = -(1.0 + cos_omega);
        b2 = (1.0 + cos_omega) / 2.0;
        a0 = 1.0 + alpha;
        a1 = -2.0 * cos_omega;
        a2 = 1.0 - alpha;
        break;

    case BandPass:
        b0 = Q * alpha;
        b1 = 0.0;
        b2 = -Q * alpha;
        a0 = 1.0 + alpha;
        a1 = -2.0 * cos_omega;
        a2 = 1.0 - alpha;
        break;
    }

    // Normalize the coefficients
    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= a0;
    a2 /= a0;
}

float myDsp::BiquadFilter::process(float input) {
    float output = b0 * input + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;

    x2 = x1;
    x1 = input;
    y2 = y1;
    y1 = output;

    return output;
}

void myDsp::BiquadFilter::play() { playing = true; }
void myDsp::BiquadFilter::stop() { playing = false; }
bool myDsp::BiquadFilter::isPlaying() { return playing; }
