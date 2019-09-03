#ifndef PhysicalModule_h
#define PhysicalModule_h
#include "Arduino.h"
#include "PhysicalSocket.h"
#include "PhysicalControl.h"
#include "VirtualModule.h"

// Represents a real, physical module

class PhysicalModule {
  public:
    PhysicalModule(int initID);
    ~PhysicalModule();
    PhysicalSocket physicalSockets[8];
    PhysicalControl physicalControls[8];
    void updateID(int newID);
    VirtualModule *virtualModule;
    int id = 0;
  private:

};

#endif
