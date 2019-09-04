#include "Arduino.h"
#include "PhysicalPatchCable.h"

PhysicalPatchCable::PhysicalPatchCable(int initSocketA, int initSocketB) {
	physicalSocketA = initSocketA;
	physicalSocketB = initSocketB;
	Serial.print("Added physical patch cable: ");
	Serial.print(physicalSocketA);
	Serial.print("<--->");
	Serial.println(physicalSocketB);
}

PhysicalPatchCable::~PhysicalPatchCable() {
	Serial.print("Removed physical patch cable: ");
	Serial.print(physicalSocketA);
	Serial.print("<--->");
	Serial.println(physicalSocketB);
	delete virtualPatchCable;
}

void PhysicalPatchCable::update(VirtualSocket *updateVirtualSocketA, VirtualSocket *updateVirtualSocketB) {
	if(updateVirtualSocketA == NULL || updateVirtualSocketB == NULL) {
		// at least one of the sockets doesn't exist - invalid patch cable
		isValid = false;
	} else if(updateVirtualSocketA->type == OUTPUT && updateVirtualSocketB->type == INPUT) {
		//virtualSocketSrc = updateVirtualSocketA;
		//virtualSocketDest = updateVirtualSocketB;
		isValid = true;
		// temp, not the proper way of doing this:
		if(virtualPatchCable == NULL) {
			virtualPatchCable = new VirtualPatchCable(updateVirtualSocketA->audioStreamSet,updateVirtualSocketB->audioStreamSet);
		}
	} else if(updateVirtualSocketB->type == OUTPUT && updateVirtualSocketA->type == INPUT) {
		//virtualSocketSrc = updateVirtualSocketB;
		//virtualSocketDest = updateVirtualSocketA;
		isValid = true;
		// temp, not the proper way of doing this:
		if(virtualPatchCable == NULL) {
			virtualPatchCable = new VirtualPatchCable(updateVirtualSocketB->audioStreamSet,updateVirtualSocketA->audioStreamSet);
		}
	} else {
		// cable is linking two sockets of the same type (input to input or output to output)
		isValid = false;
		Serial.println("Bad connection");
	}
}
