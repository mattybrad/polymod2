#include "Arduino.h"
#include "ModuleMaster.h"

ModuleMaster::ModuleMaster() {
  Serial.println("New master module created");
  socketInputs[0] = new SocketInput(); // main input

  // currently supporting up to 8 channels
  for(int i=0; i<MAX_POLYPHONY; i++) {
    if(i<4) {
       _mixer1.gain(i,1);
      _pc[i] = new AudioConnection(*socketInputs[0]->audioStreamSet.audioStreams[i], 0, _mixer1, i);
    } else {
      _mixer2.gain(i%4,1);
      _pc[i] = new AudioConnection(*socketInputs[0]->audioStreamSet.audioStreams[i], 0, _mixer2, i%4);
    }
  }
  _mixer1Cable = new AudioConnection(_mixer1, 0, _mixer3, 0);
  _mixer2Cable = new AudioConnection(_mixer2, 0, _mixer3, 1);
  _finalConnectionL = new AudioConnection(_mixer3, 0, _out, 0);
  _finalConnectionR = new AudioConnection(_mixer3, 0, _out, 1);
}

ModuleMaster::~ModuleMaster() {
  // destroy
}

void ModuleMaster::update() {

}
