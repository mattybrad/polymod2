#include "Arduino.h"
#include "VirtualModule.h"

VirtualModule::VirtualModule() {
	Serial.println("INITIALISE SOCKETS");
	for(byte i=0;i<8;i++) {
		sockets[i] = NULL;
	}
}
