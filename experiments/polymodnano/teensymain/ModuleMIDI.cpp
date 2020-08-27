#include "Arduino.h"
#include "ModuleMIDI.h"

ModuleMIDI::ModuleMIDI() {
  Serial.println("New MIDI module created");
  socketOutputs[0] = new SocketOutput(); // note
  socketOutputs[1] = new SocketOutput(); // gate
  socketOutputs[0]->audioStreamSet.hardcodedPoly = true;
  socketOutputs[0]->audioStreamSet.hardcodedPoly = true;
  
  for(int i=0; i<MAX_POLYPHONY; i++) {
    _noteSet.audioStreams[i] = &_note[i];
    _gateSet.audioStreams[i] = &_gate[i];
    _note[i].amplitude(random(0,250)/1000.0);
  }
  _patchCable1 = new VirtualPatchCable(_noteSet, 0, socketOutputs[0]->audioStreamSet, 0);
  _patchCable2 = new VirtualPatchCable(_gateSet, 0, socketOutputs[1]->audioStreamSet, 0);
  
}

ModuleMIDI::~ModuleMIDI() {
  // destroy
}

void ModuleMIDI::update() {

}
