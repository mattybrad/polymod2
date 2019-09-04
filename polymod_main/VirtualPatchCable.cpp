#include "Arduino.h"
#include "VirtualPatchCable.h"

VirtualPatchCable::VirtualPatchCable(AudioStreamSet initSourceSet, AudioStreamSet initDestSet) {
	sourceSet = &initSourceSet;
	destSet = &initDestSet;
	Serial.print("Added virtual patch cable: ");
	Serial.print(sourceSet->testInt);
	Serial.print("--->");
	Serial.println(destSet->testInt);
	for(int i=0; i<MAX_POLYPHONY; i++) {
		audioConnections[i] = new AudioConnection(*sourceSet->audioStreams[i], 0, *destSet->audioStreams[i], 0);
	}
}

VirtualPatchCable::~VirtualPatchCable() {
	Serial.print("Removed virtual patch cable: ");
	Serial.print(sourceSet->testInt);
	Serial.print("--->");
	Serial.println(destSet->testInt);
	// important to handle disconnections/deletions here to prevent memory leaks
	for(int i=0; i<MAX_POLYPHONY; i++) {
		audioConnections[i]->disconnect();
	}
}
