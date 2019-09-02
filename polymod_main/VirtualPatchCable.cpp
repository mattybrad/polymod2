#include "Arduino.h"
#include "VirtualPatchCable.h"

VirtualPatchCable::VirtualPatchCable(AudioStreamSet& initSourceSet, AudioStreamSet& initDestSet) {
	sourceSet = &initSourceSet;
	destSet = &initDestSet;
	Serial.println("CREATED PATCH CABLE");
	Serial.println(sourceSet->testInt);
	Serial.println(destSet->testInt);
	audioConnections[0] = new AudioConnection(*sourceSet->audioStreams[0], 0, *destSet->audioStreams[0], 0);
	audioConnections[1] = new AudioConnection(*sourceSet->audioStreams[1], 0, *destSet->audioStreams[1], 0);
	audioConnections[2] = new AudioConnection(*sourceSet->audioStreams[2], 0, *destSet->audioStreams[2], 0);
	audioConnections[3] = new AudioConnection(*sourceSet->audioStreams[3], 0, *destSet->audioStreams[3], 0);
}
