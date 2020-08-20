#include "Arduino.h"
#include "VirtualPatchCable.h"

VirtualPatchCable::VirtualPatchCable(AudioStreamSet sourceSet, int sourceSocketNum, AudioStreamSet destSet, int destSocketNum) {
  Serial.println("Added virtual patch cable");
  destSet.addInput(&sourceSet);
  Serial.print(sourceSet.ref);
  Serial.print(" to ");
  Serial.println(destSet.ref);
  for(int i=0; i<MAX_POLYPHONY; i++) {
    audioConnections[i] = new AudioConnection(*sourceSet.audioStreams[i], sourceSocketNum, *destSet.audioStreams[i], destSocketNum);
  }
}

VirtualPatchCable::~VirtualPatchCable() {
  Serial.println("Removed virtual patch cable");
  // important to handle disconnections/deletions here to prevent memory leaks
  for(int i=0; i<MAX_POLYPHONY; i++) {
    audioConnections[i]->disconnect();
  }
}
