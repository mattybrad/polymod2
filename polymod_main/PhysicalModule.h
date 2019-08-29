#ifndef PhysicalModule_h
#define PhysicalModule_h
#include "Arduino.h"

class PhysicalModule {
  public:
    PhysicalModule();
    void updateID(int newID);
  private:
    byte _id = 0;
        
};

#endif
