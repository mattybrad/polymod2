#ifndef AnalogInput_h
#define AnalogInput_h
#include "Arduino.h"
#include "AudioStreamSet.h"
#include <Audio.h>

class AnalogInput {
  public:
    AnalogInput();
    void setValue(int tempVal);
    AudioSynthWaveformDc dc;
    AudioStreamSet audioStreamSet;
  private:

};

#endif
