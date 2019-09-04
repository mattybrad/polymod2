#include "Arduino.h"
#include "VCO.h"

VCO::VCO() {
	Serial.println("New virtual VCO module created");
	sockets[0] = new VirtualSocket(OUTPUT);
	sockets[1] = new VirtualSocket(OUTPUT);
	sockets[2] = new VirtualSocket(OUTPUT);
	sockets[3] = new VirtualSocket(OUTPUT);
	for(int i=0; i<MAX_POLYPHONY; i++) {
		Serial.print("GOT TO HERE: ");
		Serial.println(i);
		_oscSawSet.audioStreams[i] = &_oscSaw[i];
		_oscSquareSet.audioStreams[i] = &_oscSquare[i];
		_oscTriangleSet.audioStreams[i] = &_oscTriangle[i];
		_oscSineSet.audioStreams[i] = &_oscSine[i];
		_oscSaw[i].begin(0.2,150+50*i,WAVEFORM_SAWTOOTH);
		_oscSquare[i].begin(0.2,150+50*i,WAVEFORM_SQUARE);
		_oscTriangle[i].begin(0.2,150+50*i,WAVEFORM_TRIANGLE);
		_oscSine[i].begin(0.2,150+50*i,WAVEFORM_SINE);
	}
	_patchCable1 = new VirtualPatchCable(_oscSawSet, sockets[0]->audioStreamSet);
	_patchCable2 = new VirtualPatchCable(_oscSquareSet, sockets[1]->audioStreamSet);
	_patchCable3 = new VirtualPatchCable(_oscTriangleSet, sockets[2]->audioStreamSet);
	_patchCable4 = new VirtualPatchCable(_oscSineSet, sockets[3]->audioStreamSet);
}

VCO::~VCO() {
	// destroy
}

void VCO::update() {

}
