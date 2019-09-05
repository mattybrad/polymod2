#ifndef VirtualPatchCable_h
#define VirtualPatchCable_h
#include "Arduino.h"
#include "AudioStreamSet.h"
#include "VirtualSocket.h"
#include "Constants.h"
#include <Audio.h>

class VirtualPatchCable {
  public:
    VirtualPatchCable(AudioStreamSet initSourceSet, int sourceSocketNum, AudioStreamSet initDestSet, int destSocketNum);
    ~VirtualPatchCable();
    AudioStreamSet *sourceSet;
    AudioStreamSet *destSet;
    AudioConnection *audioConnections[MAX_POLYPHONY];
};

#endif
