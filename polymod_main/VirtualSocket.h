#ifndef VirtualSocket_h
#define VirtualSocket_h
#include "Arduino.h"
#include "AudioStreamSet.h"

class VirtualSocket {
  public:
    VirtualSocket(int initType);
    int type;
    AudioStreamSet audioStreamSet;
  private:
    AudioAmplifier _amplifiers[4];
};

#endif
