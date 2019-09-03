#ifndef VirtualPatchCable_h
#define VirtualPatchCable_h
#include "Arduino.h"
#include "AudioStreamSet.h"
#include "VirtualSocket.h"
#include <Audio.h>

class VirtualPatchCable {
  public:
    VirtualPatchCable(AudioStreamSet& initSourceSet, AudioStreamSet& initDestSet);
    ~VirtualPatchCable();
    AudioStreamSet* sourceSet;
    AudioStreamSet* destSet;
    AudioConnection *audioConnections[4];
};

#endif
