#include "Arduino.h"
#include "Master.h"

Master::Master() {
  sockets[4].setInput(_main, 0);
}

AudioAmplifier& Master::getMainOutput() {
  return _main;
}

void Master::update() {
  _main.gain(1.0);
}
