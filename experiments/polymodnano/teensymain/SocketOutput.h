#ifndef SocketOutput_h
#define SocketOutput_h
#include "Arduino.h"
#include <AudioStream.h>
#include "AudioStreamSet.h"

class SocketOutput {
  public:
    SocketOutput();
    AudioStreamSet audioStreamSet;
    int patchCableNum = -1;
  private:
    AudioAmplifier _gain[MAX_POLYPHONY];
};

#endif
