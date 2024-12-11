#pragma once
#include "Globals.h"

class Envelope {
public:
    // Envelope parameters struct;
    struct EnvelopeParameters {
        float attack;
        float decay;
        float sustain;
        float release;
    };
    // Default constructor
    // Parameters: void
    Envelope();

    ~Envelope();

    // Set the parameters of the envelope and reset the time, state and
    // increment 
    // Parameters: EnvelopeParameters envelopeParameters,
    // Returns: void
    void starEnvelope(EnvelopeParameters envelopeParameters);

    // Star the release of the envelope
    // Parameters: void
    // Returns: void
    void startRelease();

    // Get the envelope value
    // Parameters: void
    // Returns: float
    float getEnvelopeValue();

    // enum envelope state
    enum EnvelopeState { a, d, s, r, f };

    // Get the envelope state
    // Parameters: void
    // Returns: EnvelopeState
    EnvelopeState getEnvelopeState();

private:
    // Enum envelope state
    EnvelopeState envelopeState;
    // Times
    float attack, decay, release;
    // Susutain level
    float sustain;

    // Last value to use in case release is called
    float lastValue, lastValueForRelease;

    // Time meausre in samples (1/sampleRate)
    float time;
    float increment;
};
