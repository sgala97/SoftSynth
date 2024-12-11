#include "Globals.h"
#include "LFO.h"

double Globals::sampleRate = 0.0;
LFO *Globals::lfoF = nullptr;
std::mt19937 Globals::gen(std::random_device{}());
std::uniform_real_distribution<float> Globals::dis(-1.0f, 1.0f);
