#include "Arduino.h"
#include "AnalogInput.h"

AnalogInput::AnalogInput() {
  dc.amplitude(0);
  for(int i=0; i<MAX_POLYPHONY; i++) {
    audioStreamSet.audioStreams[i] = &dc;
  }
}

void AnalogInput::setValue(int tempVal) {
  dc.amplitude(tempVal / 1023.0);
}
