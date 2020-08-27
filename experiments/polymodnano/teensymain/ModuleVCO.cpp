#include "Arduino.h"
#include "ModuleVCO.h"

ModuleVCO::ModuleVCO() {
  Serial.println("New VCO module created");
  socketInputs[0] = new SocketInput(); // freq mod 1
  socketInputs[1] = new SocketInput(); // freq mod 2
  socketOutputs[0] = new SocketOutput(); // saw
  analogInputs[0] = new AnalogInput();
  
  for(int i=0; i<MAX_POLYPHONY; i++) {
    _oscSawSet.audioStreams[i] = &_oscSaw[i];
    _oscSaw[i].begin(0.2,500,WAVEFORM_SAWTOOTH);
    _oscSaw[i].frequencyModulation(5);
    _modMixerSet.audioStreams[i] = &_modMixer[i];
    _modMixer[i].gain(0,1);
    _modMixer[i].gain(1,1);
    _modMixer[i].gain(2,1);
  }
  _patchCable1 = new VirtualPatchCable(_oscSawSet, 0, socketOutputs[0]->audioStreamSet, 0);
  _patchCable2 = new VirtualPatchCable(socketInputs[0]->audioStreamSet, 0, _modMixerSet, 0);
  _patchCable3 = new VirtualPatchCable(socketInputs[1]->audioStreamSet, 0, _modMixerSet, 1);
  _analogCable = new VirtualPatchCable(analogInputs[0]->audioStreamSet, 0, _modMixerSet, 2);
  _modCable = new VirtualPatchCable(_modMixerSet, 0, _oscSawSet, 0);
  
}

ModuleVCO::~ModuleVCO() {
  // destroy
}

void ModuleVCO::update() {

}
