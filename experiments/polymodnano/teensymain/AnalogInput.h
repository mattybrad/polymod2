#ifndef AnalogInput_h
#define AnalogInput_h
#include "Arduino.h"
#include <Audio.h>

class AnalogInput {
  public:
    AnalogInput();
  private:
    AudioSynthWaveformDc _dc;

};

#endif
