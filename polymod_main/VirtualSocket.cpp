#include "Arduino.h"
#include "VirtualSocket.h"

<<<<<<< HEAD
#define OUTPUT_SOCKET 0
#define INPUT_SOCKET 1

VirtualSocket::VirtualSocket() {
  _stream = NULL;
}

void VirtualSocket::setOutput(AudioStream &stream, int connectionIndex) {
  Serial.println("SET OUTPUT");
  isSet = true;
  _socketType = OUTPUT_SOCKET;
  _stream = &stream;
  _connectionIndex = connectionIndex;
}

void VirtualSocket::setInput(AudioStream& stream, int connectionIndex) {
  Serial.println("SET INPUT");
  isSet = true;
  _socketType = INPUT_SOCKET;
  _stream = &stream;
  _connectionIndex = connectionIndex;
}

boolean VirtualSocket::isOutput() {
  return _socketType == OUTPUT_SOCKET;
}

AudioStream* VirtualSocket::getStream() {
  return _stream;
}

int VirtualSocket::getConnectionIndex() {
  return _connectionIndex;
}

=======
VirtualSocket::VirtualSocket(int initType) {
	Serial.println("New virtual socket created");
	type = initType;
	for(int i=0; i<MAX_POLYPHONY; i++) {
		audioStreamSet.audioStreams[i] = &_amplifiers[i];
		_amplifiers[i].gain(1);
	}
}
>>>>>>> 47bf06dffeea8e06460e3808825af276b3af3bc4
