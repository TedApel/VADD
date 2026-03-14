# VADD — Volcano Auditory Display Device

Ted Apel and Jeffrey Johnson

A portable real-time infrasound sonification device built on the Teensy 3.6 microcontroller. VADD transposes sub-audible infrasonic signals (below 20 Hz) into the audible range using frequency modulation, allowing researchers to listen to volcanic activity in the field.

The sonification algorithm is written in [Faust](https://faust.grame.fr/) and compiled to C++ for the Teensy Audio Library. An infrasonic microphone captures low-frequency pressure waves, which are low-pass filtered, scaled, and used to frequency-modulate a carrier oscillator. Bandstop filters remove the unmodulated carrier frequency, leaving only the sonified infrasonic content.

## Hardware

- Teensy 3.6 with Audio Shield
- Infrasonic microphone (electret with sub-20 Hz response)
- Two potentiometers: carrier frequency and modulation depth
- Volume knob (added in v3)
- PAM8302A amplifier module

The circuit design is in `circuit/VADD_v2.0.fzz` (Fritzing format).

## Software

- `sonify.dsp` — Faust source code for the sonification algorithm
- `sonify.cpp` — Generated C++ (compiled with `faust2teensy sonify.dsp`)
- `sonify.h` — Generated header
- `infrasonic_03.ino` — Arduino/Teensy sketch (v3, with volume control)

### Version History

1. **v1** — Sonification implemented directly in Teensy Audio Library
2. **v2** — Algorithm rewritten in Faust, compiled to Teensy Audio object
3. **v3** (2025) — Added volume knob

## Publications

Apel, T. & Johnson, J. (2021). Portable Realtime Volcano Infrasound Auditory Display Devices. *Proceedings of the 26th International Conference on Auditory Display (ICAD 2021)*. [PDF](https://vud.org/assets/pdf/ICAD_2021_Apel_Johnson.pdf) | [Video](https://www.youtube.com/watch?v=VQlbgFXnrPo)
