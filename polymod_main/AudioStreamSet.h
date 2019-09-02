#ifndef AudioStreamSet_h
#define AudioStreamSet_h
#include "Arduino.h"
#include <Audio.h>

class AudioStreamSet {
  public:
    AudioStreamSet();
    AudioStream* audioStreams[4];
  private:


};

#endif
