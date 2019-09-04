#ifndef VirtualSocket_h
#define VirtualSocket_h
#include "Arduino.h"
#include "AudioStreamSet.h"
#include "Constants.h"

class VirtualSocket {
  public:
    VirtualSocket(int initType);
    int type;
    AudioStreamSet audioStreamSet;
  private:
    AudioAmplifier _amplifiers[MAX_POLYPHONY];
};

#endif
