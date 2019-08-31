#ifndef PhysicalSocket_h
#define PhysicalSocket_h
#include "Arduino.h"

// Represents a real, physical socket, which can be an input or an output

class PhysicalSocket {
  public:
    PhysicalSocket();
    bool active = false;
    byte type = INPUT; // use "INPUT" or "OUTPUT" since already defined as constants
  private:

};

#endif
