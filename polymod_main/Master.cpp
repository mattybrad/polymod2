#include "Arduino.h"
#include "Master.h"

Master::Master() {
	Serial.println("New master module created");
	sockets[0] = new VirtualSocket(INPUT);
}

Master::~Master() {
	// destroy
}

void Master::update() {

}
