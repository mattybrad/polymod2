#ifndef PhysicalModule_h
#define PhysicalModule_h
#include "Arduino.h"
#include "VirtualModule.h"

// Represents a real, physical module (or an absence of one)
// The "updateID" function refers to a module ID, not the DIP switch ID number set by the user

class PhysicalModule {
  public:
    PhysicalModule();
    void updateID(int newID);
  private:
    byte _id = 0;
    VirtualModule *virtualModules[4]; // TODO: link to single MAX_POLYPHONY constant
        
};

#endif
