#include "Arduino.h"
#include "VirtualPatchCable.h"

VirtualPatchCable::VirtualPatchCable(AudioStreamSet initSourceSet, AudioStreamSet initDestSet) {
	sourceSet = &initSourceSet;
	destSet = &initDestSet;
	Serial.println("Added virtual patch cable");
	for(int i=0; i<MAX_POLYPHONY; i++) {
		audioConnections[i] = new AudioConnection(*sourceSet->audioStreams[i], 0, *destSet->audioStreams[i], 0);
	}
}

VirtualPatchCable::~VirtualPatchCable() {
	Serial.println("Removed virtual patch cable");
	// important to handle disconnections/deletions here to prevent memory leaks
	for(int i=0; i<MAX_POLYPHONY; i++) {
		audioConnections[i]->disconnect();
	}
}
