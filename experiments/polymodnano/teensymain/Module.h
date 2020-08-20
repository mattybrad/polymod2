#ifndef Module_h
#define Module_h
#include "Arduino.h"
#include "AnalogInput.h"
#include "SocketInput.h"
#include "SocketOutput.h"
#include "AudioStreamSet.h"
#include "VirtualPatchCable.h"

class Module {
  public:
    Module();
    AnalogInput *analogInputs[8];
    SocketInput *socketInputs[8];
    SocketOutput *socketOutputs[8];
  private:
  

};

#endif
