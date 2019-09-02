#ifndef VirtualModule_h
#define VirtualModule_h
#include "Arduino.h"
#include "AudioStreamSet.h"
#include "VirtualSocket.h"
#include "Constants.h"

class VirtualModule {
  public:
    VirtualModule();
    virtual void update() {};
    virtual void destroy() {};
    VirtualSocket *sockets[8];
  private:


};

#endif
