#ifndef VirtualModule_h
#define VirtualModule_h
#include "Arduino.h"
#include "AudioStreamSet.h"
#include "VirtualSocket.h"
#include "VirtualPatchCable.h"

class VirtualModule {
  public:
    VirtualModule();
    virtual ~VirtualModule();
    VirtualSocket getVirtualSocket(int socketNum);
    virtual void update() {};
    VirtualSocket *sockets[8];
  private:


};

#endif
