# SoftSynth
![alt text](https://github.com/sgala97/SoftSynth/blob/main/media/SoftSynthLogo.png?raw=true)

A simple software synthesizer.
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
```bash
cmake -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build
```

## Interface
![alt text](https://github.com/sgala97/SoftSynth/blob/main/media/SoftSynthImage.png?raw=true)
