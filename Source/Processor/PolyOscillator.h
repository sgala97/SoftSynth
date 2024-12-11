#pragma once
#include "Oscillator.h"
#include "LFO.h"
#include "Globals.h"
#include <set>
#include <vector>

class PolyOscillator {
public:

    PolyOscillator(int midi_notes);
    ~PolyOscillator();
    // Function to play an oscillator given an index and a sample rate
    // The oscillator is added to the oscillator set and the increment of
    // the particular oscillator is set
    // Parameters: int index, double sampleRate
    // Returns: void
    void playOscillator(int index, int waveTableSize,
                        Envelope::EnvelopeParameters envelopeParameters);
    // Function to stop an oscillator given an index
    // The oscillator is removed from the oscillator set
    // Parameters: int index
    // Returns: void
    void stopOscillator(int index);

    // Function to iterate through all the oscillators in the oscillator set
    // and get the next sample of each oscillator, summing them up and returning
    // Parameters: const std::vector<float> &waveTable
    // Returns: float
    float getNextSample(const std::vector<float> &waveTable, bool noise);


    // Function to check if there are any oscillators playing
    // Parameters: void
    // Returns: bool
    bool isProcessing();


private:
    std::vector<Oscillator> oscillators;
    std::set<int> oscillatorSet;

    // Function to initialize the oscillators
    // Parameters: void
    // Returns: void
    void initializeOscillators(int midi_notes);
};
