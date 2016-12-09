//hardware midi library
#include <MIDI.h>

//dac library and guitool code
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioSynthWaveform       waveform1;      //xy=188,240
AudioEffectEnvelope      envelope1;      //xy=371,237
AudioConnection          patchCord1(waveform1, envelope1);
AudioOutputAnalog        dac1;           //xy=389,105
AudioConnection          patchCord2(envelope1, dac1);

void setup() {
//midi
  MIDI.begin(MIDI_CHANNEL_OMNI); // This only affects reception. You can send on any MIDI channel regardless of this setting.

//usbmidi setup
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleVelocityChange(OnVelocityChange);
  usbMIDI.setHandleControlChange(OnControlChange);
  usbMIDI.setHandleProgramChange(OnProgramChange);
  usbMIDI.setHandleAfterTouch(OnAfterTouch);
  usbMIDI.setHandlePitchChange(OnPitchChange);
  usbMIDI.setHandleRealTimeSystem(RealTimeSystem);

//dac pulse code
  AudioMemory(12);
  waveform1.pulseWidth(0.5);
  waveform1.begin(1, 220, WAVEFORM_PULSE);
  envelope1.attack(50);
  envelope1.decay(50);
  envelope1.release(250);

//logginf
  Serial.begin(9600);
}

void loop() {
//usb midi
usbMIDI.read();

//dac
int sensorValue = 1000;
  Serial.println(sensorValue);
  float w;
  for (uint32_t i =1; i<20; i++) {
    w = i / 20.0;
    waveform1.pulseWidth(w);
    envelope1.noteOn();
    delay(sensorValue);
    envelope1.noteOff();
    delay(sensorValue);
  }
}

// RealTimeSystem functions for midi clock
void RealTimeSystem(byte realtimebyte) {
    if(realtimebyte == Clock) {
      MIDI.sendRealTime(Clock);
    }
    if(realtimebyte == Start){
      MIDI.sendRealTime(Start);
    }
    if(realtimebyte == Stop) {
      MIDI.sendRealTime(Stop);
    }
    if(realtimebyte == Continue) {
      MIDI.sendRealTime(Continue);
    }
};
void OnNoteOn(byte channel, byte note, byte velocity){
  MIDI.sendNoteOn(note, velocity, channel);
};
void OnNoteOff(byte channel, byte note, byte velocity){
  MIDI.sendNoteOff(note, velocity, channel);
};
void OnVelocityChange(byte channel, byte note, byte velocity){  
  MIDI.sendPolyPressure(note, velocity, channel);
};
void OnControlChange(byte channel, byte control, byte value){
  MIDI.sendControlChange(control, value, channel);
};
void OnProgramChange(byte channel, byte program){
  MIDI.sendProgramChange(program, channel);
};
void OnAfterTouch(byte channel, byte pressure){
  MIDI.sendAfterTouch(pressure, channel);
};
void OnPitchChange(byte channel, int pitch){
  MIDI.sendPitchBend(pitch, channel);
};
