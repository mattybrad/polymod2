#ifndef Master_h
#define Master_h
#include "Arduino.h"
#include "VirtualModule.h"
#include <Audio.h>

class Master : public VirtualModule {
  public:
    Master();
  private:
    AudioAmplifier _main;
};

#endif
