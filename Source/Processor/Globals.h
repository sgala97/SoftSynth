#pragma once
#include "LFO.h"
#include <random>

// Forward declaration 
class LFO;

struct Globals {
    // Global variables
    static double sampleRate;
    static LFO *lfoF;
    // Random number generator 
    static std::mt19937 gen;
    static std::uniform_real_distribution<float> dis;
};
