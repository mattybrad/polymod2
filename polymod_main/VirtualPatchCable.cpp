#include "Arduino.h"
#include "VirtualPatchCable.h"

VirtualPatchCable::VirtualPatchCable(AudioStreamSet initSourceSet, int sourceSocketNum, AudioStreamSet initDestSet, int destSocketNum) {
	sourceSet = &initSourceSet;
	destSet = &initDestSet;
	Serial.println("Added virtual patch cable");
	for(int i=0; i<MAX_POLYPHONY; i++) {
		audioConnections[i] = new AudioConnection(*sourceSet->audioStreams[i], sourceSet->socketNum, *destSet->audioStreams[i], destSet->socketNum);
	}
}

VirtualPatchCable::~VirtualPatchCable() {
	Serial.println("Removed virtual patch cable");
	// important to handle disconnections/deletions here to prevent memory leaks
	for(int i=0; i<MAX_POLYPHONY; i++) {
		audioConnections[i]->disconnect();
	}
}
