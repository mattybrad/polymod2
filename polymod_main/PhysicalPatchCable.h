#ifndef PhysicalPatchCable_h
#define PhysicalPatchCable_h
#include "Arduino.h"
<<<<<<< HEAD

class PhysicalPatchCable {
  public:
    PhysicalPatchCable();
    boolean inUse = false;
    int socket1;
    int socket2;
  private:
    
=======
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
>>>>>>> 47bf06dffeea8e06460e3808825af276b3af3bc4
};

#endif
