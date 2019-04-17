#include "Arduino.h"
#include "VirtualSocket.h"

#define OUTPUT_SOCKET 0
#define INPUT_SOCKET 1

VirtualSocket::VirtualSocket() {
  _stream = NULL;
}

void VirtualSocket::setOutput(AudioStream &stream, int connectionIndex) {
  _socketType = OUTPUT_SOCKET;
  _stream = &stream;
  _connectionIndex = connectionIndex;
}

void VirtualSocket::setInput(AudioStream& stream, int connectionIndex) {
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

