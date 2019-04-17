#ifndef TestOscillator_h
#define TestOscillator_h
#include "Arduino.h"
#include "VirtualModule.h"
#include <Audio.h>

class TestOscillator : public VirtualModule {
  public:
    TestOscillator();
  private:
    AudioSynthWaveformModulated _square;
};

#endif
