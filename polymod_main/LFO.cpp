#include "Arduino.h"
#include "LFO.h"

LFO::LFO() {
	Serial.println("New virtual LFO module created");
	sockets[0] = new VirtualSocket(OUTPUT);
	for(int i=0; i<MAX_POLYPHONY; i++) {
		_oscSineSet.audioStreams[i] = &_oscSine[i];
		_oscSine[i].begin(0.5,1,WAVEFORM_SINE);
	}
	_patchCable1 = new VirtualPatchCable(_oscSineSet, 0, sockets[0]->audioStreamSet, 0);
}

LFO::~LFO() {
	// destroy
}

void LFO::update() {

}
