#ifndef VirtualPatchCable_h
#define VirtualPatchCable_h
#include "Arduino.h"
<<<<<<< HEAD
#include "VirtualSocket.h"

class VirtualPatchCable {
  public:
    VirtualPatchCable();
    void initialise(VirtualSocket& socket1, VirtualSocket& socket2);
  private:
    
=======
#include "AudioStreamSet.h"
#include "VirtualSocket.h"
#include "Constants.h"
#include <Audio.h>

class VirtualPatchCable {
  public:
    VirtualPatchCable(AudioStreamSet sourceSet, int sourceSocketNum, AudioStreamSet destSet, int destSocketNum);
    ~VirtualPatchCable();
    AudioStreamSet *sourceSet;
    AudioStreamSet *destSet;
    AudioConnection *audioConnections[MAX_POLYPHONY];
>>>>>>> 47bf06dffeea8e06460e3808825af276b3af3bc4
};

#endif
