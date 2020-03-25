#ifndef VirtualModule_h
#define VirtualModule_h
#include "Arduino.h"
#include "VirtualSocket.h"
#include "Control.h"
#include <Audio.h>

class VirtualModule {
  public:
    VirtualModule();
    VirtualSocket sockets[8];
    VirtualSocket& getSocket(int moduleSocketNumber);
  private:
    int _id;
    
};

#endif
