#include "Arduino.h"
#include "VCO.h"

VCO::VCO() {
	Serial.println("new VCO module created");
	sockets[0] = new VirtualSocket(OUTPUT);
	sockets[1] = new VirtualSocket(OUTPUT);
	_oscSquare.testInt = 8;
	sockets[0]->audioStreamSet.testInt = 5;
	_oscSquare.audioStreams[0] = &_oscSquareX[0];
	_oscSquare.audioStreams[1] = &_oscSquareX[1];
	_oscSquare.audioStreams[2] = &_oscSquareX[2];
	_oscSquare.audioStreams[3] = &_oscSquareX[3];
	_oscSine.audioStreams[0] = &_oscSineX[0];
	_oscSine.audioStreams[1] = &_oscSineX[1];
	_oscSine.audioStreams[2] = &_oscSineX[2];
	_oscSine.audioStreams[3] = &_oscSineX[3];
	_oscSquareX[0].begin(0.5,50,WAVEFORM_SQUARE);
	_oscSquareX[1].begin(0.5,75,WAVEFORM_SQUARE);
	_oscSquareX[2].begin(0.5,100,WAVEFORM_SQUARE);
	_oscSquareX[3].begin(0.5,150,WAVEFORM_SQUARE);
	_oscSineX[0].begin(0.5,200,WAVEFORM_SINE);
	_oscSineX[1].begin(0.5,251,WAVEFORM_SINE);
	_oscSineX[2].begin(0.5,300,WAVEFORM_SINE);
	_oscSineX[3].begin(0.5,400,WAVEFORM_SINE);
	_patchCable1 = new VirtualPatchCable(_oscSquare, sockets[0]->audioStreamSet);
	_patchCable2 = new VirtualPatchCable(_oscSine, sockets[1]->audioStreamSet);
}

VCO::~VCO() {
	// destroy
}

void VCO::update() {

}
