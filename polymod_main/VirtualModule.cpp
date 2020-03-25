#include "Arduino.h"
#include "VirtualModule.h"

VirtualModule::VirtualModule() {

}

VirtualSocket& VirtualModule::getSocket(int moduleSocketNumber) {
  return sockets[moduleSocketNumber];
}
