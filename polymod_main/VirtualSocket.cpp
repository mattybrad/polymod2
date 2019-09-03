#include "Arduino.h"
#include "VirtualSocket.h"

VirtualSocket::VirtualSocket(int initType) {
	type = initType;
	audioStreamSet.audioStreams[0] = &_amplifiers[0];
	audioStreamSet.audioStreams[1] = &_amplifiers[1];
	audioStreamSet.audioStreams[2] = &_amplifiers[2];
	audioStreamSet.audioStreams[3] = &_amplifiers[3];
	_amplifiers[0].gain(1);
	_amplifiers[1].gain(1);
	_amplifiers[2].gain(1);
	_amplifiers[3].gain(1);
}
