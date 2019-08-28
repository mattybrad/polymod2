// required for teensy audio
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// other libraries
#include <Bounce2.h>

// include menu, base module, master module and patch cable classes
#include "VirtualModule.h"
#include "PhysicalModule.h"
#include "Master.h"
#include "PatchCable.h"
#include "Menu.h"
#include "TestOscillator.h"

// include specific modules - this part can be generated automatically
// AUTO GENERATED INCLUDE STATEMENTS GOES HERE

// define constants
#define MAX_MODULES 64
#define MAX_POLYPHONY 4
#define MAX_CABLES 200

// define pins
#define DEC_BUTTON_PIN 2
#define INC_BUTTON_PIN 3
#define YES_BUTTON_PIN 4
#define NO_BUTTON_PIN 5

// more definitions
byte moduleIDReadings[MAX_MODULES];
PhysicalModule physicalModules[MAX_MODULES]; // all physical modules
VirtualModule *virtualModules[MAX_MODULES][MAX_POLYPHONY]; // all virtual modules
PatchCable *patchCables[MAX_CABLES]; // all currently connected patch cables
AudioControlSGTL5000 sgtl; // teensy audio board chip
AudioOutputI2S mainOutput; // teensy audio board output
Menu menu = Menu();

// test audio board
AudioSynthWaveformSine   sine1;
AudioConnection          patchCord1(sine1, 0, mainOutput, 0);
AudioConnection          patchCord2(sine1, 0, mainOutput, 1);

// buttons
Bounce incButton = Bounce();
Bounce decButton = Bounce();
Bounce yesButton = Bounce();
Bounce noButton = Bounce();

// serial stuff
byte nextPosition = 0;
byte currentCommand[8];
float tempFreq = 100.0;

void setup() {
  Serial1.begin(500000);
  Serial.begin(500000);
  incButton.attach(INC_BUTTON_PIN,INPUT_PULLUP);
  decButton.attach(DEC_BUTTON_PIN,INPUT_PULLUP);
  yesButton.attach(YES_BUTTON_PIN,INPUT_PULLUP);
  noButton.attach(NO_BUTTON_PIN,INPUT_PULLUP);
  incButton.interval(25);
  decButton.interval(25);
  yesButton.interval(25);
  noButton.interval(25);

  // test audio board
  AudioMemory(15);
  sgtl.enable();
  sgtl.volume(0.5);
  sine1.amplitude(0.5);

  //Physical

  Serial.println("STARTED SKETCH");
}

int a,b,c,d,e,f; // loop index variables
void loop() {
  sine1.frequency(tempFreq);
  
  while(Serial1.available()) {
    byte thisByte = Serial1.read();
    if(nextPosition==0) {
      // new command!
      currentCommand[0] = thisByte;
      if(thisByte>0) nextPosition ++; // don't increment position for command 0 because it doesn't have any other data expected
      //else Serial.println("LOOP STARTED");
    } else {
      currentCommand[nextPosition] = thisByte;
      switch(currentCommand[0]) {
        case 1:
        nextPosition++;
        if(nextPosition>6) {
          nextPosition=0;
          Serial.print("PATCH CONNECTION: ");
          Serial.print(currentCommand[1]);
          Serial.print("-");
          Serial.print(currentCommand[2]);
          Serial.print("-");
          Serial.print(currentCommand[3]);
          Serial.print(" to ");
          Serial.print(currentCommand[4]);
          Serial.print("-");
          Serial.print(currentCommand[5]);
          Serial.print("-");
          Serial.println(currentCommand[6]);
        }
        break;

        case 2:
        // analog reading
        nextPosition++;
        if(nextPosition>4) {
          nextPosition=0;
          Serial.print("ANALOG READING: ");
          Serial.print(currentCommand[1]);
          Serial.print("-");
          Serial.print(currentCommand[2]);
          Serial.print("-");
          Serial.print(currentCommand[3]);
          Serial.print(": ");
          Serial.println(currentCommand[4]);
          if(currentCommand[1]==0&&currentCommand[2]==0&&currentCommand[3]==0) tempFreq = 100.0 + 10*currentCommand[4];
        }
        break;

        case 3:
        // module ID reading
        nextPosition++;
        if(nextPosition>3) {
          nextPosition=0;
          moduleIDReadings[(currentCommand[1]<<3)+currentCommand[2]] = currentCommand[3];
          Serial.print("MODULE ID READING: ");
          Serial.print(currentCommand[1]);
          Serial.print("-");
          Serial.print(currentCommand[2]);
          Serial.print(": ");
          Serial.println(currentCommand[3]);
        }
        break;
      }
    }
  }

  updatePhysicalModuleList();

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

void updatePhysicalModuleList() {
  for(int i=0; i<MAX_MODULES; i++) {
    if(moduleIDReadings[i] != physicalModules[i].id) {
      // update physical module instance
      physicalModules[i].id = moduleIDReadings[i];

      // kill previous virtual modules

      // check user module mappings

      // initialise new virtual modules
      if(physicalModules[i].id==136) virtualModules[i][0] = new TestOscillator();
    }
  }
}
