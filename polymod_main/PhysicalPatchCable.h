#ifndef PhysicalPatchCable_h
#define PhysicalPatchCable_h
#include "Arduino.h"
#include "VirtualPatchCable.h"

class PhysicalPatchCable {
  public:
    PhysicalPatchCable(int initSocketA, int initSocketB);
    ~PhysicalPatchCable();
    void update(VirtualSocket *updateVirtualSocketA, VirtualSocket *updateVirtualSocketB);
    int physicalSocketA;
    int physicalSocketB;
    VirtualSocket *virtualSocketSrc = NULL;
    VirtualSocket *virtualSocketDest = NULL;
    VirtualPatchCable *virtualPatchCable = NULL;
    bool isValid = false;
};

#endif
