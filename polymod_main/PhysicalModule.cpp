#include "Arduino.h"
#include "PhysicalModule.h"
#include "VCO.h"

PhysicalModule::PhysicalModule() {

}

void PhysicalModule::updateID(int newID) {
  byte prevID = _id;
  _id = newID;
  if(prevID != newID) {
    // module has changed

    if(prevID == 0) {
      // no previous module in slot, no need to remove any virtual modules
    } else {
      // module has been removed from slot, remove virtual module
      virtualModule->destroy();
      delete virtualModule;
      virtualModule = NULL;
    }

    if(newID == 0) {
      // no module in slot, nothing to add
    } else {
      // new module added - create appropriate virtual module
      switch(newID) {
        case 136:
        virtualModule = new VCO();
        Serial.println("HELLO");
        break;
      }
    }
  }
}
