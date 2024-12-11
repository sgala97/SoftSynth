#include "Oscillator.h"
#include <algorithm>

Oscillator::Oscillator(int oscillatorIndex)
    : frequency(indextToFrequency(oscillatorIndex)), index(0.0f),
      baseIndexIncrement(0.0f),
      oscillatorIndex(oscillatorIndex) {}

Oscillator::~Oscillator() {}

void Oscillator::setFrequency(int oscillatorIndex) {
    frequency = indextToFrequency(oscillatorIndex);
}

void Oscillator::play(int waveTableSize,
                      Envelope::EnvelopeParameters envelopeParameters) {

    // Restart index just in case Envelope is not active
    if (Envelope::EnvelopeState::f == envelope.getEnvelopeState())
        index = 0.0f;

    baseIndexIncrement =
        frequency * waveTableSize / static_cast<float>(Globals::sampleRate);

    // star the envelope
    envelope.starEnvelope(envelopeParameters);

}

void Oscillator::stop() {
    envelope.startRelease();
}

Oscillator::NextSample
Oscillator::getNextSample(const std::vector<float> &waveTable) {
    // Get the modIncrement of the index if the LFO is playing or just the base if
    // not
    if (Globals::lfoF->isPlaying()) {
        modIndexIncrement = baseIndexIncrement *
                            centsToFrequencyRatio(Globals::lfoF->getLFOSample());
    } else {
        modIndexIncrement = baseIndexIncrement;
    }

    // Iterate through the wave table and get the sample, just 1 iteration
    const int startIndex = static_cast<int>(index);
    const int endIndex = (static_cast<int>(index) + 1) % waveTable.size();

    const float startValue = waveTable[startIndex];
    const float endValue = waveTable[endIndex];

    const float sample =
        myDsp::linearInterpolation(startValue, endValue, std::fmod(index, 1));

    index = std::fmod(index + modIndexIncrement,
                      static_cast<float>(waveTable.size()));

    // Get the amplitude of the envelope
    float envelopeValue = envelope.getEnvelopeValue();

    // Check if the envelope is finished to remove the oscillator
    bool remove = false;

    if (envelope.getEnvelopeState() == Envelope::EnvelopeState::f){
        remove = true;
    }

    return {sample * envelopeValue, remove};
}

Oscillator::NextSample Oscillator::getNextSampleWhiteNoise() {
    // Get the amplitude of the envelope
    float sample = Globals::dis(Globals::gen);
    float envelopeValue = envelope.getEnvelopeValue();

    // Check if the envelope is finished to remove the oscillator
    bool remove = false;

    if (envelope.getEnvelopeState() == Envelope::EnvelopeState::f){
        remove = true;
    }

    return {sample * envelopeValue, remove};
}

float Oscillator::indextToFrequency(float index) {
    return 440.0f * pow(2.0f, (index - 69) / 12.0f);
}

float Oscillator::centsToFrequencyRatio(float cents) {
    return pow(2.0f, cents / 1200.0f);
}
