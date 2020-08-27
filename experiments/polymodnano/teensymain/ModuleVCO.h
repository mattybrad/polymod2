#ifndef ModuleVCO_h
#define ModuleVCO_h
#include "Arduino.h"
#include "Module.h"
#include "Constants.h"

class ModuleVCO : public Module {
  public:
    ModuleVCO();
    ~ModuleVCO();
    virtual void update();
  private:
    AudioSynthWaveformModulated _oscSaw[MAX_POLYPHONY];
    AudioMixer4 _modMixer[MAX_POLYPHONY];
    AudioStreamSet _oscSawSet;
    AudioStreamSet _modMixerSet;
    VirtualPatchCable *_patchCable1;
    VirtualPatchCable *_patchCable2;
    VirtualPatchCable *_patchCable3;
    VirtualPatchCable *_analogCable;
    VirtualPatchCable *_modCable;
};

#endif
