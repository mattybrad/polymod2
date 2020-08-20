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
    AudioConnection *_pc1;
    AudioConnection *_pc2;
    AudioConnection *_pc3;
    AudioConnection *_pc4;
    AudioConnection *_finalConnectionL;
    AudioConnection *_finalConnectionR;
};

#endif
