#include "Arduino.h"
#include "VirtualSocket.h"

VirtualSocket::VirtualSocket(int initType) {
	type = initType;
	for(int i=0; i<MAX_POLYPHONY; i++) {
		audioStreamSet.audioStreams[i] = &_amplifiers[i];
		_amplifiers[i].gain(1);
	}
}
