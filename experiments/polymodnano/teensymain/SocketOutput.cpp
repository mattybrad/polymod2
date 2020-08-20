#include "Arduino.h"
#include "SocketOutput.h"

SocketOutput::SocketOutput() {
  for(int i=0; i<MAX_POLYPHONY; i++) {
    audioStreamSet.audioStreams[i] = &_gain[i];
    _gain[i].gain(1.0);
  }
}
