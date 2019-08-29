#include "Arduino.h"
#include "PhysicalModule.h"

PhysicalModule::PhysicalModule() {
   
}

void PhysicalModule::updateID(int newID) {
  byte prevID = _id;
  _id = newID;
  if(prevID == 0) {
    // no previous module in slot, no need to remove any virtual modules
  } else {
    // module has been removed from slot, remove any virtual modules etc
    
  }

  if(newID == 0) {
    // no module in slot, nothing to add
  } else {
    
  }
}
