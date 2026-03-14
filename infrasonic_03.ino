#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "sonify.h"

sonify mysonify;
AudioInputI2S  input; 
AudioOutputI2S out;
AudioControlSGTL5000   audioShield; 
AudioAmplifier ampVol;    // new: volume control

AudioConnection patchCord1(input, 0, mysonify, 0);
AudioConnection patchCord2(input, 1, mysonify, 0);
//AudioConnection patchCord3(mysonify, 0, out, 0);
//AudioConnection patchCord4(mysonify, 0, out, 1);

AudioConnection patchCord3(mysonify, 0, ampVol, 0);   // changed
AudioConnection patchCord4(ampVol, 0, out, 0);        // changed
AudioConnection patchCord5(ampVol, 0, out, 1);        // new

// Infrasonic Modulator
// Ted Apel
// 2019-2020
// Version 1: All in the Teensy Audio language
// Version 2: Moved the Algorithm to Faust
// Version 3: Nov 2025. Added Volume knob. 

/*
 * SineModulated.frequency(freq);
Set the center frequency, from 0 to 11000.
The output will be this center frequency when 
the input modulation signal is zero. 
Modulation input 1.0 causes the frequency to double, 
and input -1.0 causes zero Hz (DC) output. 
For less modulation, attenuate the input signal 
(perhaps with a mixer object) before it arrives here.
 */


// Parameters for potentiometers:
int val1;
int val2;
int depth;  // 1: Depth of FM, amp1.gain
int carrierfreq; // 2: Carrier Frequency sine_fm1.frequency


void setup() {
// put your setup code here, to run once:
AudioMemory(80); // 950 max
audioShield.enable();
audioShield.inputSelect(AUDIO_INPUT_MIC);
audioShield.muteHeadphone();
audioShield.micGain(30);  // 30 is good. 63 is the max gain.
audioShield.adcHighPassFilterDisable(); //Completely disable the analog input filter. DC and sub-audible low frequencies are allowed to enter the digital signal. 
audioShield.dacVolumeRampDisable(); 

//filter1.frequency(20);
//filter1.resonance(0.7);

//amp1.gain(3.0);  // set gain on FM. 3.0 works
//amp2.gain(20.0);  // set output gain.

//sine_fm1.amplitude(0.8); // sine amplitude
//sine_fm1.frequency(100); // base frequency
  
  ampVol.gain(0.5);  // reasonable starting volume (0.0–1.0 typical)
  Serial.begin(38400); // remove when not in uses

}

void loop() {
  depth = analogRead(A16);
  carrierfreq = map(analogRead(A14), 0, 1023, 150, 0);

  mysonify.setParamValue("Range", depth);
  mysonify.setParamValue("Frequency", carrierfreq);

  int volRaw = analogRead(A18);
  float volNorm = volRaw / 1023.0f;    // 0.0–1.0
  float vol = volNorm * volNorm;  // quadratic taper
  ampVol.gain(vol * 10.0f);  // Turn it up by 5.


  Serial.print("depth 16 is: ");
  Serial.print(depth);
  Serial.print("     carrierfreq 14 is: ");
  Serial.print(carrierfreq);
  Serial.print("     volume 18 is: ");
  Serial.println(vol);

  delay(30);
}



/* Backup Faust code
 * // Will FM and AM the infrasonic signal

freq = hslider("Frequency",100,0,150,0.01);
range = hslider("Range",100,0,100,0.01);
width = 8;  // 10 works.
input = _ : fi.lowpass(1,20);
//scaledinput = ((input + 1.0) * .5) * range ;
scaledinput = input * range ;
fm = scaledinput  + freq ; // This should be + not *.
fl = freq - width;
fu = freq + width;
process =  osc(fm) : fi.bandstop(1, fl, fu) : fi.bandstop(1, fl, fu) : fi.bandstop(1, fl, fu);
//process =  osc(fm) : fi.bandstop(2, fl, fu)  ;
//process =   fi.bandstop(0, fl, fu)  ;

osc(freq) = rdtable(tablesize, os.sinwaveform(tablesize), int(os.phasor(tablesize,freq)))
with{
    tablesize = 1 << 15; // instead of 1 << 16
};

 */
