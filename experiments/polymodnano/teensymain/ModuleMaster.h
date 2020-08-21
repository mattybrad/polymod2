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
    AudioMixer4 _mixer1;
    AudioMixer4 _mixer2;
    AudioMixer4 _mixer3;
    AudioOutputI2S _out;
    AudioConnection *_pc[MAX_POLYPHONY];
    AudioConnection *_mixer1Cable;
    AudioConnection *_mixer2Cable;
    AudioConnection *_finalConnectionL;
    AudioConnection *_finalConnectionR;
};

#endif
