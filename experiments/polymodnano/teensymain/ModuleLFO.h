#ifndef ModuleLFO_h
#define ModuleLFO_h
#include "Arduino.h"
#include "Module.h"
#include "Constants.h"

class ModuleLFO : public Module {
  public:
    ModuleLFO();
    ~ModuleLFO();
    virtual void update();
  private:
    AudioSynthWaveformModulated _oscSaw[MAX_POLYPHONY];
    AudioMixer4 _modMixer[MAX_POLYPHONY];
    AudioStreamSet _oscSawSet;
    AudioStreamSet _modMixerSet;
    VirtualPatchCable *_patchCable1;
    VirtualPatchCable *_patchCable2;
    VirtualPatchCable *_analogCable;
    VirtualPatchCable *_modCable;
};

#endif
