#ifndef PhysicalModule_h
#define PhysicalModule_h
#include "VirtualModule.h"
#include "Arduino.h"

class PhysicalModule {
  public:
    PhysicalModule();
    byte id = 0;
    VirtualModule *virtualModule;
    
  private:
    
};

#endif
