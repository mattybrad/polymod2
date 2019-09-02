#include "Arduino.h"
#include "VirtualPatchCable.h"

VirtualPatchCable::VirtualPatchCable(AudioStreamSet& initSourceSet, AudioStreamSet& initDestSet) {
	sourceSet = &initSourceSet;
	destSet = &initDestSet;
	Serial.println("CREATED PATCH CABLE");
	Serial.println(sourceSet->testInt);
	Serial.println(destSet->testInt);
	audioConnections[0] = new AudioConnection(*sourceSet->audioStreams[0], 0, *destSet->audioStreams[0], 0);
}
