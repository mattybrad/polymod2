#include "Arduino.h"
#include "VirtualPatchCable.h"

<<<<<<< HEAD
VirtualPatchCable::VirtualPatchCable() {

}

void VirtualPatchCable::initialise(VirtualSocket& socket1, VirtualSocket& socket2) {
  if(socket1.isSet) {
    Serial.println("SOCKET 1 IS SET");
  } else {
    Serial.println("SOCKET 1 NOT SET");
  }
  if(socket1.isSet) {
    Serial.println("SOCKET 2 IS SET");
  } else {
    Serial.println("SOCKET 2 NOT SET");
  }
  if(socket1.isSet && socket2.isSet) {
    Serial.println("BOTH SOCKETS SET!");
  }
}

=======
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
>>>>>>> 47bf06dffeea8e06460e3808825af276b3af3bc4
