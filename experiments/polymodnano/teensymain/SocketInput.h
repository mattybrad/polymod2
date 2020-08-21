#ifndef SocketInput_h
#define SocketInput_h
#include "Arduino.h"
#include <AudioStream.h>
#include "AudioStreamSet.h"

class SocketInput {
  public:
    SocketInput();
    AudioStreamSet audioStreamSet;
    int patchCableNum = -1;
  private:
    AudioAmplifier _gain[MAX_POLYPHONY];
};

#endif
