#ifndef PhysicalModule_h
#define PhysicalModule_h
#include "VirtualModule.h"
#include "Arduino.h"
#include "PhysicalSocket.h"
#include "PhysicalControl.h"
#include "VirtualModule.h"

// Represents a real, physical module

class PhysicalModule {
  public:
    PhysicalModule();
    byte id = 0;
    VirtualModule *virtualModule;
  private:

};

#endif
