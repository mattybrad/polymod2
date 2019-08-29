#ifndef Master_h
#define Master_h
#include "Arduino.h"
#include "VirtualModule.h"
#include <Audio.h>

class Master : public VirtualModule {
  public:
    Master();
    AudioAmplifier& getMainOutput();
    virtual void update();
  private:
    AudioAmplifier _main;
};

#endif
