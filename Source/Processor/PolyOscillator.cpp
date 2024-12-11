#include "PolyOscillator.h"

PolyOscillator::PolyOscillator(int midi_notes) {
    initializeOscillators(midi_notes);
}

PolyOscillator::~PolyOscillator() {}

void PolyOscillator::playOscillator(
    int index, int waveTableSize,
    Envelope::EnvelopeParameters envelopeParameters) {
    oscillators[index].play(waveTableSize, envelopeParameters);
    oscillatorSet.insert(index);
}

void PolyOscillator::stopOscillator(int index) { oscillators[index].stop(); }

void PolyOscillator::initializeOscillators(int midi_notes) {
    for (int i = 0; i < midi_notes; i++) {
        oscillators.push_back(Oscillator(i));
    }
}

float PolyOscillator::getNextSample(const std::vector<float> &waveTable,
                                    bool noise) {
    float sample = 0.0f;
    Oscillator::NextSample nextSample;

    for (auto it = oscillatorSet.begin(); it != oscillatorSet.end();) {
        if (noise) {
            nextSample = oscillators[*it].getNextSampleWhiteNoise();
        } else {
            nextSample = oscillators[*it].getNextSample(waveTable);
        }
        sample += nextSample.sample;
        if (nextSample.remove) {
            it = oscillatorSet.erase(it);
        } else {
            ++it;
        }
    }
    return sample;
}

bool PolyOscillator::isProcessing() { return !oscillatorSet.empty(); }
