![alt text](https://github.com/sgala97/SoftSynth/blob/main/media/SoftSynthLogo.png?raw=true)

A simple software synthesizer. Standalone application and VST3 plugin.

Characteristics:
- MIDI input.
- Polyphonic.
- Two oscillators: Sine, Sawtooth, Square, Triangle, Custom (using harmonics)
- White Noise.
- ADSR envelope.
- LFO Amplitude modulation.
- LFO Frequency modulation.
- Delay effect.
- Low pass, high pass, band pass filter.

## Builing Requirements
- CMake
- C++17
- Linux gcc/g++
- See Also: [Dependencies](https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md)

## Building
First, clone the repository:
```bash
git clone --recurse-submodules https://github.com/sgala97/SoftSynth.git
```

Then, build the project:
```bash
cmake -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build
```
The standalone application will be located in
```bash 
build/SoftSynth_artefacts/Release/Standalone
```
The VST3 plugin will be in
```bash
build/SoftSynth_artefacts/Release/VST3/SoftSynth.vst3/Contents/x86_64-linux
```

## Interface
![alt text](https://github.com/sgala97/SoftSynth/blob/main/media/SoftSynthImage.png?raw=true)
