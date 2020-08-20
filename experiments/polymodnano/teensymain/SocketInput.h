#ifndef SocketInput_h
#define SocketInput_h
#include "Arduino.h"
#include <AudioStream.h>
#include "AudioStreamSet.h"

class SocketInput {
  public:
    SocketInput();
    AudioStreamSet audioStreamSet;
  private:
    AudioAmplifier _gain[MAX_POLYPHONY];
};

#endif
