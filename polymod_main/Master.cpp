#include "Arduino.h"
#include "Master.h"

Master::Master() {
	Serial.println("new master module created");
	sockets[0] = new VirtualSocket(INPUT);
	sockets[0]->audioStreamSet.testInt = 11;
}

Master::~Master() {
	// destroy
}

void Master::update() {

}
