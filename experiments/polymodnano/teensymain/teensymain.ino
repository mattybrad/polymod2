// required for teensy audio
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// modules
#include "ModuleVCO.h"
#include "ModuleMaster.h"

AudioControlSGTL5000 sgtl; // teensy audio board chip

// temp module instances
ModuleVCO vco;
ModuleMaster master;

VirtualPatchCable *vpc1;

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps

  // init audio board
  AudioMemory(50);
  sgtl.enable();
  sgtl.volume(0.3);

  vpc1 = new VirtualPatchCable(vco.socketOutputs[0]->audioStreamSet,0,master.socketInputs[0]->audioStreamSet,0);
}

void loop() {
  // send data only when you receive data:
  while (Serial.available() > 0) {
    
    // read the incoming byte:
    byte incomingByte = Serial.read();
    vco.analogInputs[0]->setValue(incomingByte / 2);

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  }
  vco.analogInputs[0]->setValue(analogRead(A0));
}
