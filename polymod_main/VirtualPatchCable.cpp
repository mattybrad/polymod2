#include "Arduino.h"
#include "VirtualPatchCable.h"

VirtualPatchCable::VirtualPatchCable() {

}

void VirtualPatchCable::initialise(VirtualSocket& socket1, VirtualSocket& socket2) {
  if(socket1.isSet) {
    Serial.println("SOCKET 1 IS SET");
  } else {
    Serial.println("SOCKET 1 NOT SET");
  }
  if(socket1.isSet) {
    Serial.println("SOCKET 2 IS SET");
  } else {
    Serial.println("SOCKET 2 NOT SET");
  }
  if(socket1.isSet && socket2.isSet) {
    Serial.println("BOTH SOCKETS SET!");
  }
}

