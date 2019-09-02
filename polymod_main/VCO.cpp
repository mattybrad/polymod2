#include "Arduino.h"
#include "VCO.h"

VCO::VCO() {
	_oscSquare.audioStreams[0] = new AudioSynthWaveformModulated();
	_oscSquare.audioStreams[1] = new AudioSynthWaveformModulated();
	_oscSquare.audioStreams[2] = new AudioSynthWaveformModulated();
	_oscSquare.audioStreams[3] = new AudioSynthWaveformModulated();
}

void VCO::update() {

}
