#include "WaveTableOscillator.h"
#include "AuxDsp.h"

WaveTableOscillator::WaveTableOscillator(int waveTableSize)
    : waveTables(2), mainWaveTable(waveTableSize, 0.0f) {
    waveTables[0] = std::vector<float>(waveTableSize, 0.0f);
    waveTables[1] = std::vector<float>(waveTableSize, 0.0f);
}

WaveTableOscillator::WaveTableOscillator(
    std::vector<WaveTableType> waveTableTypes,
    std::vector<std::vector<Pair>> harmonics, int waveTableSize)
    : waveTables(2), mainWaveTable(waveTableSize, 0.0f) {

    setWaveTable(0, waveTableTypes[0], harmonics[0]);
    setWaveTable(1, waveTableTypes[1], harmonics[1]);
}

WaveTableOscillator::~WaveTableOscillator() {}

void WaveTableOscillator::setWaveTable(int index, WaveTableType waveTableType,
                                       std::vector<Pair> harmonics) {
    switch (waveTableType) {

    case WaveTableType::custom:
        myDsp::harmonicsToWave(harmonics, waveTables[index]);
        break;

    case WaveTableType::sine:
        for (int i = 0; i < waveTables[index].size(); i++) {
            waveTables[index][i] =
                sin(2.0f * M_PI * i / waveTables[index].size());
        }
        break;

    case WaveTableType::square:
        for (int i = 0; i < waveTables[index].size(); i++) {
            waveTables[index][i] =
                (i < waveTables[index].size() / 2) ? 1.0f : -1.0f;
        }
        break;

    case WaveTableType::saw:
        for (int i = 0; i < waveTables[index].size(); i++) {
            waveTables[index][i] = 1.0f - 2.0f * i / waveTables[index].size();
        }

        break;
    case WaveTableType::triangle:
        for (int i = 0; i < waveTables[index].size(); i++) {
            waveTables[index][i] =
                (i < waveTables[index].size() / 2)
                    ? -1.0f + 4.0f * i / waveTables[index].size()
                    : 3.0f - 4.0f * i / waveTables[index].size();
        }
        break;

    default:
        break;
    }
}

void WaveTableOscillator::updateMainWaveTable(
    const std::vector<bool> waveTablePlaying,
    const std::vector<float> waveTableVolumes) {

    for (int i = 0; i < mainWaveTable.size(); i++) {
        mainWaveTable[i] = 0.0f;
        if (waveTablePlaying[0])
            mainWaveTable[i] += waveTables[0][i] * waveTableVolumes[0];
        if (waveTablePlaying[1])
            mainWaveTable[i] += waveTables[1][i] * waveTableVolumes[1];
    }
}

const std::vector<float> &WaveTableOscillator::getWaveTable() {
    return mainWaveTable;
}
