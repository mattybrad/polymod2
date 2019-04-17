#ifndef PhysicalPatchCable_h
#define PhysicalPatchCable_h
#include "Arduino.h"

class PhysicalPatchCable {
  public:
    PhysicalPatchCable();
    boolean inUse = false;
    int socket1;
    int socket2;
  private:
    
};

#endif
