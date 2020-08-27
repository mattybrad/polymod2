#ifndef ModuleMIDI_h
#define ModuleMIDI_h
#include "Arduino.h"
#include "Module.h"
#include "Constants.h"

class ModuleMIDI : public Module {
  public:
    ModuleMIDI();
    ~ModuleMIDI();
    virtual void update();
  private:
    AudioSynthWaveformDc _note[MAX_POLYPHONY];
    AudioSynthWaveformDc _gate[MAX_POLYPHONY];
    AudioStreamSet _noteSet;
    AudioStreamSet _gateSet;
    VirtualPatchCable *_patchCable1;
    VirtualPatchCable *_patchCable2;
};

#endif
