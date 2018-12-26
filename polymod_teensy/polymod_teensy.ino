// required for teensy audio
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// other libraries
#include <Bounce2.h>

// include menu, base module, master module and patch cable classes
#include "Module.h"
#include "Master.h"
#include "PatchCable.h"
#include "Menu.h"

// include specific modules - this part can be generated automatically
// AUTO GENERATED INCLUDE STATEMENTS GOES HERE

// define constants
#define MAX_MODULES 8 // maybe go up to 16+ later
#define MAX_POLYPHONY 4
#define MAX_CABLES 32 // should be >= 8 * MAX_POLYPHONY * MAX_MODULES / 2 to avoid problems

// define pins
#define DEC_BUTTON_PIN 0
#define INC_BUTTON_PIN 1
#define YES_BUTTON_PIN 2
#define NO_BUTTON_PIN 3

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
Menu menu = Menu();

// buttons
Bounce incButton = Bounce();
Bounce decButton = Bounce();
Bounce yesButton = Bounce();
Bounce noButton = Bounce();

void setup() {
  Serial.begin(9600);
  incButton.attach(INC_BUTTON_PIN,INPUT_PULLUP);
  decButton.attach(DEC_BUTTON_PIN,INPUT_PULLUP);
  yesButton.attach(YES_BUTTON_PIN,INPUT_PULLUP);
  noButton.attach(NO_BUTTON_PIN,INPUT_PULLUP);
  incButton.interval(25);
  decButton.interval(25);
  yesButton.interval(25);
  noButton.interval(25);
}

int a,b,c,d,e,f; // loop index variables
void loop() {

  for(a=0;a<8;a++) {
    // if more than one module group...
    // set multiplexer to route connection test voltage to group A

    for(b=0;b<8;b++) {
      // set multiplexer to route connection test voltage to group A, module B
      
      for(c=0;c<8;c++) {
        // set multiplexeter to route connection test voltage to group A, module B, socket C
  
        for(d=0;d<8;d++) {
            // if more than one module group...
            // set multiplexer to route ID number data from group D
            // set multiplexer to route connection readings from group D
            // set multiplexer to route analog readings from group D
            // set multiplexer to route shift register latch to group D (for shift out, not shift in)
            // set multiplexer to route auxiliary data from group D (either from shift register or multiplexer)
        
          for(e=0;e<8;e++) {
            // set multiplexer to route ID number data from group D, module E
            // set multiplexer to route connection readings from group D, module E
            // set multiplexer to route analog readings from group D, module E
            // set multiplexer to route shift register latch to group D, module E (for shift out, not shift in)
            // set multiplexer to route auxiliary data from group D, module E (either from shift register or multiplexer)
    
            // if applicable, send binary data to group D, module E shift register (for LEDs etc)
    
            // if applicable, read binary data from group D, module E via shift register (switches, buttons, etc)
            
            for(f=0;f<8;f++) {
              // set multiplexer to route ID number data from group D, module E, switch F
              // set multiplexer to route connection reading from group D, module E, socket F
              // set multiplexer(s) to route analog reading from group D, module E, channel F, including auxiliary multiplexer if used

              // read ID number data (group D, module E, bit F)
    
              // read whether module A, socket B, is connected to group D, module E, socket F
    
              // read analog data from group D, module E, channel F
    
              // if applicable, read analog data from auxiliary multiplexer on group D, module E, channel F
    
              // read serial data from MIDI port
              
            }
          }
        }
      }
    }
  }

  // menu button update code, here for now but will move inside loop at some point
  incButton.update();
  decButton.update();
  yesButton.update();
  noButton.update();
  if(incButton.fell()) {
    menu.incrementValue();
  } else if(decButton.fell()) {
    menu.decrementValue();
  } else if(yesButton.fell()) {
    menu.confirm();
  } else if(noButton.fell()) {
    menu.cancel();
  }
}


