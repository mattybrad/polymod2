#include "Arduino.h"
#include "ModuleVCO.h"

ModuleVCO::ModuleVCO() {
  Serial.println("New VCO module created");
  _socketInputs[0] = new SocketInput(); // freq mod 1
  _socketOutputs[0] = new SocketOutput(); // saw
  
  for(int i=0; i<MAX_POLYPHONY; i++) {
    _oscSawSet.audioStreams[i] = &_oscSaw[i];
    _oscSaw[i].begin(0.2,150+50*i,WAVEFORM_SINE);
  }
  _patchCable1 = new VirtualPatchCable(_oscSawSet, 0, _socketOutputs[0]->audioStreamSet, 0);
  _patchCable2 = new VirtualPatchCable(_socketInputs[0]->audioStreamSet, 0, _oscSawSet, 0);
}

ModuleVCO::~ModuleVCO() {
  // destroy
}

void ModuleVCO::update() {

}
