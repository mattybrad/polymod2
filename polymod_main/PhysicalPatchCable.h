#ifndef PhysicalPatchCable_h
#define PhysicalPatchCable_h
#include "Arduino.h"

class PhysicalPatchCable {
  public:
    PhysicalPatchCable(int initSocketA, int initSocketB);
    ~PhysicalPatchCable();
    int socketA;
    int socketB;
};

#endif
