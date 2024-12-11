#pragma once
#include "AuxDsp.h"
#include "Globals.h"
#include <cmath>
#include <utility>
#include <vector>

class LFO {
public:
    enum WaveTableType {
        sine,
        square,
        saw,
        triangle
    };
    LFO(int waveTableSize);
    ~LFO();
    void setLFOParameters(float frequency, float min, float max);
    void setWaveTable(WaveTableType waveTableType);

    void play();
    void stop();
    void resetIndex();

    float getLFOSample();
    void nextSample();

    bool isPlaying();


private:
    bool playing;

    float frequency;
    float min, max;

    float sample;
    float index;
    float indexIncrement;

    std::vector<float> waveTable;
};
