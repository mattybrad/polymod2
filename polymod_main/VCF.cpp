#include "Arduino.h"
#include "VCF.h"

VCF::VCF() {
	Serial.println("New virtual VCF module created");
	sockets[0] = new VirtualSocket(INPUT); // signal in
	sockets[1] = new VirtualSocket(INPUT); // freq mod in
	sockets[2] = new VirtualSocket(OUTPUT); // lowpass out
	sockets[3] = new VirtualSocket(OUTPUT); // bandpass out
	sockets[4] = new VirtualSocket(OUTPUT); // highpass out
	sockets[0]->audioStreamSet.ref = 'F';
	sockets[1]->audioStreamSet.ref = 'G';
	sockets[2]->audioStreamSet.ref = 'B';
	sockets[3]->audioStreamSet.ref = 'C';
	sockets[4]->audioStreamSet.ref = 'D';
	_filterSet.ref = 'E';
	for(int i=0; i<MAX_POLYPHONY; i++) {
		_filterSet.audioStreams[i] = &_filter[i];
		_filter[i].frequency(100);
		_filter[i].resonance(4.0);
		_filter[i].octaveControl(2.5);
	}
	_patchCable1 = new VirtualPatchCable(sockets[0]->audioStreamSet, 0, _filterSet, 0);
	_patchCable2 = new VirtualPatchCable(sockets[1]->audioStreamSet, 0, _filterSet, 1);
	_patchCable3 = new VirtualPatchCable(_filterSet, 0, sockets[2]->audioStreamSet, 0);
	_patchCable4 = new VirtualPatchCable(_filterSet, 1, sockets[3]->audioStreamSet, 0);
	_patchCable5 = new VirtualPatchCable(_filterSet, 2, sockets[4]->audioStreamSet, 0);
}

VCF::~VCF() {
	// destroy
}

void VCF::update() {

}
