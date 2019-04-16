#include "Arduino.h"
#include "VirtualModule.h"

VirtualModule::VirtualModule() {
  _id = 1;
}

int VirtualModule::getID() {
  return _id;
}

Socket& VirtualModule::getSocket(int moduleSocketNumber) {
  return sockets[moduleSocketNumber];
}

void VirtualModule::removePatchCable(int moduleSocketNumber) {
  
}

void VirtualModule::updateControlValue(int controlIndex, int controlReading) {
  controls[controlIndex].rawValue = controlReading;
  controls[controlIndex].updateSmoothedValue();
}

