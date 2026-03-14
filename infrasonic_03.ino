// Infrasonic Modulator — VADD
// Ted Apel
// 2019-2020
// Version 1: All in the Teensy Audio language
// Version 2: Moved the algorithm to Faust
// Version 3: Nov 2025. Added volume knob.

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "sonify.h"

sonify mysonify;
AudioInputI2S input;
AudioOutputI2S out;
AudioControlSGTL5000 audioShield;
AudioAmplifier ampVol;

AudioConnection patchCord1(input, 0, mysonify, 0);
AudioConnection patchCord2(input, 1, mysonify, 0);
AudioConnection patchCord3(mysonify, 0, ampVol, 0);
AudioConnection patchCord4(ampVol, 0, out, 0);
AudioConnection patchCord5(ampVol, 0, out, 1);

int depth;        // modulation depth (potentiometer on A16)
int carrierfreq;  // carrier frequency (potentiometer on A14)

void setup() {
  AudioMemory(80);
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.muteHeadphone();
  audioShield.micGain(30);
  audioShield.adcHighPassFilterDisable();
  audioShield.dacVolumeRampDisable();

  ampVol.gain(0.5);
  Serial.begin(38400);
}

void loop() {
  depth = analogRead(A16);
  carrierfreq = map(analogRead(A14), 0, 1023, 150, 0);

  mysonify.setParamValue("Range", depth);
  mysonify.setParamValue("Frequency", carrierfreq);

  int volRaw = analogRead(A18);
  float volNorm = volRaw / 1023.0f;
  float vol = volNorm * volNorm;  // quadratic taper
  ampVol.gain(vol * 10.0f);

  Serial.print("depth 16 is: ");
  Serial.print(depth);
  Serial.print("     carrierfreq 14 is: ");
  Serial.print(carrierfreq);
  Serial.print("     volume 18 is: ");
  Serial.println(vol);

  delay(30);
}
