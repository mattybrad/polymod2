#include "Arduino.h"
#include "VirtualModule.h"

VirtualModule::VirtualModule() {
	for(byte i=0;i<8;i++) {
		sockets[i] = NULL;
	}
}

VirtualModule::~VirtualModule() {
	// destroy
}
