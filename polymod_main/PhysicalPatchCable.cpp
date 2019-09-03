#include "Arduino.h"
#include "PhysicalPatchCable.h"

PhysicalPatchCable::PhysicalPatchCable(int initSocketA, int initSocketB) {
	socketA = initSocketA;
	socketB = initSocketB;
	Serial.print("Added physical patch cable: ");
	Serial.print(socketA);
	Serial.print("<--->");
	Serial.println(socketB);
}

PhysicalPatchCable::~PhysicalPatchCable() {
	Serial.print("Removed physical patch cable: ");
	Serial.print(socketA);
	Serial.print("<--->");
	Serial.println(socketB);
}
