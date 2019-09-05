#include "Arduino.h"
#include "VCO.h"

VCO::VCO() {
	Serial.println("New virtual VCO module created");
	sockets[0] = new VirtualSocket(OUTPUT); // saw
	sockets[1] = new VirtualSocket(OUTPUT); // square
	sockets[2] = new VirtualSocket(OUTPUT); // triangle
	sockets[3] = new VirtualSocket(OUTPUT); // sine
	sockets[4] = new VirtualSocket(INPUT); // freq mod 1
	for(int i=0; i<MAX_POLYPHONY; i++) {
		_oscSawSet.audioStreams[i] = &_oscSaw[i];
		_oscSquareSet.audioStreams[i] = &_oscSquare[i];
		_oscTriangleSet.audioStreams[i] = &_oscTriangle[i];
		_oscSineSet.audioStreams[i] = &_oscSine[i];
		_oscSaw[i].begin(0.2,150+50*i,WAVEFORM_SAWTOOTH);
		_oscSquare[i].begin(0.2,150+50*i,WAVEFORM_SQUARE);
		_oscTriangle[i].begin(0.2,150+50*i,WAVEFORM_TRIANGLE);
		_oscSine[i].begin(0.2,150+50*i,WAVEFORM_SINE);
	}
	_patchCable1 = new VirtualPatchCable(_oscSawSet, 0, sockets[0]->audioStreamSet, 0);
	_patchCable2 = new VirtualPatchCable(_oscSquareSet, 0, sockets[1]->audioStreamSet, 0);
	_patchCable3 = new VirtualPatchCable(_oscTriangleSet, 0, sockets[2]->audioStreamSet, 0);
	_patchCable4 = new VirtualPatchCable(_oscSineSet, 0, sockets[3]->audioStreamSet, 0);
	_patchCableMod1 = new VirtualPatchCable(sockets[4]->audioStreamSet, 0, _oscSawSet, 0);
	_patchCableMod2 = new VirtualPatchCable(sockets[4]->audioStreamSet, 0, _oscSquareSet, 0);
	_patchCableMod3 = new VirtualPatchCable(sockets[4]->audioStreamSet, 0, _oscTriangleSet, 0);
	_patchCableMod4 = new VirtualPatchCable(sockets[4]->audioStreamSet, 0, _oscSineSet, 0);
}

VCO::~VCO() {
	// destroy
}

void VCO::update() {

}
