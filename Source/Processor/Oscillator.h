#pragma once

#include "AuxDsp.h"
#include "Envelope.h"
#include "LFO.h"
#include "Globals.h"
#include <set>
#include <vector>
#include <random>

class Oscillator {
public:
    // Constructor of Oscillator, just set the fequency to calculate
    // the increment of the index after set the sample rate
    // Parameters: float frequency
    Oscillator(int oscillatorIndex);
    ~Oscillator();
    // Set the frequency of the oscillator with index, don't needeed probably
    // Change the increment of the index
    // Parameters: float frequency
    // Return: void
    void setFrequency(int oscillatorIndex);

    // Set the frequency of the oscillator with frequency, and change the
    // increment

    // Set the increment of the index
    // Parameters: double sampleRate, int waveTableSize
    // Return: void
    void play(int waveTableSize,
              Envelope::EnvelopeParameters envelopeParameters);

    // just star the release
    // Parameters: void
    // Return: void
    void stop();

    // Return struct for next sample, if the bool is true, the polyOscllator
    // will remove the oscillator
    struct NextSample {
        float sample;
        bool remove;
    };

    // Get the next sample of the oscillator
    // Parameters: void
    // Return: float
    NextSample getNextSample(const std::vector<float> &waveTable);

    NextSample getNextSampleWhiteNoise();


    

private:
    // Referene to the sample rate

    float frequency;
    float index;
    float baseIndexIncrement;
    float modIndexIncrement;

    Envelope envelope;

    int oscillatorIndex;

    float indextToFrequency(float index);

    float centsToFrequencyRatio(float cents);

};
