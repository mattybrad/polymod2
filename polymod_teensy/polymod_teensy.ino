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
// AUTO GENERATED INCLUDE STATEMENTS GOES HERE

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
  // initialise pins
}

int a,b,c,d; // loop index variables
void loop() {
  /*  The following four-level nested for-loop contains the main read/write operations of the synth.
   *  The four levels of the loop are a, b, c, and d.
   *  Level 'd' is run the most often, followed by 'c', 'b', and then 'a'.
   *  Detecting socket connections requires all four levels because there are so many possible combinations.
   *  Other operations, such as reading analog values or sending LED data, only require two levels.
   *  Latency-critical operations should be done on the inner loops, to run frequently.
   *  Non-critical operations should run on the outer loops, to preserve resources.
   */

  for(a=0;a<8;a++) {
    // set multiplexer to route connection test voltage to module A
    // set multiplexer to route ID number data from module A

    // update module list
    
    for(b=0;b<8;b++) {
      // set multiplexeter to route connection test voltage to module A, socket B
      // set multiplexer to route ID number data from module A, switch B

      // read ID number data (module A, bit B)
      
      for(c=0;c<8;c++) {
        // set multiplexer to route connection readings from module C
        // set multiplexer to route analog readings from module C
        // set multiplexer to route shift register latch to module C (for shift out, not shift in)
        // set multiplexer to route auxiliary data from module C (either from shift register or multiplexer)

        // if applicable, send binary data to module C shift register (for LEDs etc)

        // if applicable, read binary data from module C shift register via shift register (switches, buttons, etc)
        
        for(d=0;d<8;d++) {
          // set multiplexer to route connection reading from module C, socket D
          // set multiplexer(s) to route analog reading from module C, channel D, including auxiliary multiplexer if used

          // read whether module A, socket B, is connected to module C, socket D

          // read analog data from module C, channel D

          // if applicable, read analog data from auxiliary multiplexer on module C, channel D

          // read serial data from MIDI port
          
        }
      }
    }
  }
}
