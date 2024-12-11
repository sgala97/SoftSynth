#include "LFO.h"

LFO::LFO(int waveTableSize) : waveTable(waveTableSize, 0.0f), playing(false) {}

LFO::~LFO() {}

void LFO::setLFOParameters(float frequency, float min, float max) {
    indexIncrement =
        frequency * waveTable.size() / static_cast<float>(Globals::sampleRate);

    this->min = min;
    this->max = max;
}

void LFO::setWaveTable(WaveTableType waveTableType) {
    // Wave Table with range from 0 to 1
    switch (waveTableType) {
    case WaveTableType::sine:
        for (int i = 0; i < waveTable.size(); i++) {
            waveTable[i] =
                (std::sin(2 * M_PI * i / waveTable.size()) + 1.0f) / 2.0f;
        }
        break;
    case WaveTableType::square:
        for (int i = 0; i < waveTable.size(); i++) {
            waveTable[i] = i < waveTable.size() / 2 ? 1.0f : 0.0f;
        }
        break;
    case WaveTableType::saw:
        for (int i = 0; i < waveTable.size(); i++) {
            waveTable[i] = 1.0f - 1.0f * i / waveTable.size();
        }
        break;
    case WaveTableType::triangle:
        for (int i = 0; i < waveTable.size(); i++) {
            if (i < waveTable.size() / 2) {
                waveTable[i] = 2.0f * i / waveTable.size();  // Sube de 0 a 1
            } else {
                waveTable[i] = 2.0f * (waveTable.size() - i) / waveTable.size();  // Baja de 1 a 0
            }
        }
        break;
    }
}

void LFO::play() {
    index = 0;
    playing = true;
}

void LFO::stop() { playing = false; }

void LFO::resetIndex() { index = 0; }

float LFO::getLFOSample() {
    // from 0 to 1 to range (min, max)
    return sample * (max - min) + min;
}

void LFO::nextSample() {
    const int startIndex = static_cast<int>(index);
    const int endIndex = (static_cast<int>(index) + 1) % waveTable.size();

    const float startValue = waveTable[startIndex];
    const float endValue = waveTable[endIndex];

    sample =
        myDsp::linearInterpolation(startValue, endValue, std::fmod(index, 1));

    index =
        std::fmod(index + indexIncrement, static_cast<float>(waveTable.size()));
}

bool LFO::isPlaying() { return playing; }


