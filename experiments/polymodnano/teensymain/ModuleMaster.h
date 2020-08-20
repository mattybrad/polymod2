#ifndef ModuleMaster_h
#define ModuleMaster_h
#include "Arduino.h"
#include "Module.h"
#include "Constants.h"

class ModuleMaster : public Module {
  public:
    ModuleMaster();
    ~ModuleMaster();
    virtual void update();
  private:
    AudioMixer4 _mixer;
    AudioOutputI2S _out;
    AudioConnection *pc1;
    AudioConnection *pc2;
    AudioConnection *pc3;
    AudioConnection *pc4;
    AudioConnection *finalConnectionL;
    AudioConnection *finalConnectionR;
};

#endif
