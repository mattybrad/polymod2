#include "Arduino.h"
#include "Master.h"

Master::Master() {
	Serial.println("New master module created");
	sockets[0] = new VirtualSocket(INPUT);
	_testOsc.begin(0.5,50,WAVEFORM_SAWTOOTH);
	for(int i=0; i<MAX_POLYPHONY; i++) {
		if(i<4) {
			Serial.println("Attempting to make master connection");
			_mainConnections[i] = new AudioConnection(*sockets[0]->audioStreamSet.audioStreams[i], 0, _mixer1, i);
			_mixer1.gain(i,1);
		} else if(i<8) {
			_mainConnections[i] = new AudioConnection(*sockets[0]->audioStreamSet.audioStreams[i], 0, _mixer2, i%4);
			_mixer2.gain(i%4,1);
		}
	}
	_mixer1Connection = new AudioConnection(_mixer1, 0, _finalMixer, 0);
	_mixer2Connection = new AudioConnection(_mixer2, 0, _finalMixer, 1);
	_finalConnection1 = new AudioConnection(_finalMixer, 0, _mainOutput, 0);
	_finalConnection2 = new AudioConnection(_finalMixer, 0, _mainOutput, 1);
	_finalMixer.gain(0,1);
	_finalMixer.gain(1,1);
	//_finalConnection1 = new AudioConnection(_testOsc, 0, _mainOutput, 0);
}

Master::~Master() {
	// destroy
}

void Master::update() {

}
