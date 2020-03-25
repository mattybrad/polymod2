#ifndef VirtualPatchCable_h
#define VirtualPatchCable_h
#include "Arduino.h"
#include "VirtualSocket.h"

class VirtualPatchCable {
  public:
    VirtualPatchCable();
    void initialise(VirtualSocket& socket1, VirtualSocket& socket2);
  private:
    
};

#endif
