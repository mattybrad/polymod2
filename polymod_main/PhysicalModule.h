#ifndef PhysicalModule_h
#define PhysicalModule_h
#include "Arduino.h"
#include "PhysicalSocket.h"
#include "PhysicalControl.h"
#include "VirtualModule.h"

// Represents a real, physical module (or an absence of one)
// The "updateID" function refers to a module ID, not the DIP switch ID number set by the user

class PhysicalModule {
  public:
    PhysicalModule();
    PhysicalSocket physicalSockets[8];
    PhysicalControl physicalControls[8];
    void updateID(int newID);
  private:
    byte _id = 0;
    VirtualModule *virtualModule;

};

#endif
