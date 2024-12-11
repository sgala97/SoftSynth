#pragma once
#include "Globals.h"
#include <vector>
#include "AuxDsp.h"

class Delay {
public:
    // Delay constructor with sample rate as parameter
    // Parameters: double sampleRate
    Delay();
    ~Delay();

    // Set delay parameters
    void setDelayParameters(float delayTime, float feedback, float wetLevel,
                             float dryLevel);

    //play delay
    void play();

    //stop delay
    void stop();

    //Process delay and return the sample
    float getDelaySample(float inputSample);

    // get if delay is playing
    bool isPlaying();


    // get if delay is still playing
    bool isProcessing();



private:
    std::vector<float> delayBuffer;
    int delayBufferIndex;

    int maxDelayIndex;
    float feedback;
    float wetLevel;
    float dryLevel;

    bool firstIteration;

    // process if delay is still playing
    void processIfDelayIsProcessing(float sample);

    bool playing, stillPlaying;
};
