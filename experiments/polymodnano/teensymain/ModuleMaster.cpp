#include "Arduino.h"
#include "ModuleMaster.h"

ModuleMaster::ModuleMaster() {
  Serial.println("New master module created");
  _socketInputs[0] = new SocketInput(); // main input
  
  for(int i=0; i<MAX_POLYPHONY; i++) {
    _mixer.gain(i,1);
  }
  pc1 = new AudioConnection(*_socketInputs[0]->audioStreamSet.audioStreams[0], 0, _mixer, 0);
  pc2 = new AudioConnection(*_socketInputs[0]->audioStreamSet.audioStreams[1], 0, _mixer, 1);
  //pc3 = new AudioConnection(*_socketInputs[0]->audioStreamSet.audioStreams[2], 0, _mixer, 2);
  //pc4 = new AudioConnection(*_socketInputs[0]->audioStreamSet.audioStreams[3], 0, _mixer, 3);
  finalConnectionL = new AudioConnection(_mixer, 0, _out, 0);
  finalConnectionR = new AudioConnection(_mixer, 0, _out, 1);
}

ModuleMaster::~ModuleMaster() {
  // destroy
}

void ModuleMaster::update() {

}
