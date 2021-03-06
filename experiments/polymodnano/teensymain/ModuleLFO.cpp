#include "Arduino.h"
#include "ModuleLFO.h"

ModuleLFO::ModuleLFO() {
  Serial.println("New LFO module created");
  socketInputs[0] = new SocketInput(); // freq mod 1
  socketOutputs[0] = new SocketOutput(); // saw
  analogInputs[0] = new AnalogInput();
  
  for(int i=0; i<MAX_POLYPHONY; i++) {
    _oscSawSet.audioStreams[i] = &_oscSaw[i];
    _oscSaw[i].begin(0.2,0.1,WAVEFORM_SINE);
    _oscSaw[i].frequencyModulation(10);
    _modMixerSet.audioStreams[i] = &_modMixer[i];
    _modMixer[i].gain(0,1);
    _modMixer[i].gain(1,1);
  }
  _patchCable1 = new VirtualPatchCable(_oscSawSet, 0, socketOutputs[0]->audioStreamSet, 0);
  _patchCable2 = new VirtualPatchCable(socketInputs[0]->audioStreamSet, 0, _modMixerSet, 0);
  _analogCable = new VirtualPatchCable(analogInputs[0]->audioStreamSet, 0, _modMixerSet, 1);
  _modCable = new VirtualPatchCable(_modMixerSet, 0, _oscSawSet, 0);
  
}

ModuleLFO::~ModuleLFO() {
  // destroy
}

void ModuleLFO::update() {

}
