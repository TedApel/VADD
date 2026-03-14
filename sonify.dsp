// sonify.dsp — Infrasonic sonification algorithm
// Ted Apel, 2019-2020
// FM and AM modulation of infrasonic signal into audible range
// Compiled to C++ with: faust2teensy sonify.dsp

import("stdfaust.lib");

freq = hslider("Frequency",100,0,150,0.01);
range = hslider("Range",100,0,100,0.01);
width = 8;
input = _ : fi.lowpass(1,20);
scaledinput = input * range ;
fm = scaledinput  + freq ;
fl = freq - width;
fu = freq + width;
process =  osc(fm) : fi.bandstop(1, fl, fu) : fi.bandstop(1, fl, fu) : fi.bandstop(1, fl, fu);

osc(freq) = rdtable(tablesize, os.sinwaveform(tablesize), int(os.phasor(tablesize,freq)))
with{
    tablesize = 1 << 15;
};
