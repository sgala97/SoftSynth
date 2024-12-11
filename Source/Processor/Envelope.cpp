#include "Envelope.h"
#include "Globals.h"
#include <cmath>

Envelope::Envelope() : lastValue(0.0f), envelopeState(EnvelopeState::f) {}

Envelope::~Envelope() {}

void Envelope::starEnvelope(EnvelopeParameters envelopeParameters) {
    attack = envelopeParameters.attack;
    decay = envelopeParameters.decay;
    sustain = envelopeParameters.sustain;
    release = envelopeParameters.release;
    time = lastValue * attack;
    increment = 1.0f / Globals::sampleRate;
    envelopeState = EnvelopeState::a;
}

void Envelope::startRelease() {
    time = 0.0f;
    envelopeState = EnvelopeState::r;
    lastValueForRelease = lastValue;
}

float Envelope::getEnvelopeValue() {
    switch (envelopeState) {
    case EnvelopeState::a: {
        float value = time / attack;
        time += increment;
        if (time >= attack) {
            time = std::fmod(time, attack);
            envelopeState = EnvelopeState::d;
        }
        lastValue = value;
        return value;
    } break;
    case EnvelopeState::d: {
        float value = 1.0f - (1.0f - sustain) * (time / decay);
        time += increment;
        if (time >= decay) {
            envelopeState = EnvelopeState::s;
            lastValue = sustain;
        }
        lastValue = value;
        return value;
    } break;
    case EnvelopeState::s: {
        return sustain;
    } break;
    case EnvelopeState::r: {
        float value =
            lastValueForRelease - lastValueForRelease * (time / release);
        time += increment;
        if (time >= release) {
            envelopeState = EnvelopeState::f;
            lastValue = 0.0f;
        }
        lastValue = value;
        return value;
    } break;
    case EnvelopeState::f: {
        return 0.0f;
    } break;
    default:
        return 0.0f;
    }
}

Envelope::EnvelopeState Envelope::getEnvelopeState() { return envelopeState; }
