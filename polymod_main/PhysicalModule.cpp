#include "Arduino.h"
#include "PhysicalModule.h"
#include "VCO.h"
#include "LFO.h"
#include "Master.h"

PhysicalModule::PhysicalModule(int initID) {
  Serial.println("Added physical module");
  id = initID;
  switch(id) {
    case 88:
    virtualModule = new LFO();
    break;
    case 136:
    virtualModule = new VCO();
    break;
    case 255:
    virtualModule = new Master();
    break;
  }
}

PhysicalModule::~PhysicalModule() {
  Serial.println("Removed physical module");
  delete virtualModule;
}
