#include "Arduino.h"
#include "TestOscillator.h"

TestOscillator::TestOscillator() {
  _square.begin(0.5,220,WAVEFORM_SAWTOOTH);
  sockets[0].setOutput(_square, 0);
}

void TestOscillator::update() {
  
}

