#ifndef VirtualModule_h
#define VirtualModule_h
#include "Arduino.h"
#include <Audio.h>

class VirtualModule {
  public:
    VirtualModule();
    virtual void update() {};
    virtual void destroy() {};
  private:


};

#endif
