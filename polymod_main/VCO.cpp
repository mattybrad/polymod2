#include "Arduino.h"
#include "VCO.h"

VCO::VCO() {
	Serial.println("new VCO module created");
	sockets[0] = new VirtualSocket(OUTPUT);
	_oscSquare.testInt = 8;
	sockets[0]->audioStreamSet.testInt = 5;
	_oscSquare.audioStreams[0] = &_oscSquareX[0];
	_oscSquare.audioStreams[1] = &_oscSquareX[1];
	_oscSquare.audioStreams[2] = &_oscSquareX[2];
	_oscSquare.audioStreams[3] = &_oscSquareX[3];
	_oscSquareX[0].begin(0.5,150,WAVEFORM_SQUARE);
	_oscSquareX[1].begin(0.5,170,WAVEFORM_SQUARE);
	_oscSquareX[2].begin(0.5,190,WAVEFORM_SQUARE);
	_oscSquareX[3].begin(0.5,210,WAVEFORM_SQUARE);
	_patchCable = new VirtualPatchCable(_oscSquare, sockets[0]->audioStreamSet);
}

VCO::~VCO() {
	// destroy
}

void VCO::update() {

}
