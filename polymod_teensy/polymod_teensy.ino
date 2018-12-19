// required for teensy audio
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// include base module, master module and patch cable classes
#include "Module.h"
#include "Master.h"
#include "PatchCable.h"

// include specific modules - this part can be generated automatically
// AUTO GENERATED CODE GOES HERE

// define constants
#define MAX_MODULES 8 // maybe go up to 16+ later
#define MAX_POLYPHONY 4
#define MAX_CABLES 32 // should be >= 8 * MAX_POLYPHONY * MAX_MODULES / 2 to avoid problems

// more definitions
Module *modules[MAX_MODULES][MAX_POLYPHONY]; // all currently used modules
PatchCable *patchCables[MAX_CABLES]; // all currently connected patch cables
AudioControlSGTL5000 sgtl; // teensy audio board chip
AudioOutputI2S mainOutput; // teensy audio board output
AudioMixer4 mainMixer; // mix polyphonic channels into single output (only works if polyphony <= 4)
AudioConnection con1(mainMixer,0,mainOutput,0); // connect main mixer to output (left)
AudioConnection con2(mainMixer,0,mainOutput,1); // connect main mixer to output (right)
AudioConnection* masterConnections[MAX_POLYPHONY]; // connections from polyphonic channels to output mixer
Master masterModules[MAX_POLYPHONY]; // array of master modules

void setup() {
  
}

void loop() {
  
}
