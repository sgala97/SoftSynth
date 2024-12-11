#include "Delay.h"

Delay::Delay() : delayBuffer(192000 * 4), playing(false), stillPlaying(false) {}

Delay::~Delay() {}

void Delay::setDelayParameters(float delayTime, float feedback, float wetLevel,
                               float dryLevel) {
    this->maxDelayIndex = static_cast<int>(delayTime * Globals::sampleRate);
    this->feedback = feedback;
    this->wetLevel = wetLevel;
    this->dryLevel = dryLevel;

    if (delayBuffer.size() < maxDelayIndex)
        delayBuffer.resize(maxDelayIndex);
}

void Delay::play() {
    playing = true;
    delayBufferIndex = 0;
    firstIteration = true;
}

void Delay::stop() {
    playing = false;
    stillPlaying = false;
}

float Delay::getDelaySample(float inputSample) {
    float delaySample = 0.0f;

    if (firstIteration) {

        delayBuffer[delayBufferIndex] = inputSample * feedback;
        delayBufferIndex++;

        if (delayBufferIndex >= maxDelayIndex) {
            delayBufferIndex = 0;
            firstIteration = false;
        }

    } else {

        delaySample = delayBuffer[delayBufferIndex];

        delayBuffer[delayBufferIndex] =
            (inputSample + delayBuffer[delayBufferIndex]) * feedback;

        delayBufferIndex = (delayBufferIndex + 1) % maxDelayIndex;
    }

    delaySample = inputSample * dryLevel + delaySample * wetLevel;
    processIfDelayIsProcessing(delaySample);
    return delaySample;
}

bool Delay::isPlaying() { return playing; }

void Delay::processIfDelayIsProcessing(float sample) {
    static int zeroCounter = 0;
    
    if (myDsp::linearToDb(sample) <= -60.0f){
        zeroCounter++;
        if (zeroCounter >= maxDelayIndex)
        {
            stillPlaying = false;
            zeroCounter = 0;
        }
    }
    else{
        zeroCounter = 0;
        stillPlaying = true;
    }

};

bool Delay::isProcessing() { return stillPlaying; }
