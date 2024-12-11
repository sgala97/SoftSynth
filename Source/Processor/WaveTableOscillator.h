#pragma once
#include "AuxDsp.h"
#include <random>
#include <string.h>
#include <vector>

class WaveTableOscillator {
public:
    // WaveForm enum
    enum WaveTableType { custom, sine, square, saw, triangle};
    // Default constructor
    // Parameters: void
    WaveTableOscillator(int waveTableSize);

    // Constructor with parameters
    // Parameters: float volume, bool playing, WaveTableType waveTableType,
    WaveTableOscillator(std::vector<WaveTableType> waveTableTypes,
                        std::vector<std::vector<Pair>> harmonics,
                        int waveTableSize);
    ~WaveTableOscillator();

    // Set one waveTable
    void setWaveTable(int index, WaveTableType waveTableType,
                      std::vector<Pair> harmonics);

    // Update mainWaveTable
    void updateMainWaveTable(const std::vector<bool> waveTablePlaying,
                             const std::vector<float> waveTableVolumes);

    // Get waveTable reference
    const std::vector<float> &getWaveTable();

private:
    std::vector<std::vector<float>> waveTables;
    std::vector<float> mainWaveTable;

};
